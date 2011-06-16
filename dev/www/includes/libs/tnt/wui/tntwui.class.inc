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
 * WUI Class 
 * ~~~~~~~~~
 *
 * Singleton class used to support the WUI framework
 *
 * @package TNT.Wui
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
global $tnt;
$tnt['_tntwui'] = null;


/*
 * Defines
 */

/**
 * Class to support the WUI framework.
 *
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWui extends TNTObject {

    /**
     * Tag register
     * @var     array                   associative array of registrations
     * @access  private
     */
    var $_tagRegister = null;

    /**
     * Tag prefixes
     * @var     array                   associative array of prefixes
     * @access  private
     */
    var $_tagPrefixes = null;

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
     * Return a list of all registered tag details for this component.
     *
     * @access  public
     * @return  array                   reference to the tag register
     */
    function &TagRegister() {
        if (!$this->_tagRegister) {

            $db =& TNTDatabase::Singleton();

            $rows = $db->Fetch('SELECT * FROM %wui_tags%'); 
            if (is_null($rows)) {
                TNT::Abort('Failed to read WUI tags table from database: '.$db->Error());
            }

            // The database only returns valid data so we don't need to call
            // RegisterTag() as that slows things down by revalidating the tags.
            foreach ($rows as $row) {
                $this->_tagRegister["{$row['tagprefix']}:{$row['tagname']}"] = array('tagprefix' => $row['tagprefix'], 'tagname' => $row['tagname'], 'classname' => $row['classname'], 'classfile' => $row['classfile']);
            }

            $this->_tagRegister['wui:widget'] = array('tagprefix' => 'wui', 'tagname' => 'widget', 'classname' => 'tntwuiwidget', 'classfile' => null);
        }

        return $this->_tagRegister;
    }

    /**
     * Return details on a registered tag name.
     *
     * The tag name should include its prefix and is case sensitive.
     *
     * @access  public
     * @param   string      $name       tag name (case sensitive)
     * @return  array&                  reference to details (or null)
     */
    function &RegisteredTag($name) {

        $ret = null;

        $tagRegister =& $this->TagRegister();

        if (isset($tagRegister[$name])) {
            $ret = $tagRegister[$name];
        }

        return $ret;
    }

    /**
     * Return true if the value supplied is a known tag.
     *
     * The tag name should include its prefix and is case sensitive.
     *
     *
     * @access  public
     * @param   string      $name       tag name
     * @return  boolean                 true if a known name
     */
    function IsRegisteredTag($name) {
        return ($this->_RegisteredTag(strtolower($name)) != null);
    }

    /**
     * Register a tags class information
     *
     * This maps a tag name to a PHP class name for all this nodes descendants,
     * (unless overridden in a descendant).
     *
     * It will validate that the class exists, unless a file name is given
     * then it will assume that the class is defined in the specified file
     * and will only check that the file exists and is readable.
     *
     * The class file can be a virtual path.
     *
     * @access  public
     * @param   string      $tagprefix  tag prefix
     * @param   string      $tagname    tag name
     * @param   string      $classname  php class name
     * @param   string      $classfile  file to include to get class definition
     * @return  boolean                 true if successfull
     */
    function RegisterTag($tagprefix, $tagname, $classname, $classfile = null) {
        $ret = true;

        if (!is_valid_varname($tagprefix)) {
            $this->_SetError('Invalid tag prefix: '.$tagprefix);
            $ret = false;
        } else if (!is_valid_varname($tagname)) {
            $this->_SetError('Invalid tag name: '.$tagname);
            $ret = false;
        } else if (!is_valid_varname($classname)) {
            $this->_SetError('Invalid class name: '.$classname);
            $ret = false;
        }

        TNTLoader::RequireClass('TNTFileSystem');

        if ($ret && $classfile) {
            $path = TNTFileSystem::ExpandPath($classfile, SYNERGY_DIR);
            if (!file_exists($path) || !is_readable($path)) {
                $this->_SetError("Tag '$tagprefix:$tagname' using class '$classname' has missing or unreadable class file '$classfile'");
                $ret = false;
            }
        }

        if ($ret) {
            $tagprefix = strtolower($tagprefix);
            $tagname = strtolower($tagname);
            $classname = strtolower($classname);
            $this->_tagRegister["{$tagprefix}:{$tagname}"] = array('tagprefix' => $tagprefix, 'tagname' => $tagname, 'classname' => $classname, 'classfile' => $classfile);
            if (!in_array($tagprefix, $this->_tagPrefixes))
                $this->_tagPrefixes[] = $tagprefix;
        }

        return $ret;
    }

    /**
     * Return list of registered tag prefixes for this component and all
     * ancestral components.
     *
     * The result is cached.
     *
     * @access  public
     * @return  array                   array of tag prefixes
     */
    function &TagPrefixes() {
        return $this->_tagPrefixes;
    }

    /**
     * Construct the object
     *
     * This will construct object, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWui() {

        TNTObject::TNTObject();

    }

    /**
     * Destruct the object.
     *
     * This will destruct the object and any children.
     *
     * @access  private
     */
    function _TNTWui() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
