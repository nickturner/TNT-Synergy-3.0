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
 * Loader Class
 * ~~~~~~~~~~~~
 *
 * A static class to ease the loading of PHP functions and classes.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */

/*
 * Globals
 */
global $tnt;
$tnt['tntloader']['scanned'] = null;
$tnt['tntloader']['declare'] = null;
$tnt['tntloader']['include'] = null;
$tnt['tntloader']['rescanned'] = false;
$tnt['tntloader']['enable_profiling'] = true;
$tnt['tntloader']['profile'] = null;
$tnt['tntloader']['depth'] = 0;

/*
 * Defines
 */
require_once('tntclass.class.inc');

/**
 * Static loader class
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTLoader extends TNTClass {

    /**
     * Scan the given directory for PHP declarations.
     *
     * The method scans all source files for function and class definitions.
     *
     * If a class or function definition is found in more than one file then
     * the file with the newer modification timestamp is used.
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $dir        the directory to scan
     * @param   boolean     $incSubDirs true if all subdirs should be scanned
     * @return  void
     */
    /*static*/ function _ScanDir($dir = TNT_ROOT_DIR, $incSubDirs = true) {
        global $tnt;

        // Mark directory as scanned
        $tnt['tntloader']['scanned'][$dir] = true;

        $dh = opendir($dir);
        if (!$dh) {
            //echo "Failed to scan directory: $dir<br />";
            return;
        }
        //echo "Scanning directory: $dir<br />";
        while (($file = readdir($dh)) !== false) {
            if ($file{0} == '.') {
                continue;
            }
            // Skip certian directories we know are safe not to scan -
            // unless they are seen within an includes directory.
            if (!strstr($dir, 'includes') && in_array($file, array('temp', 'cache', 'media'))) {
                continue;
            }
            $path = $dir.'/'.$file;
            //echo "Found file: $path<br />";
            if (is_file($path) && (strpos($file, '.php') !== false || strpos($file, '.inc') !== false)) {
                // Scan the file
                //echo "Checking file: $path<br />";
                $code = file_get_contents($path);
                //$code = preg_replace('/\/\*.*\*\//Uis', '', $code);
                if (strlen($code) < 10) {
                    //echo "File is too short: $path<br />";
                    continue;
                }
                // Quick search before doing slow preg_match
                if (strpos($code, 'class') === false &&
                    strpos($code, 'function') === false) {
                    //echo "No declarations in file: $path<br />";
                    unset($code); // free some memory ?
                    continue;
                }
                if (!preg_match_all('/^(class|function)\s+(\w+)(\s+|\()/ims', $code, $matches, PREG_SET_ORDER)) {
                    //echo "No declarations in file: $path<br />";
                    unset($code); // free some memory ?
                    continue;
                }
                unset($code); // free some memory ?

                $max_i = count($matches);
                //echo "Found $max_i declarations in file: $path<br />";
                for ($i = 0; $i < $max_i; $i++) {
                    $what = strtolower($matches[$i][1]);
                    $name = strtolower($matches[$i][2]);
                    $d =& $tnt['tntloader']['declare'][$what][$name];
                    if (is_string($d)) {
                        // We have already seen a declaration
                        if (filemtime($d) < filemtime($path)) {
                            //echo "Newer declaration of $what $name in file: $file<br />";
                            $d = $path;
                        }
                    } else {
                        // First time we have seen a declaration
                        //echo "Declaration of $what $name in file: $file<br />";
                        $d = $path;
                    }
                }
                unset($matches); // free some memory ?
                continue;
            }

            if ($incSubDirs && is_dir($path) && empty($tnt['tntloader']['scanned'][$path])) {
                TNTLoader::_ScanDir($path);
                continue;
            }

        }
        closedir($dh);
    }

    /**
     * Scan the installation filesystem for PHP declarations.
     *
     * The method scans all source files for function and class definitions.
     *
     * If a class or function definition is found in more than one file then
     * the file with the newer modification timestamp is used.
     *
     * This allows classes and functions to be included using the method
     * {@link RequireClass()} and {@link RequireFunction()} without having to
     * know where the class/function is defined.
     *
     * Warning: To be detected by the scanner a class or function declaration
     * must have the 'class' or 'function' keyword as at the beginning of the
     * line. This allows 'function' keywords that declare mark class methods
     * to be skipped over as they are normally indented.
     *
     * This is a static method.
     *
     * @access  public
     * @param   boolean     $clean      perform a clean scan ignoring cache
     * @return  void
     */
    /*static*/ function Scan($clean = false) {
        global $tnt;

        // Clear list of directories already scanned and declarations found
        $tnt['tntloader']['scanned'] = array();
        $tnt['tntloader']['declare'] = array();

        // Attempt to read a previous scan from the cache
        $path = TNT_CACHE_DIR.'/tntloader';

        // Try to read in our cached copy
        if (!$clean && file_exists($path) &&
            ($temp = unserialize(file_get_contents($path))) &&
            is_array($temp) && count($temp)) {

            // Cached copy loaded
            $tnt['tntloader']['declare'] = $temp;
        } else {

            // Scan everything
            TNTLoader::_ScanDir();

            // Finding nothing is a fatal error
            if (empty($tnt['tntloader']['declare'])) { 
                TNT::Abort('TNTLoader::Scan() - the scanner could not find any class or function definitions');
            }

            // Cache the results
            $temp = serialize($tnt['tntloader']['declare']);
            if (!($fp = fopen($path, 'w')) || !fwrite($fp, $temp)) {
                TNT::Abort('TNTLoader::Scan() - can\'t write to file: '.$path);
            }
            fclose($fp);
        }
    }

    /**
     * Request that the given class be declared.
     *
     * This will include the file that declares the given class and add it
     * to the list of included class files.
     *
     * Normally the correct file location should have been detected by the
     * loader. If no declaration for this class as been detected then the
     * method will abort.
     *
     * If the class definition is already available in the current PHP scope
     * then the method does nothing.
     *
     * If '$fatal' is true then the method will call {@link TNT::Abort()} if it
     * can not include a declaration for the specified function.
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $name       name of class to include
     * @param   boolean     $fatal      failure to get declaration is fatal
     * @return  boolean                 true if declaration is included
     */
    /*static*/ function RequireClass($name, $fatal = true) {
        // A little optimisation
        if ($name && class_exists($name)) {
            return true;
        }

        global $tnt;

        $ret = false;

        // Start Error Section
        do {
            if (!$name) {
                if ($fatal)
                  TNT::Abort('TNTLoader::RequireClass() - no class name');
                break;
            }
    
            // Check to see if we know the source file for this class
            $path = TNTLoader::Source($name, 'class');

            if (!$path) {
                if ($fatal)
                    TNT::Abort('TNTLoader::RequireClass() - no source for class: '.$name, E_USER_ERROR);
                break;
            }

            if (!file_exists($path) || !is_readable($path)) {
                if ($fatal)
                    TNT::Abort('TNTLoader::RequireClass() - unreadable source file: '.$path);
                break;
            }

            if ($tnt['tntloader']['enable_profiling']) {
                $in = microtime();
                ++$tnt['tntloader']['depth']; 
                require_once($path);
                --$tnt['tntloader']['depth']; 
                $out = microtime();
                $tnt['tntloader']['profile'][$path] = array($in, $out, $tnt['tntloader']['depth']);
            } else {
                require_once($path);
            }

            if (!class_exists($name)) {
                if ($fatal)
                    TNT::Abort('TNTLoader::RequireClass() - no declaration for class: '.$name);
                break;
            }

            $tnt['tntloader']['include']['class'][$name] = $path;

            $ret = true;

        } while (false);
        /* End Error Section */

        return $ret;
    }

    /**
     * Request that the given function be declared.
     *
     * This will include the file that declares the given function and add it
     * to the list of included function files.
     *
     * Normally the correct file location should have been detected by the
     * loader. If no declaration for this function as been detected then the
     * method will abort.
     *
     * If the function definition is already available in the current PHP scope
     * then the method does nothing.
     *
     * If '$fatal' is true then the method will call {@link TNT::Abort()} if it
     * can not include a declaration for the specified function.
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $name       name of function to include
     * @param   boolean     $fatal      failure to get declaration is fatal
     * @return  boolean                 true if declaration is included
     */
    /*static*/ function RequireFunction($name, $fatal = true) {

        // A little optimisation
        if ($name && function_exists($name)) {
            return true;
        }

        global $tnt;

        $ret = false;

        // Start Error Section
        do {

            if (!$name) {
                if ($fatal)
                    TNT::Abort('TNTLoader::RequireFunction() - no function name');
                break;
            }
    
            // Check to see if we know the source file for this function
            $path = TNTLoader::Source($name, 'function');
    
            if (!$path) {
                if ($fatal)
                    TNT::Abort('TNTLoader::RequireFunction() - no source for function: '.$name);
                break;
            }
            if (!file_exists($path) || !is_readable($path)) {
                if ($fatal)
                    TNT::Abort('TNTLoader::RequireFunction() - unreadable source file: '.$path);
                break;
            }

            if ($tnt['tntloader']['enable_profiling']) {
                $in = microtime();
                ++$tnt['tntloader']['depth']; 
                require_once($path);
                --$tnt['tntloader']['depth']; 
                $out = microtime();
                $tnt['tntloader']['profile'][$path] = array($in, $out, $tnt['tntloader']['depth']);
            } else {
                require_once($path);
            }

            if (!class_exists($name)) {
                if ($fatal)
                    TNT::Abort('TNTLoader::RequireFunction() - no declaration for function: '.$name);
                break;
            }

            $tnt['tntloader']['include']['function'][$name] = $path;

            $ret = true;

        } while (false);
        /* End Error Section */

        return $ret;
    }

    /**
     * Request that the given file be included.
     *
     * This will include the file if it has not already been included and add it
     * to the list of included files.
     *
     * If the file has been previously included by us or by a PHP function
     * then the method does nothing.
     *
     * Any file name should include any path required to find the file, any
     * relative paths are assumed to be relative to the installation root as
     * given by the constant TNT_ROOT_DIR.
     *
     * If '$fatal' is true then the method will call {@link TNT::Abort()} if it
     * can not include the specified file.
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $name       name of file to include (including path)
     * @param   boolean     $fatal      failure to get declaration is fatal
     * @return  boolean                 true if file is included
     */
    /*static*/ function RequireFile($name, $fatal = true) {
        global $tnt;

        $ret = false;

        // Start Error Section
        do {

            if (!$name) {
                if ($fatal) {
                    TNT::Abort('TNTLoader::RequireFile() - no file name');
                }
                break;
            }

            if (!is_abspath($name)) {
                $name = TNT_ROOT_DIR.'/'.$name;
            }

            // normalize any slashes
            $path = str_replace('\\', '/', $name);

            if (!in_array(get_included_files(), $path)) {

                if (!file_exists($path) || !is_readable($path)) {
                    if ($fatal)
                        TNT::Abort('TNTLoader::RequireFunction() - unreadable source file: '.$path);
                    break;
                }

                if ($tnt['tntloader']['enable_profiling']) {
                    $in = microtime();
                    ++$tnt['tntloader']['depth']; 
                    require_once($path);
                    --$tnt['tntloader']['depth']; 
                    $out = microtime();
                    $tnt['tntloader']['profile'][$path] = array($in, $out, $tnt['tntloader']['depth']);
                } else {
                    require_once($path);
                }

                $tnt['tntloader']['include']['file'][$name] = $path;
            }

            $ret = true;

        } while (false);
        /* End Error Section */


        return $ret;
    }

    /**
     * Return the path to the file in which the given class/function is
     * declared.
     *
     * The Loader scans the file system on start up for all class and function
     * declarations and then uses this knowledge to return the path to the file
     * where a given declaration was found.
     *
     * The '$used' parameter can be given to indicate that only files that
     * have * actually be checked and included via the {@link RequireClass()}
     * and {@link RequireFunction()} should be returned.
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $name       name of class/function
     * @param   string      $what       what is it 'class' or 'function'
     * @param   boolean     $used       only return if path has been included
     * @return  string                  path to source file (or null)
     */
    /*static*/ function Source($name, $what = 'class', $used = false) {
        global $tnt;

        if (is_null($tnt['tntloader']['declare'])) {
            // Populate the source list
            TNTLoader::Scan();
        }

        $ret = null;

        $name = ($what != 'file' ? strtolower($name) : $name);

        if ($used) {
            if (isset($tnt['tntloader']['include'][$what][$name])) {
                $ret = $tnt['tntloader']['include'][$what][$name];
            }
        } else {
            if (isset($tnt['tntloader']['declare'][$what][$name])) {
                $ret = $tnt['tntloader']['declare'][$what][$name];
            }
            if (!$ret || !file_exists($ret)) {
                // ReScan file system for missing file (this is done once)
                if (!$tnt['tntloader']['rescanned']) {
                    TNTLoader::Scan(true);
                    $tnt['tntloader']['rescanned'] = true;
                }

                // check again
                if (isset($tnt['tntloader']['declare'][$what][$name])) {
                    $ret = $tnt['tntloader']['declare'][$what][$name];
                }
            }
        }


        return $ret;
    }

    /**
     * Return an array of all files included.
     *
     * The '$what' parameter can be given to reduce the returned list as
     * follows;
     *   + null - all files included.
     *   + 'class' - all files included by {@link RequireClass()}
     *   + 'function' - all files included by {@link RequireFunction()}
     *   + 'file' - all files included by {@link RequireFile()}
     *
     * The returned array is indexed by the 'name' used to include it,
     * therefore it is assumed that 'class', 'function' and 'file' names can
     * not collide (if they do then 'class' takes precedence over 'function'
     * which takes precedence over 'file').
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $what       what is it 'class' or 'function'
     * @return  array                   associated array of paths
     */
    /*static*/ function Includes($what = null) {
        global $tnt;

        $ret = null;

        if ($what) {
            if (isset($tnt['tntloader']['include'][$what])) {
                $ret = $tnt['tntloader']['include'][$what];
            }
        } else {
            $ret = array();
            if (isset($tnt['tntloader']['include']['class'])) {
                $ret += $tnt['tntloader']['include']['class'];
            }
            if (isset($tnt['tntloader']['include']['class'])) {
                $ret += $tnt['tntloader']['include']['class'];
            }
            if (isset($tnt['tntloader']['include']['file'])) {
                $ret += $tnt['tntloader']['include']['file'];
            }
        }
        
        if (empty($ret)) {
            $ret = null;
        }

        return $ret;
    }

    /**
     * Dump the stats in text format.
     *
     * The loader keeps its own record of how many files were loaded and how
     * long each file took to load. You can dump these stats at the end of the
     * program when profiling by calling this method.
     *
     * This is a static method.
     *
     * @access  public
     * @param   boolean     $return     return string as opposed to echoing
     * @return  string                  optional profile message
     */
    /*static*/ function DumpProfile($return = false) {
        global $tnt;

        $num = 0;
        $total = 0;
        $max_time = 0;
        $max_path = '';
        $max_depth = 0;

        $ret = '';

        if (!$tnt['tntloader']['profile']) {
            $ret .= "TNTLoader: No files were loaded\n";
        } else {
            $ret .= "TNTLoader: Files profiled:\n";

            foreach ($tnt['tntloader']['profile'] as $path => $times) {
                // Convert checkpoint microtime string to float
                $in = explode(' ', $times[0]);
                $in_usecs = ((float)$in[0] + (float)$in[1]);
    
                $out = explode(' ', $times[1]);
                $out_usecs = ((float)$out[0] + (float)$out[1]);
    
                $diff = $out_usecs - $in_usecs;
    
                $ret .= sprintf("%02.04f %s> '%s'\n", $diff, str_repeat('-', $times[2]), $path);
    
                if ($times[2] > $max_depth) {
                    $max_depth = $times[2];
                }
    
                if ($diff > $max_time) {
                    $max_time = $diff;
                    $max_path = $path;
                }
    
                $total += $diff;
    
                ++$num;
            }
    
            $ret .= sprintf("Total time spent loading %d files was %02.04f\nLongest was %02.04f - '%s'\n", $num, $total, $max_time, $max_path);
            $ret .= "Maximum inclusion depth was $max_depth\n";
        }

        if (!$return) {
            echo $ret;
            $ret = null;
        }

        return $ret;
    }
}

function unserialize_callback_func($class) {
    TNTLoader::RequireClass($class);
}

ini_set('unserialize_callback_func', 'unserialize_callback_func');

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
