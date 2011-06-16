<?php

///////////////////////////////////////////////////////////////////////////////
// TNT-SYNERGY: PHP Frontlication Framework                            v2.0.00 //
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
 * Ctrl HTTP Front Class
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * The TNTCtrlHTTPFront is an implementation of the {@link TNTCtrlFront} class
 * to handle HTTP requests.
 *
 * @package TNT.Ctrl.Http
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTCtrlFront');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to process a front end HTTP requests.
 *
 * A single instance of this class is created when a HTTP request is processed.
 * The instance lasts for the duration of the request.
 *
 * The class uses the following singleton classes to help in the processing
 * of the request.
 *  + {@link TNTCtrlHttpRequest} - represents the actual request sent.
 *  + {@link TNTCtrlHttpResponse} - represents the actual response to return.
 *
 * Each of these classes has a datastore whose use is as follows;
 *  + {@link TNTCtrlHttpRequest} - data sent from client as part of the request.
 *  + {@link TNTCtrlHttpResponse} - data to store on client as part of response.
 *
 * Data sent in the request will consist of data stored at the client for
 * this server (cookies) and any query, form and file sent in the request.
 *
 * Data sent in the response is stored on the client using HTTP permanent
 * cookies for sending back in subsequest requests.
 *
 * The class executes the following methods in the sequence given in order
 * to handle the action. You can derive from this class to override the
 * default methods when required.
 *  + {@link Init()} - Initialise the controller
 *  + {@link Auth()} - Authenticate the request
 *  + {@link Exec()} - Execute the request
 *  + {@link Term()} - Terminate the controller
 *
 * @package TNT.Ctrl.Http
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlHttpFront extends TNTCtrlFront {

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
     *   + {@link $request} - reference to TNTCtrlHttpRequest object
     *   + {@link $response} - reference to TNTCtrlHttpResponse object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlHttpFront($options = null) {

        TNTCtrlFront::TNTCtrlFront($options);

        /*
         * Request information (only ever one instance).
         */
        TNTLoader::RequireClass('TNTCtrlHttpRequest');
        $this->request =& TNTCtrlHttpRequest::Singleton();

        /*
         * Response information (only ever one instance).
         */
        TNTLoader::RequireClass('TNTCtrlHttpResponse');
        $this->response =& TNTCtrlHttpResponse::Singleton();
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlHttpFront() {
        destroy($this->request);
        destroy($this->response);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
