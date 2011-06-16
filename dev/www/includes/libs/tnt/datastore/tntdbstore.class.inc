<?php

///////////////////////////////////////////////////////////////////////////////
// TNT-SYNERGY: PHP Application Framework                            v2.0.00 //
// ======================================                                    //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-synergy.com                                                 //
// http://www.tnt-synergy.com/                                               //
//                                                                           //
// TNT-SYNERGY is a PHP Application Framework. It encourages application     //
// structured application architectures based on the "Model2" version of the //
// Model View Controller (MVC) design paradigm. The framework supports the   //
// following key features:                                                   //
//     - Model                                                               //
//         - session persistance (PHP Sessions)                              //
//         - permanent persistance (Database library abstraction)            //
//         - authentication (Multi-user)                                     //
//         - authorization (Access control)                                  //
//         - application model (Object based view of web page)               //
//     - Control                                                             //
//        - state processing (Page states)                                   //
//        - event processing (User supplied events)                          //
//     - View                                                                //
//        - themes (application and/or user themes)                          //
//        - template based presentation (Template library abstraction)       //
//        - internationalisation (Multi-language support)                    //
//                                                                           //
// This program is free software. You can redistribute it and/or modify it   //
// under the terms of the GNU General Public License as published by the     //
// Free Software Foundation; either version 2 of the License, or (at your    //
// option) any later version.                                                //
//                                                                           //
//   #####################################################################   //
//   # This program is distributed in the hope that it will be useful,   #   //
//   # but WITHOUT ANY WARRANTY; without even the implied warranty of    #   //
//   # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU #   //
//   # General Public License for more details.                          #   //
//   #####################################################################   //
//                                                                           //
// TNT-Synergy is a wholly owned subsidiary of TurnerTronics. The project is //
// open development and is the culmination of many peoples time and effort.  //
// For a full list of developers and copyrights please refer to CREDITS.TXT  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * Database DataStore Class
 * ~~~~~~~~~~~~~~~~~~i~~~~~
 *
 * This class defines an implementation of the TNTDataStore interface which
 * stores its data in a database. As such the data is only available for the
 * lifetime of the database record.
 *
 * @package TNT.DataStore
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTDbRec'); 
TNTLoader::RequireClass('TNTDataStore'); 

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to define a database based implementation of the TNTDataStore
 * interface.
 *
 * This implementation of the TNTDataStore interface uses a database record to
 * store the data and as such the data is only available for the lifetime of
 * the record.
 *
 * It assumes the record has the standard 'mtime', 'ctime' and 'atime' fields
 * to hold the timestamps.
 *
 * @package TNT.DataStore
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @extends TNTObject
 * @implements TNTDataStore
 */
class TNTDbStore extends TNTObject /* implements TNTDataStore */ {

    /**
     * DataStore name (used to identify one store from another)
     * @var     string                  datastore name
     * @access  private
     */
    var $_name = null;

    /**
     * Database
     * @var     TNTDatabase             reference to database object
     * @access  private
     */
    var $_db = null;

    /**
     * DataStore name components
     * @var     string[]                datastore name components
     * @access  private
     */
    var $_ni = null;

    /**
     * Prepare the database record
     *
     * @access  public
     * @return  boolean                 true if prepared ok
     */
    function _Prepare() {
        $ret = false;

        // Start of error section
        do {
            $this->_ni = null;
            unset($this->_db);
            $this->_db = null;

            $ni = parse_url($this->_name);

            if (empty($ni['path']) || empty($ni['query']) || !empty($ni['fragment'])) {
                $this->_SetError('Invalid name format: '.$this->_name);
                break;
            }

            // Split path
            $pi = explode('.', $ni['path']);
            $npi = count($pi);

            // Last 2 components of the path are the table and field
            if ($npi < 2) {
                $this->_SetError('Invalid name format: '.$this->_name);
                break;
            }

            $table = $pi[$npi - 2];
            $field = $pi[$npi - 1];

            $sfx = (($npi > 2) ? '.' : '')."{$table}.{$field}"; 

            $ni['table'] = strtolower(trim($table));
            $ni['field'] = strtolower(trim($field));

            // Remove the table and field name from the name to get the dsn
            $pos = strpos($this->_name, $sfx);
            $ni['dsn'] = trim(substr($this->_name, 0, $pos));

            // Create database to open this DSN
            $db =& new TNTDatabase();

            if (!$db->Connect($ni['dsn']) || !$db->IsOpen()) {
                $this->_SetError('Can\'t get open database: '.$ni['dsn']);
                break;
            }

            // Check that this table/field exist
            if (!($schema = $db->Schema($ni['table']))) {
                $this->_SetError('Invalid name format - no table: '.$this->_name);
                break;
            }
            if (!isset($schema[$ni['field']])) {
                $this->_SetError('Invalid name format - no field: '.$this->_name);
                break;
            }

            // Find the primary key field
            if (!($ni['key'] = $db->TableIndex($ni['table']))) {
                $this->_SetError('Invalid name format - no primary key: '.$this->_name);
            }

            $ni['key']['value'] = null;

            if (!empty($ni['query'])) {
                // Split the query (if no select then a new record is created
                // when the data is first stored).
                $qry = array();
                parse_str($ni['query'], $qry);
                $ni['key']['value'] = (isset($qry['record']) ? $qry['record'] : null);
                $q = preg_replace('/record=[^&]*(&)?/', '', $ni['query']);
                if (!empty($q)) {
                    $ni['dsn'] .= '?'.$q;
                }
            }

            // Can we auto-create/auto-delete records
            $s = $schema;
            unset($s[$ni['key']['name']]);
            unset($s[$ni['field']]);
            unset($s['atime']);
            unset($s['ctime']);
            unset($s['mtime']);
            $ni['auto'] = (count($s) == 0);

            $this->_ni = $ni;
            $this->_db =& $db;

            $ret = true;
        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Get datastore name
     *
     * @access  public
     * @return  string                  datastore name
     */
    function GetName() {
        return $this->_name;
    }

    /**
     * Set datastore name
     *
     * The name should be specified using the following extended DNS notation.
     *
     *  phptype(dbsyntax)://user:pass@protocol+host/name.table.field?opts
     *
     * where the fields are as documented in {@link TNTDatabase::ParseDSN()}
     * with the addition of;
     *  table: name of database table to use
     *  field: name of field in the table to store data
     *  opts:  supported options are;
     *         record=X - use the record whose primary key is X 
     *
     * 
     * If no 'record' option is given or a key value of 0 is used then a
     * new record is created when the data is first stored. Records are
     * deleted when NULL data is stored only if the table does not contain
     * fields other than the primary key, the data storage field and any stats
     * fields (mtime, ctime, atime).
     *
     * If the primary key is not an numeric auto_increment key then a value
     * must be specified and auto-creation/deletion is not supported.
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function SetName($name) {
        $this->_name = $name;
        $this->_ni = null;
        unset($this->_db);
        $this->_db = null;
    }

    /**
     * Get data store stats
     *
     * Elements are:
     *    null     - associative array of all components
     *    'size'   - size in bytes
     *    'atime'  - time of last access
     *    'mtime'  - time of last modification
     *    'ctime'  - time of last change
     *
     * If a datastore can not provide a given statistic then it should ommit
     * it from the returned structure or return a null value.
     *
     * If the store does not exist then the method should return null.
     *
     * @access  public
     * @param   string      $elm       component element to get
     * @return  array                  file statistic(s) (or null if error)
     */
    function Stats($elm = null) {
        $ret = null;

        // Start of error section
        do {
            if (!$this->_name) {
                $this->_SetError('no store name specified');
                break;
            }
            if (!$this->_db && !$this->_Prepare()) {
                break;
            }

            if (is_null($this->_ni['key']['value'])) {
                $this->_SetError('no store record');
                break;
            }

            $dbrec =& new TNTDbRec($this->_ni['table'], $this->_ni['key']['name']);

            if (!($rec = $dbrec->Obtain($this->_ni['key']['value']))) {
                $this->_SetError($this->_db->Error());
                break;
            }
            
            $data = $rec[$this->_ni['field']];

            $size = strlen(is_string($data) ? $data : serialize($data));

            $stats = array(
              'atime' => (isset($rec['atime']) ? $rec['atime'] : null),
              'ctime' => (isset($rec['ctime']) ? $rec['ctime'] : null),
              'mtime' => (isset($rec['mtime']) ? $rec['mtime'] : null),
              'size' => $size,
              );

            $ret = ($elm ? $stats[$elm] : $stats);

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Get the data from the data store and update last accessed timestamp.
     *
     * Returns null if it can't get the data.
     *
     * @access  public
     * @return  mixed                   data held in store (or null).
     */
    function GetData() {
        $ret = null;

        // Start of error section
        do {
            if (!$this->_name) {
                $this->_SetError('no store name specified');
                break;
            }
            if (!$this->_db && !$this->_Prepare()) {
                break;
            }

            if (is_null($this->_ni['key']['value'])) {
                $this->_SetError('no store record');
                break;
            }

            $dbrec =& new TNTDbRec($this->_ni['table'], $this->_ni['key']['name']);

            if (!($rec = $dbrec->Obtain($this->_ni['key']['value']))) {
                $this->_SetError($this->_db->Error());
                break;
            }

            $ret = $rec[$this->_ni['field']];

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Set the data in the datastore and update last modified timestamp.
     *
     * If the data is not null and the datastore does not exist then it
     * should be created.
     *
     * If the data is set to NULL then the datastore is deleted.
     *
     * Returns false if it can't set the data.
     *
     * @access  public
     * @param   mixed       $data       data to hold in datastore
     * @return  boolean                 true if set ok.
     */
    function SetData($data) {
        $ret = false;

        // Start of error section
        do {
            if (!$this->_name) {
                $this->_SetError('no store name specified');
                break;
            }
            if (!$this->_db && !$this->_Prepare()) {
                break;
            }

            $nullkey = (is_null($this->_ni['key']['value']) ? true : false);

            $dbrec =& new TNTDbRec($this->_ni['table'], $this->_ni['key']['name']);

            if (is_null($data)) {
                // Can we try to delete the record (not an error if fail)
                if (!$nullkey && $this->_ni['auto']) {
                    $dbrec->Delete($this->_ni['id']);
                }

                $ret = true;
            
            } else {
                if ($nullkey || !($rec = $dbrec->Obtain($this->_ni['key']['value']))) {
                    if (!$this->_ni['auto']) {
                        $this->_SetError('can\'t auto create database record');
                        break;
                    }
                    $rec = $dbrec->Define($this->_ni['table']);
                }

                $rec[$this->_ni['key']['name']] = $this->_ni['key']['value'];
                $rec[$this->_ni['field']] = $data;

                if (!$dbrec->Update($rec)) {
                    $this->_SetError($this->_db->Error());
                    break;
                }

                $this->_ni['key']['value'] = $rec[$this->_ni['key']['name']];

                $ret = true;
            }

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function TNTDbStore($name = null) {

        TNTObject::TNTObject();

        $this->_implements[] = 'tntdatastore';

        $this->_db = null;

        $this->SetName($name);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDbStore() {
        destroy($this->_db);
    }

}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
