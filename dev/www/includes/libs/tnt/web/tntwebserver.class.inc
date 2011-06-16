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
 * Server Class
 * ~~~~~~~~~~~~
 *
 * This class handles interaction with the underlying server. Understands
 * the PHP and CGI interfaces etc.
 *
 * @package TNT.Web
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
 * Class to handle the server interface
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTWebServer extends TNTObject {

    /**
     * Description of web server
     * @var     string               web server description
     * @access  public
     */
    var $desc = null;

    /**
     * Type of web server (APACHE | IIS)
     * @var     string               web server type
     * @access  public
     */
    var $type = null;

    /**
     * Version of web server in "Major.Minor.Release" format
     * @var     string               web server version
     * @access  public
     */
    var $version = null;

    /**
     * Version of PHP Script in "Major.Minor.Release" format
     * @var     string               PHP version
     * @access  public
     */
    var $phpVersion = null;

    /**
     * Server Operating System
     * @var     TNTSystem&          reference to operating system object
     * @access  public
     */
    var $system = null;

    /**
     * Directory where code installed (ie. filesystem path to /index.php)
     * (Alias to {@link TNTSystem::$systemPath})
     * @var     string              source directory
     * @access  public
     */
    var $srcDir = TNT_ROOT_DIR;

    /**
     * URI where code installed (ie. webserver path to /index.php)
     * @var     string              webserver directory
     * @access  public
     */
    var $webDir = null;

    /**
     * Name of index file
     * @var     string              index file
     * @access  public
     */
    var $index = null;

    /**
     * The host address of the server
     * @var     string              the host name or ip address
     * @access  public
     */
    var $host = null;

    /**
     * The IP address of the client
     * @var     string                  the ip address
     * @access  public
     */
    var $ip = null;
    
    /**
     * The port number of the server
     * @var     integer             the port number
     * @access  public
     */
    var $port = null;

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
     * @access  public
     * @return  TNTSession&         reference to global TNTSession object
     * @static
     */
    /*static*/ function &Singleton() {
        return TNTObject::Singleton(__CLASS__);
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTWebServer() {

        TNTObject::TNTObject();

        TNTLoader::RequireClass('TNTSystem');
        $system =& TNTSystem::Singleton();

        $this->srcDir =& $system->systemPath;
        $this->webDir =& $system->serverPath;

        $this->reqUri = @$_SERVER['REQUEST_URI'];

        $this->index = '/index.'.PHP_EXT;

        $this->desc = @$_SERVER['SERVER_SOFTWARE'];

        if (preg_match('/Apache\/([\d.]*)/', $this->desc, $matches)) {
            $this->type = 'APACHE';
            $this->version = $matches[1];
        }

        $this->phpVersion = phpversion();

        $this->ip = @$_SERVER['SERVER_ADDR'];
        $this->host = $this->ip;
        $this->port = @$_SERVER['SERVER_PORT'];

        if (!empty($_SERVER['HTTP_HOST'])) {
            list($this->host, $this->port) = explode(':', $_SERVER['HTTP_HOST']);
        }

        //TNTDebug::Dump('TNTWebServer::TNTWebServer(): desc = '.$this->desc);
        //TNTDebug::Dump('TNTWebServer::TNTWebServer(): type = '.$this->type);
        //TNTDebug::Dump('TNTWebServer::TNTWebServer(): version = '.$this->version);
        //TNTDebug::Dump('TNTWebServer::TNTWebServer(): srcDir = '.$this->srcDir);
        //TNTDebug::Dump('TNTWebServer::TNTWebServer(): webDir = '.$this->webDir);
        //TNTDebug::Dump('TNTWebServer::TNTWebServer(): reqUri = '.$this->reqUri);
        //TNTDebug::Dump('TNTWebServer::TNTWebServer(): index = '.$this->index);

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTWebServer() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
