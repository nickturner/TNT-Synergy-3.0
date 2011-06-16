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
 * Wui List Class 
 * ~~~~~~~~~~~~~~~~
 *
 * Abstract base class for list controls on Wui forms.
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
 * The ListCtrl class serves as the abstract base class that defines the
 * properties, methods, and events common for all list-type controls.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiListCtrl extends TNTWuiCtrl {

    /**
     * A value indicating that the list allows multiple items to be selected.
     * @access private
     * @var boolean                     true if a multi selection list
     */
    var $_multiple = null;

    /**
     * How are selected indexes held in the value
     * @access private
     * @var string                      separator or reserved keyword
     */
    var $_valsep = null;

    /**
     * An array of selected values
     * @access private
     * @var array                       array of the 'value' of selected items
     */
    var $_values = null;

    /**
     * An array of list option structures
     *
     * @access private
     * @var array                       array of the list items
     */
    var $_items = null;

    /**
     * Synchronizes the private 'Values' property from the current value of
     * the 'Value' property.
     *
     * If multiple selection is allowed and the 'ValSep' property is set then
     * the 'Value' property is set to the correct concatenated value and
     * the 'Values' property is set to the unconcatenated array of values.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function _SyncValues() {

        $ret = true;

        $multiple = $this->GetMultiple();
        $valsep = $this->GetValSep();
        $value = $this->GetValue();
        $binary = (strcasecmp($valsep, 'binary') == 0);

        $this->_values = $value;

        if ($multiple && !empty($valsep)) {
            if (is_string($value)) {
                $this->_value = $value;
                if ($binary) {
                    if (empty($values)) {
                        $this->_values = 0;
                    }
                    if (!is_numeric($values)) {
                        $this->_SetError('Value must be numeric for binary list boxes');
                        $ret = false;
                    }
                    $this->_values = (integer)$values;
                } else if ($values != '') {
                    $this->_values = explode($valsep, $values);
                } else {
                    $this->_values = array();
                }
            } else if (is_array($value)) {
                $this->_values = $value;
                if ($binary) {
                    $m = count($this->_values);
                    $v = 0;
                    for ($i = 0; $i < $m; $i++) {
                        if (empty($this->_values[$i])) {
                            $this->_values[$i] = 0;
                        }
                        if (!is_numeric($this->_values[$i])) {
                            $this->_SetError('Value must be numeric for binary list boxes');
                            $ret = false;
                            break;
                        }
                        $v += $this->_values[$i];
                    }
                    $this->_value = $v;
                } else if (count($this->_values)) {
                    $this->_value = implode($valsep, $this->_values);
                } else {
                    $this->_value = null;
                }
            }
        } else {
            if (is_array($value)) {
                $this->_SetError('Only single values allowed for non multiple selection or binary selection lists');
                $ret = false;

                // Reset value back to a suitable one
                reset($value);
                $this->_value = current($value);
                $this->_values = $this->_value;
            }
        }

        // Check that it is ok to have multiple values
        if ($ret && is_array($this->_values) && !$multiple) {
            $this->_SetError('Only single values allowed for non multiple selection or binary selection lists');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Return the value of a html tag attribute.
     *
     * This base method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {

        switch (strtolower($name)) {
            default:
                $ret = parent::TagAttribute($name);
                break;
        }

        return $ret;
    }

    /**
     * Set the value
     *
     * This member specifies the initial value of the control. The intial
     * value of a list is the value of the selected item or an array of
     * the values of the selected items if multiple selection is allowed.
     *
     * @access  public
     * @param   string      $value      string to set the value to
     * @return  boolean                 true if successful
     * @see     GetValue();
     */ 
    function SetValue($value) {
        if (is_null($value)) {
            $this->_value = null;
            $ret = true;
        } else {
            $this->_value = $value;
            $ret = $this->_SyncValues();
        }

        return $ret;
    }

    /**
     * Get the value
     *
     * If there is no value set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current value
     * @see     SetValue();
     */ 
    function GetValue() {

        $ret = $this->_value;

        return $ret;
    }

    /**
     * Set the value separation string.
     *
     * If the controls value is set to a string then this separation string is
     * used to split the value into an array of values. Each value in the array
     * then represents the 'Value' of an item to show as selected.
     *
     * The special value of 'binary' can be specified, in this case then the
     * value is assumed to be a binary number and each item is assumed to have
     * a binary 'Value'. an item is then selected if a bit comparison between
     * the two values matches.
     *
     * @access  public
     * @param   string      $valsep     string to set the valsep to
     * @return  boolean                 true if successful
     * @see     GetValSep();
     */ 
    function SetValSep($valsep) {

        if (is_null($valsep)) {
            $this->_valsep = null;
            $ret = true;
        } else {
            $this->_valsep = $valsep;
            $ret = $this->_SyncValues();
        }

        return $ret;
    }

    /**
     * Get the value separation string.
     *
     * If there is no valsep set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current valsep
     * @see     SetValSep();
     */ 
    function GetValSep() {

        $ret = $this->_valsep;

        return $ret;
    }

    /**
     * Set a flag indicating that multiple selections are allowed.
     *
     * If multiple selections are allowed then the controls value can be
     * an array of selected values, or a string of selected values separated
     * by the 'ValSep' property.
     *
     * @access  public
     * @param   string      $multiple   value to set the multiple to
     * @return  boolean                 true if successful
     * @see     GetMultiple();
     */ 
    function SetMultiple($multiple) {

        if (is_null($multiple)) {
            $this->_multiple = null;
            $ret = true;
        } else {
            $this->_multiple = strtoboolean($multiple);
            $ret = $this->_SyncValues();
        }

        return $ret;
    }

    /**
     * Get a flag indicating that multiple selections are allowed.
     *
     * If there is no multiple set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current multiple
     * @see     SetMultiple();
     */ 
    function GetMultiple() {

        $ret = $this->_multiple;

        return $ret;
    }

    /**
     * Checks to see if a value is selected.
     *
     * This method checks the 'value' property to see if the given value is
     * selected.
     *
     * @access  public
     * @param   mixed       $value      value of list item to check
     * @return  boolean                 true if the item is selected
     */
    function IsValueSelected($itemValue) {

        //$multiple = $this->GetMultiple();
        $valsep = $this->GetValSep();
        $value = $this->GetValue();
        $binary = (strcasecmp($valsep, 'binary') == 0);
        
        if ($binary) {
            // Quicker to check combined value than search the array
            $ret = (($value & $itemValue) ? true : false);
        } else {
            $ret = (($this->_values == $itemValue) || (is_array($this->_values) && in_array($itemValue, $this->_values)));

        }

        return $ret;
    }

    /**
     * Set the items
     *
     * This member specifies an array of items to be displayed by the list
     * control.
     *
     * Each item is an associative array containing the following optional
     * members;
     *   + value    - string: the value to submit when the option is selected
     *   + label    - string: the HTML to display for this option
     *   + group    - boolean: the option for grouping only
     *   + items    - array: child items for heirachical lists
     *   + id       - string: a HTML element id
     *   + class    - string: a css class for the option
     *   + style    - string: a css style for the option
     *   + tooltip  - string: a tooltip to display for this option
     *
     * If no 'label' is specified then one is made up by the control.
     *
     * If no 'value' is given then the 'label' is used. 
     *
     * The 'group' member can be used to indicate option grouping, different
     * list widgets will display groups in different ways.
     *
     *
     * @access  public
     * @param   mixed       $value      value to set the items to
     * @return  boolean                 true if successful
     * @see     GetItems();
     */ 
    function SetItems($value) {

        if (is_null($value)) {
            $this->_items = null;
            $ret = true;
        } else if (!is_array($value)) {
            $this->_SetError('value must be either null or an array');
            $ret = false;
        } else {
            $this->_items = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the items
     *
     * If there is no items set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current value
     * @see     SetItems();
     */ 
    function GetItems() {

        $ret = $this->_items;

        return $ret;
    }

    /**
     * Load post data
     *
     * Overridden to synchronize the values when initialised.
     *
     * @access  public
     * @param   string[]    $postData   collection of incoming post data
     * @return  boolean                 true if the controls value changed
     */ 
    function LoadPostData(&$postData) {

        $ret = parent::LoadPostData($postData);

        $this->_SyncValues();

        return $ret;
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiListCtrl() {

        TNTWuiCtrl::TNTWuiCtrl();

        $this->tagName = null;

    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiListCtrl() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
