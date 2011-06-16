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
 * WUI LinkLine Class 
 * ~~~~~~~~~~~~~~~~~~
 *
 * A LinkLine widget draws a series of anchor tags in a horizontal line
 * as a navigational menu.
 *
 * @package TNT.WUI
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
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
 * Class to represent a hyperlink.
 *
 * @package TNT.WUI
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiLinkData {
    /**
     * The hyperlink to use in the anchor tag
     * @access  public
     * @var     string                  valid URL string
     */
    var $href = null;

    /**
     * The html to show as the content of the anchor tag
     * @access  public
     * @var     string                  valid HTML string
     */
    var $content = null;

    /**
     * The tooltip to show when the hyperlink is hovered over
     * @access  public
     * @var     string                  valid URL string
     */
    var $tooltip = null;

    /**
     * Flag to indicate that the hyperlink should be displayed active
     * @access  public
     * @var     string                  true if hyperlink is active
     */
    var $active = null;

    /**
     * Xtra HTML attribute definitions to insert within the anchors opening tag
     * @access  public
     * @var     string                  XHTML attribute string
     */
    var $extras = null;
}

/**
 * Class to represent a line of HTML links in the Web User Interface.
 *
 * A LinkLine widget draws a series of anchor tags in a horizontal line
 * as a navigational menu.
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiLinkLine extends TNTWuiWidget {

    /**
     * The links to display
     * @access  public
     * @var     TNTWuiLinkData[]           array of link data
     */
    var $_links = null;

    /**
     * Separator string to render between pathway elements.
     * @access  public
     * @var     string                  XHTML separator between links
     */
    var $_separator = null;

    /**
     * Sets the links.
     *
     * The links are given is an array of {@link TNTLinkData} structures.
     *
     * If the input is a string then it is assumed to be formated as a CSV
     * list of url encoded "element=value[&element=value]" values.
     *
     * @access  public
     * @param   mixed       $value      link elements
     * @return  boolean                 true if successful
     * @see     GetLinks();
     */ 
    function SetLinks($value) {

        // A blank link entry
        static $blank = array('href' => '#', 'html' => '', 'title' => '', 'active' => false, 'attrs' => '');

        $ret = false;
        if (is_null($value)) {
            $this->_links = null;
            $ret = true;
        } else if (is_array($value)) {
            $this->_links = array();
            foreach ($value as $link) {
               $this->_links[] = cast($link,'TNTWuiLinkData');
            }
            $ret = true;
        } else if (is_string($value)) {
            $this->_links = array();
            foreach (explode(',', $value) as $s) {
                $this->_links[] = cast(urlquery_decode($s),'TNTWuiLinkData');
            }
            $ret = true;
        } else {
            $this->_SetError('value must be an array or CSV string');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the links.
     *
     * If there is no links defined then null is returned.
     *
     * @access  public
     * @return  array                   array of link elements
     * @see     SetLinks();
     */ 
    function GetLinks() {
        return $this->_links;
    }

    /**
     * Sets the separator string to use between pathway elements.
     *
     * If no separator is specified then the template may use a default one.
     *
     * @access  public
     * @param   string      $value      separator string (XHTML)
     * @return  boolean                 true if successful
     * @see     GetSeparator();
     */ 
    function SetSeparator($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_separator = null;
            $ret = true;
        } else if (is_string($value)) {
            $this->_separator = $value;
            $ret = true;
        } else {
            $this->_SetError('value must be a string');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the separator string to use between pathway elements.
     *
     * If there is no separator defined then null is returned.
     *
     * @access  public
     * @return  string                  separator string (XHTML)
     * @see     SetSeparator();
     */ 
    function GetSeparator() {
        return $this->_separator;
    }

    /**
     * Render the window.
     *
     * This just sets up the default template to use if none has been specified
     * already.
     *
     * @access  public
     * @return  string                  HTML required to render the window
     */
    function Render() {
        if (!$this->_template && !$this->SetTemplate('includes/libs/tnt/view/tntlinkline,tntlinkline')) {
            return null;
        }
        return parent::Render();
    }

    /**
     * Construct the object
     *
     * @access  public
     */
    function TNTWuiLinkLine() {

        TNTWuiWidget::TNTWuiWidget();

    }

    /**
     * Destruct the box object.
     *
     * This will destruct the box object.
     *
     * @access  private
     */
    function _TNTWuiLinkLine() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
