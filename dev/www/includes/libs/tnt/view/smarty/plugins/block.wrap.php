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
 * Smarty Block Plugin: Wrap
 * ~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Smarty Block Plugin to wrap its contents within another template.
 *
 * @package TNT.View
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
 * Smarty Block Plugin to wrap its contents within another template.
 *
 * @access  public
 * @param   string[]    $params     parameters passed to import directive
 * @param   string      $content    content of block
 * @param   object      $caller     reference to calling Smarty class
 * @param   boolean     $repeat     set to true to repeat the block
 * @return  string                  content to replace block in output
 */
function smarty_block_wrap($params, $content, &$smarty, &$repeat) {
    if (empty($params['file'])) {
        $smarty->_syntax_error("missing 'file' attribute in wrap tag", E_USER_ERROR, __FILE__, __LINE__);
    }

    $file = $params['file'];
    unset($params['file']);

    if (extname($file) == '') {
        $file .= '.tpl';
    }

    $assign = null;
    if (!empty($params['assign'])) {
        $assign = $params['assign'];
        unset($params['assign']);
    }

    $insert = 'content';
    if (!empty($params['insert'])) {
        $insert = $params['insert'];
        unset($params['insert']);
    }

    // Remember the template variables
    $_smarty_tpl_vars = $smarty->_tpl_vars;

    $params[$insert] = $content;

    ob_start();

    $smarty->_smarty_include(array(
        'smarty_include_tpl_file' => $file,
        'smarty_include_vars' => $params));

    $output = ob_get_contents();
    ob_end_clean();

    $smarty->_tpl_vars = $_smarty_tpl_vars;
    unset($_smarty_tpl_vars);

    if ($assign) {
        $smarty->assign($assign, $output);
        return '';
    }

    return $output;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
