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
 * Control Action Class
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * TNTCtrlAction is an abstract class which implements an Action Controller for
 * use with the Front Controller ({@link TNTCtrlFront}) pattern used in the
 * Model-View-Controller (MVC) pattern. 
 *
 * To use the class you will need to derive from it and create action methods
 * that correspond to the various actions you wish the controller to handle.
 *
 * The classes {@link TNTCtrlAction::Dispatch()} will automatically route
 * requested actions to these action methods when named correctly.
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
 * Class to action a front end request.
 *
 * An instance of this class is created by the front controller to handle a
 * requested action.
 *
 * @package TNT.Ctrl
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTCtrlAction extends TNTObject {

    /**
     * Reference to the single instance of the Front Controller
     * @var     TNTCtrlFront&           reference to front controller
     * @access  public
     */
    var $front = null;

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
     * @return  object                  reference to global TNTCtrlAction object
     */
    /*static*/ function &Singleton() {
        return TNTObject::Singleton(__CLASS__);
    }

    /**
     * Forward to another module/controller/action
     *
     * This modifies the current request object with the values supplied
     * clears the IsDispatched flag thus allowing the Front Controller to
     * redispatch the request again.
     *
     * The forwarding only happens when the currect action returns back to
     * the dispatching loop.
     *
     * A fourth argument, $vars, will be used to set the request variables.
     *
     * @access  public
     * @param   string      $action     new action name (or null)
     * @param   string      $controller new controller name (or null)
     * @param   string      $module     new module name (or null)
     * @param   array       $vars       request variables
     * @return  boolean                 true if successfull
     */
    function Forward($action = null, $controller = null, $module = null, $vars = null) {

        if ($action) {
            $this->response->SetAction($action);
        }

        if ($controller) {
            $this->response->SetController($controller);
        }

        if ($module) {
            $this->response->SetModule($module);
        }

        if ($vars) {
            $this->response->SetVar($vars);
        }

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
     * Render a view.
     *
     * Renders the view object sending its output to the response object.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function Render() {
        TNT::Timestamp('TNTCtrlAction::Render()');

        $ret = true;

        TNT::Timestamp('TNTCtrlAction::Render() - done');
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
     * Dispatch the current request.
     *
     * This method will take the name of the action from the current request
     * and look for a method with the name of the action postfixed by 'Action'.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function Dispatch() {
        TNT::Timestamp('TNTCtrlAction::Dispatch()');

        $ret = false;

        // Start error section
        do {
            $this->request->isDispatched = true;

            if (!$this->PreDispatch()) {
                break;
            }

            if (!$this->request->isDispatched) {
                // PreDispatch changed the request state - did it forward ?
                $ret = true;
                break;
            }

            $action = $this->request->GetAction();

            $method = preg_replace('/[^\w]/', '_', $action).'Action';
            if (!method_exists($this, $method)) {
                $this->_SetError("No such action: $action");
                break;
            }

            if (!$this->$method()) {
                break;
            }

            if (!$this->PostDispatch()) {
                break;
            }

            $ret = true;

        } while (false);
        // End error section

        TNT::Timestamp('TNTCtrlAction::Dispatch() - done');
        return $ret;
    }

    /**
     * Initialise the action controller.
     *
     * This method is called when the controller is first constructed and
     * allows the controller to initialise. Unlike any initialisation done
     * in the constructor this method allows a failure to be reported back.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function Init() {
        return true;
    }

    /**
     * Terminate the action controller.
     *
     * This method is called when the controller is finished with. At the
     * moment that is as soon as it has dispatched any action. However
     * future versions may allow the controller to be reused if an request
     * is forwarded to it again in the same workflow.
     *
     * @access  public
     * @return  boolean                 true if successfull
     */
    function Term() {
        return true;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void                constructor
     */
    function TNTCtrlAction() {
        TNTObject::TNTObject();

        $this->front =& TNTCtrlFront::Singleton();

        if ($this->front) {
            $this->request =& $this->front->request;
            $this->response =& $this->front->response;
            $this->user =& $this->front->user;
        }
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCtrlAction() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
