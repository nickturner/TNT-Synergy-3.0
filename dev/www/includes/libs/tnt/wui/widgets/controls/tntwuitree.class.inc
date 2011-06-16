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
 * WUI Tree Class 
 * ~~~~~~~~~~~~~~
 *
 * The WUI Tree class is a widget that displays a tree. The tree is normally
 * displayed as a expandable tree using javascript on browsers that can support
 * such functionality.
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */


/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiWidget');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a Tree in the Web User Interface.
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiTree extends TNTWuiWidget {

    /**
     * The behaviour of the tree.
     * @var     string                  the current tree behaviour
     * @access  public
     */
    var $_behaviour = null;

    /**
     * Does the tree allow multiple selection
     * @var     boolean                 true to allow multiple selection
     * @access  public
     */
    var $_multiple = null;

    /**
     * The default hyperlink reference for all TreeItems.
     * @access private
     * @var string                      hyperlink string
     */
    var $_href = null;

    /**
     * The default hyperlink target for all TreeItems.
     * @access private
     * @var string                      href target string
     */
    var $_target = null;

    /**
     * The path name for the default tree icons.
     * @access private
     * @var string                      document path to icon directory
     */
    var $_iconPath = null;

    /**
     * The path name to default tree style icons.
     * @access private
     * @var string                      document path to icon directory
     */
    var $_skinPath = null;

    /**
     * Array of tree items.
     * @access  private
     * @var     array                   array of tree items
     */
    var $_items = null;

    /**
     * Set the behaviour
     *
     * Two current behaviours are allowed as follows;
     *
     *   + 'classic' - TreeItems can be either folders or leaves, folders are
     *     items with children and leaves are items without children. Folders
     *     can be childless by setting the attribute on the TreeItem.
     *
     *   + 'explorer' - All TreeItems are treated as Folders.
     *
     * @access  public
     * @param   string      $value      string to set the value to
     * @return  boolean                 true if successful
     * @see     GetBehavior();
     */ 
    function SetBehavior($value) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('value' =>& $value));

        static $_behaviours = array('classic', 'explorer');

        if (is_null($value)) {
            $this->_behaviour = null;
            $ret = true;
        } else if (!is_string($value) ||
            !in_array($value = strtolower($value), $_behaviours)) {
            $this->_SetError('value must be one of '.join(',', $_behaviours));
            $ret = false;
        } else {
            $this->_behaviour = $value;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the behaviour
     *
     * If there is no value set then null is returned.
     *
     * @access  public
     * @return  string                  the widgets current value
     * @see     SetBehavior();
     */ 
    function GetBehavior() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_behaviour;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Set a flag indicating that multiple selections are allowed.
     *
     * @access  public
     * @param   string      $value      string to set the value to
     * @return  boolean                 true if successful
     * @see     GetMultiple();
     */ 
    function SetMultiple($value) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('value' =>& $value));

        if (is_null($value)) {
            $this->_multiple = null;
            $ret = true;
        } else {
            $this->_multiple = strtoboolean($value);
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get a flag indicating that multiple selections are allowed.
     *
     * If there is no multiple set then null is returned.
     *
     * @access  public
     * @return  string                  the widgets current value
     * @see     SetMultiple();
     */ 
    function GetMultiple() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_multiple;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Set the hyperlink associated with the Tree.
     *
     * Each item in the tree will be hyperlinked to this reference unless the
     * individual TreeItem 'href' attribute is set.
     
     * A Virtual path can be specified, otherwise a path relative to the
     * document source should be provided.
     *
     * @access  public
     * @param   string      $value      string to set the value to
     * @return  boolean                 true if successful
     * @see     GetHref();
     */ 
    function SetHref($value) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('value' =>& $value));

        if (is_null($value)) {
            $this->_href = null;
            $ret = true;
        } else if (!is_scalar($value)) {
            $this->_SetError('value must be either null or a scalar');
            $ret = false;
        } else {
            $this->_href = $value;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the default hyperlink associated with the Tree.
     *
     * If there is no value set then null is returned.
     *
     * @access  public
     * @return  string                  the current value
     * @see     SetHref();
     */ 
    function GetHref() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_href;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Set the hyperlink target associated with the TreeItem.
     *
     * Each items hyperlink will be directed at this default target unless the
     * individual TreeItem 'tarrget' attribute is set.
     *
     * @access  public
     * @param   string      $value      string to set the value to
     * @return  boolean                 true if successful
     * @see     GetTarget();
     */ 
    function SetTarget($value) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('value' =>& $value));

        if (is_null($value)) {
            $this->_target = null;
            $ret = true;
        } else if (!is_scalar($value)) {
            $this->_SetError('value must be either null or a scalar');
            $ret = false;
        } else {
            $this->_target = $value;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the default hyperlink target associated with the Tree.
     *
     * If there is no value set then null is returned.
     *
     * @access  public
     * @return  string                  the current value
     * @see     SetTarget();
     */ 
    function GetTarget() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_target;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the path to the tree style folder.
     *
     * Trees can be skinned to display in numerous ways, the folder contains
     * the appropriate icons used to draw the tree framework and any default
     * node icons. It also contains a CSS file used to style the tree.
     *
     * A Virtual path can be specified, otherwise a path relative to the
     * document source should be provided.
     *
     * @access  public
     * @param   string      $value      document source path to the icons.
     * @return  boolean                 true if successful
     * @see     GetSkinPath();
     */ 
    function SetSkinPath($value) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('value' =>& $value));

        if (is_null($value)) {
            $this->_skinPath = null;
            $ret = true;
        } else if (!is_string($value)) {
            $this->_SetError('src must be either null or a string');
            $ret = false;
        } else {
            $this->_skinPath = $value;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the path to the tree style folder.
     *
     * If there is no value set then null is returned.
     *
     * @access  public
     * @return  string                  document source path to the iconPath
     * @see     SetIconPath();
     */ 
    function GetSkinPath() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_skinPath;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Set the path to the trees icon folder.
     *
     * This is used to set the path for icons used to draw the nodes in the
     * tree.
     *
     * If not set then then any path specified by {@link SetSkinPath()} will
     * be used. Each node can specify its own path to its open and closed
     * icons.
     *
     * A Virtual path can be specified, otherwise a path relative to the
     * document source should be provided.
     *
     * @access  public
     * @param   string      $value      document source path to the icons.
     * @return  boolean                 true if successful
     * @see     GetIconPath();
     */ 
    function SetIconPath($value) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('value' =>& $value));

        if (is_null($value)) {
            $this->_iconPath = null;
            $ret = true;
        } else if (!is_string($value)) {
            $this->_SetError('src must be either null or a string');
            $ret = false;
        } else {
            $this->_iconPath = $value;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the path to the tree icons folder.
     *
     * If there is no value set then null is returned.
     *
     * @access  public
     * @return  string                  document source path to the iconPath
     * @see     SetIconPath();
     */ 
    function GetIconPath() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_iconPath;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Sets the tree items.
     *
     * This member specifies an array of items to be displayed by the tree.
     *
     * Each item is an associative array containing the following optional
     * members;
     *   + value    - string: the value to store with the item
     *   + label    - string: the HTML to display for this item
     *   + icon     - string: the path to the icon to display
     *   + tooltip  - string: the a tooltip to display for this item
     *   + action   - string: the function to call or URL to goto when clicked
     *   + options  - array: an associative array of extended options
     *   + items    - array: an array of child items
     *   + class    - string: the a css class for the item
     *   + style    - string: the a css style for the item
     *   + disabled - boolean: if true then item is disabled
     *   + visible  - boolean: if false then item is not added to the tree
     *   + target   - string: the target for any URL specified in 'action'
     *   + type     - string: the item type 'default|checkbox|radiobox'
     *   + group    - string: the group name for radio button items
     *   + checked  - boolean: the checked state of aby radio/check box item
     *   + iconOpen - string: the path to the icon to display when item open
     *   + onclick  - string: client side 'onclick' script
     *
     * If 'items' is set to a string value then it is assumed to be the
     * input to a data source object and be of the format 'type:data'.
     *
     * If no 'label' is specified then one is made up by the tree.
     *
     * @access  public
     * @param   array       $items      array of tree items
     * @return  boolean                 true if successful
     * @see     GetItems();
     */ 
    function SetItems($value) {

        $ret = false;
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
     * Gets the tree items.
     *
     * If there are no items defined then null is returned.
     *
     * @access  public
     * @return  array                   array of tree items
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

        TNTLoader::RequireClass('TNTHtml');

        $html = '';

        $app =& TNTApp::Self();
        $included = (array)$app->GetOpt('jsincludes');
        $include = '/synergy/components/tnt/tree/tnttree.js';

        if (!in_array($include, $included)) {
            $html .= "<head>\n<script type=\"text/javascript\" src=\"$include\"></script>\n</head>";
        }

        $var = ($this->_id ? $this->_id : $this->id);

        $props = $this->GetProperties() + (array)$this->_properties;

        $jsprops = TNTHtml::ValueToJavaScript($props, false);

        $html .= <<<EOS
<script type="text/javascript">
<!--
{$var} = TNT_TreeCreate($jsprops);
{$var}.write();
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
     * Construct the object
     *
     * @access  public
     */
    function TNTWuiTree() {

        TNTWuiWidget::TNTWuiWidget();

        array_push($this->_propertyNames,
            'items',
            'behaviour',
            'multiple',
            'href',
            'target',
            'iconPath',
            'skinPath');
    }

    /**
     * Destruct the box object.
     *
     * This will destruct the box object.
     *
     * @access  private
     */
    function _TNTWuiTree() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
