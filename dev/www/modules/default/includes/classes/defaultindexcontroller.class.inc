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
 * Default Index Controller
 * ~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * DefaultIndexController is an example Action Controller class which handles
 * the actions for the Default module.
 *
 * This default controller does nothing but serve up views of pages.
 *
 * @package Default.Index
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTCtrlAction');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to action a front end request.
 *
 * An instance of this class is created by the front controller to handle a
 * requested action.
 *
 * @package Default.Index
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class DefaultIndexController extends TNTCtrlAction {

    /**
     * File extensions for files that will be compiled into SmartPages,
     * requests for other files will result in a redirection to the actual file.
     * @access  public
     * @var string[]                    array of SmartPage file extensions
     */
    var $extensions = array('.htm', '.html', '.shtm', '.shtml');

    /**
     * File extension mappings where files given as one extention are stored as
     * a different extension on disk.
     * @access  public
     * @var array                       map of url ext => real ext
     */
    var $extMap = null;

    /**
     * Path to page being processed
     * @access  public
     * @var string                      absolute filesystem path to page
     */
    var $path = null;

    /**
     * URL of page being processed
     * @access  public
     * @var string                      absolute webserver url to page
     */
    var $url = null;

    /**
     * Directory in the page files are based
     * @var     string                  absolute path to page file directory
     * @access  public
     */
    var $docRoot = '~data/pages';

    /**
     * Render a view.
     *
     * Renders the view object sending its output to the response object.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function Render() {
        TNT::Timestamp('DefaultIndexController::Render()');

        $ret = false;

        // Start of error scope which we will break out of on error
        do {
            // Disable caching of this page by
            $this->response->NoCache();

            TNT::Timestamp('Constructing template');

            TNTLoader::RequireClass('TNTTemplate');

            $template =& new TNTTemplate($this->path);

            TNT::Timestamp('Rendering template');

            $output = $template->Parse();

            if (is_null($output)) {
                $this->_SetError('Failed to parse the template: '.$this->path, $template);
                $ret = false;
                break;
            }

            $this->response->Output($output);

            $ret = true;

        } while (false);
        // End of error scope

        TNT::Timestamp('DefaultIndexController::Render() - done');
        return $ret;
    }

    /**
     * PreDispatch method.
     *
     * This method is called by the {@link Dispatch()} method before the action
     * method. If using the class with {@link TNTCtrlFront}, it may modify the
     * {@link $request}Request object{/link} and reset its
     * {@link TNTCtrlRequest::isDispatched} flag in order to skip processing
     * the current action.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function PreDispatch() {
        return true;
    }

    /**
     * PostDispatch method.
     *
     * This method is called after the {@link Dispatch()} method. If using
     * the class with {@link TNTCtrlFront}, it may modify the
     * {@link $request}Request object{/link} and reset its
     * {@link TNTCtrlRequest::isDispatched} flag in order to process an
     * additional action.
     *
     * This method will not be called if {@link PreDispatch()} or the action
     * method return unsuccessfull. It will also not be called if the
     * {@link PreDispatch()} method marks the request as being dispatched.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function PostDispatch() {
        return true;
    }

    /**
     * IndexAction the current request.
     *
     * This is the default action of the controller.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function IndexAction() {
        $ret = false;

        // Start error section
        do {

            // Does the path exist - if not return a '404 Not Found' status
            if (!file_exists($this->path)) {
                $this->response->AddHeader('HTTP/1.1 404 Not Found');
                break;
            }

            // Is it the path to a template file - if not redirect browser to it
            if (!$this->isTemplate) {
                // A non template file has been requested
                // redirect the browser to the real location
                $this->response->Redirect($this->url);
                break;
            }

            // Render the page
            $ret = $this->Render();

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Initialise the application.
     *
     * This method is hookable.
     *
     * @access  public
     * @return  boolean                 true if initialisation successfull
     */
    function Init() {
        TNT::Timestamp('DefaultIndexController::Init()');

        $ret = false;

        // Start error section
        do {
            if (!parent::Init()) {
                break;
            }

            $this->docRoot = TNT::RegGetVar('module.default.document_root', $this->docRoot);
            $extensions = TNT::RegGetVar('module.default.extensions', null);
            if ($extensions) {
                $this->extensions = explode(',', $extensions);
            }

            $extmap = TNT::RegGetVar('module.default.extmap', null);
            if (is_string($extmap)) {
                // Formatted as 'x=y,x=y,x=y'
                $extmap = urldecode(str_replace(',', '&', $extmap));
            }
            if ($extmap) {
                $this->extMap = $extmap;
            }

            //TNTDebug::Dump('$this->docRoot='.$this->docRoot);
            //TNTDebug::Dump('$this->extensions='.$this->extensions);
            //TNTDebug::Dump('$this->extMap='.$this->extMap);

            // We check for a page request as early as we can so we can bail
            // out if needed before initialising all the application
            if (isset($_REQUEST['page'])) {
                $path = $_REQUEST['page'];
            } else if (!empty($_SERVER['PATH_INFO'])) {
                $path = substr($_SERVER['PATH_INFO'], 1);
            } else {
                $path = 'index';
            }

            $pi = pathinfo($path);
            if (empty($pi['extension'])) {
                $pi['extension'] = '.tpl';
                $path .= '.tpl';
            }

            // Is it the path to a template file - if not redirect browser to it
            $this->isTemplate = true;
            if ($pi['extension'] != '.tpl') {
                $this->isTemplate = false;
                foreach ($this->extensions as $ext) {
                    if (strcasecmp($pi['extension'], $ext) == 0) {
                        $this->isTemplate = true;
                        break;
                    }
                }
            }

            // Ensure the document root path exists
            $this->path = TNTFileSystem::ExpandPath("{$this->docRoot}/{$path}", TNT_ROOT_DIR);
            $this->url = TNTFileSystem::ExpandPath("{$this->docRoot}/{$path}", TNT_ROOT_URI);
       
            //tnt_dump('$this->path='.$this->path);
            //tnt_dump('$this->url='.$this->url);

            $ret = true;

        } while (false);
        // End error section

        TNT::Timestamp('DefaultIndexController::Init() - done');
        return $ret;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void                constructor
     */
    function DefaultIndexController() {
        TNTCtrlAction::TNTCtrlAction();
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _DefaultIndexController() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
