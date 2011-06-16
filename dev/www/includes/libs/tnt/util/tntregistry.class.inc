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
 * Class to represent a registry of values.
 *
 * The registry class encapsulates a standard PHP associative array, with
 * methods to access any path through the array using the 'x/y/z' syntax as
 * opposed to the PHP ['x']['y']['z] syntax. This allows any branch in the
 * registry tree to be queried without worrying about the non existance of
 * parent branches.
 *
 * The class supports the read and writing of registries from either INI
 * format files or XML format files.
 *
 * Registry keys are not case sensitive, and can not '/' characters.
 *
 * Due to a limitation in PHP the registry can not be used to contain NULL
 * values. If a NULL value is stored it will effective clear the registry
 * key, {@link HasVar()} will return false, and {@link GetVar()} will return
 * the default value.
 *
 * When reading INI format files the registry keys are taken as a concatenation
 * of the section name with the value name.
 *
 * For example;
 *   a)
 *          [x/y]
 *          z = v
 *
 *   b)
 *          [x]
 *          y/z = v
 *
 * both a) and b) result in a registry value 'v' being stored under the key
 * 'x/y/z'. The INI section names make it easy to group common key prefixes.
 *
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTRegistry extends TNTVarMap {

    /**
     * Internal method to return the PHP used to reference the given key.
     *
     * @access  public
     * @param   string      $key        key to return a reference to
     * @return  string                  key in PHP syntax notation
     */
    function _PHPKey($key, $base = null) {
        $php = ($base ? '$'.$base : '$this->_vars');
        foreach (explode('.', strtolower($key)) as $k) {
            $php .= '[\''.addcslashes($k, '\0..\37\177..\377"\'').'\']';
        }
        return $php;
    }

    /**
     * Gets the number of entries in the branch at the given key.
     *
     * If a branch doesn't exist then null is returned.
     *
     * @access  public
     * @return  integer                 number of variables stored
     */
    function Count($key = null) {
        if (is_null($key)) {
            return count($this->_vars);
        }

        $a = $this->_PHPKey($key);
        return eval("return (isset($a) ? count($a) : null);");
    }

    /**
     * Gets a copy of the branch at the given key.
     *
     * If a branch doesn't exist then null is returned.
     *
     * @access  public
     * @return  mixed[]                 the internal map
     */
    function Items($key = null) {
        if (is_null($key)) {
            return $this->_vars;
        }

        $a = $this->_PHPKey($key);
        return eval("return (isset($a) ? $a : null);");
    }

    /**
     * Gets a array of keys stored at the given key.
     *
     * If a branch doesn't exist then null is returned.
     *
     * @access  public
     * @return  string[]                array of keys
     */
    function Keys($key = null) {
        if (is_null($key)) {
            return array_keys($this->_vars);
        }

        $a = $this->_PHPKey($key);
        return eval("return (isset($a) ? array_keys($a) : null);");
    }

    /**
     * Gets a array of values stored at the given key.
     *
     * If a branch doesn't exist then null is returned.
     *
     * @access  public
     * @return  mixed[]                 array of values
     */
    function Values($key = null) {
        if (!$key) {
            return array_values($this->_vars);
        }

        $a = $this->_PHPKey($key);
        return eval("return (isset($a) ? array_values($a) : null);");
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
                $a = $this->_PHPKey($k);
                if (!eval("return isset($a);")) {
                    return false;
                }
            }
            return true;
        }

        $a = $this->_PHPKey($key);
        return eval("return isset($a);");
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
            foreach ($key as $i => $k) {
                $a = $this->_PHPKey($k);
                $ret[$i] = eval("return (isset($a) ? $a : \$def)");
            }
            return $ret;
        }

        $a = $this->_PHPKey($key);
        return eval("return (isset($a) ? $a : \$def);");
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
            foreach ($key as $i => $k) {
                $a = $this->_PHPKey($k);
                eval("if (isset($a)) { \$ret[\$i] =& $a; } else { \$ret[\$i] =  \$def); }");
            }
            return $ret;
        }

        $a = $this->_PHPKey($key);
        eval("if (isset($a)) { \$ret =& $a; } else { \$ret =  \$def); }");
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
            foreach ($key as $k => $v) {
                $a = $this->_PHPKey($k);
                eval("unset(\$r); \$r =& $a; $a = \$v; unset(\$r);");
            }
            return true;
        }

        $a = $this->_PHPKey($key);
        eval("unset(\$r); \$r =& $a; $a = \$val; unset(\$r);");

        return true;
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
            foreach ($key as $k => $v) {
                $a = $this->_PHPKey($k);
                eval("unset(\$r); \$r =& $a; $a =& \$key[$k]; unset(\$r);");
            }
            return true;
        }

        $a = $this->_PHPKey($key);
        eval("unset(\$r); \$r =& $a; $a =& \$val; unset(\$r);");

        return true;
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

        if (is_a($data, 'TNTRegistry')) {
            $data = $data->_vars;
        }

        $this->_MergeData($this->_vars, $data);

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
     * @param   boolean     $cache      use file caching
     * @return  boolean                 true if file loaded ok
     */
    function LoadFile($path, $merge = true, $type = null, $cache = false) {

        TNTLoader::RequireClass('TNTFileSystem');

        $ret = false;

        // Start error section
        do {
            if (is_null($type)) {
                $type = extname($path);
            }

            if ($type != 'ini' && $type != 'xml') {
                $this->_SetError('Unknown INI file type: '.$type);
                break;
            }

            $data = null;

            if ($cache) {
                $cachePath = '~cache/reg/'.preg_replace('/[^\w_]/', '_', $path);
                $data = TNTFileSystem::GetCachedData($cachePath);
            }

            if (!$data) {
                switch ($type) {
                    case 'ini':
                        $_path = TNTFileSystem::ExpandPath($path);
    
                        $ini = parse_ini_file($_path, true);
    
                        if (!$ini) {
                           $this->_SetError('Can\'t parse file: '.$_path);
                           break;
                        }
    
                        $data = array();
    
                        foreach ($ini as $sectname => $sectdata) {
                            foreach ($sectdata as $name => $value) {
                                if ($name{0} == '@') {
                                    // Command
                                    switch ($name) {
                                        case '@import':
                                            // Import another file at this scope
                                            $import =& new TNTRegistry();
                                            if (!($import->LoadFile($value, false, $type, $cache))) {
                                                $this->_SetError('Can not import: '.$value, $import);
                                                $data = null;
                                                break;
                                            }

                                            $d = $import->Items();
                                            $a = $this->_PHPKey($sectname);
                                            eval("unset(\$r); \$r =& $a; $a = array_merge_recursive((array)$a, \$d); unset(\$r);");

                                            break;

                                        default:
                                            $this->_SetError('Unknown INI command: '.$name);
                                            $data = null;
                                            break;
                                    }
                                    if (is_null($data)) {
                                        break;
                                    }

                                } else {
                                    $a = $this->_PHPKey($sectname.'.'.$name, 'data');
                                    eval("unset(\$r); \$r =& $a; $a = \$value; unset(\$r);");
                                }
                            }
                            if (!$data) {
                                break;
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
                    TNTFileSystem::SetCachedData($cachePath, $data);
                }
            }

            if ($this->_vars && $merge) {
                $this->_MergeData($this->_vars, $data);
            } else {
                $this->_vars = $data;
            }

            $ret = true;

        } while(false);
        // End error section

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
                    foreach ($this->_vars as $name => $value) {
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

        $this->_vars = null;

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
