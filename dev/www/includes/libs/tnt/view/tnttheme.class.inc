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
 * Theme Class
 * ~~~~~~~~~~~
 * 
 * The theme class is responsible for theme selection and initialisation.
 *
 * @package TNT.View
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
 * Class to handle the theme interface.
 *
 *
 * @package TNT.View
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTTheme extends TNTObject {

    /**
     * Theme name - name of the current theme
     * @var     string                  site name
     * @access  public
     */
    var $_name = '';

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
     * @return  TNTTheme&               reference to global TNTSession object
     * @static
     */
    /*static*/ function &Singleton() {
        return TNTObject::Singleton(__CLASS__);
    }

    /** 
     * Set the name of the theme used by the site.
     *
     * This will also fix the Virtual File System to replace '/synergy/theme'
     * with '/synergy/$name'.
     *
     * @access  public
     * @param   string      $name       theme name
     * @return  boolean                 true if set ok
     */
    function SetName($name) {

        $this->_name = strtolower($name);

        TNTSystem::SetIni('vfs', '/synergy/theme', '/synergy/'.$name);

        $ret = true;

        return $ret;
    }

    /** 
     * Get the name of the theme used by the site.
     *
     * @access  public
     * @return  string                  theme name
     */
    function GetName() {

        $ret = $this->_name;

        return $ret;
    }

    /** 
     * Return a template from the given theme.
     *
     * The template name can either a single name, an array of names or a
     * comma separated list of names. When list of names is used then the
     * first template found in the list is returned.
     *
     * @access  public
     * @param   string      $name       template name or list of names
     * @return  TNTTemplate&            reference to template object
     */
    function &Template($name) {

        TNTLoader::RequireClass('TNTTemplate');

        $ret =& new TNTTemplate();
        if (!$ret->LoadFile($name)) {
            $this->_SetError('template resource could not be found: '.$name);
            destroy($ret);
            $ret = null;
        }

        return $ret;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void                    constructor
     */
    function TNTTheme($options = null) {
        $args = array('options' =>& $options);
        $ret = null;

        if (!$this->_Enter(__CLASS__, __FUNCTION__, $args, $ret)) {
            return;
        }

        global $tnt;
        if ($tnt['_tnttheme']) {
            TNT::Abort('TNTTheme::TNTTheme() - possible duplicate instance: '.htmlentities(var_info($tnt['_tnttheme'])));
        }
        $tnt['_tnttheme'] =& $this;

        TNTObject::TNTObject();

        if ($options['name']) {
            $this->SetName($options['name']);
        } else if ($name = TNTSystem::GetIni('theme', 'name', TNTSystem::GetIni('config', 'theme', null))) {
            $this->SetName($name);
        }
        
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTTheme() {
        $args = null;
        $ret = null;

        if (!($this->_Enter(__CLASS__, __FUNCTION__, $args, $ret))) {
            return $ret;
        }

        global $tnt;
        unset($tnt['_tnttheme']);
        $tnt['_tnttheme'] = null;

        $this->_Exit(__CLASS__, __FUNCTION__, $args, $ret);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
