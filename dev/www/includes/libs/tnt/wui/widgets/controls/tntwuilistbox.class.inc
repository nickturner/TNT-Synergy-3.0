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
// item) any later version.                                                //
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
 * Wui ListBox Class 
 * ~~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic list box control on a Wui form.
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
TNTLoader::RequireClass('TNTWuiListCtrl');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * The ListBox class allows you to add a single-selection or multi-selection
 * list box to a Form.
 * 
 * <pre>
 * <wui:editbox
 *   id="string"
 *   name="string"
 *   autopostback="true|false"
 *   multiple="true|false"
 *   indent="string"
 *   rows="number"
 *   value="string"
 *   valsep="string"
 *   items=<array>
 *   />
 * </pre>
 *
 * Use the ListBox control to create a list control that allows single or
 * multiple item selection. Use the 'rows' property to specify the number of
 * items to display in the list, additional items are seen by scrolling the
 * list. To enable multiple item selection, set the 'Multiple' property to
 * true.
 *
 * The items to show as selected can be specified using the 'Value' property of
 * the ListBox control. This property can be specified as the value of the
 * item to selected. In the case of multi-selection ListBox controls the value
 * can be an array containing the values of any items selected, or a string
 * which will be split using the 'valsep' property to create the array. If the
 * 'valsep' property is given as 'binary' the the value is assumed to be a
 * binary number and the value of each item should also be a binary number. An
 * item is then selected if it matches the ListBox controls 'value' using binary
 * bit comparison.
 *
 * After a Post back the 'value' property will relect the selection state of
 * the client ListBox. The method {@link IsValueSelected()} can be used to test
 * if a given value has been selected.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiListBox extends TNTWuiListCtrl {

    /**
     * The number of rows displayed in the ListBox control..
     * @access private
     * @var integer                     number of rows to display
     */
    var $_rows = null;

    /**
     * The string to use to indent hierachical lists, this string is repeated
     * according to the depth of the list item.
     * @access private
     * @var string                      string to use to indent items
     */
    var $_indent = null;

    /**
     * Set the options
     *
     * This is an alias for {@link TNTWuiListCtrl::SetItems()} to closer follow
     * the HTML notation of calling the items in a list box - options.
     *
     * @access  public
     * @param   mixed       $value      value to set the items to
     * @return  boolean                 true if successful
     * @see     GetOptions();
     */ 
    function SetOptions($value) {
        return parent::SetItems($value);
    }

    /**
     * Get the options
     *
     * This is an alias for {@link TNTWuiListCtrl::GetItems()} to closer follow
     * the HTML notation of calling the items in a list box - options.
     *
     * If there is no items set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current value
     * @see     SetOptions();
     */ 
    function GetOptions() {
        return parent::GetOptions();
    }

    /**
     * Return the value of a html tag attribute.
     *
     * This base method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'readonly' - custom encoding (true => 'disabled', false => null)
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
     * Set the number of rows displayed in the ListBox control.
     *
     * @access  public
     * @param   string      $rows       string to set the rows to
     * @return  boolean                 true if successful
     * @see     GetRows();
     */ 
    function SetRows($rows) {

        $ret = false;
        if (is_null($rows)) {
            $this->_rows = null;
            $ret = true;
        } else {
            $this->_rows = $rows;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the number of rows displayed in the ListBox control.
     *
     * If there is no rows set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current rows
     * @see     SetRows();
     */ 
    function GetRows() {
        return $this->_rows;
    }

    /**
     * Set the number of rows displayed in the ListBox control.
     *
     * This is an alias for {@link SetRows()}
     *
     * @access  public
     * @param   string      $rows       string to set the rows to
     * @return  boolean                 true if successful
     * @see     GetSize();
     */ 
    function SetSize($rows) {
        return $this->SetRows($rows);
    }

    /**
     * Get the number of rows displayed in the ListBox control.
     *
     * If there is no rows set then null is returned.
     *
     * This is an alias for {@link GetRows()}
     *
     * @access  public
     * @return  string                  the controls current rows
     * @see     SetSize();
     */ 
    function GetSize() {
        return $this->GetRows();
    }

    /**
     * Set the indentation string.
     *
     * @access  public
     * @param   string      $indent     string to set the indent to
     * @return  boolean                 true if successful
     * @see     GetIndent();
     */ 
    function SetIndent($indent) {

        $ret = false;
        if (is_null($indent)) {
            $this->_indent = null;
            $ret = true;
        } else {
            $this->_indent = $indent;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the indentation string.
     *
     * If there is no indent string then null is returned.
     *
     * @access  public
     * @return  string                  the controls current indent string
     * @see     SetIndent();
     */ 
    function GetIndent() {
        return $this->_indent;
    }

    /**
     * _RenderItem
     *
     * Renders a single list item.
     *
     * A item is rendered as an HTML <item> element.
     *
     * Although HTML has no concept of nested items we support this by
     * rendering each level indented from the previous using spaces.
     *
     * @access  protected
     * @param   array           $item item to render
     * @param   integer         $depth  depth of traversal
     * @return  void
     */
    function _RenderItem($item, $depth) {

        $label = $item['label'];
        $value = (isset($item['value']) ? $item['value'] : $label);

        $indent = str_repeat(($this->_indent ? $this->_indent : '&nbsp;&nbsp;'), $depth);

        $attrs['class'] = (isset($item['class']) ? $item['class'] : null);
        $attrs['style'] = (isset($item['style']) ? $item['style'] : null);
        $attrs['title'] = (isset($item['title']) ? $item['title'] : null);

        $attrs['value'] = $value;
        $attrs['selected'] = ($this->IsValueSelected($value) ? 'selected' : null);        
    
        TNTLoader::RequireClass('TNTTag');

        $tag = new TNTTag();

        $tag->SetElement('name', 'option');
        $tag->SetElement('attrs', $attrs);
        $tag->SetElement('type', TNT_TAG_OPEN);
    
        $ret = $tag->ToString().$indent.$label.'</option>'."\n";
    
        if (isset($item['items']) && is_array($item['items']) && count($item['items'])) {
            $ret .= $this->_RenderItems($item['items'], $depth + 1);
        }

        return $ret;
    }

    /**
     * _RenderItemGroup
     *
     * Renders an item group.
     *
     * A item group is rendered as an HTML <optgroup> element.
     *
     * Although HTML has no concept of nested items we support this by
     * rendering each level indented from the previous using spaces.
     *
     * @access  protected
     * @param   array           $item item to render
     * @param   integer         $depth  depth of traversal
     * @return  void
     */
    function _RenderItemGroup($item, $depth) {

        $label = $item['label'];
        $indent = str_repeat(($this->_indent ? $this->_indent : '&nbsp;&nbsp;'), $depth);

        $attrs['class'] = (isset($item['class']) ? $item['class'] : null);
        $attrs['style'] = (isset($item['style']) ? $item['style'] : null);
        $attrs['title'] = (isset($item['title']) ? $item['title'] : null);
        $attrs['label'] = $indent.$label;

        TNTLoader::RequireClass('TNTTag');
    
        $tag = new TNTTag();
    
        $tag->SetElement('name', 'optgroup');
        $tag->SetElement('attrs', $attrs);
        $tag->SetElement('type', TNT_TAG_OPEN);
    
        $ret = $tag->ToString()."\n";
    
        if (isset($item['items']) && is_array($item['items']) && count($item['items'])) {
            $ret .= $this->_RenderItems($item['items'], $depth + 1);
        }

        $ret .= '</optgroup>'."\n";

        return $ret;
    }

    /**
     * _RenderItems
     *
     * Renders the items by calling _RenderItemGroup() or _RenderItem()
     * on each item recursively.
     *
     * @access  protected
     * @param   array           $items to array if items
     * @param   integer         $depth  depth of traversal
     * @return  void
     */
    function _RenderItems($items, $depth) {

        $ret = '';
        foreach ($items as $index => $item) {
            if (!isset($item['label'])) {
                $item['label'] = "{$depth}:{$index}";
            }
            if (isset($item['group']) && strtoboolean($item['group'])) {
                $ret .= $this->_RenderItemGroup($item, $depth);
            } else {
                $ret .= $this->_RenderItem($item, $depth);
            }
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
     * Render the list items.
     *
     * @access  public
     * @return  void
     */
    function RenderContents() {

        $ret = '';

        $numItems = count($this->_items);
        $ret .= "\n<!-- ListBox Contents ($numItems) -->\n";
        if ($numItems) {
            $ret .= $this->_RenderItems($this->_items, 0);
        }
        $ret .= "\n<!-- ListBox Contents (End) -->\n";

        return $ret;
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiListBox() {

        TNTWuiListCtrl::TNTWuiListCtrl();

        $this->tagName = 'select';
        $this->tagComplete = false;

        array_remove($this->tagAttrNames, array('value'), false);

        array_push($this->tagAttrNames, 'size');

    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiListBox() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
