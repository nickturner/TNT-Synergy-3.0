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
 * CSS Style Class
 * ~~~~~~~~~~~~~~~
 *
 * This class manipulates CSS style string values.
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
 * Class to manage a CSS style string.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTCssStyle extends TNTFmtStr {

    /**
     * Return the number of items found in the CSS string
     *
     * @access  public
     * @return  integer                 number of items in the CSS string
     */
    function Count() {
        if (!$this->_elements) {
            $this->_Explode();
        }
        return count($this->_elements);
    }

    /**
     * Has the CSS got the given style
     *
     * @access  public
     * @param   string      $name       name of item to check for
     * @return  boolean                 true if it exists
     */
    function HasItem($name) {
        if (!$this->_elements) {
            $this->_Explode();
        }
        return isset($this->_elements[strtolower($name)]);
    }

    /**
     * Get the value of a single item in the CSS string
     *
     * @access  public
     * @param   string      $name       name of item to return
     * @return  string                  value of the item (or null)
     */
    function GetItem($name) {

        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        $name = strtolower($name);

        if (isset($this->_elements[$name])) {
            $ret = $this->_elements[$name];
        }

        return $ret;
    }

    /**
     * Set the value of a single item in the CSS string
     *
     * Set the value of an item to null to remove it from the CSS string.
     *
     * @access  public
     * @param   string      $name       name of item to set
     * @param   string      $value      value to set item to
     * @return  boolean                 true if successfull
     */
    function SetItem($name, $value) {

        $ret = false;

        // Start error section
        do {

            if (!is_string($name) || !preg_match('/^[\w_-]*$/', $name)) {
                $this->_SetError('value must be a valid CSS style name: '.$name);
                break;
            }
            if (!is_null($value) && !is_scalar($value)) {
                $this->_SetError('value must be a valid scalar value');
                break;
            }

            if (!$this->_elements) {
                $this->_Explode();
            }

            $name = strtolower($name);

            if (is_null($value)) {
                if (isset($this->_elements[$name])) {
                    unset($this->_elements[$name]);
                    $this->_string = null;
                }
            } else {
                $this->_elements[$name] = (string)$value;
                $this->_string = null;
            }

            $ret = true;
        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Return an associative array of strings consisting of items in the CSS
     * string.
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
     * Set the CSS string from an array of strings
     *
     * @access  public
     * @param   string[]    $items      items to convert to CSV string
     * @return  boolean                 true if successfull
     */
    function SetItems($items) {

        $ret = true;

        if (is_null($items)) {
            $this->_elements = null;
            $this->_string = null;
        } else if (is_array($items)) {
            foreach ($items as $name => $value) {
                if (!is_string($name) || !preg_match('/^[\w_-]*$/', $name)) {
                    $this->_SetError('value must be a valid CSS style name');
                    $ret = false;
                    break;
                }
                if (!is_null($value) && !is_scalar($value)) {
                    $this->_SetError('value must be a valid scalar value');
                    $ret = false;
                    break;
                }

                $this->_elements[strtolower($name)] = $value;
            }
            $this->_string = null;
        } else {
            $this->_SetError('value must be either an array or null');
            $ret = false;
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

        $ret = true;

        $this->_elements = null;

        if (is_string($this->_string)) {
            // Parse the given style string
            $codes = explode(';', $this->_string);
            if (is_array($codes) && ($max_c = count($codes)) > 0) {
                for ($c = 0; $c < $max_c; $c++) {
                    $code = trim($codes[$c]);
                    if (empty($code)) {
                        continue;
                    }
                    if (!preg_match('/^\s*([\w_-]+)\s*:\s*(.+)\s*$/', $code, $matches)) {
                        $this->_SetError("invalid style '$code'");
                        $ret = false;
                        break;
                    }
                    $this->_elements[strtolower($matches[1])] = $matches[2];
                }
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

        $this->_string = '';
        foreach ((array)$this->_elements as $name => $value) {
            if (!is_null($value)) {
                $this->_string .= "{$name}:{$value};";
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
    function TNTCssStyle($data = null) {
        TNTFmtStr::TNTFmtStr($data);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCssStyle() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
