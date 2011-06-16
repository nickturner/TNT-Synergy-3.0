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
 * INI File Class
 * ~~~~~~~~~~~~~~
 *
 * This class represents reading and writing of initialisation files etc.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version     2.0
 */

/**
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTVarMap');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent an INI file.
 *
 * Although this class allows the writing of INI files it will not preserve
 * any comments if a file is read and then overwritten.
 *
 * If handles standard format INI files and more advanced XML format files.
 *
 * It also supports the collapsing of INI section and variable names to
 * comply with PHP variable naming conventions.
 *
 * While parsing standard format INI files it also supports simple array
 * values using names of the form 'var[index]' or 'var.index'.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTIni extends TNTVarMap {

    /**
     * Gets the number of sections or the number of variables
     * stored in the section.
     *
     * If a section does not exist then -1 will be returned as opposed to
     * zero for a section with no variables.
     *
     * @access  public
     * @param   string      $sect       INI section to count
     * @return  integer                 number of sections/variables stored
     */
    function Count($sect = null) {

        $sect = strtolower($sect);

        if ($sect) {
            $ret = (isset($this->_vars[$sect]) ? count($this->_vars[$sect]) : -1);
        } else {
            $ret = count($this->_vars);
        }

        return $ret;
    }

    /**
     * Gets a copy of the ini section or the whole ini map.
     *
     * @access  public
     * @param   string      $sect       INI section to return
     * @return  mixed[]                 the internal map
     */
    function Items($sect = null) {

        $sect = strtolower($sect);

        if ($sect) {
            $ret = (isset($this->_vars[$sect]) ? $this->_vars[$sect] : null);
        } else {
            $ret = $this->_vars;
        }

        return $ret;
    }

    /**
     * Gets a array of keys stored in the section or the whole ini map.
     *
     * @access  public
     * @param   string      $sect       INI section to return
     * @return  string[]                array of keys
     */
    function Keys($sect = null) {

        $sect = strtolower($sect);

        if ($sect) {
            $ret = (isset($this->_vars[$sect]) ? array_keys($this->_vars[$sect]) : null);
        } else {
            $ret = array_keys($this->_vars);
        }

        return $ret;
    }

    /**
     * Gets a array of values stored in the section or the whole ini map.
     *
     * @access  public
     * @param   string      $sect       INI section to return
     * @return  mixed[]                 array of values
     */
    function Values($sect = null) {

        $sect = strtolower($sect);

        if ($sect) {
            $ret = (isset($this->_vars[$sect]) ? array_values($this->_vars[$sect]) : null);
        } else {
            $ret = array_values($this->_vars);
        }

        return $ret;
    }

    /**
     * Does a variable(s) exist in the given section
     *
     * If name is an array then it is assumed to be an array of variables
     * to check for and the return is only true if all exist.
     *
     * If name is null then it checks the section exits.
     *
     * @access  public
     * @param   string      $sect       section to look in
     * @param   string      $name       name of variable
     * @return  bool                    true if variable exists
     */
    function HasVar($sect, $name) {

        $sect = strtolower($sect);

        if (is_array($name)) {
            $ret = true;
            foreach ($name as $n) {
               if (!isset($this->_vars[$sect][$n])) {
                   $ret = false;
                   break;
               }
            }
        } else if ($name) {
            $name = strtolower($name);

            $ret = (isset($this->_vars[$sect][$name]));
        } else {
            $ret = (isset($this->_vars[$sect]));
        }

        return $ret;
    }

    /**
     * Get the value of a variable(s).
     *
     * If name is an array then it is assumed to be an array of variables
     * to get and in which case the return value is an array of values
     * indexed by key.
     *
     * If name is null then an array of all the variable in the section is
     * returned.
     *
     * @access  public
     * @param   string      $sect       section to look in
     * @param   string      $name       name of variable (or array of names)
     * @param   mixed       $default    default value to return if no variable
     * @return  mixed                   value of the variable(or null)
     */
    function GetVar($sect, $name, $default = null) {

        $sect = strtolower($sect);

        if (is_array($name)) {
            foreach ($name as $n) {
                $n = strtolower($n);
                $ret[$n] = (isset($this->_vars[$sect][$n]) ? $this->_vars[$sect][$n] : $default);
            }
        } else if ($name) {
            $name = strtolower($name);

            $ret = (isset($this->_vars[$sect][$name]) ? $this->_vars[$sect][$name] : $default);
        } else {
            $ret = (isset($this->_vars[$sect]) ? $this->_vars[$sect] : $default);
        }

        return $ret;
    }
    
    /**
     * Get the value of a session variable (by reference)
     *
     * If name is an array then it is assumed to be an array of variables
     * to get and in which case the return value is an array of values
     * indexed by key.
     *
     * If name is null then an array of all the variable in the section is
     * returned.
     *
     * @access  public
     * @param   string      $sect       section to look in
     * @param   string      $name       name of variable (or array of names)
     * @return  mixed                   reference to the variable
     */
    function &GetVarByRef($sect, $name) {

        $ret = null;
    
        $sect = strtolower($sect);

        if (is_array($name)) {
            foreach ($name as $n) {
                $n = strtolower($n);
                if (isset($this->_vars[$sect][$n])) {
                    $ret[$n] =& $this->_vars[$sect][$n];
                } else {
                    $ret[$n] = null;
                }
            }
        } else if ($name) {
            $name = strtolower($name);

            if (isset($this->_vars[$sect][$name])) {
                $ret =& $this->_vars[$sect][$name];
            } else {
                $ret = null;
            }
        } else {
            if (isset($this->_vars[$sect])) {
                $ret =& $this->_vars[$sect];
            } else {
                $ret = null;
            }
        }

        return $ret;
    }

    /**
     * Set the value of a session variable
     *
     * If the map is read only then this fails. If variable doesn't exist and
     * the map is fixed size then this fails. Otherwise the named variable is
     * modified or added.
     *
     * If called with $name == null and $val is an associated array
     * then each key => value pair in $val is set in the map.
     *
     * @access  public
     * @param   string      $sect       section to look in
     * @param   string      $name       name of variable
     * @param   mixed       $val        value of variable
     * @return  boolean                 true if set successfull
     */
    function SetVar($sect, $name, $val) {

        $ret = true;

        $sect = strtolower($sect);
        $name = strtolower($name);

        if ($this->isReadOnly) {
            $this->_SetError('variables are read only');
            $ret = false;
        } else {

            if ($name == null && is_array($val)) {
                $v = $val;
            } else {
                $v = array($name => $val);
            }

            foreach ($v as $name => $val) {
                if (!isset($this->_vars[$sect][$name]) && $this->isFixedSize) {
                    $this->_SetError('variable does not exist and map is fixed size');
                    $ret = false;
                    break;
                } else {
                    if (!isset($this->_vars[$sect])) {
                        $this->_vars[$sect] = array();
                    }
                    $this->_vars[$sect][$name] = $val;
                }
            }
        }

        return $ret;
    }
    
    /**
     * Set the value of a session variable (by reference)
     *
     * If the map is read only then this fails. If variable doesn't exist and
     * the map is fixed size then this fails. Otherwise the named variable is
     * modified or added.
     *
     * If called with $name == null and $val is an associated array
     * then each key => value pair in $val is set in the map.
     *
     * @access  public
     * @param   string      $sect       section to look in
     * @param   string      $name       name of variable
     * @param   mixed       $val        reference to variable
     * @return  boolean                 true if set successfull
     */
    function SetVarByRef($sect, $name, &$val) {

        $ret = true;

        $sect = strtolower($sect);
        $name = strtolower($name);

        if ($this->isReadOnly) {
            $this->_SetError('variables are read only');
            $ret = false;
        } else {

            if ($name == null && is_array($val)) {
                $v = $val;
            } else {
                $v = array($name => &$val);
            }

            foreach ($v as $name => $val) {
                if (!isset($this->_vars[$sect][$name]) && $this->isFixedSize) {
                    $this->_SetError('variable does not exist and map is fixed size');
                    $ret = false;
                    break;
                } else {
                    if (!isset($this->_vars[$sect])) {
                        $this->_vars[$sect] = array();
                    }
                    $this->_vars[$sect][$name] =& $v[$name];
                }
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
     * Merge the given data with the existing data
     *
     * The argument can either be an array of INI data correctly formatted or
     * another TNTIni Object to take the data from.
     *
     * @access  public
     * @param   array       $data       array of initialisation data
     * @return  boolean                 true if set successfull
     */
    function MergeData($data) {

        $ret = true;

        if (is_a($data, 'TNTIni')) {
            $data = $data->_vars;
        }

        foreach ($data as $s => $d) {
            if (isset($this->_vars[$s]) && is_array($this->_vars[$s])) {
                $this->_vars[$s] = array_merge($this->_vars[$s], (array)$d);
            } else {
                $this->_vars[$s] = $d;
            }
        }

        return $ret;
    }

    /**
     * Load an INI file.
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
                $cachePath = '~cache/ini/'.preg_replace('/[^\w_]/', '_', $path);
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

                        // Expand all embedded array key
                        foreach ($ini as $sectname => $sectdata) {
                            $sectname = strtolower($sectname);
                            settype($data[$sectname], 'array');
                            foreach ($sectdata as $name => $value) {
                                $name = strtolower($name);
                                if ($name{0} == '@') {
                                    // Command
                                    switch ($name) {
                                        case '@import':
                                            // Import another file at this scope
                                            $import =& new TNTIni();
                                            if (!($import->LoadFile($value, false, $type, $cache))) {
                                                $this->_SetError('Can not import: '.$value, $import);
                                                $data = null;
                                                break;
                                            }

                                            $d = $import->Items();
                                            foreach ($ini as $_sectname => $_sectdata) {
                                                $data[$sectname][$_sectname] = $_sectdata;
                                            }

                                            break;

                                        default:
                                            $this->_SetError('Array conflicts with existing ini value: '.$base);
                                            $data = null;
                                            break;
                                    }
                                    if (is_null($data)) {
                                        break;
                                    }
                                } else if (preg_match('/^([^[]*)(\[.*\])$/', $name, $matches)) {
                                    $base = $matches[1];
                                    $dims = $matches[2];
                                    // make the array as required
                                    if (isset($sectdata[$base])) {
                                        $this->_SetError('Array conflicts with existing ini value: '.$base);
                                        $data = null;
                                        break;
                                    }

                                    // Ensure the element exists by making a
                                    // dummy reference to it (referencing an
                                    // array element creates the element if it
                                    // didn't previously exist)
                                    $php = "unset(\$_ref); \$_ref =& \$data['$sectname']['$base']$dims; \$_ref = \$value;";
                                    eval($php);
                                    if (!isset($_ref)) {
                                        $this->_SetError('Invalid array definition: '.$name);
                                        $data = null;
                                        break;
                                    }
                                } else if (strpos($name, '.') !== FALSE) {
                                    $dims = explode('.', trim($name, '.'));
                                    $base = array_shift($dims);
                                    // make the array as required
                                    if (isset($sectdata[$base])) {
                                        $this->_SetError('Array conflicts with existing ini value: '.$base);
                                        $data = null;
                                        break;
                                    }

                                    $_dims = '';
                                    foreach ($dims as $d) {
                                        $_dims .= '[\''.addcslashes($d, '\0..\37\177..\377"\'').'\']';
                                    }

                                    // Ensure the element exists by making a
                                    // dummy reference to it (referencing an
                                    // array element creates the element if it
                                    // didn't previously exist)
                                    $php = "unset(\$_ref); \$_ref =& \$data['$sectname']['$base']$_dims; \$_ref = \$value;";
                                    eval($php);
                                    if (!isset($_ref)) {
                                        $this->_SetError('Invalid array definition: '.$name);
                                        $data = null;
                                        break;
                                    }
                                } else {
                                    $data[$sectname][$name] = $value;
                                }
                            }
                            if ($data == null) {
                                break;
                            }
                        }
                        break;

                    case 'xml':
                        // TODO
                        $data = false;
                        break;
                }

                if (!$data) {
                    $this->_SetError('Can\'t parse file: '.$path);
                    break;
                }

                // Cache it
                if ($cache) {
                    TNTFileSystem::SetCachedData($cachePath, $data);
                }
            }

            if ($merge) {
                $this->MergeData($data);
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
                if (is_string($var)) {
                    $ret .= $this->_Fmt("{$name}['{$var}']", $val);
                } else {
                    $ret .= $this->_Fmt("{$name}[{$var}]", $val);
                }
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
                $type = extname($path);
            }

            if ($type != 'ini' && $type != 'xml') {
                $this->_SetError('Unknown INI file type: '.$type);
                break;
            }

            $data = '';
            switch ($type) {
                case 'ini':
                    foreach ($this->_vars as $sectname => $sectdata) {
                        $data .= "\n[$sectname]\n";
                        foreach ($sectdata as $name => $value) {
                            $data .= $this->_Fmt($name, $value);
                        }
                    }
                    break;

                case 'xml':
                    // TODO
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
    function TNTIni() {

        TNTVarMap::TNTVarMap();

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTIni() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
