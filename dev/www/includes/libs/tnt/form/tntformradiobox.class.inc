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
 * Wui RadioBox Class 
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic radio box control on a Wui form.
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
TNTLoader::RequireClass('TNTWuiToggleBox');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * The RadioBox class is a ToggleBox with the type preset to 'radio'.
 *
 * <pre>
 * {tnt_ctrl ctrl="radiobox"
 *   id="string"
 *   name="string"
 *   autopostback="true|false"
 *   value="string"
 *   checked="true|false"
 * }
 * </pre>
 *
 * Use the RadioBox control to allow the user to toggle between a 'checked'
 * and 'unchecked' state.
 *
 * The 'value' property allows you to specify the value the RadioBox should
 * return when posted in its 'checked' state. When not 'checked' then no
 * value is posted back.
 *
 * Multiple RadioBox controls can be grouped by giving them all the same value
 * for the 'name' property. When RadioBox controls are grouped then only one of
 * the controls can be 'checked' at any time, and checking a control will
 * uncheck all the others in the same group. If you plan to use multiple
 * RadioBox controls, the RadioBoxList control is an alternative control that
 * provides convenient data binding capabilities. However, individual RadioBox
 * controls provide greater control over layout.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiRadioBox extends TNTWuiToggleBox {

    /**
     * Construct the control
     *
     * This will construct control, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiRadioBox() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        TNTWuiToggleBox::TNTWuiToggleBox();

        $this->_type = 'radio';

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }

    /**
     * Destruct the control.
     *
     * This will destruct the control and any children.
     *
     * @access  private
     */
    function _TNTWuiRadioBox() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
