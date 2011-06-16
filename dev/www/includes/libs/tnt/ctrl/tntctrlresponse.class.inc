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
 * Ctrl Response Class
 * ~~~~~~~~~~~~~~~~~~~
 *
 * This abstract class encapsulates response information.
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
 * Abstract class to encapsulates HTTP response information.
 *
 * The methods and properties of this class are exposed through the
 * {@link TNTCtrlFront::response} property of the {@link TNTCtrlFront} class.
 *
 * It is also a singleton class and can be accessed via the static
 * method {@link TNTCtrlResponse::Singleton()}.
 *
 * @package TNT.Ctrl
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlResponse extends TNTVarMap {

    /**
     * A value indicating whether to send content to the client.
     * @var     boolean                 true to withhold content
     * @access  public
     */
    var $suppressContent = false;

    /**
     * A value indicating whether the client is still connected to the server.
     * @var     boolean                 true if client connected.
     * @access  public
     */
    var $isClientConnected = false;

    /**
     * Array of output filter callbacks
     * @var     callback[]              filter array
     * @access  private
     */
    var $_filters = array();

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
     * Redirects a client to a new URL.
     *
     * This will set up the redirect headers.
     *
     * Any relative URL will be attempted to be converted to an absolute URL
     * if this fails then the method will return in error.
     *
     * If '$end' is true then the method will automatically call {@link End()}
     * which will terminate the application allowing the redirection to take
     * place immediately.
     *
     * If '$replace' is specified then URL replacement is performed as opposed
     * to HTTP redirection. This means that no HTTP redirect header is sent,
     * instead a dummy page is returned to the browser containing a javascript
     * 'location.replace()' call. If javascript is not supported then a META
     * refresh tag is added as a backup. Failing that the dummy page contains
     * a link to click. The idea is that 'location.replace()' will not leave
     * a trace of this request in the clients history and not hence any client
     * 'Back' button can't return to this request.
     *
     * @access  public
     * @param   string      $url        target location
     * @param   boolean     $end        end current request
     * @param   boolean     $replace    do url replacement not redirection
     * @return  boolean                 true if successfull
     */
    function Redirect($url, $end = true, $replace = false) {

        $ret = false;

        TNTLoader::RequireClass('TNTUri');

        $uri =& new TNTUri($url);

        if (($ret = $uri->MakeAbsolute(null))) {
            $this->redirectURL = $uri->ToString();
        }

        destroy($uri);

        $this->redirectReplace = $replace;
 
        if ($end) {
            if ($ret) {
                $this->End();
            } else {
                TNT::Abort("Failed to set redirection URL to '$url':", $uri->Error());
            }
        }

        return $ret;
    }

    /**
     * Flushes all currently buffered output to the client.
     *
     * @access  public
     * @return  void
     */
    function Flush() {
        ob_flush();
        flush();
    }

    /**
     * Outputs data to the output stream.
     *
     * Supports the writing of TNTObjects by automatically calling the
     * {@link TNTObject::ToText()} method.
     *
     * The standard PHP functions {@link echo()}, {@link print()},
     * {@link printf()} can also be used to write to the output stream.
     *
     * @access  public
     * @param   mixed       $data       data to output
     * @return  void
     */
    function Output($data) {

        if (is_object($data) && implements($data, 'tntobject')) {
            $data = $data->ToHTML();
        }

        if (!is_scalar($data)) {
            $data = var_info($data);
        }

        echo $data;
        
    }

    /**
     * Adds an alert to the output stream.
     *
     * Alerts are out of band messages intended to alert the user of error
     * situations. How they are processes is depended on the response type.
     *
     * Passing NULL will clear all alerts !!!
     *
     * @access  public
     * @param   string      $msg        message to add (or null)
     * @return  void
     */
    function Alert($msg) {

        $alerts = $this->GetVar('tnt_alerts');
        if (!$alerts) {
            TNTLoader::RequireClass('TNTCtrlRequest');
            $request =& TNTCtrlRequest::Singleton();
            $alerts = $request->GetVar('tnt_alerts');
        }

        if (empty($alerts)) {
            $alerts = array();
        } else if (!is_array($alerts)) {
            $alerts = (array)$alerts;
        }

        if (is_null($msg)) {
            $alerts = null;
        } else {
            $alerts[] = $msg;
        }

        $this->SetVar('tnt_alerts', $alerts);

    }

    /**
     * Add an output filter.
     *
     * Output filters take the output as their only argument and should
     * return the filtered output.
     *    string Filter(string)
     *
     * @access  public
     * @param   callback    $filter     filter to use
     * @return  true                    true if successfull
     */
    function AddFilter($filter) {

        if (!is_callable($filter)) {
            $this->_SetError('Filter is not callable');
            $ret = false;
        } else {
            $this->_filters[] = $filter;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Close the response and send to the client.
     *
     * Once closed the VarMap becomes readOnly.
     *
     * @access  public
     * @return  void
     */
    function Close() {
        $this->isClientConnected = false;
        flush();
    }

    /**
     * Construct the object
     *
     * It adds the default URIFilter which expands virtual paths in the
     * output to absolute paths.
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlResponse() {

        TNTVarMap::TNTVarMap();

        // Discard any current output
        @ob_end_clean();

        // Start buffering any new output
        ob_start();

        $this->isClientConnected = true;

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlResponse() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
