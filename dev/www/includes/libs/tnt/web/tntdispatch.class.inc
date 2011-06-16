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
 * Dispatcher 
 * ~~~~~~~~~~
 *
 * Small helper function to aid in dispatching operations to an object.
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
 * This is helper class that is used to dispatch operations to the member
 * functions of a parent object.
 *
 * It supports default operations, operation redirection.
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
class TNTDispatch extends TNTObject {

    /**
     * The operation being dispatched
     * @access  public
     * @var string                      operation being dispatched
     */
    var $op = null;

    /**
     * Has an operation been redirected.
     *
     * @access  public
     * @param   string  $op             name of operation to check
     * @return  boolean                 true if operation redirected
     * @see SetRedirection(), GetRedirection()
     */
    function IsRedirectedOp($op) {
        return !empty($this->_redirect[$op]);
    }

    function SetRedirectOp($op, $file) {
        $this->_redirect[$op] = $file;
    }

    function GetRedirectOp($op) {
        if (empty($this->_redirect[$op]))
            return null;
        return $this->_redirect[$op];
    }

    function RedirectOp($op) {
        if (empty($this->_redirect[$op]))
            return false;
        return $this->Redirect($this->_redirect[$op]);
    }

    /**
     * Support redirecting an operation to another file
     * (either using HTTP redirection or by PHP inclusion
     *
     * File can contain a redirection method of 'include:<path>' or
     * 'location:<path>'. If no method is specified then 'include' is assumed.
     * A 'location:' is handled using HTTP redirection whilst an 'include:' is
     * handled by including the file and pretending that it is at global scope
     * as if run by the server directly.
     *
     * @access  public
     * @param   string      $file       name of file to redirect to
     * @param   boolean     $end        end current request
     * @return  boolean                 true if successfull (or never if $end)
     */
    function Redirect($file) {

        $type = null;
        if (strncmp($file, 'location:', 9) == 0) {
            $type = 'location';
            $file = trim(substr($file, 9));
        } else if (strncmp($file, 'include:', 8) == 0) {
            $type = 'include';
            $file = trim(substr($file, 8));
        } else if (TNT_PathFileExists($file)) {
            $type = 'include';
        } else {
            $type = 'location';
        }

        // Do we require template substitution on the file name
        $req_subs = (strstr($file, '{') ? true : false);

        // Make all globals visible at this scope and also create an array
        // of variables to insert into the file if required
        $vars = array();
        reset($GLOBALS);
        while (list($var) = each($GLOBALS)) {
            global ${$var};
            if ($req_subs &&
                (is_numeric($GLOBALS[$var]) || is_string($GLOBALS[$var]))) {
                $vars[$var] = $GLOBALS[$var];
                $vars[$var.'_url'] = urlencode($GLOBALS[$var]);
            }
        }

        // Do template substitution on the given file name
        if ($req_subs) {
            $vars['op'] = $this->op;
            foreach($vars as $var => $val) {
                $file = str_replace($var, $val, $file);
            }
        }

        if ($type == 'location') {
            // Redirect used HTTP Redirection

            // Expand the file incase is in the virtual file system
            $file = TNTFileSystem::ExpandPath($path, TNT_ROOT_URI);

            // Send redirection
            TNTLoader::RequireClass('TNTResponse');
            $response =& TNTReponse::Singleton();
            $ret = $response->Redirect($path, $end);

        } else if ($type == 'include') {
            // Redirect via PHP inclusion

            // Expand the file incase is in the virtual file system
            $file = TNTFileSystem::ExpandPath($path, TNTFileSystem::SystemPath());

            // This is a cheating way but save a redirection to the correct file
            // (it only works for files under the installation root)
            // The file will probably have expected to be included a global
            // scope so make all global variables avaiable at this scope
            $PHP_SELF = TNTFileSystem::SwitchRootPath($file, TNT_FILESYSTEM_SYSTEMTOSERVER);

            $ret = include($file);

            if ($end) {
                TNTLoader::RequireClass('TNTResponse');
                $response =& TNTReponse::Singleton();
                $response->Term();
                TNT::Shutdown();
            }
        } else {
            TNT::Abort('TNTDispatch::Redirect() - can not redirect to '.$file);
        }

        return false;
    }

    //************************************************************************
    // Operation handling
    //************************************************************************

    /**
     * Called when all processing is finished and we should return to caller
     *
     * This sets up a redirection which will be performed when the current
     * application finishes.
     *
     * If '$end' is specified then the application is terminated immediately
     * by this call and the method will not return.
     *
     * If '$replace' is specified then URL replacement is performed as opposed
     * to HTTP redirection.
     *
     * Refer to {@link TNTResponse::Redirect()} for more information.
     *
     * @access  public
     * @param   boolean     $end        end current request
     * @param   boolean     $replace    do url replacement not redirection
     * @return  boolean                 true if successfull (or never if $end)
     */
    function Finish($end = true, $replace = false) {
        TNTLoader::RequireClass('TNTResponse');
        $response =& TNTResponse::Singleton();
        $response->Redirect($this->returl, $end);

    }

    /**
     * Dispatch a given operation.
     *
     * If no operation is specified then the method will look for a request
     * variable with the name 'op'. Failing that it will use any default
     * operation set in the member variable {@link $defaultOP}.
     *
     * The method will check to see if the operation should be redirected. If
     * no redirection is specified then a check is made for a method with the
     * same name as the operation (prefixed by 'Op'), and call it and return
     * its return value.
     *
     * The method will use the currect application security context to check
     * that the caller has permission to perform the operation.
     *
     * @access  public
     * @param   string          $op     operation to dispatch (optional)
     * @return  boolean                 result of operation (or null)
     */
    function Dispatch($op) {
        $ret = false;

        // Start error section
        do {

            TNTLoader::RequireClass('TNTResponse');
            $request =& TNTRequest::Singleton();

            if (!$op)) {
                $op = $request->GetVar('op', $this->defaultOp);
                if (!$op) {
                    $this->_SetError('No operation to perform');
                    break;
                }
                
            }

            // Save the op away for future use
            $this->op = $op;

            // Check for an op redirection
            if ($this->IsRedirectedOp($this->op)) {
                // We redirect the op to the file that handles it
                return $this->RedirectOp($this->op);
            }

            // By default we try to call an member function with the same name
            // as the operation prefixed with 'Op' - derived classes can either
            // override this method or create the required functions. 
            $method = 'Op'.preg_replace('/[^\w]/', '_', $op);
            if (!method_exists($this, $method)) {
                $this->SetError("Can't dispatch operation: $op");
                break;
            }

            $ret = $this->$method();

        } while (false);
        // End error section

        return false;
    }

    /**
     * Constructor
     *
     * @access  public
     * @param   TNTObject&  $object     object to dispatch to
     * @param   string      $defaultOp  default operation to dispatch
     */
    function TNTDispatch(&$object, $defaultOp = null) {

        TNTObject::TNTObject();

        TNTLoader::RequireClass('TNTResponse');
        $request =& TNTRequest::Singleton();

        // Clear any operation redirections
        $this->_opredir = null;

        $this->_object =& $object;

        $this->defaultOp = $defaultOp;

        // See if an return address has been requested
        $this->returl = $request->GetVar('returl');
        if (empty($this->returl)) {
            $this->returl  = $request->urlReferrer();

            // If a viewstate was posted then the referrer was a form whose
            // state should be remembered.
            if (strstr($this->returl, 'tnt_viewstate=') &&
                ($viewstate = $request->GetVar('tnt_viewstate'))) {
                $this->returl .= (strpos($this->returl, '?') === false ? '?' : '&').urlquery_encode($viewstate));
            }
        }

    }

    /**
     * Destructor
     *
     * @access  private
     */
    function _TNTDispatch() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
