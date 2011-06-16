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
 * Path Manipulation Class
 * ~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class represents filesystem/webserver paths as objects which can be
 * manipulated.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTFmtStr');

/*
 * Globals
 */
global $tnt;
$tnt['tntpath']['_elementNames'] = array('dirname', 'filename', 'extension');

/*
 * Defines
 */

/**
 * Class to represent a filesystem path
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTPath extends TNTFmtStr {

    /**
     * Explode the Path into the elements
     *
     * @access  private
     * @return  boolean                 true if successfull
     * @see TNTFmtStr
     */
    function _Explode() {

        $ret = true;

        if (!empty($this->_string)) {
            $this->_elements['dirname'] = dirname($this->_string);
            $this->_elements['filename'] = basename($this->_string);
            $dot = strrpos($this->_elements['filename'], '.');
            if ($dot) {
                $this->_elements['extension'] = substr($this->_elements['filename'], $dot + 1);
                $this->_elements['filename'] = substr($this->_elements['filename'], 0, $dot);
            } else {
                $this->_elements['extension'] = '';
            }
        }
    
        return $ret;
    }
    
    /**
     * Implode the elements into the Path
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function _Implode() {

        $path = '';
    
        if (!empty($this->_elements['filename'])) {
            $path .= $this->_elements['filename'];
        }
    
        if (!empty($this->_elements['extension'])) {
            if ($this->_elements['extension']{0} != '.') {
                $path .= '.';
            }
            $path .= $this->_elements['extension'];
        }
    
        if (!empty($this->_elements['dirname'])) {
            $path = $this->_elements['dirname'].'/'.$path;
        }

        $this->_string = $path;

        return $ret;
    }

    /**
     * Expand a possible virtual path to a real path
     *
     * Calls {@link TNTFileSystem::ExpandPath()} on built path.
     *
     * @access  public
     * @return  void
     */
    function Expand() {
        TNTLoader::RequireClass('TNTFileSystem');
        $this->Set(TNTFileSystem::ExpandPath($this->Get()));
    }

    /**
     * Remove all multiple slashes and relative paths from a path
     * It will take into account symbolic directory links if possible.
     *
     * Calls {@link TNTFileSystem::NormalisePath()} on 'dirname'.
     *
     * @access  public
     * @return  void
     */
    function Normalise() {
        TNTLoader::RequireClass('TNTFileSystem');
        $this->SetElement('dirname', TNTFileSystem::NormalisePath($this->GetElement('dirname')));
    }

    /**
     * Test if the path exists as one of the given types.
     *
     * Accepts paths in the virtual file system.
     *
     * Type can be one of: null, 'file', 'dir', 'link'
     *
     * Calls {@link TNTFileSystem::Exists()} on built path.
     *
     * @access  public
     * @param   string      $type       type to test for (optional)
     * @return  boolean                 true if path exists
     */
    function Exists($type = null) {
        TNTLoader::RequireClass('TNTFileSystem');
        return TNTFileSystem::Exists($this->Get(), $type);
    }
    
    /**
     * Test if the path is a child path to another.
     *
     * Accepts paths in the virtual file system.
     *
     * Calls {@link TNTFileSystem::IsChildPath()} on 'dirname'.
     *
     * @access  public
     * @param   string      $parentpath directory path should be within
     * @param   boolean     $exists     check the file exists
     * @return  boolean                 true if path is in dir (and exists?)
     */
    function IsChild($parentpath, $exists = true) {
        TNTLoader::RequireClass('TNTFileSystem');
        return TNTFileSystem::IsChildPath($this->GetElement('dirname'), $parentpath, $exists);
    }
    
    /**
     * Test if the path is in the virtual file system
     *
     * Calls {@link TNTFileSystem::IsVirtualPath()} on 'dirname'.
     *
     * @access  public
     * @return  boolean                 true if path is absolute
     */
    function IsVirtual() {
        TNTLoader::RequireClass('TNTFileSystem');
        return TNTFileSystem::IsVirtualPath($this->GetElement('dirname'));
    }
    
    /**
     * Test if the path is absolute
     *
     * Calls {@link TNTFileSystem::IsAbsolutelPath()} on 'dirname'.
     *
     * @access  public
     * @return  boolean                 true if path is absolute
     */
    function IsAbsolute() {
        TNTLoader::RequireClass('TNTFileSystem');
        return TNTFileSystem::IsAbsolutePath($this->GetElement('dirname'));
    }
    
    /**
     * Make the path absolute to given $root.
     *
     * No alteration is made to already absolute paths, otherwise the given
     * $root path is prepended. No check is made to ensure that the resultant
     * path makes the given path absolute. 
     *
     * For example, if the $path="includes/synergy.inc" and
     * $root="/home/synergy" then the result is
     * "/home/synergy/includes/synergy.inc".
     *
     * If no root path (or null) is specified then the default file system
     * installation path is used as returned by {@link SystemPath()}. To
     * prefix no path specify an empty string ''.
     *
     * Calls {@link TNTFileSystem::MakeAbsolutePath()} on 'dirname'.
     *
     * @access  public
     * @return  void
     */
    function MakeAbsolute($root = null) {
        TNTLoader::RequireClass('TNTFileSystem');
        $this->SetElement('dirname', TNTFileSystem::MakeAbsolutePath($this->GetElement('dirname'), $root));
    }
    
    /**
     * Make the path relative to given $root.
     *
     * If the given $path is a child path of the given $root, then the
     * $root prefix is removed to make $path a relative path to $root.
     *
     * For example, if the $path="/home/synergy/includes/synergy.inc" and
     * $root="/home/synergy" then the result is "includes/synergy.inc".
     *
     * If no root path (or null) is specified then the default file system
     * installation path is used as returned by {@link SystemPath()}. To
     * prefix no path specify an empty string ''.
     *
     * Calls {@link TNTFileSystem::MakeRelativePath()} on 'dirname'.
     *
     * @access  public
     * @return  void
     */
    function MakeRelative($root = null) {
        TNTLoader::RequireClass('TNTFileSystem');
        $this->SetElement('dirname', TNTFileSystem::MakeRelativePath($this->GetElement('dirname'), $root));
    }
    
    /**
     * Fixup given path so that, absolute paths are relative to installation
     * path and relative paths are relative to this files base path.
     *
     * Calls {@link TNTFileSystem::FixupPath()} on 'dirname'.
     *
     * @access  public
     * @param   string      $base       base path for relative paths
     * @return  void
     */
    function Fixup($base) {
        TNTLoader::RequireClass('TNTFileSystem');
        $this->SetElement('dirname', TNTFileSystem::FixupPath($this->GetElement('dirname'), $root));
    }

    /**
     * Helper function to get a element in a path string
     *
     * @access  public
     * @param   mixed       $data        initial Path data
     * @param   string      $elm         element name
     * @return  string                   element value (or null)
     * @static
     */
    /*static*/ function GetPathElement($data, $elm) {
        return parent::GetDataElement(__CLASS__, $data, $elm);
    }

    /**
     * Helper function to set a element in a path string
     *
     * @access  public
     * @param   mixed       $data        initial Path data
     * @param   string      $elm         element name
     * @param   string      $val         element value
     * @return  mixed                    data with element set
     * @static
     */
    /*static*/ function SetPathElement($path, $elm, $val) {
        return parent::SetDataElement(__CLASS__, $data, $elm, $val);
    }

    /**
     * Construct the object
     *
     * Any initial value is passed to {@link TNTPath::Set()}
     *
     * @access  public
     * @param   mixed       $data       initial value for object
     * @return  void
     */
    function TNTPath($data = null) {
        TNTFmtStr::TNTFmtStr($data, $GLOBALS['tnt']['tntpath']['_elementNames']);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTPath() {
    }
}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
