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
 * Ctrl HTTP Request Class
 * ~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class encapsulates HTTP request information.
 *
 * @package TNT.Ctrl.Http
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTCtrlRequest');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to encapsulate HTTP request information.
 *
 * The methods and properties of this class are exposed through the
 * {@link TNTCtrlFront::request} property.
 *
 * It is also a singleton class and can be accessed via the static
 * method {@link TNTCtrlRequest::Singleton()}.
 *
 * @package TNT.Ctrl.Http
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlHttpRequest extends TNTCtrlRequest {

    /**
     * The URL requested.
     * @var     string                  URL requested
     * @access  public
     */
    var $url = null;

    /**
     * Any extra path info on the current request.
     * @var     string                  path info
     * @access  public
     */
    var $pathInfo = null;

    /**
     * The URL of the referring request.
     * (URL of the client's previous request that linked to the current URL.)
     * @var     string                  URL that referred this request
     * @access  public
     */
    var $urlReferrer = null;

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
     * The agent identification string sent in the request.
     * @var     string                  the agent string
     * @access  private
     */
    var $agent = null;

    /**
     * The HTTP data transfer method (such as GET, POST, or HEAD) used by
     * the request.
     * @var     string                  File collection
     * @access  public
     */
    var $method = null;

    /**
     * A value indicting whether the HTTP connection uses secure sockets
     * (that is, HTTPS). 
     * @var     boolean                 True if is secure connection
     * @access  public
     */
    var $isSecure = null;

    /**
     * The URL query variables sent in the request.
     * @var     string                  Any query string sent in the request
     * @access  public
     */
    var $query = null;

    /**
     * Collection of Cookies sent in the request
     * @var     TNTVarMap&              Cookie collection
     * @access  public
     */
    var $cookies = null;

    /**
     * Collection of Form variables sent in the request
     * @var     TNTVarMap&              Form collection
     * @access  public
     */
    var $form = null;

    /**
     * Collection of File variables sent in the request
     * @var     TNTVarMap&              File collection
     * @access  public
     */
    var $file = null;

    /** 
     * Unserialize an array where required - recursively
     *
     * @access  public
     * @param   array&      $a      reference to the array to unserialize
     * @return  boolean             true if successfull, false otherwise 
     * @static
     */
    /*static*/ function _UnSerialize(&$a) {
        foreach ($a as $n => $v) {
            if (is_string($v) && $v && $v{0} == 'z' && $v{1} == ':') {
                if (($d = unserialize(substr($v, 2))) !== false) {
                    $a[$n] = $d;
                } else {
                    return false;
                }
            } else if (is_array($v)) {
                if (!TNTCtrlHttpRequest::_UnSerialize($a[$n])) {
                    return false;
               }
            }
        }

        return true;
    }
    /**
     * Construct the object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlHttpRequest() {

        TNTCtrlRequest::TNTCtrlRequest();

        $this->acceptTypes = @explode(',', (array)$_SERVER['HTTP_ACCEPT']);
        $this->acceptLanguages = @explode(',', (array)$_SERVER['HTTP_ACCEPT_LANGUAGE']);
        $this->acceptEncodings = @explode(',', (array)$_SERVER['HTTP_ACCEPT_ENCODING']);
        $this->acceptCharsets = @explode(',', (array)$_SERVER['HTTP_ACCEPT_CHARSET']);
        $this->agent = @$_SERVER['HTTP_USER_AGENT'];

        $this->_vars =& $_REQUEST;

        $this->url = TNT_REQUEST;
        $this->urlReferrer = (isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER'] : null);

        // The extra path info
        $this->pathInfo = @$_SERVER['PATH_INFO'];

        // The HTTP data transfer method (such as GET, POST, or HEAD) used.
        $this->method = @$_SERVER['REQUEST_METHOD'];

        // Value indicting whether the HTTP connection uses secure sockets
        $this->isSecure = TNT_REQUEST_SECURE;

        // The URL query variables sent in the request.
        $this->query = @$_SERVER['QUERY_STRING'];

        // Collection of Cookies sent in the request
        $this->cookies =& new TNTVarMap();
        $this->cookies->_vars =& $_COOKIE;

        // Collection of Form variables sent in the request
        $this->form =& new TNTVarMap();
        $this->form->_vars =& $_POST;

        // Collection of File variables sent in the request
        $this->file =& new TNTVarMap();
        $this->file->_vars =& $_FILE;

        // Automatically unserialize any possibly serialized variables
        TNTCtrlHttpRequest::_UnSerialize($this->_vars);
        TNTCtrlHttpRequest::_UnSerialize($this->form->_vars);
        TNTCtrlHttpRequest::_UnSerialize($this->cookies->_vars);

        // Try and determine the module/controller/action name from the
        // request.
        if (isset($_SERVER['PATH_INFO'])) {
            $d = explode('/', trim($_SERVER['PATH_INFO'], '/'));
        } else {
            $d = explode('/', trim(substr(TNT_REQUEST_PATH, strlen(TNT_ROOT_URI)), '/'));
        }

        if (!empty($d[0])) { $this->SetModule($d[0]); }
        if (!empty($d[1])) { $this->SetController($d[1]); }
        if (!empty($d[2])) { $this->SetAction($d[2]); }

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlHttpRequest() {
        destroy($this->cookies);
        destroy($this->form);
        destroy($this->files);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
