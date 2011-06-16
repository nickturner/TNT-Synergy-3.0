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
 * System Class
 * ~~~~~~~~~~~~
 *
 * This class handles interaction with the underlying operating system.
 *
 * @package TNT.System
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');
TNTLoader::RequireClass('TNTFileSystem');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to handle the system interface.
 *
 * This class exists at all times. It is initialised during the loading
 * process. It can be derived from to added features.
 *
 * It contains methods to control the PHP environment.
 *
 * It also contains and a global data store used to configure the system
 * this datastore is automatically initalised with any 'system.ini' file
 * found in the installation directory.
 *
 * @package TNT.System
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTSystem extends TNTObject {

    /**
     * Description of operating system 
     * @var     string               operating system description 
     * @access  public
     */
    var $desc = '';

    /**
     * Type of operating system (WIN32 | UNIX)
     * @var     string               operating system type
     * @access  public
     */
    var $type = 'UNIX';

    /**
     * Version of operating system in "Major.Minor.Release" format
     * @var     string               operating system version
     * @access  public
     */
    var $version = null;

    /**
     * Line separator for text files on current operating system
     * @var     string              line separator
     * @access  public
     */
    var $lineSep = null;

    /**
     * Directory separator for filesystem paths on current operating system
     * @var     string              path separator
     * @access  public
     */
    var $pathSep = DIRECTORY_SEPARATOR;

    /**
     * List separator for path lists on current operating system
     * @var     string              list separator
     * @access  public
     */
    var $listSep = PATH_SEPARATOR;

    /**
     * File system uses case sensitive paths
     * @var     string              file system paths are case sensitive
     * @access  public
     */
    var $pathCaseSensitive = true;

    /**
     * System path - where the Synergy directory tree sits in filesystem
     * @var     string              path under which the code is installed
     * @access  public
     */
    var $systemPath = TNT_ROOT_DIR;

    /**
     * Server path - where the Synergy directory tree sits on the webserver
     * @var     string              path under which the code is served
     * @access  public
     */
    var $serverPath = '';

    /**
     * Internal flag to show this is singleton object
     * @var     string                  name of singleton class
     * @access  private
     * @see     Singleton()
     */
    var $__singleton = __CLASS__;

    /** 
     * Get a reference to this object - only a single instance should exist
     *
     * @static
     * @access  public
     * @return  object                  reference to global TNTApp object
     */
    /*static*/ function &Singleton() {
        return TNTObject::Singleton(__CLASS__);
    }

    /**
     * Get the include path
     *
     * The raw include path is returned formated as per the operating system
     * and per the PATH_SEPARATOR and DIRECTORY_SEPARATOR definitions.
     *
     * @access  public
     * @return  string              current value of the include path
     */
    function GetIncludePath() {
        return ini_get('include_path');
    }

    /**
     * Set the include path
     *
     * The raw include path is required formated as per the operating system
     * and per the PATH_SEPARATOR and DIRECTORY_SEPARATOR definitions.
     *
     * @access  public
     * @param   string  $value      new value of the include path
     * @return  string              original value of the include path
     */
    function SetIncludePath($value) {
        $inc_path = ini_get('include_path');
        ini_set('include_path', $value);
        return $inc_path;
    }

    /**
     * Add a directory to the PHP include path
     *
     * The directory can use either the DIRECTORY_SEPARATOR or the '/'
     * character where different.
     *
     * @access  public
     * @param   string  $dir        directory to add
     * @return  string              new value of the include path
     */
    function AddIncludePath($dir) {
        $inc_path = ini_get('include_path');

        if (DIRECTORY_SEPARATOR != '/') {
            $dir = str_replace('/', DIRECTORY_SEPARATOR, $dir);
        }

        $dirs = explode(PATH_SEPARATOR, $inc_path);
        if (!in_array($dirs)) {
           $dirs[] = $dir;
        }
        $inc_path = implode(PATH_SEPARATOR, $dirs);

        ini_set('include_path', $inc_path);

        return $inc_path;
    }

    /**
     * Delete a directory from the PHP include path
     *
     * The directory can use either the DIRECTORY_SEPARATOR or the '/'
     * character where different.
     *
     * @access  public
     * @param   string  $dir        directory to delete
     * @return  string              new value of the include path
     */
    function DelIncludePath($dir) {

        $inc_path = ini_get('include_path');

        if (DIRECTORY_SEPARATOR != '/') {
            $dir = str_replace('/', DIRECTORY_SEPARATOR, $dir);
        }

        if (($dirs = explode(PATH_SEPARATOR, $inc_path))) {
            $inc_path = implode(PATH_SEPARATOR, array_diff($dirs, array($dir)));
            ini_set('include_path', $inc_path);
        }

        return $inc_path;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTSystem() {

        TNTObject::TNTObject();

        // Work out some derived parameters
        $this->desc = php_uname();
        if (TNT_OS == 'WIN') {
            // Running on WIN32
            $this->type = 'WIN32';
            $this->pathCaseSensitive = false;
            $this->pathSep = DIRECTORY_SEPARATOR;
            $this->listSep = PATH_SEPARATOR;
            $this->lineSep = "\r\n";
        } else if (TNT_OS == 'MAC') {
            // Running on MAC
            $this->type = 'MAC';
            $this->pathCaseSensitive = true;
            $this->pathSep = DIRECTORY_SEPARATOR;
            $this->listSep = PATH_SEPARATOR;
            $this->lineSep = "\r";
        } else {
            // Assume on UNIX (or derivative)
            $this->type = 'UNIX';
            $this->pathCaseSensitive = true;
            $this->pathSep = DIRECTORY_SEPARATOR;
            $this->listSep = PATH_SEPARATOR;
            $this->lineSep = "\n";
        }

        // Attempt to work out the server path automatically
        $scriptSystemPath = @$_SERVER['SCRIPT_FILENAME'];
        $scriptServerPath = @$_SERVER['SCRIPT_NAME'];

        //TNTDebug::Dump('TNTSystem::TNTSystem() - systemPath='.$this->systemPath);
        //TNTDebug::Dump('TNTSystem::TNTSystem() - scriptSystemPath='.$scriptSystemPath);
        //TNTDebug::Dump('TNTSystem::TNTSystem() - scriptServerPath='.$scriptServerPath);

        $serverPath = null;

        $lenSystemPath = strlen($this->systemPath);
        if (strncmp($scriptSystemPath, $this->systemPath, $lenSystemPath) == 0) {
            // Remove system root path from system script path
            $script = substr($scriptSystemPath, $lenSystemPath);
            $lenScript = strlen($script);

            // Remove script path from end of server script path
            if (strcmp(substr($scriptServerPath, -$lenScript), $script) == 0) {
                $serverPath = substr($scriptServerPath, 0, -$lenScript); 
            }
        }

        if (is_null($serverPath)) {
            $this->serverPath = '';
        } else {
            $this->serverPath = $serverPath;
        }

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTSystem() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
