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
 * Database Access Class
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * This class represents the result of an database query.
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Database Result class
 *
 * @package TNT.Database
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTDbResult extends TNTObject {

    /**
     * Handle to the database which incurred the result
     * @var     object      $db        reference to TNTDatabase object
     * @access  public
     */
    var $db = null;

    /**
     * The actual sql string that produced the results
     * @var     string      $sql        SQL command string
     * @access  public
     */
    var $sql = null;

    /**
     * Handle to the open database result
     * @var     resource    $_result   database result resource handle
     * @access  private
     */
    var $_result = null;

    /**
     * Fetch row data as an associative array
     *
     * If field names are returned in the array then the original value of any
     * expanded subfields is return by prefixing the field name with a '_' to
     * indicate a 'private' field.
     *
     * @access  public
     * @param   integer $type       return numeric, associative or both array
     * @param   boolean $csf        automatically expand compound sub fields
     * @return  array               associative or numeric array of field values
     */
    function FetchArray($type = TNT_DB_FETCH_BOTH, $csf = false) {
        if (!($row = mysql_fetch_array($this->_result, $type)) || !$csf) {
            return $row;
        }
    
        // Expand any compound subfields
        foreach ($row as $n => $v) {
            if ($v && $v{0} == 'c') {
                $_v = $this->db->ExpandCSF($v, $n);
                if ($_v !== $v) {
                    if (!is_integer($n)) {
                        $row["_$n"] = $v;
                    }
                    $row[$n] = $_v;
                }
            }
        }
    
        return $row;
    }
    
    /**
     * Fetch row data as an enumerated array
     *
     * @access  public
     * @param   boolean $csf        automatically expand compound sub fields
     * @return  array               numeric array of field values
     */
    function FetchRow($csf = false) {
        if (!($row = mysql_fetch_row($this->_result)) || !$csf) {
            return $row;
        }
    
        // Expand any compound subfields
        foreach ($row as $n => $v) {
            if ($v && $v{0} == 'c') {
                $_v = $this->db->ExpandCSF($v, $n);
                if ($_v !== $v) {
                    $row[$n] = $_v;
                }
            }
        }
    
        return $row;
    }
    
    /**
     * Fetch row data as an object
     *
     * Each field name is used as a member name in the returned object.
     * The original value of any expanded subfields is return by prefixing the
     * field name with a '_' to indicate a 'private' member.
     *
     * @access  public
     * @param   boolean $csf        automatically expand compound sub fields
     * @return  object              object with members holding the field values
     */
    function FetchObject($csf = false) {
        if (!($row = mysql_fetch_object($this->_result)) || !$csf) {
            return $row;
        }
    
        // Expand any compound subfields
        foreach ((array)$row as $n => $v) {
            if ($v && $v{0} == 'c') {
                $_v = $this->db->ExpandCSF($v, $n);
                if ($_v !== $v) {
                    $backup = "_$n";
                    $row->$backup = $v;
                    $row->$n = $_v;
                }
            }
        }
    
        return $row;
    }
    
    /**
     * Fetch row data but return the value of a single field from row
     *
     * @access  public
     * @param   string  $fld        field name            
     * @param   boolean $csf        automatically expand compound sub fields
     * @return  mixed               value of the field in the current result row
     */
    function FetchField($fld, $csf = false) {
        $row = $this->FetchArray(TNT_DB_FETCH_ASSOC, $csf);
        return (($row && isset($row[$fld])) ? $row[$fld] : null);
    }
    
    /**
     * Return the number of rows in a result set
     *
     * @access  public
     * @return  integer             number of rows in result
     */
    function NumRows() {
        return mysql_num_rows($this->_result);
    }
    
    /**
     * Return the number of fields in a result set
     *
     * @access  public
     * @return  integer             number of fields in result
     */
    function NumFields() {
        return mysql_num_fields($this->_result);
    }
    
    /**
     * Return the name of the field at offset in result
     *
     * @access  public
     * @return  integer $field      field index
     * @return  string              name of field
     */
    function FieldName() {
        return mysql_field_name($this->_result, $field);
    }
    
    /**
     * Seek to a particlur row in the result set
     *
     * @access  public
     * @param   integer $row        number of row to seek to
     * @return  void
     */
    function SeekRow($row) {
        return mysql_data_seek($this->_result, $row);
    }
    
    /**
     * Return results of one field in a row
     *
     * This does not advance the row - nor does it expand CSFs.
     *
     * @access  public
     * @param   integer $row        number of row to select field from
     * @param   integer $field      number of field to return
     * @return  mixed               value of field
     */
    function Result($row = 0, $field = 0) {
        return mysql_result($this->_result, $row, $field);
    }
    
    /**
     * Free the results
     *
     * @access  public
     * @return                      true if successfull
     */
    function Free() {
        return mysql_free_result($this->_result);
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   resource    $result     native db result resource
     * @param   string      $sql        sql query that the result is for
     * @param   TNTDatabase $db         database that made the query
     * @return  void
     */
    function TNTDbResult($result, $sql, &$db) {

        TNTObject::TNTObject();

        $this->_result = $result;
        $this->sql = $sql;
        $this->db =& $db;
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDbResult() {
        $this->Free();
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
