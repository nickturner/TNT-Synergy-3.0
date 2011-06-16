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
 * Wui PasswordBox Class 
 * ~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents a password edit box control on a Wui form.
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
 * The PasswordBox class allows you to add a single line password editing box to a
 * form.
 *
 * <pre>
 * {tnt_ctrl ctrl="passwordbox"
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
 * The PasswordBox control is an input control that lets the user enter a single
 * line of text. The width of the input field is set using the 'cols' property.
 *
 * Use the 'value' property to determine the contents of the PasswordBox control.
 * You can constrain the number of characters to be within an lower and upper
 * limit using the 'minlength' and 'maxlength' properties.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiPasswordBox extends TNTWuiEditBox {

    /**
     * Return the value of a html tag attribute.
     *
     * This method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'type' - returns 'password'
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('name' =>& $name));

        $ret = null;

        switch (strtolower($name)) {
            case 'type':
                $ret = 'password';
                break;
            default:
                $ret = parent::TagAttribute($name);
                break;
        }
  
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
    function TNTWuiPasswordBox() {
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
    function _TNTWuiPasswordBox() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
