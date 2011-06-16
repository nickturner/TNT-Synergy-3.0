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
 * Comma Separated Value Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class manipulates CSV string values.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTFmtStr'); 

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to manage a comma separated value (CSV) string.
 *
 * The class has options to specify the delimiter string (,),
 * and the quote character (").
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTCsv extends TNTFmtStr {

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
     * Return the number of items found in the CSV
     *
     * @access  public
     * @return  integer                 number of items in the CSV
     */
    function Count() {

        if (!$this->_elements) {
            $this->_Explode();
        }

        return count($this->_elements);
    }

    /**
     * Return the value of a single items in the CSV string
     *
     * @access  public
     * @param   integer     $index      index of item to return
     * @return  string                  value of the item (or false)
     */
    function GetItem($index) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if ($index >= 0 && $index < $this->Count()) {
            $ret = $this->_elements[$index];
        }

        return $ret;
    }

    /**
     * Set the CSV string from an array of strings
     *
     * @access  public
     * @param   integer     $index      index of item to return
     * @param   string      $value      value to set item to
     * @return  boolean                 true if successfull
     */
    function SetItem($index, $value) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if ($index >= 0 && $index < $this->Count()) {
            $this->_elements[$index] = (string)$value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Return an array of strings consisting of items in the CSV string
     *
     * @access  public
     * @return  array                   array of items in the CSV string
     */
    function GetItems() {

        if (!$this->_elements) {
            $this->_Explode();
        }

        return $this->_elements;
    }

    /**
     * Set the CSV string from an array of strings
     *
     * @access  public
     * @param   string[]    $items      items to convert to CSV string
     */
    function SetItems($items) {

        $ret = false;

        if (!$items || is_array($items)) {
            $this->_elements = $items;
            $this->_string = null;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Search for an item in the list of items
     *
     * @access  public
     * @param   string      $item       item to search for
     * @return  integer                 index of item, or false
     */
    function SearchItem($item) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        foreach ($this->_elements as $k => $v) {
            if (($this->_caseSensitive && strcmp($v, $item) == 0) ||
                (!$this->_caseSensitive && strcasecmp($v, $item) == 0)) {
                $ret = $k;
            }
        }

        return $ret;
    }

    /**
     * Append an item to the list of items
     *
     * @access  public
     * @param   string      $item       item to delete
     * @param   boolean     $unique     only append if not already in list
     * @return  integer                 index of item added, or false
     */
    function AppendItem($item, $unique = false) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if (!$unique || $this->SearchItem($item) === false) {
            $ret = count($this->_elements);
            $this->_elements[$ret] = $item;
            $this->_string = null; // so it is rebuilt from the item list
        }

        return $ret;
    }

    /**
     * Delete an item from the list of items
     *
     * @access  public
     * @param   string      $item       item to delete
     * @param   boolean     $all        delete all occurrance (otherwise first)
     * @return  integer                 index of last item deleted, or false
     */
    function DeleteItem($item, $all = true) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        while (($ret = $this->SearchItems($item)) !== false) {
            unset($this->_elements[$ret]);
            $this->_string = null; // so it is rebuilt from the item list
            if (!$all) {
                break;
            }
        }

        return $ret;
    }

    /**
     * Explode a value into a list of items
     *
     * @access  protected
     * @return  boolean                 true if successfull
      */
    function _Explode() {

        $quote = $this->_quote;
        $delim = $this->_delim;

        $this->_elements = array();
        $line = (string)$this->_string;
        $fields = &$this->_elements;
        $fldCount = 0;
        $inQuotes = false;
        $removeQuotes = true;
        $line_len = strlen($line);
        $delim_len = strlen($delim);
        for ($i = 0; $i < $line_len; $i++) {
            if (!isset($fields[$fldCount]))
                $fields[$fldCount] = "";
            $tmp = substr($line, $i, $delim_len);
            if ($tmp === $delim && !$inQuotes) {
                $fldCount++;
                $i += $delim_len - 1;
            } else if ($fields[$fldCount] == '' && $line{$i} == $quote && !$inQuotes) {
                if (!$removeQuotes)
                    $fields[$fldCount] .= $line{$i};
                $inQuotes = true;
            } else if ($line{$i} == $quote) {
                if ($line{$i+1} == $quote) {
                    $i++;
                    $fields[$fldCount] .= $line{$i};
                } else {
                    if (!$removeQuotes)
                        $fields[$fldCount] .= $line{$i};
                    $inQuotes = false;
                }
            } else {
                $fields[$fldCount] .= $line{$i};
            }
        }

        return true;
    }

    /**
     * Implode a value from a list of items
     *
     * @access  protected
     * @return  boolean                 true if successfull
     */
    function _Implode() {

        $quote = $this->_quote;
        $delim = $this->_delim;

        $this->_string = '';
        foreach ((array)$this->_elements as $item) {
            if (!empty($this->_string)) {
                $this->_string .= $delim;
            }
            if (!empty($item) && is_numeric($item)) {
                $this->_string .= $item;
            } else {
                // if a deliminator char, a quote char or a newline 
                // are in the field, add quotes
                if (strstr($item, $delim) ||
                    strstr($item, $quote) ||
                    strstr($item, "\n")) {
                    $this->_string .= $quote.str_replace($quote, $quote.$quote, $item).$quote;
                } else {
                    $this->_string .= $item;
                }
            }
        }

        return true;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   mixed       $data       initial value for object
     * @param   string      $delim      delimiter string
     * @param   string      $quote      quote character
     * @param   string      $case       strings are case sensitive
     * @return  void
     */
    function TNTCsv($data = null, $delim = null, $quote = null, $case = null) {
        $this->SetOpts($delim, $quote, $case);

        TNTFmtStr::TNTFmtStr($data);

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCsv() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
