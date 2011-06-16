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
 * Wui NumberBox Class 
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic number editbox control on a Wui form.
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
TNTLoader::RequireClass('TNTWuiEditBox');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * The NumberBox class allows you to add a single line text edit box to a form
 * which only allows the entry of numerical values.
 *
 * <pre>
 * {tnt_ctrl ctrl="editbox"
 *   id="string"
 *   name="string"
 *   autopostback="true|false"
 *   cols="integer"
 *   minlength="integer"
 *   maxlength="integer"
 *   minNumber="integer"
 *   maxNumber="integer"
 *   value="text"
 * }
 * </pre>
 *
 * The NumberBox control is an input control that lets the user enter a single
 * line of numerical text. The width of the input field is set using the 'cols'
 * property.
 *
 * Use the 'value' property to determine the contents of the NumberBox control.
 * You can constrain the number of characters to be within an lower and upper
 * limit using the 'minlength' and 'maxlength' properties. You can also
 * constrain the entered number to be within an lower and upper limit using the
 * 'minnumber' and 'maxnumber' properties.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiNumberBox extends TNTWuiEditBox {

    /**
     * Minumum length of text to allow
     * @access private
     * @var string                      minimum number of characters
     */
    var $_minNumber = null;

    /**
     * Maximum length of text to allow
     * @access private
     * @var string                      maximum number of characters
     */
    var $_maxNumber = null;

    /**
     * Sets the minimum number allowed in the edit box.
     *
     * @access  public
     * @return  integer                 minimum number
     * @return  boolean                 true if successful
     * @see     GetMinNumber();
     */ 
    function SetMinNumber($minNumber) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('minNumber' =>& $minNumber));

        if (is_null($minNumber)) {
            $this->_minNumber = null;
            $ret = true;
        } else if (!is_numeric($minNumber)) {
            $this->_SetError('value must be either null or a number');
            $ret = false;
        } else {
            $this->_minNumber = $minNumber;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Gets the minimum number allowed in the edit box.
     *
     * @access  public
     * @return  integer                 minimum number
     * @see     SetMinNumber();
     */ 
    function GetMinNumber() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_minNumber;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Sets the maximum number allowed in the edit box.
     *
     * @access  public
     * @return  integer                 maximum number
     * @return  boolean                 true if successful
     * @see     GetMaxNumber();
     */ 
    function SetMaxNumber($maxNumber) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('maxNumber' =>& $maxNumber));

        if (is_null($maxNumber)) {
            $this->_maxNumber = null;
            $ret = true;
        } else if (!is_numeric($maxNumber)) {
            $this->_SetError('value must be either null or a number');
            $ret = false;
        } else {
            $this->_maxNumber = $maxNumber;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Gets the maximum number allowed in the edit box.
     *
     * @access  public
     * @return  integer                 maximum number
     * @see     SetMaxNumber();
     */ 
    function GetMaxNumber() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_maxNumber;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Construct the control
     *
     * This will construct control, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiNumberBox() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        TNTWuiEditBox::TNTWuiEditBox();

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }

    /**
     * Destruct the control.
     *
     * This will destruct the control and any children.
     *
     * @access  private
     */
    function _TNTWuiNumberBox() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);


        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
