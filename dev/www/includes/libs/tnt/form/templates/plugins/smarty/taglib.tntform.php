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
 * Form TagLib Plugin Function
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Smarty Template Plugin function to register the Form tag library.
 *
 * @package TNT.Form
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
 * Register the Smarty plugins for the TNTForm library.
 *
 * This pre-registers the Smarty template plugins defined by the Form library
 * so the template doesn't have to search for the implementation.
 *
 * @access  public
 * @param   string[]    $params     parameters passed to import directive
 * @param   object      $caller     reference to calling Smarty class
 * @return  true                    true if successful
 */
function smarty_import_tntform($params, &$caller) {
    //                      Type        Plugin Name         Plugin Callback                                     Cacheble
    $caller->RegisterPlugin('block',    'TNTForm',          array('TNTForm', '_TemplatePluginBlock'),           false);
    $caller->RegisterPlugin('block',    'TNTFormCell',      array('TNTFormCell', '_TemplatePluginBlock'),       false);
    $caller->RegisterPlugin('block',    'TNTFormEditBox',   array('TNTFormEditBox', '_TemplatePluginBlock'),    false);
//  $caller->RegisterPlugin('block',    'TNTFormToggleBox', array('TNTFormToggleBox', '_TemplatePluginBlock'),  false);
//  $caller->RegisterPlugin('block',    'TNTFormCheckBox',  array('TNTFormCheckBox', '_TemplatePluginBlock'),   false);
//  $caller->RegisterPlugin('block',    'TNTFormRadioBox',  array('TNTFormRadioBox', '_TemplatePluginBlock'),   false);
//  $caller->RegisterPlugin('block',    'TNTFormTickBox',   array('TNTFormTickBox', '_TemplatePluginBlock'),    false);
//  $caller->RegisterPlugin('block',    'TNTFormData',      array('TNTFormData', '_TemplatePluginBlock'),       false);
//  $caller->RegisterPlugin('block',    'TNTFormTextBox',   array('TNTFormTextBox', '_TemplatePluginBlock'),    false);
//  $caller->RegisterPlugin('block',    'TNTFormUploadBox', array('TNTFormUploadBox', '_TemplatePluginBlock'),  false);
//  $caller->RegisterPlugin('block',    'TNTFormNumberBox', array('TNTFormNumberBox', '_TemplatePluginBlock'),  false);
//  $caller->RegisterPlugin('block',    'TNTFormPasswordBox',array('TNTFormPasswordBox', '_TemplatePluginBlock'), false);
//  $caller->RegisterPlugin('block',    'TNTFormListBox',   array('TNTFormListBox', '_TemplatePluginBlock'),    false);
//  $caller->RegisterPlugin('block',    'TNTFormToggleList',array('TNTFormToggleList', '_TemplatePluginBlock'), false);
//  $caller->RegisterPlugin('block',    'TNTFormButton',    array('TNTFormButton', '_TemplatePluginBlock'),     false);
//  $caller->RegisterPlugin('block',    'TNTFormImageButton',array('TNTFormImageButton', '_TemplatePluginBlock'), false);
//  $caller->RegisterPlugin('block',    'TNTFormMenuBar',   array('TNTFormMenuBar', '_TemplatePluginBlock'),    false);
//  $caller->RegisterPlugin('block',    'TNTFormMenu',      array('TNTFormMenu', '_TemplatePluginBlock'),       false);
//  $caller->RegisterPlugin('block',    'TNTFormTree',      array('TNTFormTree', '_TemplatePluginBlock'),       false);
//  $caller->RegisterPlugin('block',    'TNTFormOutBar',    array('TNTFormOutBar', '_TemplatePluginBlock'),     false);
//  $caller->RegisterPlugin('block',    'TNTFormToolBar',   array('TNTFormToolBar', '_TemplatePluginBlock'),    false);
//  $caller->RegisterPlugin('block',    'TNTFormListView',  array('TNTFormListView', '_TemplatePluginBlock'),   false);

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
function smarty_taglib_tntform($params, &$caller) {

    //require_once(dirname(__FILE__).'/import.tntform.php');
    
    smarty_import_tntform($params, $caller);

    //   Tag name          Template Command             Block
    $ret = array(
        'form'          => array('TNTForm',             true),
        'editbox'       => array('TNTFormEditBox',      true),
        'cell'          => array('TNTFormCell',         true),
//      'togglebox'     => array('TNTFormToggleBox',    true),
//      'checkbox'      => array('TNTFormCheckBox',     true),
//      'radiobox'      => array('TNTFormRadioBox',     true),
//      'tickbox'       => array('TNTFormTickBox',      true),
//      'data'          => array('TNTFormData',         true),
//      'textbox'       => array('TNTFormTextBox',      true),
//      'passwordbox'   => array('TNTFormPasswordBox',  true),
//      'numberbox'     => array('TNTFormNumberBox',    true),
//      'uploadbox'     => array('TNTFormUploadBox',    true),
//      'listbox'       => array('TNTFormListBox',      true),
//      'togglelist'    => array('TNTFormToggleList',   true),
//      'button'        => array('TNTFormButton',       true),
//      'imagebutton'   => array('TNTFormImageButton',  true),
//      'menubar'       => array('TNTFormMenuBar',      true),
//      'menu'          => array('TNTFormMenu',         true),
//      'tree'          => array('TNTFormTree',         true),
//      'outbar'        => array('TNTFormOutBar',       true),
//      'toolbar'       => array('TNTFormToolBar',      true),
//      'listview'      => array('TNTFormListView',     true),
    );

    return $ret;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
