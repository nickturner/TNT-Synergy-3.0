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
 * Web App Class
 * ~~~~~~~~~~~~~
 *
 * Defines the framework for handling web applications.
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
 * Class to define the application environment.
 *
 * A single instance of this class is create when a Web request is processed.
 * The instance lasts for the duration of the request.
 *
 * The class uses the following singleton classes to help in the processing
 * of the request.
 *  + {@link TNTWebClient} - represents the client that sent the request.
 *  + {@link TNTWebServer} - represents the server that received the request.
 *  + {@link TNTWebRequest} - represents the actual request sent.
 *  + {@link TNTWebResponse} - represents the actual response to return.
 *  + {@link TNTWebSession} - represents the session between client and server.
 *
 * Each of these classes has a datastore whose use is as follows;
 *  + {@link TNTWebRequest} - data sent from client as part of the request.
 *  + {@link TNTWebResponse} - data to store on client as part of response.
 *  + {@link TNTWebSession} - data stored on server for this session only.
 *
 * Data sent in the request will consist of data stored at the client for
 * this server (cookies) and any query, form and file sent in the request.
 *
 * Data sent in the response is stored on the client using HTTP permanent
 * cookies for sending back in subsequest requests. To use any other type
 * of cookie then use {@link TNTWebClient::StoreCookie()} directly. Null values
 * are not stored as cookies and any existing cookie will be deleted.
 *
 * The application triggers various events in the following sequence.
 *  + {@link Init()} - Initialise the application
 *  + {@link Auth()} - Authenticate and Authorize the request
 *  + {@link Exec()} - Execute the request
 *  + {@link Term()} - Terminate the application
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTWebApp extends TNTObject {

    /**
     * Path of application relative to SYNERGY_DIR
     * @var     string               application path
     * @access  public
     */
    var $path = TNT_SCRIPT_PATH;

    /**
     * Name of application (Defaults to file name minus extension)
     * @var     string               application name
     * @access  public
     */
    var $name = TNT_SCRIPT_NAME;

    /**
     * Reference to the intrinsic session object for the current request.
     * Can also be obtained using TNTWebSession::Singleton().
     * @var     TNTWebSession&          reference to session object
     * @access  public
     */
    var $session = null;

    /**
     * Reference to the client that originated the request
     * Can also be obtained using TNTWebClient::Singleton().
     * @var     TNTWebClient&           reference to client object
     * @access  public
     */
    var $client = null;

    /**
     * Reference to the server that received the request
     * Can also be obtained using TNTWebServer::Singleton().
     * @var     TNTWebServer&           reference to server object
     * @access  public
     */
    var $server = null;

    /**
     * Reference to the intrinsic request object for the current request.
     * Can also be obtained using TNTWebRequest::Singleton().
     * @var     TNTWebRequest&          reference to request object
     * @access  public
     */
    var $request = null;

    /**
     * Reference to the intrinsic response object for the current request.
     * Can also be obtained using TNTWebResponse::Singleton().
     * @var     TNTWebResponse&         reference to response object
     * @access  public
     */
    var $response = null;

    /**
     * Reference to the intrinsic user object for the current request..
     * @var     TNTObject&              reference to TNTTrustee implementor
     * @access  public
     */
    var $user = null;

    /**
     * Reference to the intrinsic locale object.
     * Can also be obtained using TNTLocale::Singleton().
     * @var     TNTLocal&               reference to TNTLocale object
     * @access  public
     */
    //var $locale = null;

    /**
     * Reference to the intrinsic permissions object.
     * Can also be obtained using TNTPerm::Singleton().
     * @var     object              reference to TNTPerm object
     * @access  public
     */
    //var $perm = null;

    /**
     * Reference to the intrinsic theme object
     * Can also be obtained using TNTLocale::Singleton().
     * @var     object              reference to TNTTheme object
     * @access  public
     */
    //var $theme = null;

    /**
     * Initialisation variable store.
     * @var     TNTIni&                 reference to ini store object
     * @access  public
     */
    var $ini = null;

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
     * Initialise the application.
     *
     * @access  public
     * @return  boolean                 true if initialisation successfull
     */
    function Init() {
        TNT::Timestamp('TNTWebApp::Init()');

        $ret = true;

        /*
         * Permissions handler (only ever one instance).
         */
        //TNT::Timestamp('TNTWebApp::Init() - initialising TNTPerm');
        //TNTLoader::RequireClass('TNTPerm');
        //$this->perm =& TNTPerm::Singleton();

        /*
         * Initialise the site locale information (only ever one instance).
         */
        //TNT::Timestamp('TNTWebApp::Init() - initialising TNTLocal');
        //TNTLoader::RequireClass('TNTLocale');
        //$this->locale =& TNTLocale::Singleton();

        /*
         * Initialise the theme information
         */
        //TNT::Timestamp('TNTWebApp::Init() - initialising TNTTheme');
        //TNTLoader::RequireClass('TNTTheme');
        //$this->theme =& TNTTheme::Singleton();

        TNT::Timestamp('TNTWebApp::Init() - done');
        return $ret;
    }

    /**
     * Authenticate and Authorize the request
     *
     * This will firstly check to see if the current session has details of
     * a registered owner (trustee). If so it will attempt to authenticate that
     * owner. Secondly if there is no registered owner in the session it will
     * check to see if the request has sent a registered owner and
     * then attempt to authenticate that owner.
     *
     * This will set up the {@link $user} member variable to represent the
     * trustee that is the current request owner.
     *
     * If there is no registered owner for this request then {@link $user}
     * will be set to the 'anonymous' trustee, and $user->IsAuthenticated()
     * will be false and $user->IsAnonymous() will be true.
     *
     * If there is a registered owner but the authentication of that owner
     * fails then the $user will be the correct owner but
     * $user->IsAuthenticated() will be false. 
     *
     * If processing of this request is not to be authorised due to the user
     * having insufficient permissions or privileges then the method will set
     * up the {@link response} to return a 'permission denied' status, however
     * derived classes may choose to set the {@link response} differently.
     *
     * NB: If a request is blocked due to the client or user being banned from
     * accessing the application then the request will be aborted by calling
     * {@link TNTWebApp::Abort()} with a suitable reason message.
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function Auth() {
        TNT::Timestamp('TNTWebApp::Auth()');

        $ret = false;

        // Start error section
        do {
            TNTLoader::RequireClass('TNTBans');

            $bans =& new TNTBans();

            // Check that the client is not banned
            if ($bans->IsBannedClient($this->client->ip)) {
                // Terminate the request immediately
                $this->Abort('Client is banned: '.$this->client->ip);
            }

            // Is there a trustee stored in the session
            $reason = TNT_TRUSTEE_SESSION;
            $data = $this->session->GetVar('_tnttrustee');

            if (!$data) {
                // Is there a trustee stored in the request
                $reason = TNT_TRUSTEE_REQUEST;
                $data = $this->request->GetVar('_tnttrustee');
            }

            if ($data) {
                // Restore and authenticate the trustee
                TNTLoader::RequireClass('TNTTrustee');
                $this->user =& TNTTrustee::Restore($reason, $data);
            }

            if (!$this->user) {
                // Create anonymous trustee (GenericUser)
                TNTLoader::RequireClass('TNTGenericUser');
                $this->user =& new TNTGenericUser();
            }

            // Check the user is not banned
            if ($bans->IsBannedUser($this->user->DN())) {
                // Terminate the request immediately
                $this->Abort('User is banned: '.$this->user->DN());
            }

            // Check the user has permission to execute this application
            /* TODO
            if (!$this->user->IsPermitted('app:', TNT_PERM_EXECUTE)) {
                $this->_SetError('Insufficient permissions to excute app');
                break;
            }
            */

            $ret = true;

        } while (false);
        // End error section

        if ($bans) {
            destroy($bans);
        }

        TNT::Timestamp('TNTWebApp::Auth() - done');
        return $ret;
    }

    /**
     * Execute the request
     *
     * This default method looks for an 'op' request variable and uses its
     * value as the name of an operation to perform.
     *
     * The method will try to call any member function with the same name
     * as the operation prefixed with 'Op' - derived classes can either
     * override this method or create the required functions. 
     *
     * If no 'op' request variable is present then it will call the method
     * 'OpDefault()'.
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function Exec() {
        TNT::Timestamp('TNTWebApp::Exec()');

        // Start of error scope which we will break out of on error
        do {
            // See what operation we have to perform
            $op = $this->request->GetVar('op', 'default');

            // By default we try to call an member function with the same name
            // as the operation prefixed with 'Op' - derived classes can either
            // override this method or create the required functions. 
            $method = 'Op'.preg_replace('/[^\w]/', '_', $op);
            if (!method_exists($this, $method)) {
                $this->SetError('Unknown operation: '.$op);
                break;
            }

            $ret = $this->$method();

        } while (false);
        // End of error scope

        TNT::Timestamp('TNTWebApp::Exec() - done');
        return $ret;
    }

    /**
     * Terminate the application
     *
     * This base method will close the response sending it to the client.
     *
     * @access  public
     * @return  boolean                 true if initialisation successfull
     */
    function Term() {
        TNT::Timestamp('TNTWebApp::Term()');

        // Close the response - this writes out the client data
        if ($this->response) {
            $this->response->Close();
        }

        // Close the session - this writes out the session data
        if ($this->session) {
            $this->session->Close();
        }

        $ret = true;

        TNT::Timestamp('TNTWebApp::Term() - done');
        return $ret;
    }

    /**
     * Adds an alert to the output stream.
     *
     * Passing NULL will clear all alerts !!!
     *
     * @access  public
     * @param   string          $msg    message to add (or null)
     * @return  void
     */
    function Alert($msg) {
        if ($this->response) {
            $this->response->Alert($msg);
        }
    }

    /**
     * Main entry point of the application.
     *
     * Construct an application and then call main to start the application
     * running.
     *
     * Calls {@link Init()}, {@link Auth()}, {@link Exec()} then {@link Term()}
     * in that order stopping at the first to fail.
     *
     * If {@link Auth()} fails then the application can be set to automatically
     * redirect the user to a login page or to return a 'HTTP 400 permission
     * denied' status to the client.
     *
     * @access  public
     * @return  boolean                 true if all successfull
     */
    function Main() {

        $ret = false;

        // Start error section
        do {
            // Intialise the application
            TNT::Timestamp('TNTWebApp::Main() - initialising');
            if (!$this->Init()) {
                break;
            }

            // Authenticate and Authorize the request
            TNT::Timestamp('TNTWebApp::Main() - authorising');
            if (!$this->Auth()) {
                break;
            }

            // Execute the request
            TNT::Timestamp('TNTWebApp::Main() - executing');
            if (!$this->Exec()) {
                break;
            }

            $ret = true;

        } while(false);
        // End error section

        // Terminate the application
        TNT::Timestamp('TNTWebApp::Main() - terminating');
        $this->Term();

        return $ret;
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
    function TNTWebApp($options = null) {

        TNTObject::TNTObject();

        if ($options) {
            $options = array_change_key_case($options, CASE_LOWER);
        }

        if (isset($options['inifile'])) {
            TNT::IniLoad($options['inifile']);
        }

        if (isset($options['inivars']) && is_array($options['inivars'])) {
            TNT::SetIni($options['inivars']);
        }

        /*
         * Server information (only ever one instance).
         */
        TNT::Timestamp('TNTWebApp::Init() - initialising TNTWebServer');
        TNTLoader::RequireClass('TNTWebServer');
        $this->server =& TNTWebServer::Singleton();

        /*
         * Client information (only ever one instance).
         */
        TNT::Timestamp('TNTWebApp::Init() - initialising TNTWebClient');
        TNTLoader::RequireClass('TNTWebClient');
        $this->client =& TNTWebClient::Singleton();

        /*
         * Request information (only ever one instance).
         */
        TNT::Timestamp('TNTWebApp::Init() - initialising TNTWebRequest');
        TNTLoader::RequireClass('TNTWebRequest');
        $this->request =& TNTWebRequest::Singleton();

        /*
         * Response information (only ever one instance).
         */
        TNT::Timestamp('TNTWebApp::Init() - initialising TNTWebResponse');
        TNTLoader::RequireClass('TNTWebResponse');
        $this->response =& TNTWebResponse::Singleton();

        /*
         * Initialise session handler (only ever one instance).
         */
        TNT::Timestamp('TNTWebApp::Init() - initialising TNTWebSession');
        TNTLoader::RequireClass('TNTWebSession');
        $this->session =& TNTWebSession::Singleton();
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTWebApp() {
        destroy($this->session);
        destroy($this->response);
        destroy($this->request);
        destroy($this->client);
        destroy($this->server);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
