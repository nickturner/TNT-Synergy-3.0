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
 * Wui List View Class 
 * ~~~~~~~~~~~~~~~~~~~
 *
 * A list view class emulates the Windows ListView object used by the Windows
 * Operating System. It is a powerfull and flexible control allowing the data
 * to be display in many forms based up a grid layout.
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
 * The ListView class allows you display a list of items in a similar way
 * to the Microsoft Windows Explorer.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiListView extends TNTWuiListCtrl {

    /*
     * View to display
     */
    var $_view = null;

    /*
     * List of view information
     */
    var $_views = null;

    /*
     * List of column information
     */
    var $_columns = null;

    /**
     * Set the current view
     *
     * This must be one of the views available as set by the {@link SetViews()}
     * method.
     *
     * If a null value is specified then the first view in the list of available
     * views is used, this is normally 'icon' view.
     *
     * @access  public
     * @param   string      $value      value to set the items to
     * @return  boolean                 true if successful
     * @see     GetView();
     */ 
    function SetView($value) {

        if (is_null($value)) {
            $this->_view = null;
            $ret = true;
        } else if (!is_string($value) || !isset($this->_views[$value])) {
            $this->_SetError('value must be either null or one of \''.join('\',\'', array_keys((array)$this->_views)).'\'');
            $ret = false;
        } else {
            $this->_view = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the current view
     *
     * If there is no view set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current value
     * @see     SetView();
     */ 
    function GetView() {
        return $this->_view;
    }

    /**
     * Set the views
     *
     * This member specifies an array of views to be displayed by the list
     * control.
     *
     * Each views is an associative array containing the following optional
     * members;
     *   + id       - string: unique view id
     *   + name     - string: name of the view (i.e. 'Icon|Title|Thumbnail')
     *   + title    - string: description of the view
     *   + layout   - string: the item layout ('flow|list|grid')
     *   + itemDim  - string: the css dimension of each item in the view
     *   + iconDim  - string: the css dimension of the items icon (px only)
     *   + iconPos  - string: the icon position ('left|right|top|bottom');
     *   + columns  - string: comma separated list of columns
     *
     * If 'views' is set to a string value then it is assumed to be the
     * input to a data source object and be of the format 'type:data'.
     *
     * The 'layout' member details the basic layout. In 'flow' layout the item
     * flow accross the containing element wrapping at the right margin. In
     * 'list' layout the items are lists below each other against the left
     * margin. In 'grid' view the item and all there details are rendered using
     * TNTWuiGrid control.
     *
     * If the padding and dimensions are ommitted then default ones are
     * selected by the control.
     *
     * The 'columns' consists of a comma separated list of column names whos
     * data will be displayed with the item in addition to the label when in
     * 'grid' layout mode.
     *
     *
     * @access  public
     * @param   mixed       $value      value to set the items to
     * @return  boolean                 true if successful
     * @see     GetItems();
     */ 
    function SetViews($value) {

        if (is_null($value)) {
            $this->_views = null;
            $ret = true;
        } else if (!is_array($value)) {
            $this->_SetError('value must be either null or an array');
            $ret = false;
        } else {
            $this->_views = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the views
     *
     * If there is no views set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current value
     * @see     SetViews();
     */ 
    function GetViews() {
        return $this->_views;
    }

    /**
     * Set the columns
     *
     * This member specifies an array of columns to be displayed by the list
     * control.
     *
     * Each columns is an associative array containing the following optional
     * members;
     *   + id       - integer: id (column 0 refers to item, 1+ to subitem)
     *   + align    - string: alignment ('left', 'right' or 'center')
     *   + width    - string: width in pixels or percentage
     *   + label    - string: header (HTML)
     *   + image    - string: image (either TNTIcon or src name)
     *   + imagePos - string: image position ('left' or 'right')
     *   + order    - integer: order (zero is leftmost)
     *
     * If 'columns' is set to a string value then it is assumed to be the
     * input to a data source object and be of the format 'type:data'.
     *
     * @access  public
     * @param   mixed       $value      value to set the columns to
     * @return  boolean                 true if successful
     * @see     GetColumns();
     */ 
    function SetColumns($value) {

        if (is_null($value)) {
            $this->_columns = null;
            $ret = true;
        } else if (!is_array($value)) {
            $this->_SetError('value must be either null or an array');
            $ret = false;
        } else {
            $this->_columns = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the columns
     *
     * If there is no columns set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current value
     * @see     SetColumns();
     */ 
    function GetColumns() {
        return $this->_columns;
    }

    /**
     * Set the items
     *
     * This member specifies an array of items to be displayed by the list
     * control.
     *
     * Each items is an associative array containing the following optional
     * members;
     *   + id       - string: id
     *   + value    - string: the valye to store with this item
     *   + label    - string: label (HTML)
     *   + icon     - string: the path to the icon to display
     *   + tooltip  - string: tooltip
     *   + action   - string: the function to call or URL to goto when clicked
     *   + options  - array: an associative array of extended options
     *   + subItems - array: an array of sub items for columns
     *   + state    - integer: state (see below)
     *   + stateMask- integer: stateMask (which states are set above)
     *   + images   - array: images (per view and state)
     *   + indent   - integer: identation (in image widths)
     *   + userData - mixed: user data (string)
     *   + group    - string: group the item is in
     *   + tileCols - array: tile columns to display
     *   + subItems - array: sub items (array of sub items)
     *   + checked  - boolean: the checked state of any check box
     *   + class    - string: the a css class for the item
     *   + style    - string: the a css style for the item
     *   + events   - map: event handlers
     *
     * If 'items' is set to a string value then it is assumed to be the
     * input to a data source object and be of the format 'type:data'.
     *
     * If no 'label' is specified then one is made up by the control.
     *
     * If no 'value' is given then the 'label' is used. 
     *
     * The 'group' member can be used to indicate option grouping, different
     * list widgets will display groups in different ways.
     *
     * Each sub item is a map associative array containing the following
     * optional members;
     *   + id       - string: id
     *   + label    - string: label (HTML)
     *   + tooltip  - string: tooltip
     *   + events   - map: event handlers
     *   + class    - string: the a css class for the item
     *   + style    - string: the a css style for the item
     *   + action   - string: the function to call or URL to goto when clicked
     *   + options  - array: an associative array of extended options
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
        return $this->_items;
    }

    /**
     * Render the tree.
     *
     * @access  public
     * @return  string                  HTML required to render the tree
     */
    function Render() {

        static $jsloaded;

        $html = '';

        if (!$jsloaded) {
            $html .= <<<EOS
<head>
<script type="text/javascript" src="~/scripts/juic/source/scripts/juic.js"></script>
</head>
EOS;
        }

        $name = ($this->_id ? $this->_id : $this->id);
        $data = TNTHtml::ValueToJavaScript($this->Get());

        $html .= <<<EOS
<script type="text/javascript">
<!--
{$name} = new TNT.Juic.Ctrl.ListView({$data});
document.write({$name});
//-->
</script>
EOS;

        // Wrap the whole thing up in a div with the users attributes applied
        $attrs['id'] = $this->GetId();
        $attrs['class'] = $this->GetClass();
        $attrs['style'] = $this->GetStyle();

        $html = TNTHtml::BlockTag('div', $attrs, $html).'</div>';

        return $html;
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiListView() {

        TNTWuiListCtrl::TNTWuiListCtrl();

        $this->_views = array(
            'icon' => array(
                'view' => 'icon',
                'name' => 'Large Icons',
                'title' => 'Displays items using large icons.',
                'layout' => 'flow',
                'itemDim' => '73px',
                'itemPad' => '1px',
                'iconDim' => '32px',
                'iconPad' => '4px',
                'iconPos' => 'top',
                'columns' => ''),
            'mini' => array(
                'view' => 'mini',
                'name' => 'Small Icons',
                'title' => 'Displays items using small icons.',
                'layout' => 'flow',
                'itemDim' => '16px',
                'itemPad' => '1px',
                'iconDim' => '16px',
                'iconPad' => '1px',
                'iconPos' => 'left',
                'columns' => ''),
            'list' => array(
                'view' => 'list',
                'name' => 'List',
                'title' => 'Displays items in a List.',
                'layout' => 'flow',
                'itemDim' => '75px',
                'itemPad' => '1px',
                'iconDim' => '32px',
                'iconPad' => '0px',
                'iconPos' => 'top',
                'columns' => ''),
            'tile' => array(
                'view' => 'tile',
                'title' => 'Tile',
                'layout' => 'flow',
                'itemDim' => '75px',
                'itemPad' => '1px',
                'iconDim' => '32px',
                'iconPad' => '0px',
                'iconPos' => 'top',
                'columns' => 'type,size'),
            'grid' => array(
                'view' => 'full',
                'name' => 'Details',
                'title' => 'Details information about each item.',
                'layout' => 'grid',
                'itemDim' => '75px',
                'itemPad' => '1px',
                'iconDim' => '32px',
                'iconPad' => '0px',
                'iconPos' => 'top',
                'columns' => ''),
            'nail' => array(
                'view' => 'nail',
                'name' => 'Thumbnails',
                'title' => 'Displays thumbnails of each item.',
                'layout' => 'flow',
                'itemDim' => '75px',
                'itemPad' => '1px',
                'iconDim' => '32px',
                'iconPad' => '0px',
                'iconPos' => 'top',
                'columns' => ''),
             );

    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiListView() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
