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
 * Database Record Access Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class is a extension to the basic {@link TNTDatabase} class to provide
 * a series of methods for manipulating database records as associative arrays.
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTDatabase');

/*
 * Globals
 */

/*
 * Defines
 */
define('TNT_DBREC_PRE', true);
define('TNT_DBREC_PST', false);

/**
 * Database Record Access Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class is a extension to the basic {@link TNTDatabase} class to provide
 * a series of methods for manipulating database records as associative arrays.
 *
 * Each database record is seen as an associative array of data. The class
 * provides support for auto-incrementing record keys and record timestamps.
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTDbRec extends TNTObject {

    /**
     * Reference to database object used to manipulate the database
     * @var     object                  reference to TNTDatabase object
     * @access  private
     */
    var $db = null;

    /**
     * Automatically update records timestamps where appropriate.
     * @var     boolean                 true if timestamps are updated
     * @access  public
     */
    var $uts = true;

    /**
     * Automatically convert complex data into compound subfields.
     * @var     boolean                 true if using compound subfields
     * @access  public
     */
    var $csf = true;

    /**
     * Psuedo name of the table storing the tree nodes
     * @var     string                  psuedo database table name
     * @access  private
     */
    var $_table = null;

    /**
     * Real name of the table
     * @var     string                  real database table name
     * @access  private
     */
    var $_tab = null;

    /**
     * Schema for the specified table
     * @var     array                   table schema
     * @access  private
     */
    var $_schema = null;

    /**
     * Field name of the record key field
     * @var     string                  database column name
     * @access  private
     */
    var $_rkf = 'id';

    /**
     * Is the record key and auto-increment key
     * @var     boolean                 true if $_rkf column is auto-increment
     * @access  private
     */
    var $_aik = true;

    /**
     * Has the table got timestamp fields
     * @var     boolean                 true if table has timestamp fields
     * @access  private
     */
    var $_hts = false;

    /**
     * Array of fields names (or indices to the _schema members)
     * @var     string[]                array of database column names
     */
    var $_flds;

    /**
     * Called just before and after a record is to be created.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the create will have been done so it
     * is up to this method to reverse the create before returning failed.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL INSERT
     * @param   array&      $rec        reference to record being created
     * @return  boolean                 true if successfull
     */
    function _OnCreate($pre, &$rec) {
        return true;
    }

    /**
     * Called just before and after a record is to be modified.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the update will have been done so it
     * is up to this method to reverse the update before returning failed.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL UPDATE
     * @param   array&      $rec        reference to record being modified
     * @return  boolean                 true if successfull
     */
    function _OnModify($pre, &$rec) {
        return true;
    }

    /**
     * Called just before and after records are to be changed.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the update will have been done so it
     * is up to this method to reverse the update before returning failed.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL UPDATE
     * @param   array&      $set        array of changes to perform
     * @param   string      $where      SQL where condition for records effected
     * @return  boolean                 true if successfull
     */
    function OnChange($pre, &$set, $where) {
        return true;
    }

    /**
     * Called just before and after records are to be deleted.
     *
     * If the method returns false then the triggering method will fail. This
     * method should call {@link _SetError()} to indicate why it failed.
     *
     * If the after event is failed then the delete will have been done so it
     * is up to this method to reverse the delete before returning failed.
     *
     * @access  private
     * @param   boolean     $pre        true if called before SQL DELETE
     * @param   string      $where      SQL where condition for records effected
     * @return  boolean                 true if successfull
     */
    function _OnDelete($pre, $where) {
        return true;
    }

    /**
     * Return the psuedo table name.
     *
     * This name is set in the object constructor and can not change for the
     * life of the object.
     *
     * @access  public
     * @return  string                  psuedo table name for records
     */
    function Table() {
        return $this->_table;
    }

    /**
     * Return the record key field column name.
     *
     * This name is set in the object constructor and can not change for the
     * life of the object.
     *
     * @access  public
     * @return  string                  database column name of record key
     */
    function RecordKeyName() {
        return $this->_rkf;
    }

    /**
     * Return if a given table is timestamped.
     *
     * The table is assumed to be timestamped if it contains a 'mtime', 'ctime'
     * and 'atime' fields of type 'integer'.
     *
     * When a record handling function is used the timestamps are automatically
     * updated (unless the {@link $uts} member is false), any timestamp fields
     * specified in the record will not be used to update the timestamps.
     *
     * This does not support the MySQL 'timestamp' field type as this is
     * formatted differently to the UNIX/PHP timestamp we use and instead of
     * perform conversion each time we just use an integer field to store the
     * UNIX/PHP timestamp. If a MySQL timestamp is required then the
     * 'FROM_UNIXTIME()' SQL function can be used.
     *
     * @access  public
     * @return  boolean                 true if table has timestamp fields
     */
    function HasTimestamp() {
        return $this->_hts;
    }

    /**
     * Returns the table schema.
     *
     * Returned schema is an array of structure containing the
     * following fields;
     *   'name'     - field name
     *   'type'     - normalized type (see below)
     *   'phptype'  - appropriate php type (ie. 'number', 'string')
     *   'sqltype'  - raw SQL type (ie. 'VARCHAR(32)')
     *   'length'   - field length (or display length)
     *   'key'      - type of key 'primary', 'secondary'
     *   'default'  - default value
     *   'null'     - true is null values are allowed
     *   'extra'    - extra info returned from implimentation
     *
     * @access  public
     * @return  array                   array of table schema data
     * @see TNTDatabase::Schema()
     */
    function Schema() {
        return $this->_schema;
    }

    /**
     * Returns the table field names.
     *
     * @access  public
     * @return  string[]                array of database column names
     */
    function FieldNames() {
        return $this->_flds;
    }

    /**
     * Prepare a selection criteria into a SQL where clause.
     *
     * Certain method allow a selection criteria to be given to select which
     * records it should operate on.
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar record key value of a specific record.
     *
     * b) You can specify an array of scalar record key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     * In the case of (a) and (b) the method will set $num to indicate the
     * number of specific records selected.
     *
     * The return value is either;
     *  null        - an error occurred
     *  ''          - no where condition - select everything
     *  'WHERE xxx' - the SQL WHERE clause
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @param   integer&    $num        number of selected record (or -1)
     * @return  string                  SQL WHERE clause
     */
    function PrepareSel($sel, &$num) {

        if (is_string($sel)) {
            if ($sel == '*') {
                $ret = 'WHERE 1';
                $num = -1;
            } else if (strncasecmp($sel, 'WHERE ', 6) == 0) {
                $ret = $sel;
                $num = -1;
            } else {
                $ret = "WHERE `{$this->_rkf}` = ".$this->db->PrepareVal($sel);
                $num = 1;
            }
        } else if (is_array($sel)) {
            $ids = array();
            foreach ($sel as $v) {
                $ids[] = $this->db->PrepareVal($v);
            }
            if ($ids) {
                $ids = join(',', $ids);
                $ret = "WHERE `{$this->_rkf}` IN ($ids)";
                $num = count($ids);
            } else {
                $ret = 'WHERE 1';
                $num = -1;
            }
        } else if (is_null($sel)) {
            $ret = 'WHERE 1';
            $num = -1;
        } else {
            $ret = "WHERE `{$this->_rkf}` = ".$this->db->PrepareVal($sel);
            $num = 1;
        }

        return $ret;
    }

    /**
     * Make a SET clause from the given data.
     *
     * Only fields in the record can actually be included in the set. Any
     * other fields are ignored. Any timestamp fields can not be included
     * in the set.
     *
     * @access  public
     * @param   array       $set        associative array of field => value
     * @return  string                  SQL SET clause
     */
    function PrepareSet($set) {
    
        $ret = false;
    
        if (!$set) {
            TNT::Abort('TNTDatabase::PrepareSet() - invalid arguments');
        }

        if ($this->_hts) {
            unset($set['atime']);
            unset($set['mtime']);
            unset($set['ctime']);
        }
    
        // Fields to set are stored here
        $sets = null;
        foreach ($this->_flds as $fld) {
            if (array_key_exists($fld, $set)) {
                $sets[] = "`{$fld}` = ".$this->db->PrepareVal($set[$fld], $this->csf);
            }
        }

        $ret = ($sets ? 'SET '.join(',', $sets) : '');
    
        return $ret;
    }
    
    /**
     * Update an access time stamp for this record.
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar record key value of a specific record.
     *
     * b) You can specify an array of scalar record key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     * This is automatically done if the {@link $uts} member variable is
     * true. This method can be used where greater control of the timestamps
     * is required.
     *
     * If the timestamp field specified does not exist then an error will be
     * returned.
     *
     * @access  public
     * @param   mixed       $sel        record selection criteria
     * @param   string      $tsf        name of timestamp field to update
     * @return  boolean                 true if timestamp updated successfully
     */
    function UpdateTimeStamp($sel, $tsf = 'atime') {
    
        $ret = false;

        // Start error section
        do {
            if (!$this->_hts) {
                $this->_SetError('Record does not contain timestamps');
                break;
            }

            if ($tsf != 'atime' && $tsf != 'ctime' && $tsf != 'mtime') {
                $this->_SetError('Timestamp field must be \'atime\', \'ctime\' or \'mtime\'');
                break;
            }

            $time = (int)time();
            $where = $this->PrepareSel($sel);

            if (!$this->db->Query("UPDATE `{$this->_tab}` SET `$tsf` = $time $where")) {
                $this->_SetError('Failed to update timestamp on records: '.$where, $this->db);
                break;
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Define a record stored in the database
     *
     * @access  public
     * @return  array                   structure defining records structure
     */
    function Define() {
    
        $ret = null;
    
        foreach ($this->_schema as $field) {
            $ret[$field['name']] = $field['default'];
        }
    
        return $ret;
    }

    /**
     * Check if a record exists in the database.
     *
     * @access  public
     * @param   scalar      $key        key of record to check
     * @return  boolean                 true if the record exists
     */
    function Exists($key) {

        $ret = ($this->db->Count($this->_table, $this->_rkf, $key) == 1);

        return $ret;
    }
    
    /**
     * Count how many records in a selection.
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar record key value of a specific record.
     *
     * b) You can specify an array of scalar record key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @return  integer                 count of records in selection
     */
    function Count($sel) {

        $ret = null;

        // Start error section
        do {

            $num = -1;

            $where = $this->PrepareSel($sel, $num);

            if (is_null($where)) {
                break;
            }

            $count = $this->db->FetchValue("SELECT COUNT(*) FROM `{$this->_tab}` $where");

            if (is_null($count)) {
                $this->_SetError('Can not calculate how many records selected by criteria: '.$where, $this->db);
                break;
            }

            $ret = (int)$count;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Select record(s) from the database.
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar record key value of a specific record.
     *
     * b) You can specify an array of scalar record key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     * Can also accept a limit clause using the standard MySQL 'LIMIT x,y'
     * syntax or the enhanced 'PAGE x,y' syntax.
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @param   string      $limit      limit criteria
     * @return  array                   array of structures containing records
     */
    function Select($sel = null, $limit = null) {
    
        $ret = null;

        // Start error section
        do {
            $num = -1;

            $where = $this->PrepareSel($sel, $num);

            if (is_null($where)) {
                break;
            }

            $count = 1;

            if ($limit) {
                if (!preg_match('/^(LIMIT|PAGE)\s*(\d+)\s*,\s*(\d+)$/i', $limit, $matches)) {
                    $this->_SetError('Invalid limit syntax: '.$limit);
                    break;
                }

                if (strcasecmp($matches[1], 'PAGE') == 0) {
                    // Count how many records the where clause actually matches
                    $count = $this->db->FetchValue("SELECT COUNT(*) FROM `{$this->_tab}` $where");

                    if (is_null($count)) {
                        $this->_SetError('Can not calculate how many records selected by criteria: '.$where, $this->db);
                        break;
                    }

                    if ($count == 0) {
                        // No record are selected - so no need to go any further
                        $ret = array();
                        break;
                    }

                    $limit = '';

                    $page_no = $matches[1];
                    $page_size = $matches[2];
                    $page_max = ceil($count / $page_size);

                    // Page must be between 1 and $page_max
                    if ($page_no <= 0) {
                        $page_no = $page_max;
                    } else if ($page_no > $page_max) {
                        $page_no = 1;
                    }
            
                    $start = ($page_size * ($page_no - 1));
    
                    $limit = "LIMIT $start,$page_size";
                }
            }

            $recs = $this->db->Fetch("SELECT * FROM `{$this->_tab}` $where $limit", 0, TNT_DB_FETCH_ASSOC, $this->csf);

            if (is_null($recs)) {
                $this->_SetError('Can not fetch records', $this->db);
                break;
            }
    
            if ($recs && $this->uts && $this->_hts) {
                // We update the access time in the database but not the object
                // hence the time return is the time of the last access not this
                // current access - otherwise you would never be able to read it
                $ids = array();
                foreach ($recs as $rec) {
                    $ids[] = $this->db->PrepareVal($rec[$this->_rkf], $this->csf);
                }
                $ids = join(',', $ids);

                $time = (int)time();

                if (!$this->db->Query("UPDATE `{$this->_tab}` SET `atime` = $time WHERE `{$this->_rkf}` IN ($ids)")) {
                    trigger_error('TNTDbRec::Select() - can not update record access time: '.$this->db->Error(), E_USER_NOTICE);
                }
            }

            $ret = $recs;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Obtain a record in the database.
     *
     * This is the same as {@link Select()} only it returns a single
     * record and not an array of records. If the selection criteria
     * selects more than one record then only the first is returned.
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @return  array                   structure defining records structure
     */
    function Obtain($sel) {
        $recs = $this->Select($sel, 'LIMIT 0,1');
        return (isset($recs[0]) ? $recs[0] : null);
    }
    
    /**
     * Create a record in the database
     *
     * A reference to the record should be given so that the timestamps can be
     * updated. Also any auto increment record key is updated in the record.
     *
     * @access  public
     * @param   array       $rec        reference to structure defining record
     * @return  boolean                 true if updated record successfully
     */
    function Create(&$rec) {
    
        $ret = false;

        // Start error section
        do {
            if (!$rec || !is_array($rec)) {
                $this->_SetError('Invalid record');
                break;
            }

            $uts = ($this->uts && $this->_hts);

            // Correct any timestamps
            if ($uts) {
                $time = time();
                $rec['ctime'] = $time;
                $rec['mtime'] = $time;
                $rec['atime'] = $time;
            }

            // Trigger the pre create events
            if (!$this->_OnCreate(TNT_DBREC_PRE, $rec)) {
                break;
            }

            // Ensure the auto-increment key is given the next sequence value
            if ($this->_aik) {
                $rec[$this->_rkf] = null;
            }

            $flds = null;
            $vals = null;

            foreach ($this->_flds as $fld) {
                if (array_key_exists($fld, $rec)) {
                    $flds[] = $fld;
                    $vals[] = $this->db->PrepareVal($rec[$fld], $this->csf);
                }
            }

            $flds = join(',', $flds);
            $vals = join(',', $vals);

            if (!$this->db->Query("INSERT INTO `{$this->_tab}` ($flds) VALUES ($vals)")) {
                $this->_SetError('Can not create record', $this->db);
                break;
            }

            // If the record key is an auto-increment key then copy its new
            // value into the record.
            if ($this->_aik) {
                if (is_null($rkv = $this->db->InsertId()))  {
                    $this->_SetError('Can not get value of auto increment record key', $this->db);
                    break;
                }
                $rec[$this->_rkf] = $rkv;
            }

            // Trigger the post create events
            if (!$this->_OnCreate(TNT_DBREC_PST, $rec)) {
                break;
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Modify a record in the database
     *
     * A reference to the record should be given so that the modification
     * timestamp can be updated if required.
     *
     * @access  public
     * @param   array       $rec        reference to structure defining record
     * @return  boolean                 true if modified record successfully
     */
    function Modify(&$rec) {
    
        $ret = false;

        // Start error section
        do {
            if (!$rec || !is_array($rec)) {
                $this->_SetError('Invalid record');
                break;
            }

            if (!isset($rec[$this->_rkf]) || is_null($rec[$this->_rkf])) {
                $this->_SetError('Record as no value for its key field');
                break;
            }

            $rkv = $this->db->PrepareVal($rec[$this->_rkf], $this->csf);

            $where = "WHERE `{$this->_rkf}` = $rkv";

            // Trigger the pre update events
            if (!$this->_OnModify(TNT_DBREC_PRE, $rec, $where)) {
                break;
            }
 
            $set = $this->PrepareSet($rec);

            if (!$set) {
                // Nothing to modify
                $ret = true;
                break;
            }

            if ($this->uts && $this->_hts) {
                $rec['mtime'] = (int)time();
                $set .= ', `mtime` = '.$rec['mtime'];
            }
    

            if (!$this->db->Query("UPDATE `{$this->_tab}` $set $where")) { 
                $this->_SetError('Can not update record', $this->db);
                break;
            }

            // Trigger the on update events
            if (!$this->_OnModify(TNT_DBREC_PST, $rec, $where)) {
                break;
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Delete record(s) from the database
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar record key value of a specific record.
     *
     * b) You can specify an array of scalar record key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     * The strict argument when set will cause the function to return false
     * unless all the records requested for deletion actualy exist. Without
     * this asking to delete non existant records is not an error. 
     *
     * The strict value is ignored if the 'ALL' or 'WHERE' keywords are used.
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @param   boolean     $strict     fail unless all selected records deleted
     * @return  boolean                 true if deleted record(s) successfully
     */
    function Delete($sel, $strict = false) {
    
        $ret = false;

        // Start error section
        do {

            $num = -1;

            $where = $this->PrepareSel($sel, $num);

            if (is_null($where)) {
                break;
            }

            // Trigger the pre delete events
            if (!$this->_OnDelete(TNT_DBREC_PRE, $where)) {
                break;
            }

            if (!$this->db->Query("DELETE FROM `{$this->_tab}` $where")) {
                $this->_SetError('Can not delete records where: '.$where, $this->db);
                break;
            }

            if ($strict && $num != -1 && $this->db->NumAffectedRows() != $num) {
                $this->_SetError('Did not delete all selected records where: '.$where);
                break;
            }

            // Trigger the post delete events
            if (!$this->_OnDelete(TNT_DBREC_PST, $where)) {
                break;
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Change record(s) in the database
     *
     * The selection criteria are specified as follows;
     *
     * a) You can specify a scalar record key value of a specific record.
     *
     * b) You can specify an array of scalar record key values of a specific
     *    records.
     *
     * c) You can specify null or an empty array or the string '*' to mean
     *    all records.
     *
     * d) You can specify the string value 'WHERE <SqlCondition>' to specify a
     *    SQL where condition, the condition can refer to any field in the
     *    table using its real column name.
     *
     * The strict argument when set will cause the function to return false
     * unless all the records requested for deletion actualy exist. Without
     * this asking to delete non existant records is not an error. 
     *
     * The strict value is ignored if the 'ALL' or 'WHERE' keywords are used.
     *
     * Any modification timestamp is updated on the changed records if required.
     *
     * @access  public
     * @param   mixed       $sel        selection criteria
     * @param   array       $set        associative array of 'fld=>val' to set
     * @param   boolean     $strict     fail unless all selected records deleted
     * @return  boolean                 true if deleted record(s) successfully
     */
    function Change($sel, $set, $strict = false) {
    
        $ret = false;

        // Start error section
        do {
            if (!$set || !is_array($set)) {
                $this->_SetError('Invalid data set');
                break;
            }

            $num = -1;

            $where = $this->PrepareSel($sel, $num);

            if (is_null($where)) {
                break;
            }

            // Trigger the pre update events
            if (!$this->_OnChange(TNT_DBREC_PRE, $set, $where)) {
                break;
            }

            $set = $this->PrepareSet($set);

            if (!$set) {
                // Nothing to set
                $ret = true;
                break;
            }

            if ($this->uts && $this->_hts) {
                $rec['mtime'] = (int)time();
                $set .= ', `mtime` = '.$rec['mtime'];
            }
    
            if (!$this->db->Query("UPDATE `{$this->_tab}` $set $where")) {
                $this->_SetError('Can not change records where: '.$where, $this->db);
                break;
            }

            if ($strict && $num != -1 && $this->NumAffectedRows() != $num) {
                $this->_SetError('Did not change all selected records where: '.$where);
                break;
            }

            // Trigger the post update events
            if (!$this->_OnChange(TNT_DBREC_PST, $set, $where)) {
                break;
            }


            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Update a record in the database (create new or modify existing)
     *
     * Update will call {@link Create()} is the key value of $rec is empty,
     * otherwise if will call {@link Modify()}.
     *
     * @access  public
     * @param   array       $rec        reference to structure defining record
     * @return  boolean                 true if updated record successfully
     */
    function Update(&$rec) {
    
        if (empty($rec[$this->_rkf])) {
            $ret = $this->Create($rec);
        } else {
            $ret = $this->Modify($rec);
        }
    
        return $ret;
    }
    
    /**
     * Import a record from an array of data.
     *
     * This will initialise record members from the corresponding members
     * in the source array. The source member names can have an optional
     * prefix.
     *
     * For example;
     *     $rec['fred'] = $src[$pfx.'fred']
     *
     * If the '$src' argument is null then the $GLOBALS array will be used.
     *
     * Only fields that exist in the record schema are copied from the
     * source array. Any other fields in record structure are ignored.
     *
     * @access  public
     * @param   array&      $rec        structure defining record
     * @param   array&      $src        array to import from (null = $GLOBALS)
     * @param   string      $pfx        prefix all field names with this value
     * @return  boolean                 true if successfull
     */
    function Import(&$rec, &$src, $pfx = null) {
    
        $ret = false;

        // Start error section
        do {

            if (!$rec || !is_array($rec)) {
                $this->_SetError('No record provided');
                break;
            }

            if (is_null($src)) {
                $src =& $GLOBALS;
            }

            if (!is_array($src)) {
                $this->_SetError('No import source provided');
                break;
            }
    
            // Copy record value from $src array
            foreach ($this->_flds as $fld) {
                if (array_key_exists($pfx.$fld, $src)) {
                    $rec[$fld] = $src[$pfx.$fld];
                }
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Export a record into an array of data.
     *
     * This will initialise members in the destination array with the values of
     * record members. The destination member names can have an optional prefix.
     *
     * For example;
     *     $dst[$pfx.'fred'] = $rec['fred'];
     *
     * If the '$dst' argument is null then the $GLOBALS array will be used.
     *
     * Only fields that exist in the record schema are copied to the
     * destination array. Any other fields in record structure are ignored.
     *
     * @access  public
     * @param   array       $rec        reference to structure defining record
     * @param   array&      $dst        array to export to (null = $GLOBALS)
     * @param   string      $pfx        prefix all field names with this value
     * @return  boolean                 true if successfull
     */
    function Export($rec, &$dst, $pfx = null) {
    
        $ret = false;

        // Start error section
        do {

            if (!$rec || !is_array($rec)) {
                $this->_SetError('No record provided');
                break;
            }

            if (is_null($dst)) {
                $dst =& $GLOBALS;
            }

            if (!is_array($dst)) {
                $this->_SetError('No export source provided');
                break;
            }

            // Copy record value to $dst array
            foreach ($this->_flds as $fld) {
                if (array_key_exists($fld, $rec)) {
                    $dst[$pfx.$fld] = $rec[$fld];
                }
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /**
     * Check the object is valid for use.
     *
     * Checks that a valid database is available and the table and fields
     * are known.
     *
     * @access  public
     * @return  boolean                 true if object is valid
     */
    function IsValid() {
        if (!$this->db || !is_a($this->db, 'tntdatabase')) {
            $this->_SetError('No valid database object');
            $ret = false;
        } else if (!$this->db->IsValid()) {
            $this->_SetError('Invalid database', $this->db);
            $ret = false;
        } else if (!$this->_schema) {
            $this->_SetError('No valid table');
            $ret = false;
        } else if (!$this->_rkf) {
            $this->_SetError('No valid record key field');
            $ret = false;
        }
        return $ret;
    }

    /**
     * Construct the object
     *
     * Supported construction options are;
     *   'db' - reference to database instance to use
     *   'uts' - boolean to indicate timestamps should be updated
     *   'csf' - boolean to indicate compound subfields should be used
     *
     * If no record key is specified then the schema is checked for the name
     * of the primary key field and it is used.
     *
     * @access  public
     * @param   string     $table       pseudo database table name
     * @param   string     $rkf         record key field name in table
     * @param   array      $options     construction options (optional)
     * @return  void
     */
    function TNTDbRec($table, $rkf = 'id', $options = null) {

        TNTObject::TNTObject();

        $this->_table = $table;
        $this->_rkf = $rkf;

        if (!empty($options['db'])) {
            $this->db =& $options['db'];
        } else {
            $this->db =& TNTDatabase::Singleton();
        }

        if (!is_a($this->db, 'tntdatabase') || !$this->db->IsValid()) {
            TNT::Abort('TNTDbRec::TNTDbRec() - no valid database instance available');
        }

        if (!($this->_schema = $this->db->Schema($this->_table))) {
            TNT::Abort('TNTDbRec::TNTDbRec() - no schema for table: '.$this->_table);
        }

        if (!$this->_rkf) {
            if (!($index = $this->db->TableIndex($this->_table))) {
                TNT::Abort('TNTDbRec::TNTDbRec() - no primary key field for table: '.$this->_table);
            }
            $this->_rkf = $index['name'];
        } else if (empty($this->_schema[$this->_rkf])) {
            TNT::Abort('TNTDbRec::TNTDbRec() - '.$this->_rkf.' is not a field in table: '.$this->_table);
        }


        $this->_aik = (stristr($this->_schema[$this->_rkf]['extra'], 'auto_increment') !== false);

        $this->_tab = $this->db->TableName($this->_table);

        $this->_hts = (isset($this->_schema['ctime']) &&
                       isset($this->_schema['mtime']) &&
                       isset($this->_schema['ctime']) &&
                       $this->_schema['ctime']['type'] == 'integer' &&
                       $this->_schema['mtime']['type'] == 'integer' &&
                       $this->_schema['ctime']['type'] == 'integer');

        $this->_flds = array_keys($this->_schema);

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDbRec() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
