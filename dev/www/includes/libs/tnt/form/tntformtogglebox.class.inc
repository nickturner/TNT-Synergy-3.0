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
 * Wui ToggleBox Class 
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic binary state selection box control on a Wui form.
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
 * The ToggleBox class allows you to add a boolean selection control to a Form.
 *
 * <pre>
 * <wui:togglebox
 *   id="string"
 *   name="string"
 *   autopostback="true|false"
 *   type="check|radio"
 *   label="string"
 *   labelalign="left|right"
 *   value="string"
 *   checked="true|false" />
 * </pre>
 *
 * Use the ToggleBox control to allow the user to either set or clear a given
 * value.
 *
 * The current state of the ToggleBox control can be specified by either the
 * 'checked' property and the value to return using the 'value' property.
 *
 * Note: This differs from other controls in that the 'value' property does not
 * solely control the value returned by the control. If the control is not
 * checked then no value is returned.
 *
 * If you plan to use multiple ToggleBox controls, the ToggleBoxList control is
 * an alternative control that provides convenient data binding capabilities.
 * However, individual ToggleBox controls provide greater control over layout.
 *
 * The behaviour of these boxes when grouped is determined by their 'type'
 * property which can be either 'check' or 'radio'. To group boxes give them
 * all the same value for the 'name' property. When 'check' boxes are grouped
 * then multiple boxes can be 'checked', however if 'radio' boxes are grouped
 * then only one can be 'checked' and checking a box will automatically clear
 * any other checked boxes in the same group.
 *
 * Normally you would use one of the classes derived from this class, such as
 * <wui:checkbox> and <wui:radiobox>.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiToggleBox extends TNTWuiCtrl {

    /**
     * Type of toggle control needed 
     * @access private
     * @var string                      HTML control type 
     */
    var $_type = null;

    /**
     * A value indicating whether the ToggleBox control is checked
     * @access private
     * @var boolean                     true to indicate a checked state
     */
    var $_checked = null;

    /**
     * Label next to the check box
     * @access private
     * @var string                      check box label (html)
     */
    var $_label = null;

    /**
     * Label alignment (left | right)
     * @access private
     * @var string                      check box label position
     */
    var $_labelAlign = null;

    /**
     * Check is the control is part of a group of controls.
     *
     * For a control to be part of a group then it must share its name with
     * other controls in the parents form 'element' collection.
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function IsGrouped() {

        $ret = false;

/*
        $form =& $this->Form();
        if ($form) {
            $group = $form->elements->FindItems($this->_name, '_name');
            if (is_array($group) && count($group) > 1) {
                $ret = true;
            }
        }
*/

        return $ret;
    }

    /**
     * Return the value of a html tag attribute.
     *
     * This base method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'name' - appends "[]" on to end of name for group controls
     *   + 'type' - translates type 'check' to HTML input type 'checkbox'
     *   + 'checked' - custom encoding (true => 'disabled', false => null)
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {

        switch (strtolower($name)) {
            case 'name':
                // Check to see if this control is in a group, ie other controls
                // have the same name.
                $ret = parent::TagAttribute($name);
                if ($this->IsGrouped()) {
                    $ret .= '[]';
                }
                break;
            case 'type':
                $type = $this->Get($name);
                $ret = ($type == 'check' ? 'checkbox' : $type);
                break;
            case 'checked':
                $ret = ($this->Get($name) ? 'checked' : null);
                break;
            default:
                $ret = parent::TagAttribute($name);
                break;
        }

        return $ret;
    }

    /**
     * Set the ToggleBox type
     *
     * The 'type' attribute gives the basic type of the ToggleBox as follows;
     *   + 'check' - a check box
     *   + 'radio' - a radio button
     *
     * The behaviour of these boxes when grouped is determined by their type. To
     * group boxes give them all the same value for the 'name' property. When
     * 'check' ToggleBoxes are grouped then multiple boxes can be 'checked',
     * however if 'radio' boxes are grouped then only one can be 'checked' and
     * clicking on one to check it will automatically clear any other checked
     * box in the same group.
     *
     * @access  public
     * @param   string      $type       string to set the type to
     * @return  boolean                 true if successful
     * @see     GetType();
     */ 
    function SetType($type) {

        static $types = array('check', 'radio');
 
        if (is_null($type)) {
            $this->_type = null;
            $ret = true;
        } else if (!is_string($type)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else if (!empty($type) && !in_array(strtolower($type), $types)) {
            $this->_SetError('value must be one of the following types: '.join(',', $types));
            $ret = false;
        } else {
            $this->_type = $type;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the ToggleBox type
     *
     * @access  public
     * @return  string                  current value of the type
     * @see     SetType();
     */ 
    function GetType() {
        return $this->_type;
    }

    /**
     * Set a value indicating whether the ToggleBox control is checked.
     *
     * Use this property to determine whether the ToggleBox control is checked.
     * This property can also be used to programmatically set the state of the
     * ToggleBox control.
     *
     * The value can be specified as either a boolean or one of the following
     * strings;
     *   'true', 'on', 'yes', '1', 'checked' => true
     *   'false', 'off', 'no', '0', 'unchecked' => false
     *
     * @access  public
     * @param   boolean     $checked   true to indicate a checked state
     * @return  boolean                 true if successful
     * @see     GetChecked();
     */ 
    function SetChecked($checked) {

        static $_checked = array('true', 'yes', '1', 'checked');
        static $_unchecked = array('false', 'no', '0', 'unchecked');

        if (is_null($checked)) {
            $this->_checked = null;
            $ret = true;
        } else {
            if (is_string($checked)) {
                if (in_array($checked, $_checked)) {
                    $checked = true;
                } else if (in_array($checked, $unchecked)) {
                    $checked = false;
                }
            }
            if (!is_boolean($checked)) {
                $this->_SetError('value must be a boolean');
                $ret = false;
            } else {
                $this->_checked = $checked;
                $ret = true;
            }
        }

        return $ret;
    }

    /**
     * Get a value indicating whether the ToggleBox control is checked.
     *
     * @access  public
     * @return  boolean                 current value of the property
     * @see     SetChecked();
     */ 
    function GetChecked() {
        return $this->_checked;
    }

    /**
     * Set the label that is displayed on the control.
     *
     * Use this property to specify the text label associated with the control.
     * This property can also be used to programmatically get the label
     * associated with the control.
     *
     * @access  public
     * @param   string      $label      string to set the label to
     * @return  boolean                 true if successful
     * @see     GetLabel();
     */ 
    function SetLabel($label) {

        if (is_null($label)) {
            $this->_label = null;
            $ret = true;
        } else if (!is_string($label)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else {
            $this->_label = $label;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the label that is displayed on the Button control.
     *
     * @access  public
     * @return  string                  current value of the label
     * @see     SetLabel();
     */ 
    function GetLabel() {
        return $this->_label;
    }

    /**
     * Sets the alignment of the text label associated with the control.
     *
     * Use this property to specify the alignment of the label associated with
     * the control. You can specify whether the label appears to the right or
     * left of the check box. Use the Label property to specify the label.
     *
     * @access  public
     * @return  string                  alignment (left | right)
     * @return  boolean                 true if successful
     * @see     GetLabelAlign();
     */ 
    function SetLabelAlign($labelAlign) {

        static $aligns = array('left', 'right');

        if (is_null($labelAlign)) {
            $this->_labelAlign = null;
            $ret = true;
        } else if (!is_string($labelAlign) || !in_array($labelAlign, $aligns)) {
            $this->_SetError('value must be either null, "right" or "left": '.$labelAlign);
            $ret = false;
        } else {
            $this->_labelAlign = $labelAlign;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Gets the alignment of the text label associated with the control.
     *
     * @access  public
     * @return  string                  alignment (left | right)
     * @see     SetLabelAlign();
     */ 
    function GetLabelAlign() {
        return $this->_labelAlign;
    }

    /**
     * Render the forms initial value as hidden controls.
     *
     * This is overridden from {@link TNTWuiCtrl::RenderInitialValue()} as
     * a toggle control only has its 'value' if it is checked. When not checked
     * it has no value.
     *
     * @access  public
     * @return  string                  HTML required to render hidden value
     * @see     RenderCurrentValue();
     */ 
    function RenderInitialValue() {
    
        $checked = $this->GetChecked();

        $initialValue = $this->_initialValue;

        if (!$checked) {
            $this->_initialValue = null;
        }

        $ret = parent::RenderInitialValue();

        $this->_initialValue = $initialValue;

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

        $align = $this->GetLabelAlign();

        if ($this->_hidden) {
            return TNTWuiCtrl::RenderHTML();
        } else {
            $label = $this->GetLabel();
            $align = $this->GetLabelAlign();
            if ($align != 'left' && $align != 'right') {
                $align = 'right';
            }

            $check = parent::Render();

            $attrs = array('for' => $this->TagAttribute('id'),
                           'accesskey' => $this->TagAttribute('accesskey'));
            $label = TNTHtml::BlockTag('label', $attrs, $label);

            if (empty($label)) {
                $ret = $check;
            } else {
                if ($align == 'left') {
                    $ret = $label.$check;
                } else {
                    $ret = $check.$label;
                }
            }
        }

        return $ret;
    }

    /**
     * Load form data
     *
     * This is overridden from the base class as the actual 'value' of a toggle
     * box does not change, only whether it is 'checked' or not is changable.
     *
     * The Form data can indicate that the control is 'checked' if it holds
     * a value equal to the controls 'value', or in the case of grouped controls
     * if it holds an array of values that contains the controls 'value'.
     *
     * @access  public
     * @param   mixed[]     $formData   collection of form data
     * @return  boolean                 true if the data was loaded
     */ 
    function LoadFormData(&$formData) {

        $ret = true;

        if ($this->_SelectDataValue($formData, $value)) {
            if (is_array($value)) {
                $checked = in_array($this->_value, $value);
            } else {
                $checked = ($this->_value == $value);
            }
            $ret = $this->SetChecked($checked);
        }

        return $ret;
    }

    /**
     * Save form data
     *
     * The form requests each of its child controls to save its current value
     * into given associative array of data.
     *
     * This is overridden from the base class as the actual 'value' of a toggle
     * box does not change, only whether it is 'checked' or not is changable.
     *
     * This method will save the 'value' of the control if it is checked,
     * otherwise it will save null. In the case of grouped controls then this
     * method will insert its value into an array of values if it is checked,
     * otherwise it will delete itself from an array of values. The array is
     * created if required, or deleted if empty.
     * 
     * @access  public
     * @param   mixed[]     $formData   collection of form data
     * @return  boolean                 true if the data was saved
     */ 
    function SaveFormData(&$formData) {

        $ret = true;

        $checked = $this->GetChecked();
        $value = $this->GetValue();

        if ($this->IsGrouped()) {
            if (!($ret =  $this->_SelectDataValue($formData, $values))) {
                $oldValue = null;
            }
            if (!is_array($values)) {
                $values = (array)$values;
                $pos = null;
            } else {
               $pos = array_search($value, $values);
               if ($pos === false) {
                   $pos = null;
               }
            }
            if ($checked && is_null($pos)) {
                $values[] = $value;
            } else if (!$checked && !is_null($pos)) {
                unset($values[$pos]);
            }
            if (count($values) == 0) {
                $values = null;
            }
            $ret =  $this->_UpdateDataValue($formData, $values);
        } else {
            $ret =  $this->_UpdateDataValue($formData, ($checked ? $value : null));
        }

        return $ret;
    }

    /**
     * Load post data
     *
     * This is overridden from the base class as the actual 'value' of a toggle
     * box does not change, only wether it is 'checked' or not is changable.
     *
     * The Post data can indicate that the control is 'checked' if it holds
     * a value equal to the controls 'value', or in the case of grouped controls
     * if it holds an array of values that contains the controls 'value'.
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

        $newChecked = null;

        // Value is only posted back if the ToggleBox was 'checked'
        if ($this->_SelectDataValue($postData, $value)) {
            if (is_array($value)) {
                $newChecked = in_array($this->_value, $value);
            } else {
                $newChecked = ($this->_value == $value);
            }
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
    function TNTWuiToggleBox() {

        TNTWuiCtrl::TNTWuiCtrl();

        $this->tagName = 'input';
        $this->tagComplete = true;

        array_push($this->tagAttrNames, 'type', 'checked');

    }

    /**
     * Destruct the control.
     *
     * This will destruct the control and any children.
     *
     * @access  private
     */
    function _TNTWuiToggleBox() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
