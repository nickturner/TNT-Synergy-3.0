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
 * Client Class
 * ~~~~~~~~~~~~
 *
 * This class handles a the basic functionaly required by the HTTP client.
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
 * Class to provide access to client information.
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTWebClient extends TNTObject {

    /**
     * A reference to the clients browser capabilities
     * @var     TNTBrowser&             reference to browser object
     * @access  public
     */
    var $browser = null;

    /**
     * A data storage area on the client.
     * @var     TNTVarMap&              reference to map of stored variables
     * @access  public
     */
    var $store = null;

    /**
     * A string array of client-supported MIME accept types
     * @var     string                  array of MIME types
     * @access  public
     */
    var $acceptTypes = null;

    /**
     * A string array of client-supported Encoding types
     * @var     string                  array of Encoding types
     * @access  public
     */
    var $acceptEncodings = null;

    /**
     * A string array of client-supported Character sets
     * @var     string                  array of Character set codes
     * @access  public
     */
    var $acceptCharsets = null;

    /**
     * A sorted string array of client language preferences.
     * @var     string                  array of language codes
     * @access  public
     */
    var $acceptLanguages = null;

    /**
     * The host address of the client
     * @var     string                  the host name or ip address
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
     * The port number of the client
     * @var     integer                 the port number
     * @access  public
     */
    var $port = 0;

    /**
     * The agent identification string send from the client
     * @var     string                  the agent string
     * @access  private
     */
    var $agent = null;
   
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
     * Set a trustee to be associated with this client.
     *
     * This storage is persistant and allows clients to be associated with
     * trustees for auto authentication purposes.
     *
     * @access  public
     * @param   TNTTrustee& $trustee    reference to trustee to store
     * @return  boolean                 true if successfull, false otherwise
     */
    function SetTrustee($trustee) {

        $ret = false;
        
        // Start error section
        do {
            if (!implements($trustee, 'tnttrustee')) {
                $this->_SetError('Invalid Trustee');
                break;
            }
        
            // Store the trustee.
            if (!($data = TNTTrustee::Store($trustee))) {
                $this->_SetError('Could not get Trustee data to store');
                break;
            }

            $this->store->SetVar('_tnt_trustee', $data);
            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Retreive trustee credentials of the client.
     *
     * Get the trustee stored using {@link SetTrustee()}, returns a
     * new copy of the stored trustee.
     *
     * @access  public
     * @return  TNTTrustee&             reference to trustee stored
     */
    function GetTrustee() {

        if ($this->store->HasVar('_tnt_trustee')) {
            $data = $this->store->GetVar('_tnt_trustee');

            // Create and authenticate the correct stored trustee
            $ret =& TNTTrustee::Restore($data);
        }

        return $ret;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTWebClient() {

        TNTObject::TNTObject();

        //$this->browser =& new TNTWebBrowser();

        $this->acceptTypes = @explode(',', (array)$_SERVER['HTTP_ACCEPT']);

        $this->acceptLanguages = @explode(',', (array)$_SERVER['HTTP_ACCEPT_LANGUAGE']);

        $this->acceptEncodings = @explode(',', (array)$_SERVER['HTTP_ACCEPT_ENCODING']);

        $this->acceptCharsets = @explode(',', (array)$_SERVER['HTTP_ACCEPT_CHARSET']);
        $this->agent = @$_SERVER['HTTP_USER_AGENT'];

        $this->ip = @$_SERVER['REMOTE_ADDR'];
        $this->port = @$_SERVER['REMOTE_PORT'];
        $this->host = @$_SERVER['REMOTE_HOST'];

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTWebClient() {

        destroy($this->browser);

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
