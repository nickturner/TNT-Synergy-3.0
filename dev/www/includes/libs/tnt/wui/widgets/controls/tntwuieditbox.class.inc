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
 * Wui EditBox Class 
 * ~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic edit box control on a Wui form.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiCtrl');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * The EditBox class allows you to add a single line text editing box to a form.
 *
 * <pre>
 * {tntwuieditbox
 *   id="string"
 *   name="string"
 *   autopostback="true|false"
 *   cols="integer"
 *   minlength="integer"
 *   maxlength="integer"
 *   value="string"
 * }
 * </pre>
 *
 * The EditBox control is an input control that lets the user enter a single
 * line of text. The number of columns visible is 'cols' property.
 *
 * Use the 'value' property to determine the contents of the EditBox control.
 * You can constrain the number of characters to be within an lower and upper
 * limit using the 'minlength' and 'maxlength' properties.
 *
 * Any values given to the CSS style 'width' will override the corresponding
 * 'cols' property.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiEditBox extends TNTWuiCtrl {

    /**
     * Number of cols to display
     * @access private
     * @var integer                     num of cols
     */
    var $_cols = null;

    /**
     * Minumum length of text to allow
     * @access private
     * @var integer                     minimum number of characters
     */
    var $_minLength = null;

    /**
     * Maximum length of text to allow
     * @access private
     * @var integer                     maximum number of characters
     */
    var $_maxLength = null;

    /**
     * Set the display width of the box in characters.
     *
     * The display width, in characters, of the text box.
     *
     * @access  public
     * @param   integer     $cols       number of character columns
     * @return  boolean                 true if successful
     * @see     GetCols();
     */ 
    function SetCols($cols) {

        $ret = false;
        if (is_null($cols)) {
            $this->_cols = null;
            $ret = true;
        } else if (!is_numeric($cols)) {
            $this->_SetError('value must be either null or a number');
            $ret = false;
        } else {
            $this->_cols = $cols;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the display width of the box in characters.
     *
     * @access  public
     * @return  integer                 current value of the cols
     * @see     SetCols();
     */ 
    function GetCols() {

        $ret = $this->_cols;

        return $ret;
    }

    /**
     * Sets the minimum number of characters allowed in the text box.
     *
     * @access  public
     * @return  integer                 number of characters
     * @return  boolean                 true if successful
     * @see     GetMinLength();
     */ 
    function SetMinLength($minLength) {

        $ret = false;
        if (is_null($minLength)) {
            $this->_minLength = null;
            $ret = true;
        } else if (!is_numeric($minLength)) {
            $this->_SetError('value must be either null or a number');
            $ret = false;
        } else {
            $this->_minLength = $minLength;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Gets the minimum number of characters allowed in the text box.
     *
     * @access  public
     * @return  integer                 number of characters
     * @see     SetMinLength();
     */ 
    function GetMinLength() {

        $ret = $this->_minLength;

        return $ret;
    }

    /**
     * Sets the maximum number of characters allowed in the text box.
     *
     * @access  public
     * @return  integer                 number of characters
     * @return  boolean                 true if successful
     * @see     GetMaxLength();
     */ 
    function SetMaxLength($maxLength) {

        $ret = false;
        if (is_null($maxLength)) {
            $this->_maxLength = null;
            $ret = true;
        } else if (!is_numeric($maxLength)) {
            $this->_SetError('value must be either null or a number');
            $ret = false;
        } else {
            $this->_maxLength = $maxLength;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Gets the maximum number of characters allowed in the text box.
     *
     * @access  public
     * @return  integer                 number of characters
     * @see     SetMaxLength();
     */ 
    function GetMaxLength() {

        $ret = $this->_maxLength;

        return $ret;
    }

    /**
     * Return the value of a html tag attribute.
     *
     * This method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'type' - returns 'submit'
     *   + 'size' - returns value of 'cols' property
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {

        $ret = null;

        switch (strtolower($name)) {
            case 'type':
                $ret = 'text';
                break;
            case 'size':
                $ret = $this->Get('cols');
                break;
            default:
                $ret = parent::TagAttribute($name);
                break;
        }
  
        return $ret;
    }

    /**
     * Render the control
     *
     * This renders the control and any associated validation script and any
     * child controls.
     *
     * @access  public
     * @return  string                  HTML required to render the control
     * @see     Render();
     */ 
    function Render() {

        $ret = parent::Render();

        return $ret;
    }

    /**
     * Construct the control
     *
     * This will construct control, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiEditBox() {

        TNTWuiCtrl::TNTWuiCtrl();

        $this->tagName = 'input';
        $this->tagComplete = true;

        array_push($this->tagAttrNames, 'type', 'size', 'maxlength');

    }

    /**
     * Destruct the control.
     *
     * This will destruct the control and any children.
     *
     * @access  private
     */
    function _TNTWuiEditBox() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
