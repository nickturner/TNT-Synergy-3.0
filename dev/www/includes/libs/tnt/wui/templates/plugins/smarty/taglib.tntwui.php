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
 * WUI TagLib Plugin Function
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Smarty Template Plugin function to register the WUI tag library.
 *
 * @package TNT.Wui
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */

/*
 * Globals
 */


/*
 * Defines
 */

/**
 * Register the Smarty plugins for the TNTWui library.
 *
 * This pre-registers the Smarty template plugins defined by the Wui library
 * so the template doesn't have to search for the implementation.
 *
 * @access  public
 * @param   string[]    $params     parameters passed to import directive
 * @param   object      $caller     reference to calling Smarty class
 * @return  true                    true if successful
 */
function smarty_import_tntwui($params, &$caller) {
    //                      Type        Plugin Name         Plugin Callback                                     Cacheble
    $caller->RegisterPlugin('block',    'TNTWuiWidget',     array('TNTWuiWidget', '_TemplatePluginBlock'),      false);
    $caller->RegisterPlugin('block',    'TNTWuiWindow',     array('TNTWuiWindow', '_TemplatePluginBlock'),      false);
    $caller->RegisterPlugin('block',    'TNTWuiForm',       array('TNTWuiForm', '_TemplatePluginBlock'),        false);
    $caller->RegisterPlugin('block',    'TNTWuiToggleBox',  array('TNTWuiToggleBox', '_TemplatePluginBlock'),   false);
    $caller->RegisterPlugin('block',    'TNTWuiCheckBox',   array('TNTWuiCheckBox', '_TemplatePluginBlock'),    false);
    $caller->RegisterPlugin('block',    'TNTWuiRadioBox',   array('TNTWuiRadioBox', '_TemplatePluginBlock'),    false);
    $caller->RegisterPlugin('block',    'TNTWuiTickBox',    array('TNTWuiTickBox', '_TemplatePluginBlock'),     false);
    $caller->RegisterPlugin('block',    'TNTWuiData',       array('TNTWuiData', '_TemplatePluginBlock'),        false);
    $caller->RegisterPlugin('block',    'TNTWuiInput',      array('TNTWuiInput', '_TemplatePluginBlock'),       false);
    $caller->RegisterPlugin('block',    'TNTWuiEditBox',    array('TNTWuiEditBox', '_TemplatePluginBlock'),     false);
    $caller->RegisterPlugin('block',    'TNTWuiTextBox',    array('TNTWuiTextBox', '_TemplatePluginBlock'),     false);
    $caller->RegisterPlugin('block',    'TNTWuiUploadBox',  array('TNTWuiUploadBox', '_TemplatePluginBlock'),   false);
    $caller->RegisterPlugin('block',    'TNTWuiNumberBox',  array('TNTWuiNumberBox', '_TemplatePluginBlock'),   false);
    $caller->RegisterPlugin('block',    'TNTWuiPasswordBox',array('TNTWuiPasswordBox', '_TemplatePluginBlock'), false);
    $caller->RegisterPlugin('block',    'TNTWuiListBox',    array('TNTWuiListBox', '_TemplatePluginBlock'),     false);
    $caller->RegisterPlugin('block',    'TNTWuiToggleList', array('TNTWuiToggleList', '_TemplatePluginBlock'),  false);
    $caller->RegisterPlugin('block',    'TNTWuiButton',     array('TNTWuiButton', '_TemplatePluginBlock'),      false);
    $caller->RegisterPlugin('block',    'TNTWuiImageButton',array('TNTWuiImageButton', '_TemplatePluginBlock'), false);
    $caller->RegisterPlugin('block',    'TNTWuiMenuBar',    array('TNTWuiMenuBar', '_TemplatePluginBlock'),     false);
    $caller->RegisterPlugin('block',    'TNTWuiMenu',       array('TNTWuiMenu', '_TemplatePluginBlock'),        false);
    $caller->RegisterPlugin('block',    'TNTWuiTree',       array('TNTWuiTree', '_TemplatePluginBlock'),        false);
    $caller->RegisterPlugin('block',    'TNTWuiOutBar',     array('TNTWuiOutBar', '_TemplatePluginBlock'),      false);
    $caller->RegisterPlugin('block',    'TNTWuiToolBar',    array('TNTWuiToolBar', '_TemplatePluginBlock'),     false);
    $caller->RegisterPlugin('block',    'TNTWuiListView',   array('TNTWuiListView', '_TemplatePluginBlock'),    false);

    return true;
}

/**
 * Register the tag library with the template code.
 *
 * This calls the module import function to register all the modules
 * plugins and then returns an array of tag details in the following
 * format;
 *   $ret[$tag] = array($name, $type);
 *   where $tag is the name of the tag to register,
 *         $name is the plugin command to handle the tag
 *         $type is the plugin type is maps to 'compiler|function|block'
 *
 * @access  public
 * @param   string[]    $params     parameters passed to taglib directive
 * @param   object      $caller     reference to calling Smarty class
 * @return  array                   associative array of tag details
 */
function smarty_taglib_tntwui($params, &$caller) {

    //require_once(dirname(__FILE__).'/import.tntwui.php');
    
    smarty_import_tntwui($params, $caller);

    //   Tag name          Template Command             Block
    $ret = array(
        'widget'        => array('TNTWuiWidget',        true),
        'window'        => array('TNTWuiWindow',        true),
        'form'          => array('TNTWuiForm',          true),
        'togglebox'     => array('TNTWuiToggleBox',     true),
        'checkbox'      => array('TNTWuiCheckBox',      true),
        'radiobox'      => array('TNTWuiRadioBox',      true),
        'tickbox'       => array('TNTWuiTickBox',       true),
        'data'          => array('TNTWuiData',          true),
        'input'         => array('TNTWuiInput',         true),
        'editbox'       => array('TNTWuiEditBox',       true),
        'textbox'       => array('TNTWuiTextBox',       true),
        'passwordbox'   => array('TNTWuiPasswordBox',   true),
        'numberbox'     => array('TNTWuiNumberBox',     true),
        'uploadbox'     => array('TNTWuiUploadBox',     true),
        'listbox'       => array('TNTWuiListBox',       true),
        'togglelist'    => array('TNTWuiToggleList',    true),
        'button'        => array('TNTWuiButton',        true),
        'imagebutton'   => array('TNTWuiImageButton',   true),
        'menubar'       => array('TNTWuiMenuBar',       true),
        'menu'          => array('TNTWuiMenu',          true),
        'tree'          => array('TNTWuiTree',          true),
        'outbar'        => array('TNTWuiOutBar',        true),
        'toolbar'       => array('TNTWuiToolBar',       true),
        'listview'      => array('TNTWuiListView',      true),
    );

    return $ret;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
