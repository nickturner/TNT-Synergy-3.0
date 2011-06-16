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
 * Form Widget Class 
 * ~~~~~~~~~~~~~~~~~
 *
 * Represents a simple HTML FORM element.
 *
 * @package TNT.Html
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiHtml');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a simple HTML FORM element.
 *
 * @package TNT.Html
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiForm extends TNTWuiHtml {

    /**
     * Name of the control - name used to post back data
     * @access private
     * @var string                      forms name
     */
    var $_name = null;

    /**
     * Set the forms name
     *
     * The name is user to identify the form in the posted data.
     *
     * If there is no name then no value is posted.
     *
     * @access  public
     * @param   string      $name       string to set the name to
     * @return  boolean                 true if successful
     * @see     GetName();
     */ 
    function SetName($name) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('name' =>& $name));

        if (is_null($name)) {
            $this->_name = null;
            $ret = true;
        } else if (!is_string($name)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else if (!preg_match('/^[_a-z][a-z0-9_=&\'{}[\]]*$/i', $name)) {
            $this->_SetError('form name is invalid ('.$name.')');
            $ret = false;
        } else {
            $this->_name = $name;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the controls name
     *
     * If there is no name then no value is posted.
     *
     * @access  public
     * @return  string                  current value of the name
     * @see     SetName();
     */ 
    function GetName() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_name;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Construct the element
     *
     * This will construct element, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiForm() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        TNTWuiHtml::TNTWuiHtml();

        $this->_tagName = 'form';

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }

    /**
     * Destruct the element.
     *
     * This will destruct the element and any children.
     *
     * @access  private
     */
    function _TNTWuiForm() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
