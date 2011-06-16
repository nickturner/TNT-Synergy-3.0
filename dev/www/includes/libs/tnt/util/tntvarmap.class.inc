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
 * Map Collection Class
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * This class handles an unordered collection of data indexed by a key name.
 *
 * This is therefore a wrapper around the PHP associative array class.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to provides access to an associative array of variables.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTVarMap extends TNTObject {

    /**
     * Gets a value indicating whether the map is read only.
     * @var     boolean                 true if array is read only
     * @access  private
     */
    var $isReadOnly = false;

    /**
     * Gets a value indicating whether the map is fixed size.
     * @var     boolean                 true if array should not change is size
     * @access  private
     */
    var $isFixedSize = false;

    /**
     * The associative array of variables
     * @var     mixed[]                 array
     * @access  private
     */
    var $_vars = null;

    /**
     * Gets the number of variables stored in the map.
     *
     * @access  public
     * @return  integer                 number of variables stored
     */
    function Count() {
        return count($this->_vars);
    }

    /**
     * Gets a copy of the var map.
     *
     * @access  public
     * @return  mixed[]                 the internal map
     */
    function Items() {
        return $this->_vars;
    }

    /**
     * Gets a array of keys stored in the var map.
     *
     * @access  public
     * @return  string[]                array of keys
     */
    function Keys() {
        return array_keys($this->_vars);
    }

    /**
     * Gets a array of values stored in the var map.
     *
     * @access  public
     * @return  mixed[]                 array of values
     */
    function Values() {
        return array_values($this->_vars);
    }

    /**
     * Does a variable(s) exist
     *
     * If name is an array then it is assumed to be an array of variables
     * to check for and the return is only true if all exist.
     *
     * @access  public
     * @param   string      $name       name of variable
     * @return  bool                    true if variable exists
     */
    function HasVar($name) {

        if (is_array($name)) {
            $ret = true;
            foreach ($name as $n) {
               if (!isset($n)) {
                   $ret = false;
                   break;
               }
            }
        } else {
            $ret = (isset($this->_vars[$name]));
        }

        return $ret;
    }

    /**
     * Get the value of a variable(s).
     *
     * If name is an array then it is assumed to be an array of variables
     * to get and in which case the return value is an array of values
     * indexed by key.
     *
     * @access  public
     * @param   string      $name       name of variable (or array of names)
     * @param   mixed       $default    value to return if variable(s) missing
     * @return  mixed                   value of the variable(or null)
     */
    function GetVar($name, $default = null) {
    
        if (is_array($name)) {
            foreach ($name as $n) {
                $ret[$n] = (isset($this->_vars[$n]) ? $this->_vars[$n] : $default);
            }
        } else {
            $ret = (isset($this->_vars[$name]) ? $this->_vars[$name] : $default);
        }

        return $ret;
    }
    
    /**
     * Get the value of a variable (by reference)
     *
     * If name is an array then it is assumed to be an array of variables
     * to get and in which case the return value is an array of values
     * indexed by key.
     *
     * @access  public
     * @param   string      $name       name of variable (or array of names)
     * @return  mixed                   reference to the variable
     */
    function &GetVarByRef($name) {

        $ret = null;
    
        if (is_array($name)) {
            foreach ($name as $n) {
                if (isset($this->_vars[$n])) {
                    $ret[$n] =& $this->_vars[$n];
                } else {
                    $ret[$n] = null;
                }
            }
        } else {
            if (isset($this->_vars[$name])) {
                $ret =& $this->_vars[$name];
            } else {
                $ret = null;
            }
        }

        return $ret;
    }

    /**
     * Set the value of a variable
     *
     * If the map is read only then this fails. If variable doesn't exist and
     * the map is fixed size then this fails. Otherwise the named variable is
     * modified or added.
     *
     * If called with $name == null and $val is an associated array
     * then each key => value pair in $val is set in the map.
     *
     * Can be called with a single argument which is an associative array
     * of 'name' => 'val' pairs to set.
     *
     * @access  public
     * @param   string      $name       name of variable
     * @param   mixed       $val        value of variable
     * @return  boolean                 true if set successfull
     */
    function SetVar($name, $val) {

        $ret = true;

        // Start error section
        do {
            if ($this->isReadOnly) {
                $this->_SetError('variables are read only');
                $ret = false;
                break;
            }

            if (func_num_args() == 1) {
                if (!is_array($name)) {
                    $this->_SetError('variables are read only');
                    $ret = false;
                    break;
                }
                $v = $name;
            } else {
                $v = array($name => $val);
            }

            foreach ($v as $name => $val) {
                if (!isset($this->_vars[$name]) && $this->isFixedSize) {
                    $this->_SetError('variable does not exist and map is fixed size');
                    $ret = false;
                    break;
                }

                $this->_vars[$name] = $val;
            }

        } while (false);
        // End error section

        return $ret;
    }
    
    /**
     * Set the value of a variable (by reference)
     *
     * If the map is read only then this fails. If variable doesn't exist and
     * the map is fixed size then this fails. Otherwise the named variable is
     * modified or added.
     *
     * Can be called with a single argument which is an associative array
     * of 'name' => 'val' pairs to set.
     *
     * @access  public
     * @param   string      $name       name of variable
     * @param   mixed       $val        reference to variable
     * @return  boolean                 true if set successfull
     */
    function SetVarByRef($name, &$val) {

        $ret = true;

        // Start error section
        do {
            if ($this->isReadOnly) {
                $this->_SetError('variables are read only');
                $ret = false;
                break;
            }

            if (func_num_args() == 1) {
                if (!is_array($name)) {
                    $this->_SetError('variables are read only');
                    $ret = false;
                    break;
                }
                $v = $name;
            } else {
                $v = array($name => &$val);
            }

            foreach ($v as $name => $val) {
                if (!isset($this->_vars[$name]) && $this->isFixedSize) {
                    $this->_SetError('variable does not exist and map is fixed size');
                    $ret = false;
                    break;
                } 

                $this->_vars[$name] =& $v[$name];
            }

        } while (false);
        // End error section

        return $ret;
    }
    
    /**
     * Construct the object
     *
     * @access  public
     * @param   array       $vars       initial map value
     * @return  void
     */
    function TNTVarMap($vars = null) {

        TNTObject::TNTObject();

        $this->_vars = $vars;

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTVarMap() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
