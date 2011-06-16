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
 * Core functions
 * ~~~~~~~~~~~~~~
 *
 * Some miscellaneous functions or comptibility functions that may or may
 * not exist depending on PHP version in use.
 *
 * @package TNT
 * @author  Nick Turner <nick@tntdisco.co.uk>
 * @since   06/07/06
 * @version 1.0
 */

/*
 * Dependencies
 */

/*
 * Globals
 */
global $null;
$null = null; // Read-Only null reference

global $tnt;
$tnt = array(
    'name' =>       'tnt',
    'starttime' =>  microtime(),
    '_profile' =>   isset($_GET['_tnt_profile']) ? $_GET['_tnt_profile'] :
                    isset($_REQUEST['_tnt_profile']) ? $_REQUEST['_tnt_profile'] :
                    null,
             );

/*
 * Defines
 */

/**
 * DIRECTORY_SEPARATOR represents the native directory separator
 */
if (!defined('DIRECTORY_SEPARATOR')) {
    define('DIRECTORY_SEPARATOR', strtoupper(substr(PHP_OS, 0, 3) == 'WIN') ? '\\' : '/');
}

/**
 * PATH_SEPARATOR represents the native separator in path lists
 */
if (!defined('PATH_SEPARATOR')) {
    define('PATH_SEPARATOR', strtoupper(substr(PHP_OS, 0, 3) == 'WIN') ? ';' : ':');
}

/**
 * PHP_EXT represents the extension that the PHP scripts are using.
 */
if (!defined('PHP_EXT')) {
    define('PHP_EXT', substr($_SERVER['PHP_SELF'], strrpos($_SERVER['PHP_SELF'], '.')));
}

/**
 * TNT_ROOT_DIR represents the directory in which TNT-Synergy is installed.
 * (using the common '/' directory separator and no trailing separator)
 * We can calculate the root as we know the following;
 *  '__FILE__ == <TNT_ROOT_DIR>/includes/libs/tnt/core/tnt.class.inc',
 */
if (!defined('TNT_ROOT_DIR')) {
    if (DIRECTORY_SEPARATOR != '/') {
        define('TNT_ROOT_DIR', rtrim(str_replace(DIRECTORY_SEPARATOR, '/', substr(__FILE__, 0, -strlen('/includes/libs/tnt/tnt.inc')))), '/');
    } else {
        define('TNT_ROOT_DIR', rtrim(substr(__FILE__, 0, -strlen('/includes/libs/tnt/tnt.inc'))), '/');
    }
}

/**
 * TNT_OS - which operating system type are we on UNIX, WIN or MAC
 */
if (stristr(PHP_OS, 'darwin')) {
    define('TNT_OS', 'MAC');
} else if (stristr(PHP_OS, 'win')) {
    define('TNT_OS', 'WIN');
} else if (stristr(PHP_OS, 'unix') || stristr(PHP_OS, 'linux')) {
    define('TNT_OS', 'UNIX');
} else {
    die('Unknown operator system type: '.PHP_OS.' ('.php_uname().')');
}

/**
 * TNT_REQUEST_SECURE indicates if the request was via a secure protocol
 * normally 'HTTPS'.
 */
if (!defined('TNT_REQUEST_SECURE')) {
    define('TNT_REQUEST_SECURE', ((isset($_SERVER['HTTPS']) && strtolower($_SERVER['HTTPS']) == 'on') ? true : false));
}

/**
 * TNT_REQUEST_PROTOCOL is the protocol used in the current request.
 */
if (!defined('TNT_REQUEST_PROTOCOL')) {
    define('TNT_REQUEST_PROTOCOL', (TNT_REQUEST_SECURE ? 'https' : 'http'));
}

/**
 * TNT_REQUEST_HOST is the host name part of the current request.
 */
if (!defined('TNT_REQUEST_HOST')) {
    define('TNT_REQUEST_HOST', $_SERVER['HTTP_HOST']);
}

/**
 * TNT_REQUEST_URI is the uri part of the current request.
 */
if (!defined('TNT_REQUEST_URI')) {
    define('TNT_REQUEST_URI', $_SERVER['REQUEST_URI']);
}

/**
 * TNT_REQUEST_PATH is the path component of the current request
 */
if (!defined('TNT_REQUEST_PATH')) {
    define('TNT_REQUEST_PATH', (isset($_SERVER['REDIRECT_URL']) ? $_SERVER['REDIRECT_URL'] : dirname($_SERVER['REQUEST_URI'])));
}

/**
 * TNT_REQUEST_QUERY is the query string part of the current request.
 */
if (!defined('TNT_REQUEST_QUERY')) {
    define('TNT_REQUEST_QUERY', $_SERVER['QUERY_STRING']);
}

/**
 * TNT_REQUEST is the full request made.
 */
if (!defined('TNT_REQUEST')) {
    define('TNT_REQUEST', TNT_REQUEST_PROTOCOL.'://'.TNT_REQUEST_HOST.TNT_REQUEST_URI);
}

/**
 * TNT_SCRIPT is the full path to the executing script
 * (name or normalised to use '/' as path separator)
 */
if (!defined('TNT_SCRIPT')) {
    if (DIRECTORY_SEPARATOR != '/') {
        define('TNT_SCRIPT', $_SERVER['SCRIPT_FILENAME']);
    } else {
        define('TNT_SCRIPT', str_replace(DIRECTORY_SEPARATOR, '/', $_SERVER['SCRIPTNAME']));
    }
}

/**
 * TNT_SCRIPT_PATH is the relative path to the executing script
 */
if (!defined('TNT_SCRIPT_PATH')) {
    define('TNT_SCRIPT_PATH', preg_replace('/^'.preg_quote(TNT_ROOT_DIR, '/').'/', '', TNT_SCRIPT));
}

/**
 * TNT_SCRIPT_NAME is the name of the executing script
 * (Script file name - no path or extension)
 */
if (!defined('TNT_SCRIPT_NAME')) {
    define('TNT_SCRIPT_NAME', basename(TNT_SCRIPT, PHP_EXT));
}


/**
 * TNT_SCRIPT_PATH represents the directory containing the current script,
 * relative to TNT_ROOT_DIR.
 * If the code is not relative to TNT_ROOT_DIR then this is an absolute path
 * to the script directory.
 */
if (!defined('TNT_SCRIPT_PATH')) {
    define('TNT_SCRIPT_PATH', preg_replace('/^'.preg_quote(TNT_ROOT_DIR, '/').'/i', '', dirname($_SERVER['SCRIPT_FILENAME']))); 
}

/**
 * TNT_SCRIPT_NAME represents the name of the current script file minus its
 * extension.
 */
if (!defined('TNT_SCRIPT_NAME')) {
    define('TNT_SCRIPT_NAME', basename($_SERVER['SCRIPT_FILENAME'], PHP_EXT)); 
}

/**
 * TNT_ROOT_URI represents the URI in which the code is installed.
 */
if (!defined('TNT_ROOT_URI')) {
  define('TNT_ROOT_URI', (substr($_SERVER['SCRIPT_NAME'], -strlen(TNT_SCRIPT_PATH)) == TNT_SCRIPT_PATH) ? substr($_SERVER['SCRIPT_NAME'], 0, -strlen(TNT_SCRIPT_PATH)) : null); 
}

/**
 * TNT_DATA_DIR represents the directory in which to store data files.
 * (Code should use ~data from the VFS and not this define)
 */
if (!defined('TNT_DATA_DIR')) {
  define('TNT_DATA_DIR', TNT_ROOT_DIR.'/data');
}


/**
 * TNT_TEMP_DIR represents the directory in which to store temporary files.
 * (Code should use ~temp from the VFS and not this define)
 */
if (!defined('TNT_TEMP_DIR')) {
  define('TNT_TEMP_DIR', TNT_ROOT_DIR.'/temp');
}

/**
 * TNT_CACHE_DIR represents the directory in which to store cache files.
 * (Code should use ~cache from the VFS and not this define)
 */
if (!defined('TNT_CACHE_DIR')) {
  define('TNT_CACHE_DIR', TNT_TEMP_DIR.'/cache');
}

/**
 * TNT_DEBUG_DIR represents the directory in which to store debug files.
 */
if (!defined('TNT_DEBUG_DIR')) {
  define('TNT_DEBUG_DIR', TNT_TEMP_DIR.'/debug');
}



// Include the miscellaneous and compatibility functions
require_once(TNT_ROOT_DIR.'/includes/libs/tnt/tntmisc.inc');

// Include the core classes
require_once(TNT_ROOT_DIR.'/includes/libs/tnt/core/tnt.class.inc');
require_once(TNT_ROOT_DIR.'/includes/libs/tnt/core/tntdebug.class.inc');
require_once(TNT_ROOT_DIR.'/includes/libs/tnt/core/tntclass.class.inc');
require_once(TNT_ROOT_DIR.'/includes/libs/tnt/core/tntloader.class.inc');

/**
 * Dump the arguments to screen
 *
 * @access public
 * @param  string       $str        string to escape
 * @return string                   escaped string
 */
function tnt_dump() {
  $args = func_get_args();
  echo '<pre class="tnt-dump">';
  foreach ($args as $arg) {
    if (is_string($arg)) {
      echo htmlentities($arg);
    } else {
      echo htmlentities(var_info($arg));
    }
  }
  echo '</pre>';
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
