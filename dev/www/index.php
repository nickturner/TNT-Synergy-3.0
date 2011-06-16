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
 * Index
 * ~~~~~
 *
 * This is the main entry point into the web based framework.
 *
 * @package TNT
 * @author  Nick Turner <nick@tntdisco.co.uk>
 * @since   06/07/06
 * @version 1.0
 */

/*
 * Dependencies
 */
require_once(dirname(__FILE__).'/includes/libs/tnt/tnt.inc');

/*
 * Globals
 */

/*
 * Defines
 */
define('__SITE__', 'tnt');

/* Report all errors and warnings */
error_reporting(E_ALL);

/*
 * Disable session.use_trans_sid to mitigate performance-penalty
 * (do it before any output is started)
 */
if (!defined('SID')) {
    @ini_set('session.use_trans_sid', 0);
}

TNT::Initialise(__SITE__, 1);

TNTLoader::RequireClass('TNTCtrlHttpFront');

$front =& TNTCtrlHttpFront::Singleton();
if (!$front->Main()) {
    echo '<pre>Error: '.htmlentities($front->Error()).'</pre>';
}

$ini =& TNT::Registry();

echo '<hr />';
tnt_dump('TNT_ROOT_DIR: ', TNT_ROOT_DIR);
tnt_dump('TNT_ROOT_URI: ', TNT_ROOT_URI);
tnt_dump('Config: ', var_info($ini->Items()));
echo '<hr />';

TNT::Shutdown();

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
