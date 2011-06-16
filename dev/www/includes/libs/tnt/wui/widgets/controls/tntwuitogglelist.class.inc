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
 * Wui ToggleList Class 
 * ~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic list of toggle buttons on a Wui form.
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
 * The ToggleList class allows you to add a series of ToggleBox controls which
 * allow you select from a list of items.
 * 
 * <pre>
 * {tnt_ctrl ctrl="editbox"
 *   id="string"
 *   name="string"
 *   autopostback="true|false"
 *   multiple="true|false"
 *   cols="number"
 *   labelalign="left|right"
 *   value="string"
 *   valsep="string"
 *   items=<array>
 * }
 * </pre>
 *
 * Use the ToggleList control to create a series of ToggleBox controls that
 * allows single or multiple item selection. Use the 'cols' property to specify
 * the number of ToggleList controls to display all each line. If no 'cols'
 * property is set, or set to zero then the list will autowrap to the size of
 * its parent containers width were possible. To enable multiple item selection,
 * set the 'Multiple' property to true.
 *
 * The items to show as selected can be specified using the 'Value' property of
 * the ToggleList control. This property can be specified either the value of
 * the item to selected. In the case of multi-selection ToggleList controls the
 * value can be an array containing the values of any items selected, or a
 * string which will be split using the 'valsep' property to create the array.
 * If the 'valsep' property is given as 'binary' the the value is assumed to be
 * a binary number and the value of each item should also be a binary number.
 * An item is then selected if it matches the ToggleList controls 'value' using
 * binary bit comparison.
 *
 * After a Post back the 'value' property will reflect the selection state of
 * the client ToggleList. The method {@link IsValueSelected()} can be used to
 * test if a given value has been selected.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiToggleList extends TNTWuiListCtrl {

    /**
     * The number of cols displayed in the ToggleList control..
     * @access private
     * @var integer                     number of cols to display
     */
    var $_cols = null;

    /**
     * Label alignment (left | right)
     * @access private
     * @var string                      check box label position
     */
    var $_labelAlign = null;

    /**
     * Set the number of cols displayed in the ToggleList control.
     *
     * @access  public
     * @param   string      $cols       string to set the cols to
     * @return  boolean                 true if successful
     * @see     GetCols();
     */ 
    function SetCols($cols) {

        if (is_null($cols)) {
            $this->_cols = null;
            $ret = true;
        } else {
            $this->_cols = $cols;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the number of cols displayed in the ToggleList control.
     *
     * If there is no cols set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current cols
     * @see     SetCols();
     */ 
    function GetCols() {

        $ret = $this->_cols;

        return $ret;
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

        $ret = $this->_labelAlign;

        return $ret;
    }

    /**
     * _RenderItem
     *
     * Renders a single child Item node.
     *
     * The Item is rendered as a HTML '<span>' containing the items text
     * and a HTML '<input type="radio|check">' element.
     *
     * Although HTML has no concept of nested items we support this by
     * rendering each level in nested divs using a left padding style. This
     * only applies to vertical layout (ie. single columns layout)
     *
     * @access  protected
     * @param   array           $item item to render
     * @param   integer         $depth  depth of traversal
     * @param   array           $d      rendering data
     * @return  void
     */
    function _RenderItem($item, $depth, &$d) {

        $label = $item['label'];
        $value = (isset($item['value']) ? $item['value'] : $label);

        $attrs['title'] = (isset($item['title']) ? $item['title'] : null);
        $attrs['name'] = $this->TagAttribute('name').'[]';
        $attrs['value'] = $value;
        $attrs['type'] = $d['input'];
        $attrs['checked'] = ($this->IsValueSelected($value) ? 'checked' : null);

        $inner = '<span class="tntwuitogglelist-'.$d['align'].' tntwuitogglelist">';
        if ($d['align'] == 'left') {
            $inner .= $label;
        }
        $inner .= TNTHtml::Tag('input', $attrs, TNT_TAG_EMPTY);
        if ($d['align'] == 'right') {
            $inner .= $label;
        }
        $inner .= '</span>';

        // Wrap the whole thing up in a div with the users attributes applied
        $attrs = array();
        $attrs['id'] = (isset($item['domid']) ? $item['domid'] : null);
        $attrs['class'] = (isset($item['class']) ? $item['class'] : null);
        $attrs['style'] = (isset($item['style']) ? $item['style'] : null);

        $toggle = '<nobr>'.TNTHtml::BlockTag('span', $attrs, $inner).'</nobr>';

        $ret = '';

        if ($d['column'] == -1) {
            $d['column'] = 1;
            $ret .= $d['sgroup'];
        }

        $ret .= $d['scell'].$toggle.$d['ecell'];

        $d['column']++;
        
        if (isset($item['items']) && is_array($item['items']) && count($item['items'])) {
            if ($d['column'] != -1) {
                $d['column'] = -1;
                $ret .= $d['egroup'];
            }
            $ret .= '<div class="tntwuitogglelist-indent" style="padding-left: 2em;">'."\n";
            $ret .= $this->_RenderItems($item['items'], $depth + 1, $d);
            $ret .= '</div>'."\n";
        }

        return $ret;
    }

    /**
     * _RenderListGroup
     *
     * Renders a single child ListGroup node.
     *
     * The ListGroup is rendered as a HTML in a '<fieldset>' element
     * in single row or column styles. It is ignored in multicolumn styles
     * although its child Item are displayed.
     *
     * Although HTML has no concept of nested items we support this by
     * rendering each level indented from the previous using spaces. This
     * only applies to vertical layout (ie. single columns layout)
     *
     * @access  protected
     * @param   array           $item item to render
     * @param   integer         $depth  depth of traversal
     * @param   array           $d      rendering data
     * @return  void
     */
    function _RenderItemGroup($item, $depth, &$d) {

        $label = $item['label'];

        $ret = '';

        if ($d['column'] != -1) {
            $d['column'] = -1;
            $ret .= $d['egroup'];
        }

        $ret .= '<fieldset class="tntwuitogglelist">';

        // Insert a legend on the fieldset if one provided
        if ($label && $label != '') {
            $ret .= "<legend class=\"tntwuitogglelist\">$label</legend>\n";
        }

        if (isset($item['items']) && is_array($item['items']) && count($item['items'])) {
            $ret .= '<div class="tntwuitogglelist-inner">'.
                    $this->_RenderItems($item['items'], $depth + 1, $d).
                    '</div>';
        }

        if ($d['column'] != -1) {
            $d['column'] = -1;
            $ret .= $d['egroup'];
        }

        $ret .= '</fieldset>';


        // Wrap the whole thing up in a div with the users attributes applied
        $attrs['id'] = (isset($item['domid']) ? $item['domid'] : null);
        $attrs['class'] = (isset($item['class']) ? $item['class'] : null);
        $attrs['style'] = (isset($item['style']) ? $item['style'] : null);
        $attrs['title'] = (isset($item['title']) ? $item['title'] : null);
            
        $ret = TNTHtml::BlockTag('div', $attrs, $ret);

        return $ret;
    }

    /**
     * _RenderItems
     *
     * Renders the items by calling _RenderItemGroup() or _RenderItem()
     * on each item recursively. _RenderItemGroup() is used if the item
     * has no value and an array of sub items.
     *
     * @access  protected
     * @param   array           $items to array if items
     * @param   integer         $depth  depth of traversal
     * @param   array           $d      rendering data
     * @return  void
     */
    function _RenderItems($items, $depth, &$d) {

        $ret = '';
        foreach ($items as $index => $item) {

            if (!isset($item['label'])) {
                $item['label'] = "{$depth}:{$index}";
            }


            if ($d['render'] == 2) {
                // Render mode 2 is in columns
                if ($d['column'] > $d['cols']) {
                    $ret .= "  </tr>\n  <tr>\n    ";
                    $d['column'] = 1;
                }
            }

            $ret .= $d['srow'];

            if (isset($item['group']) && strtoboolean($item['group'])) {
                $ret .= $this->_RenderItemGroup($item, $depth, $d);
            } else {
                $ret .= $this->_RenderItem($item, $depth, $d);
            }

            $ret .= $d['erow'];
        }

        if ($d['column'] != -1) {
            $d['column'] = -1;
            $ret .= $d['egroup'];
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
     * Rendering is quite complex as it can be one of three styles;
     *
     * 1) Horizontal rendering - 'cols' = zero (or not specified)
     *      Items are display in a single row per group.
     * 2) Vertical rendering - 'cols' = 1
     *      Items are display in a single column per group.
     * 3) Multi-column Vertical rendering - 'cols' >= 1
     *      Items are display in multiple columns per group.
     *
     * Groups are displayed by grouping items in <group> boxes.
     * Hierachy information is displayed by indenting items.
     *
     * @access  public
     * @return  void
     */
    function RenderContents() {

        $ret = '';

        $numItems = count($this->_items);
        $ret .= "\n<!-- ToggleBox Contents ($numItems) -->\n";
        if ($numItems) {
            $d['cols'] = $this->GetCols();
            $d['align'] = $this->GetLabelAlign();
            $d['mutiple'] = $this->GetMultiple();
            $d['render'] = ($d['cols'] ? ($d['cols'] == 1 ? 1 : 2) : 0);
            $d['input'] = ($d['mutiple'] ? 'checkbox' : 'radio');

            if ($d['align'] != 'left' && $d['align'] != 'right') {
                $d['align'] = 'right';
            }

            // Render mode 3 uses a table to get the multi-colums.

            if ($d['render'] == 2) {
                // Have to render in a table
                $d['scell'] = '<td>';
                $d['ecell'] = '</td>';
                $d['srow'] = '    ';
                $d['erow'] = "\n";
                $d['sgroup'] = "\n<table class=\"tntwuitogglelist\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\">\n  <tr>\n";
                $d['egroup'] = "\n  </tr>\n</table>\n";
            } else if ($d['render'] == 1) {
                // Render vertical
                $d['scell'] = '<div>';
                $d['ecell'] = '</div>';
                $d['srow'] = '';
                $d['erow'] = "\n";
                $d['sgroup'] = '';
                $d['egroup'] = '';
            } else {
                // Render horizontal
                $d['scell'] = '';
                $d['ecell'] = '';
                $d['srow'] = '';
                $d['erow'] = "\n";
                $d['sgroup'] = '';
                $d['egroup'] = '';
            }

            $d['column'] = -1;

            $ret .= $this->_RenderItems($this->_items, 0, $d);
        }

        $ret .= "\n<!-- ToggleBox Contents (End) -->\n";

        return $ret;
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiToggleList() {

        TNTWuiListCtrl::TNTWuiListCtrl();

        $this->tagName =  'div';
        $this->tagComplete = false;
        $this->tagAttrNames = array('id', 'class', 'style');

    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiToggleList() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
