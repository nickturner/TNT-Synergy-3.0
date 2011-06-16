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
 * Control Module Class
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * Defines the framework for handling modules.
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to define the module environment.
 *
 * A single instance of this class is created by the front end controller
 * to handle an requested action intended for this module.
 *
 * @package TNT.Ctrl
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlModule extends TNTObject {

    /**
     * Path of module relative to SYNERGY_DIR
     * @var     string                  module path
     * @access  public
     */
    var $path = TNT_SCRIPT_PATH;

    /**
     * Name of module (Defaults to file name minus extension)
     * @var     string                  module name
     * @access  public
     */
    var $name = TNT_SCRIPT_NAME;

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
     * @return  object                  reference to global TNTWebApp object
     */
    /*static*/ function &Singleton() {
        return TNTClass::Singleton(__CLASS__);
    }

    /**
     * Construct the object
     *
     * Supported construction options are;
     *   + mixed  'inifile'     - initialisation file
     *   + array[] 'inivars'    - initialisation variables
     *
     * 'inifile' can be an array of initialisation file names or a comma
     * separated string of initialisation file names.
     *
     * This initialises the following member variables;
     *   + {@link $server} - reference to TNTWebServer object
     *   + {@link $client} - reference to TNTWebClient object
     *   + {@link $request} - reference to TNTWebRequest object
     *   + {@link $response} - reference to TNTWebResponse object
     *   + {@link $session} - reference to TNTWebSession object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlModule($options = null) {
        TNTObject::TNTObject();
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlModule() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
