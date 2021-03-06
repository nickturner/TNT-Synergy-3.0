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
 * Form Widget Class 
 * ~~~~~~~~~~~~~~~~~
 *
 * Represents a simple HTML FORM element.
 *
 * @package TNT.Wui
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiHtml');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a simple HTML FORM element.
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiForm extends TNTWuiHtml {

    /**
     * Name of the form - name used to post back data
     * @access private
     * @var string                      forms name
     */
    var $_name = null;

    /**
     * Values to assign to child controls
     * @access private
     * @var array                       array of values index by name
     */
    var $_values = null;

    /**
     * Validation errors to show against child controls
     * @access private
     * @var array                       array of error messages index by name
     */
    var $_errors = null;

    /**
     * Set the forms name
     *
     * The name is user to identify the form in the posted data.
     *
     * If there is no name then no value is posted.
     *
     * @access  public
     * @param   string      $name       string to set the name to
     * @return  boolean                 true if successful
     * @see     GetName();
     */ 
    function SetName($name) {

        if (is_null($name)) {
            $this->_name = null;
            $ret = true;
        } else if (!is_string($name)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else if (!preg_match('/^[_a-z][a-z0-9_=&\'{}[\]]*$/i', $name)) {
            $this->_SetError('form name is invalid ('.$name.')');
            $ret = false;
        } else {
            $this->_name = $name;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the controls name
     *
     * If there is no name then no value is posted.
     *
     * @access  public
     * @return  string                  current value of the name
     * @see     SetName();
     */ 
    function GetName() {
        return $this->_name;
    }

    /**
     * Set the values for the controls.
     *
     * @access  public
     * @param   string      $name       name of control to set (null for all)
     * @param   array       $values     the hash of values to set
     * @return  boolean                 true if successful
     * @see     GetValue();
     */
    function SetValue($name, $value) {
        if ($name) {
            $v = null;
            // The name can contain [] to indicate its an array reference
            @eval('$v =& $this->_values[\''.join('\'][\'', explode('[', str_replace(']', '', $name))).'\'];');
            $v = $value;
            $ret = true;
        } else {
            if (!is_null($value) && !is_array($value)) {
                $this->_SetError('value must be a hash');
                $ret = false;
            } else {
                $this->_values = $value;
                $ret = true;
            }
        }
        return $ret;
    }
  
    /**
     * Get the value for a named control.
     *
     * Returns a reference to the value or a reference to null
     *
     * @access  public
     * @param   string      $name       name of control to get (null for all)
     * @return  mixed                   the value or values
     * @see     SetValue();
     */
    function GetValue($name = null) {
  
        $values = $this->values;
  
        if ($this->Posted()) {
            // Overwrite with posted values
            $values = array_merge((array)$values, $_POST); 
        }

        if (!$values) {
            return null;
        }

        if ($name) {
            // The name can contain [] to indicate its an array reference
            return @eval('return $values[\''.join('\'][\'', explode('[', str_replace(']', '', $name))).'\'];');
        } else {
            return $values;
        }
    }
  
    /**
     * Set the errors that have occurred in validating the form.
     *
     * @access  public
     * @param   string      $name       name of control to get (null for all)
     * @param   array       $errors     hash of errors
     * @return  boolean                 true if successful
     * @see     GetError();
     */
    function SetError($name, $error) {
        if ($name) {
            $v = null;
            // The name can contain [] to indicate its an array reference
            @eval('$v =& $this->errors[\''.join('\'][\'', explode('[', str_replace(']', '', $name))).'\'];');
            $v = $error;
            $ret =  true;
        } else {
            if (!is_null($error) && !is_array($error)) {
                $this->_SetError('value must be a hash');
                $ret =  false;
            } else {
                $this->errors = $error;
                $ret =  true;
            }
        }
    }

    /**
     * Get the validation error for a named control.
     *
     * @access  public
     * @param   string      $name       name of control to get (null for all)
     * @return  mixed                   the value or values
     * @see     SetError();
     */
    function GetError($name = null) {
  
        $errors = $this->errors;
  
        if ($name) {
            // The name can contain [] to indicate its an array reference
            return @eval('return $errors[\''.join('\'][\'', explode('[', str_replace(']', '', $name))).'\'];');
        } else {
            return $errors;
        }
    }

    /**
     * Alias to {@link SetError()} when a forms controls are invalid
     *
     * @access  public
     * @param   array       $errors     hash of errors
     * @see     SetError(), GetError(), IsInvalid();
     */
    function Invalid($errors) {
        $this->SetError(null, $errors);
    }

    /**
     * Return true if any of the forms controls are invalid
     *
     * @access  public
     * @param   array       $errors     hash of errors
     * @return  void
     * @see     SetError(), GetError(), Invalid();
     */
    function IsInvalid($errors) {
        return $this->_errors != null;
    }

    /**
     * Construct the element
     *
     * This will construct element, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiForm() {

        TNTWuiHtml::TNTWuiHtml();

        $this->_tagName = 'form';

    }

    /**
     * Destruct the element.
     *
     * This will destruct the element and any children.
     *
     * @access  private
     */
    function _TNTWuiForm() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
