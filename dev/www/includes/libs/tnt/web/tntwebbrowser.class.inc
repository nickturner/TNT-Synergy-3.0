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
 * Browser Class
 * ~~~~~~~~~~~~~
 *
 * This class enables the application to gather information on the capabilities
 * of the browser that is running on the client.
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
TNTLoader::RequireClass('TNTVersion');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to enable the application to gather information on the capabilities
 * of the browser that is running on the client.
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTWebBrowser extends TNTObject {

    /**
     * Description of browser 
     * @var     string                  browser system description 
     * @access  public
     */
    var $desc = '';

    /**
     * Type of browser (IE|Netscape|Mozilla|Opera)
     * @var     string                  browser type
     * @access  public
     */
    var $type = '';

    /**
     * Version of the browser.
     * @var     TNTVersion              browser version
     * @access  public
     */
    var $version = new TNTVersion();

    /**
     * A value indicating whether the browser is a Web crawler search engine
     * @var     boolean                 true if Robot/Crawler, false otherwise
     * @access  public
     */
    var $isRobot = false;

    /**
     * Value indicating whether the client browser supports Active X controls.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsActiveX = false;

    /**
     * Value indicating whether the client browser supports background sounds.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsBgSound = false;

    /**
     * Value indicating whether the client browser supports cookies.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsCookies = false;

    /**
     * Value indicating whether the client browser supports frames.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsFrames = false;

    /**
     * Value indicating whether the client browser supports tables.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsTables = false;

    /**
     * Value indicating whether the client browser supports Java applets.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsJavaApplets = false;

    /**
     * Value indicating whether the client browser supports JavaScript.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsJavaScript = false;

    /**
     * Value indicating whether the client browser supports VBScript.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsVBScript = false;

    /**
     * Value indicating whether the client browser supports Microsoft DOM.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsMSDOM = false;

    /**
     * Value indicating whether the client browser supports W3C DOM.
     * @var     boolean                 true if supported, false otherwise
     * @access  public
     */
    var $supportsW3CDOM = false;

    /**
     * Version of JavaScript supported.
     * @var     TNTVersion              JavaScript version
     * @access  public
     */
    var $javaScriptVersion = new TNTVersion();
    
    /**
     * Version of Visual Basic Script supported.
     * @var     TNTVersion              VBScript version
     * @access  public
     */
    var $vbScriptVersion = new TNTVersion();

    /**
     * Version of MS Document Object Model (DOM) supported.
     * @var     TNTVersion              DOM version
     * @access  public
     */
    var $msDomVersion = new TNTVersion();

    /**
     * Version of W3C Document Object Model (DOM) supported.
     * @var     TNTVersion              DOM version
     * @access  public
     */
    var $w3cDomVersion = new TNTVersion();

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
     * @return  TNTWebBrowser&         reference to global TNTWebBrowser object
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
    function TNTWebBrowser() {

        TNTObject::TNTObject();

        // We use the phpSniff library to determine the browser capabilities
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTWebBrowser() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
