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
 * Control Front Class
 * ~~~~~~~~~~~~~~~~~~~
 *
 * TNTCtrlFront is an abstract class which implements a Front Controller
 * pattern used in Model-View-Controller (MVC) applications. Its purpose is to
 * initialize the request environment, route the incoming request, and then
 * dispatch any discovered actions; it aggregates any responses and returns
 * them when the process is complete. 
 *
 * The class also implements the Singleton pattern, meaning only a single
 * instance of it may be available at any given time.
 *
 * To use the the class you need to derive from it and initialise the correct
 * request, response, etc. objects in its constructor. An example of a subclass
 * is provided by the {@link TNTCtrlHttpFront} class which will use standard
 * HTTP requests and return HTTP responses.
 *
 * @package TNT.Ctrl
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
 * Class to process a front end request.
 *
 * A single instance of this class is created to handle a request.
 *
 * The class uses the following singleton classes to help in the processing
 * of the request.
 *  + {@link TNTCtrlRequest} - represents the actual request sent.
 *  + {@link TNTCtrlResponse} - represents the actual response to return.
 *
 * Each of these classes has a datastore whose use is as follows;
 *  + {@link TNTCtrlRequest} - data sent from client as part of the request.
 *  + {@link TNTCtrlResponse} - data to store on client as part of response.
 *
 * The class executes the following methods in the sequence given in order
 * to handle the action. You can derive from this class to override the
 * default methods when required.
 *  + {@link Init()} - Initialise the controller
 *  + {@link Auth()} - Authenticate the request
 *  + {@link Exec()} - Execute the request
 *  + {@link Term()} - Terminate the controller
 *
 * @package TNT.Ctrl
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlFront extends TNTObject {

    /**
     * Reference to the intrinsic request object for the current request.
     * Can also be obtained using TNTCtrlRequest::Singleton().
     * @var     TNTCtrlRequest&         reference to request object
     * @access  public
     */
    var $request = null;

    /**
     * Reference to the intrinsic response object for the current request.
     * Can also be obtained using TNTCtrlResponse::Singleton().
     * @var     TNTCtrlResponse&        reference to response object
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
     * @return  object                  reference to global TNTCtrlFront object
     */
    /*static*/ function &Singleton() {
        return TNTObject::Singleton(__CLASS__);
    }

    /**
     * Check to see if we can route the current request.
     *
     * If $this->request is not set then the method returns false.
     *
     * This will get the path from the request and attempt to match it to any
     * of the defined routes, stopping at the first one it matches.
     *
     * Routes are defined in the registry entry 'TNT.Ctrl.Routes'.
     *
     * @access  public
     * @return  boolean                 true if request is routable.
     */
    function Route() {

        $ret = false;

        // Start error section
        do {
            if (!$this->request) {
                $this->_SetError('No request');
                break;
            }
            
            if ($this->request->HasVar('controller')) {
                // Request has already set which controller to use
                $ret = true;
                break;
            }

            if (!($path = $this->request->Path())) {
                $this->_SetError('No request path');
                break;
            }

            if (!($routes = TNT::RegGetVar('TNT.Ctrl.Routes'))) {
                $this->_SetError('No routes');
                break;
            }

            // The path is checked with no leading and trailing slashes
            $path = trim($path, '/');

            // We find all matching routes and then take the most important one

            $routes = array();
            foreach ($routes as $pattern => $route) {
                if (!preg_match('/^\s*([^\s]*)(?\s*\[([^]]*\]\s*$/', $route, $matches)) {
                    TNT::Debug('TNBTCtrlFront::Route() - invalid route: '.$route);
                    continue;
                }

                $params = $matches[1];
                $flags = $matches[2];

                $ignore_case = ($flags && (strpos($flags, 'i') !== false));
                $important = ($flags && (strpos($flags, 'i') !== false));
                $expand_head = ($flags && (($p = strpos($flags, '<')) !== false) && isset($flags{$p + 1}) ? $flags{$p + 1} : false);
                $expand_tail = ($flags && (($p = strpos($flags, '>')) !== false) && isset($flags{$p + 1}) ? $flags{$p + 1} : false);

                $qry = $pfx = $sfx = null;

                if ($pattern{0} == '?') {
                    // Assume a regular expression route
                    if ($ignore_case) {
                        $pattern .= 'i';
                    }
                    if (preg_match($pattern, $path, $matches, PREG_OFFSET_CAPTURE)) {
                        // A match was found
                        $qry = preg_replace($pattern, $params, $path);
                        $pfx = null;
                        $sfx = null;
                    }
                } else {
                    // Assume a directory based route
                    $pd = explode('/', $path);
                    $rd = explode('/', trim($pattern, '/'));
                }
            }
        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Check if permitted to execute the action
     *
     * If $this->user is set then the method returns the result of a call
     * to the {@link TNTUser::IsPermitted()} method for the user with the
     * correct ACL resource name for this action.
     *
     * If $this->user is not set then the method returns true.
     *
     * @access  public
     * @param   string      $action     name of action
     * @param   string      $controller name of controller
     * @param   string      $module     name of module
     * @return  boolean                 true if initialisation successfull
     */
    function IsPermitted($action, $controller, $module) {
        $ret = true;
        /* TODO
        if ($this->user) {
            $ret = $this->user->IsPermitted("action:$module/$controller/$action", TNT_PERM_EXECUTE);
        }
         */
        return $ret;
    }

    /**
     * Return a reference to an ActionController to handle the current
     * request. This is an object derived from the {@link TNTCtrlAction}
     * class.
     *
     * This default method assumes the class name is formatted as
     * '<module><controller>' and attempts to load such a class.
     *
     * It will then return an instance of the class.
     *
     * @access  public
     * @param   string      $action     name of action
     * @param   string      $controller name of controller
     * @param   string      $module     name of module
     * @return  boolean                 true if initialisation successfull
     */
    function &ActionController($action, $controller, $module) {
        $ret = null;

        $class = "{$module}{$controller}Controller";

        TNTLoader::RequireClass($class, false);

        if (class_exists($class)) {
            $ret =& new $class();
        }

        return $ret;
    }

    /**
     * Initialise the front controller.
     *
     * @access  public
     * @return  boolean                 true if initialisation successfull
     */
    function Init() {
        TNT::Timestamp('TNTCtrlFront::Init()');

        $ret = true;

        TNT::Timestamp('TNTCtrlFront::Init() - done');
        return $ret;
    }

    /**
     * Authenticate the request
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
     * accessing the system then the request will be aborted by calling
     * {@link TNTCtrlFront::Abort()} with a suitable reason message.
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function Auth() {
        return true;
/*
        TNT::Timestamp('TNTCtrlFront::Auth()');

        $ret = false;


        // Start error section
        do {
            TNTLoader::RequireClass('TNTBans');

            $bans =& new TNTBans();

            // Check that requests from the given IP address are allowed.
            if ($bans->IsBannedClient($this->request->ip)) {
                // Terminate the request immediately
                $this->Abort('Client is banned: '.$this->request->ip);
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

            $ret = true;

        } while (false);
        // End error section

        if ($bans) {
            destroy($bans);
        }

        TNT::Timestamp('TNTCtrlFront::Auth() - done');
        return $ret;
*/
    }

    /**
     * Execute the request
     *
     * This default method pulls the Module/Controller/Action name out of the
     * request and checks to see if the current user is permitted to execute
     * this action. If they are the it attempts to create an ActionController
     * to handle this action.
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function Exec() {
        TNT::Timestamp('TNTCtrlFront::Exec()');

        $ret = false;

        // Start of error scope which we will break out of on error
        do {
            if (!$this->request) {
                $this->_SetError('No request object');
                break;
            }

            if ($this->request->isDispatched) {
                $this->_SetError('Request has already been dispatched?');
                break;
            }

            while (!$this->request->isDispatched) {

                $ret = false;

                /*
                if ($this->Route()) {
                    break;
                }
                */

                $action = $this->request->GetAction();
                $controller = $this->request->GetController();
                $module = $this->request->GetModule();

                if (!$this->IsPermitted($action, $controller, $module)) {
                    $this->response->PermissionDenied();
                    break;
                }

                TNT::Timestamp("Getting controller: {$module}/{$controller}/{$action}");
                $action_controller =& $this->ActionController($action, $controller, $module);
                if (!$action_controller) {
                    $this->_SetError("Unsupported action: {$module}/{$controller}/{$action}");
                    break;
                }


                if (!is_a($action_controller, 'TNTCtrlAction')) {
                    $this->Abort('TNTCtrlFront::Dispatch() - not a valid action controller');
                    break;
                }

                // At the moment action controllers are created and destroyed
                // for every action.

                $this->request->isDispatched = true;

                if (!$action_controller->Init()) {
                    $this->_SetError('Can not initialise action controller', $action_controller);
                    break;
                }

                TNT::Timestamp("Dispatching action: {$module}/{$controller}/{$action}");
                if (!$action_controller->Dispatch()) {
                    $this->_SetError("Can not dispatch action: {$module}/{$controller}/{$action}", $action_controller);
                    break;
                }

                if (!$action_controller->Term()) {
                    $this->_SetError('Can not terminate action controller', $action_controller);
                    break;
                }

                destroy($action_controller);

                $ret = true;
             }

        } while (false);
        // End of error scope

        TNT::Timestamp('TNTCtrlFront::Exec() - done');
        return $ret;
    }

    /**
     * Terminate the front controller
     *
     * This base method will close the response sending it to the client.
     *
     * @access  public
     * @return  boolean                 true if initialisation successfull
     */
    function Term() {
        TNT::Timestamp('TNTCtrlFront::Term()');

        // Close the response - this writes out the client data
        if ($this->response) {
            $this->response->Close();
        }

/*
        // Close the session - this writes out the session data
        if ($this->session) {
            $this->session->Close();
        }
*/

        $ret = true;

        TNT::Timestamp('TNTCtrlFront::Term() - done');
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
     * Main entry point of the controller.
     *
     * Construct an controller and then call main to start it running.
     *
     * Calls {@link Init()}, {@link Auth()}, {@link Exec()} then {@link Term()}
     * in that order stopping at the first to fail.
     *
     * @access  public
     * @return  boolean                 true if all successfull
     */
    function Main() {

        $ret = false;

        // Start error section
        do {
            // Intialise the controller
            TNT::Timestamp('TNTCtrlFront::Main() - initialising');
            if (!$this->Init()) {
                break;
            }

            // Authenticate the request
            TNT::Timestamp('TNTCtrlFront::Main() - authenticating');
            if (!$this->Auth()) {
                break;
            }

            // Execute the request
            TNT::Timestamp('TNTCtrlFront::Main() - executing');
            if (!$this->Exec()) {
                break;
            }

            $ret = true;

        } while(false);
        // End error section

        // Terminate the controller
        TNT::Timestamp('TNTCtrlFront::Main() - terminating');
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
     * The class should be overridden and the derived constructor should
     * initialise the following member variables;
     *   + {@link $request} - reference to TNTCtrlRequest object
     *   + {@link $response} - reference to TNTCtrlResponse object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlFront($options = null) {

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

        $this->request = null;
        $this->response = null;
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlFront() {
        destroy($this->response);
        destroy($this->request);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
