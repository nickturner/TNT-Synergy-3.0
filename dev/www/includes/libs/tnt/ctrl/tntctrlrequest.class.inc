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
 * Ctrl Request Class
 * ~~~~~~~~~~~~~~~~~~
 *
 * This class encapsulates request information.
 *
 * @package TNT.Ctrl
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTVarMap');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to encapsulate request information.
 *
 * The methods and properties of this class are exposed through the
 * {@link TNTCtrlFront::request} property of the {@link TNTCtrlFront} class.
 *
 * It is also a singleton class and can be accessed via the static
 * method {@link TNTCtrlRequest::Singleton()}.
 *
 * @package TNT.Ctrl
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlRequest extends TNTVarMap {

    /**
     * The host address the request came from
     * @var     string                  the host name or ip address
     * @access  public
     */
    var $host = null;

    /**
     * The IP address the request came from
     * @var     string                  the ip address
     * @access  public
     */
    var $ip = null;
    
    /**
     * The port number the request came from
     * @var     integer                 the port number
     * @access  public
     */
    var $port = 0;

    /**
     * Flag to say whether the request has been dispatched to a controller
     * @var     boolean                 true if dispatched
     * @access  public
     */
    var $isDispatched = false;

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
     * Get the name of the Module requested.
     *
     * This is short hand for $this->GetVar('module', 'default');
     *
     * @access  public
     * @return  string              module name
     * @static
     */
    function GetModule() {
        return $this->GetVar('module', 'default');
    }

    /** 
     * Set the name of the Module requested.
     *
     * This is short hand for $this->SetVar('module', $name);
     *
     * @access  public
     * @param   string      $name       module name
     * @return  boolean                 true if set successfull
     * @static
     */
    function SetModule($name) {
        return $this->SetVar('module', $name);
    }

    /** 
     * Get the name of the Controller requested.
     *
     * This is short hand for $this->GetVar('controller', 'index');
     *
     * @access  public
     * @return  string              controller name
     * @static
     */
    function GetController() {
        return $this->GetVar('controller', 'index');
    }

    /** 
     * Set the name of the Controller requested.
     *
     * This is short hand for $this->SetVar('controller', $name);
     *
     * @access  public
     * @param   string      $name       controller name
     * @return  boolean                 true if set successfull
     * @static
     */
    function SetController($name) {
        return $this->SetVar('controller', $name);
    }

    /** 
     * Get the name of the Action requested.
     *
     * This is short hand for $this->GetVar('action', 'index');
     *
     * @access  public
     * @return  string              action name
     * @static
     */
    function GetAction() {
        return $this->GetVar('action', 'index');
    }

    /** 
     * Set the name of the Action requested.
     *
     * This is short hand for $this->SetVar('action', $name);
     *
     * @access  public
     * @param   string      $name       action name
     * @return  boolean                 true if set successfull
     * @static
     */
    function SetAction($name) {
        return $this->SetVar('action', $name);
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlRequest() {
        TNTVarMap::TNTVarMap();

        $this->ip = @$_SERVER['REMOTE_ADDR'];
        $this->port = @$_SERVER['REMOTE_PORT'];
        $this->host = @$_SERVER['REMOTE_HOST'];
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlRequest() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
