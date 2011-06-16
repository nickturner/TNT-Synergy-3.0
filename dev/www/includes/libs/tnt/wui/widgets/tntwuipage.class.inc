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
 * Page Widget Class 
 * ~~~~~~~~~~~~~~~~~
 *
 * Represents the themed page.
 *
 * @package TNT.Html
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiWidget');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a page.
 *
 * The page widget is designed to be the outermost widget and is responsible
 * for rendering the page framework.
 *
 * It takes the given content and renders it within the template.
 *
 * By default the template names 'page.$type, page' are used where $type is
 * optional (see {@link setType()}.
 *
 * @package TNT.Wui
 * @subpackage Widgets
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiPage extends TNTWuiWidget {

    /**
     * Type of page, user defined, commonly used for screen, print, popup etc.
     * @access private
     * @var string                      optional page type
     */
    var $_type = null;

    /**
     * Set the page type
     *
     * The page type is an optional parameter which can be used to select
     * different types of pages. Most themes have different page type for
     * printing, popups etc. and this can be used to select those types.
     *
     * When specified the Widget will first look for a template of name
     * 'page.$type' and then just 'page'.
     *
     * @access  public
     * @param   string      $type       string to set the type to
     * @return  boolean                 true if successful
     * @see     GetType();
     */ 
    function SetType($value) {

        if (is_null($value)) {
            $this->_value = null;
            $ret = true;
        } else if (!is_string($value)) {
            $this->_SetError('Page.Type must be either null or a string');
            $ret = false;
        } else if (!preg_match('/^[\w]*$/', $value)) {
            $this->_SetError('Page.Type can only consist of alphanumeric characters or underscores ('.$value.')');
            $ret = false;
        } else {
            $this->_type = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the page type
     *
     * If there is no type then no value is returned.
     *
     * @access  public
     * @return  string                  current value of the page type
     * @see     SetType();
     */ 
    function GetType() {
        return $this->_type;
    }

    /**
     * Render the page
     *
     * This is overridden to ensure the correct template is used. If no
     * template has been specified by the creator then this method will
     * set up the template.
     *
     * @access  public
     * @return  string                  the XHTML to render this widget
     */
    function Render() {

        if (!$this->_template) {
            $name = ($this->_type ? 'page.'.$this->_type.',page' : 'page');
            $this->SetTemplate($name);
        }

        return parent::Render();
    }


    /**
     * Construct the element
     *
     * This will construct element, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiPage() {

        TNTWuiWidget::TNTWuiWidget();

    }

    /**
     * Destruct the element.
     *
     * This will destruct the element and any children.
     *
     * @access  private
     */
    function _TNTWuiPage() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
