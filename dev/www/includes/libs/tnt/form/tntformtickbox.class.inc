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
 * Wui TickBox Class 
 * ~~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic check box control representing true or false on a Wui
 * form.
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
 * The TickBox class allows you to add a boolean selection control to a Form.
 *
 * <pre>
 * <wui:tickbox id="string"
 *   autopostback="true|false"
 *   label="string"
 *   labelalign="left|right"
 *   value="true|false"
 *   onchange="php: code" />
 * </pre>
 *
 * Use the TickBox control to allow the user to select either a true of false
 * value.
 *
 * The current state of the TickBox control can be specified by either the
 * 'value' property.
 *
 * Unlike the base ToggleBox control, TickBox controls can not be grouped.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiTickBox extends TNTWuiToggleBox {

    /**
     * Return the value of a html tag attribute.
     *
     * This base method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'name' - prevent control grouping
     *   + 'type' - fixed to 'checkbox'
     *   + 'value' - fixed to '1'
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {

        switch (strtolower($name)) {
            case 'type':
                $ret = 'checkbox';
                break;
            case 'value':
                $ret = '1';
                break;
            default:
                $ret = parent::TagAttribute($name);
                break;
        }

        return $ret;
    }

    /**
     * Set the TickBox type
     *
     * Overridden to prevent the type from being altered.
     *
     * @access  public
     * @param   string      $type       string to set the type to
     * @return  boolean                 true if successful
     * @see     GetType();
     */ 
    function SetType($type) {
        $this->_SetError('Type can not be altered for a TickBox');
        return false;
    }

    /**
     * Get the TickBox type
     *
     * @access  public
     * @return  string                  current value of the type
     * @see     SetType();
     */ 
    function GetType() {
        return parent::GetType();
    }

    /**
     * Set the value
     *
     * This member specifies whether the control is 'checked' or not. A zero
     * (or null) value means unchecked, otherwise it will be checked.
     *
     * This calls {@link SetChecked()}.
     *
     * @access  public
     * @param   mixed       $value      string to set the value to
     * @return  boolean                 true if successful
     * @see     GetValue();
     */ 
    function SetValue($value) {

        if (is_null($value)) {
            $value = 0;
        } else if (is_numeric($value)) {
            $value = ($value ? 1 : 0);
        }
        $ret = $this->SetChecked($value);

        return $ret;
    }

    /**
     * Get the value
     *
     * This calls {@link GetChecked()}.
     *
     * If there is no value set then false is returned.
     *
     *
     * @access  public
     * @return  boolean                 the controls current value
     * @see     SetValue();
     */ 
    function GetValue() {
        return ($this->GetChecked() ? true : false);
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
        return parent::Render();
    }

    /**
     * Load post data
     *
     * This is overridden from the base class as the actual 'value' of a toggle
     * box does not change, only wether it is 'checked' or not is changable.
     *
     * The Post data can indicate that the control is 'checked' if it holds
     * a value for the control.
     *
     * The Page tracks all the control's that return true to this method call,
     * then invokes the {@link RaisePostDataChangedEvent} on those controls.
     *
     * This does not validate the posted value. The controls value must be set
     * to the given value even if it would result in an invalid value.
     *
     * @access  public
     * @param   string[]    $postData   collection of incoming post data
     * @return  boolean                 true if the controls value changed
     */ 
    function LoadPostData(&$postData) {

        $ret = false;

        $newChecked = 0;

        // Value is only posted back if the TickBox was 'checked'
        if ($this->_SelectDataValue($postData, $value)) {
            $newChecked = 1;
        }

        $oldChecked = $this->_checked;
        if ($oldChecked != $newChecked) {
            $this->_checked = $newChecked;
            $ret = true;
        }

        $this->_changed = $ret;

        return $ret;
    }

    /**
     * Construct the control
     *
     * This will construct control, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiTickBox() {

        TNTWuiToggleBox::TNTWuiToggleBox();

    }

    /**
     * Destruct the control.
     *
     * This will destruct the control and any children.
     *
     * @access  private
     */
    function _TNTWuiTickBox() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
