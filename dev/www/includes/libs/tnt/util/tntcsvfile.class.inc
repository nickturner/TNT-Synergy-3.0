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
 * Csv File Class
 * ~~~~~~~~~~~~~~
 *
 * A file containing CSV data, one record per line.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTFile');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to manipulate a CSV file.
 *
 * At present this class only supports the reading and writing of whole files
 * at a time as we rarely ever read or update parts of a file.
 *
 * File locking on reading and writing is supported.
 *
 * @package TNT.System
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTCsvFile extends TNTFile {


    /**
     * Field delimiting character
     * @var     string                  field delimiting character
     * @access  private
     */
    var $_delim = ',';

    /**
     * String quoting character
     * @var     string                  string quoting character
     * @access  private
     */
    var $_quote = '"';

    /**
     * Value indicationg is item values are case sensitive
     * @var     boolean                 true if items are case sensitive
     * @access  private
     */
    var $_caseSensitive = false;

    /**
     * Set the CSV options
     *
     * @access  public
     * @param   string      $delim      delimiter string
     * @param   string      $quote      quote character
     * @param   string      $case       strings are case sensitive
     * @return  boolean                 true if successfull
     */
    function SetOpts($delim = null, $quote = null, $case = null) {

        if (!is_null($delim) && trim($delim) != '') {
            $this->_delim = $delim;
        }
        if (!is_null($quote) && trim($quote) != '') {
            $this->_quote = $quote{0};
        }
        if (!is_null($case)) {
            $this->_caseSensitive = $case;
        }

        $this->_string = null;

        return true;
    }

    /**
     * Get the CSV options
     *
     * Returns associative array containing the elements;
     *     'delim', 'quote' & 'case'
     *
     * @access  public
     * @return  array                   the options
     */
    function GetOpts() {

        $ret['delim'] = $this->_delim;
        $ret['quote'] = $this->_quote;
        $ret['case'] = $this->_case;

        return $ret;
    }

    /**
     * Read the actual file data
     *
     * @access  public
     * @param   resource        $fp     open file handle
     * @return  string                  actual data read (or false or error)
     */
    function _ReadData($fp) {

        $ret = null;
        while (($row = fgetcsv($fp, 4096, $this->_delim, $this->_quote))) {
            $ret[] = $row;
        }

        if (!feof($fp)) {
            $this->_SetError('unable to read all rows - check for corrupt CSV record at row '.count($ret));
            return false;
        }

        return $ret;
    }
    
    /**
     * Write the actual file data
     *
     * @access  public
     * @param   resource        $fp     open file handle
     * @param   string          $data   data to write to file
     * @return  boolean                 true if written ok
     */
    function _WriteData($fp, $data) {

        if (is_array($data)) {
            TNTLoader::RequireClass('TNTCsv');

            $csv =& new TNTCsv();
            $csv->SetOpts($this->_delim, $this->_quote);

            $ret = true;
            foreach ($data as $cols) {
                $csv->SetItems($cols);

                $str = $csv->ToString()."\n";

                if (!($ret = parent::_WriteData($fp, $str))) {
                    break;
                }
            }

            destroy($csv);
        } else {
            $ret = parent::_WriteData($fp, $data);
        }

        return $ret;
    }

    /**
     * Construct the object
     *
     * File name should be specified as either an absolute path or a path
     * relative to the installation directory. Alternatively the path can
     * be specified in the virtual '/synergy' filesystem.
     *
     * @access  public
     * @param   string      $name       file name
     * @return  void
     */
    function TNTCsvFile($name = null) {
        TNTFile::TNTFile($name);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCsvFile() {
    }

}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
