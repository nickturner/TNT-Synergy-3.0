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
 * Registry Class
 * ~~~~~~~~~~~~~~
 *
 * This class supported methods to read and write a MS Windows style registry
 * of configuration data.
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
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Private class to represent a value in the registry.
 *
 * Registry keys are the base unit of organization in the registry, and can be
 * compared to folders & files in the file system.
 *
 * Unlike the MS Windows registries no distinction is made between keys and
 * values.
 *
 * The class supports the read and writing of registries from either INI
 * format files or XML format files.
 *
 * When reading INI format files the INI variable names are the registry
 * keys separated by '/' characters. If sections are used then they form
 * the root key to which the variables are added.
 *
 * Registry keys are not case sensitive, and can not '/' characters.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @access  private
 */
class _TNTRegVar {
    /**
     * Holds the value stored at a registry key node
     * @var     mixed                   value stored
     * @access  private
     */
    var $_value = null;
};

/**
 * Class to represent a registry of values.
 *
 * Registry keys are the base unit of organization in the registry, and can be
 * compared to folders & files in the file system.
 *
 * Unlike the MS Windows registries no distinction is made between keys and
 * values.
 *
 * The class supports the read and writing of registries from either INI
 * format files or XML format files.
 *
 * When reading INI format files the INI variable names are the registry
 * keys separated by '/' characters. If sections are used then they form
 * the root key to which the variables are added.
 *
 * Registry keys are not case sensitive, and can not '/' characters.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTReg extends TNTObject {

    /**
     * Gets a value indicating whether loaded files are cached.
     * @var     boolean                 true if caching should be used
     * @access  private
     */
    var $_useCaching = true;

    /**
     * Actual registry store
     * @var     array                   hierachical array of values
     * @access  private
     */
    var $_reg = array();

    /**
     * Internal method to return a reference to the variable at a given
     * key.
     *
     * If $new is specified then a reference is returned to a new unset
     * location at which to store a variable with this key.
     *
     * @access  public
     * @param   string      $key        key to return a reference to
     * @param   boolean     $new        create the key if missing
     * @return  object                  reference to _TNTRegVar (or null)
     */
    function _Var($key, $new = false) {
        $key = strtolower($key);

        $v = '$this->_reg';
        foreach (explode('/', $key) as $k) {
            $v .= '[\''.addcslashes($k, '\0..\37\177..\377"\'').'\']';
        }

        if (!$new) {
            // See if the key exists
            $php = "return isset($v);";
            $exists = eval($php);
            if (!$exists) {
                return null;
            }
        }

        // Ensure the element exists by making a dummy reference to it
        // (referencing an array element creates the element if it
        // didn't previously exist)
        $php = "unset(\$r); \$r =& $v;";
        eval($php);

        if (!is_a($r, '_tntregvar')) {
            if (!$new) {
                return null;
            }
            $r = new _TNTRegVar();
        }

        return $r;
    }

    /**
     * Does a value exist with the given key.
     *
     * If key is an array then it is assumed to be an array of keys
     * to check for and the return is only true if all exist.
     *
     * @access  public
     * @param   string      $key        key to check
     * @return  bool                    true if key exists
     */
    function HasVar($key) {

        if (is_array($key)) {
            foreach ($key as $k) {
                $val =& $this->_Var($key);
                if (!($ret = isset($val))) {
                    break;
                }
                unset($val);
            }
        } else {
            $val =& $this->_Var($key);
            $ret = ($val ? true : false);
        }

        return $ret;
    }

    /**
     * Get the value of a registry key.
     *
     * If $key is an array then it is assumed to be an array of keys to get
     * and in this case $def is an optional array of defaults for each key
     * and the return value is an array of references.
     *
     * @access  public
     * @param   string      $key        key to get value from
     * @param   mixed       $def        default value to return if not exist
     * @return  mixed                   value at given key (or null)
     */
    function GetVar($key, $def = null) {

        if (is_array($key)) {
            $ret = array();
            foreach ($key as $k) {
                if (($var =& $this->_Var($key))) {
                    $ret[$k] = $var->_value;
                } else {
                    $ret[$k] = (isset($def[$k]) ? $def[$k] : null);
                }
                unset($val);
            }
        } else {
            if (($var =& $this->_Var($key))) {
                $ret = $var->_value;
            } else {
                $ret = $def;
            }
        }

        return $ret;
    }
    
    /**
     * Return a reference to a registry key.
     *
     * If $key is an array then it is assumed to be an array of keys to get
     * and in this case $def is an optional array of defaults for each key
     * and the return value is an array of references.
     *
     * @access  public
     * @param   string      $key        key to get value from
     * @param   mixed       $def        default value to return if not exist
     * @return  mixed                   reference to given key (or void)
     */
    function &GetVarByRef($key, $def = null) {

        if (is_array($key)) {
            $ret = array();
            foreach ($key as $k) {
                if (($var =& $this->_Var($key))) {
                    $ret[$k] =& $var->_value;
                } else {
                    $ret[$k] = (isset($def[$k]) ? $def[$k] : null);
                }
                unset($var);
            }
        } else {
            if (($var =& $this->_Var($key))) {
                $ret =& $var->_value;
            } else {
                $ret = $def;
            }
        }
    
        return $ret;
    }

    /**
     * Set the value of a registry key.
     *
     * If $key is an array then it is assumed to be an array of key => value
     * pairs to set. In which case $val is ignored.
     *
     * @access  public
     * @param   string      $key        key to set value at
     * @param   mixed       $val        value to store
     * @return  boolean                 true if set successfull
     */
    function SetVar($key, $val) {

        if (is_array($key)) {
            $ret = true;
            foreach ($key as $k => $v) {
                if (!($var =& $this->_Var($key, true))) {
                    $ret = false;
                    break;
                }
                $var->_value = $v;
                unset($var);
            }
        } else {
            $ret = false;
            if (($var =& $this->_Var($key, true))) {
                $var->_value = $val;
                $ret = true;
            }
        }

        return $ret;
    }

    /**
     * Set the value of a registry key to be a reference.
     *
     * If $key is an array then it is assumed to be an array of key => value
     * pairs to set. In which case $val is ignored.
     *
     * @access  public
     * @param   string      $key        key to set value at
     * @param   mixed&      $val        reference to store
     * @return  boolean                 true if set successfull
     */
    function SetVarByRef($key, &$val) {

        if (is_array($key)) {
            $ret = true;
            foreach ($key as $k => $v) {
                if (!($var =& $this->_Var($key, true))) {
                    $ret = false;
                    break;
                }
                $var->_value =& $key[$k];
                unset($var);
            }
        } else {
            $ret = false;
            if (($var =& $this->_Var($key, true))) {
                $var->_value =& $val;
                $ret = true;
            }
        }

        return $ret;
    }
    
    /**
     * Create lower case versions of all the INI keys.
     *
     * @access  private
     * @param   array                   reference to array to change case of
     * @return  void
     */
    function _ChangeCase(&$data) {
        foreach (array_keys($data) as $n) {
            if (($c = strtolower($n)) != $n) {
                $data[$c] =& $data[$n];
            }
            if (is_array($data[$n])) {
                $this->_ChangeCase($data[$n]);
            }
        }
    }

    /**
     * Recursively merge array1 with array2 where the values in array1 override
     * the values in array2 unless both values are arrays in which case they
     * are recursively merged.
     *
     * The keys in array 2 are converted to lowercase in the process.
     *
     * @access public
     * @param  array    $a1         reference to first array
     * @param  array    $a2         reference to second array
     * @return void
     */
    function _MergeData(&$a1, &$a2) {
        foreach (array_keys($a2) as $k2) {
            $k2l = strtolower($k2);
            if (is_array($a1[$k2l]) && is_array($a2[$k2])) {
                $this->_MergeData($a1[$k2l], $a2[$k2]);
            } else {
                $a1[$k2l] = $a2[$k2];
            }
        }
    }

    /**
     * Merge the given data with the existing data
     *
     * The argument can either be an array of data correctly formatted or
     * another TNTReg object to take the data from.
     *
     * @access  public
     * @param   array       $data       array data to merge
     * @return  void
     */
    function MergeData($data) {

        if (is_a($data, 'TNTReg')) {
            $data = $data->_reg;
        }

        $this->_MergeData($this->_reg, $data);

        return $ret;
    }

    /**
     * Load data from a file.
     *
     * Files ending in .INI are assumed to be standard INI files and those
     * ending in .XML are assumed to be XML ini files. Others require the
     * type to be passed.
     *
     * Supports caching of the file so that repeated loading of the file
     * will only reparse it if altered since the last time.
     *
     * @access  public
     * @param   string      $path       path to file to load
     * @param   boolean     $merge      merge contents into existing values
     * @param   string      $type       either 'ini', 'xml' or null
     * @return  boolean                 true if file loaded ok
     */
    function LoadFile($path, $merge = true, $type = null) {

        TNTLoader::RequireClass('TNTPath');
        TNTLoader::RequireClass('TNTFileSystem');

        $ret = false;

        // Start error section
        do {
            if (is_null($type)) {
                $type = TNTPath::GetPathElement($path, 'extension');
            }

            if ($type != 'ini' && $type != 'xml') {
                $this->_SetError('Unknown INI file type: '.$type);
                break;
            }

            if (($mtime = TNTFileSystem::Stats($path, 'mtime')) <= 0) {
                $this->_SetError('Can\'t get file stats: '.$path);
                break;
            }

            $data = null;
            $cache = null;
            if ($this->useCaching) {
                TNTLoader::RequireClass('TNTFileCache');

                $cachePath = '/tnt/cache/ini/'.preg_replace('/[^\w_]/', '_', $path);

                $cache =& new TNTFileCache($cachePath);
                $cache->cacheLife = 0;
            
                $data = $cache->GetData($mtime);

                destroy($cache);
            }

            switch ($type) {
                case 'ini':
                    $_path = TNTFileSystem::ExpandPath($path);

                    if (!$data) {
                        $data = parse_ini_file($_path, true);
                    }

                    if (!$data) {
                        $this->_SetError('Can\'t parse file: '.$path);
                        break;
                    }

                    if (!$merge) {
                        $this->_reg = array();
                    }

                    foreach ($data as $sectname => $sectdata) {
                        foreach ($sectdata as $name => $value) {
                            $r =& $this->_Var($name, true);
                            $r = $value;
                        }
                    }

                    break;

                case 'xml':
                    // TODO
                    $this->_SetError('XML files are not supported yet: '.$path);
                    $data = false;
                    break;
            }

            if (!$data) {
                break;
            }

            // Cache it
            if ($cache) {
                $cache->SetData($data);
            }

            $ret = true;

        } while(false);
        // End error section

        $this->_Exit(__CLASS__, __FUNCTION__, $args, $ret);
        return $ret;
    }

    /**
     * Format an INI entry for output to file.
     *
     * @access  private
     * @param   string      $name       name of INI entry
     * @param   string      $value      value of INI entry
     * @return  string                  formatted output
     */
    function _Fmt($name, $value) {
        $ret = "";
        if (is_array($value)) {
            foreach ($value as $var => $val) {
                $ret .= $this->_Fmt("{$name}/{$var}", $val);
            }
        } else if (is_bool($value)) {
            $ret = "$name = ".($value ? 'Yes' : 'No')."\n";
        } else if (is_numeric($value)) {
            $ret = "$name = $value\n";
        } else if (is_string($value)) {
            $ret = "$name = \"$value\"\n";
        } else {
            $ret = "$name = ".serialize($value)."\n";
        }
        return $ret;
    }

    /**
     * Save an INI file.
     *
     * Files ending in .INI are assumed to be standard INI files and those
     * ending in .XML are assumed to be XML ini files. Others require the
     * type to be passed.
     *
     * If you load an INI file and then save it, any original formatting and
     * comments will be lost.
     *
     * @access  public
     * @param   string      $path       path to file to load
     * @param   string      $type       either 'ini', 'xml' or null
     * @return  boolean                 true if file saved ok
     */
    function SaveFile($path, $type = null) {
        $args = array('path' =>& $path, 'type' =>& $type);
        $ret = null;

        if (!($this->_Enter(__CLASS__, __FUNCTION__, $args, $ret))) {
            return $ret;
        }

        $ret = false;

        // Start error section
        do {
            if (is_null($type)) {
                $type = TNTPath::GetPathElement($path, 'extension');
            }

            if ($type != 'ini' && $type != 'xml') {
                $this->_SetError('Unknown INI file type: '.$type);
                break;
            }

            $data = '';
            switch ($type) {
                case 'ini':
                    foreach ($this->_reg as $name => $value) {
                        $data .= $this->_Fmt($name, $value);
                    }
                    break;

                case 'xml':
                    // TODO
                    $this->_SetError('XML files are not supported yet: '.$path);
                    $data = false;
                    break;
            }

            $opts = (TNT_FILESYSTEM_IO_CREATE|TNT_FILESYSTEM_IO_LOCK);

            if (!TNTFileSystem::SetFileContents($path, $opts, $data, 0777)) {
                $this->_SetError('Failed to write file: '.$path);
                break;
            }

            $ret = true;

        } while(false);
        // End error section

        $this->_Exit(__CLASS__, __FUNCTION__, $args, $ret);
        return $ret;
    }
            
    /**
     * Construct the object
     *
     * @access  public
     * @return  void
     */
    function TNTReg() {

        TNTObject::TNTObject();

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTReg() {

        $this->_reg = null;

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
