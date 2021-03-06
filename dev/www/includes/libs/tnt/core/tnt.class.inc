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
 * Static Class 
 * ~~~~~~~~~~~~
 *
 * This is a small static class containing methods used to configure and
 * control the TNT framework.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * This is a small static class containing methods used to configure and
 * control the TNT framework.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNT {

    /**
     * Check for a site match and if so load its data.
     *
     * @access  public
     * @access  mixed       $file       name of, array of or list of files
     * @return  void
     * @see     TNT::Ini()
     */
    /*static*/ function _SiteInit() {

        $registry =& TNT::Registry();

        $sites = $registry->GetVar('site');

        $url = $registry->GetVar('current.request.href', TNT_REQUEST);

        foreach ($sites as $name => $site) {

            if (empty($site['request'])) {
                trigger_error("TNT::_SiteInit() - site section 'Site.$name' has no 'request' entry");
                continue;
            }

            $ini = null;
            if (!empty($site['siteini'])) {
                $ini = TNTFileSystem::ExpandPath($site['siteini'], TNT_ROOT_DIR);
                if (!file_exists($ini)) {
                    TNT::Abort("TNT::_SiteInit() - site section 'Site.$name' speifies a non existant 'siteini': $ini");
                }
            }

            $dir = null;
            if (!empty($site['sitedir'])) {
                $dir = TNTFileSystem::ExpandPath($site['sitedir'], TNT_ROOT_DIR);
                if (!is_dir($dir)) {
                    TNT::Abort("TNT::_SiteInit() - site section 'Site.$name' speifies a non existant 'sitedir': $dir");
                }
            }

            $pat = $site['request'];

            TNT::Timestamp("TNT::_SiteInit() - checking pattern '$pat' against request '$url', for site '$name'");

            if (($l = strlen($pat)) > 1 && $pat{0} == '?') {
                if (!preg_match($pat, $url) == 1) {
                    continue;
                }
            } else {
                if (strncasecmp($pat, $url, $l) != 0) {
                    continue;
                }
            }

            // If here then a match was found
            $registry->SetVar('current.site', $site);
            $registry->SetVar('current.site.name', $name);

            if (empty($site['sitedir'])) {
                trigger_error("TNT::_SiteInit() - site section 'Site.$name' has no 'request' entry");
                continue;
            }

            // Merge any keys into the top
            unset($site['request']);
            unset($site['inifile']);
            unset($site['sitedir']);
            unset($site['name']);
            $registry->SetVar(null, $site, true);

            $files = array();
            if (!empty($ini)) {
               $files[] = $ini;
            }

            if (!empty($dir)) {
                $files[] = $dir.'/tnt.ini';
                $files[] = $dir.'/'.$tnt['name'].'.ini';
                $files[] = $dir.'/'.TNT_SCRIPT_PATH.'/'.TNT_SCRIPT_NAME.'.ini';
            }

            if ($files) {
                TNT::RegLoad($files);
            }

            break;
        }
    }

    /**
     * Initialise the framework
     *
     * Starts any profiling required, and sets up any cookies to maintain the
     * profiling across pages.
     *
     * Make sure you call this method before any output is sent to the browser.
     *
     * Always call {@link TNT::Shutdown()} or {@link TNT::Abort()} to exit the
     * script. This will ensure the framework is terminated and any exit
     * hooks are correctly processed.
     *
     * WARNING: Never use the PHP function {@link exit()} or {@link die()}
     * to exit the script. If you do then the the shutdown method will get
     * run after the client connection has closed and not be able to produce
     * output.
     *
     * @access  public
     * @param   string   $name          site name
     * @param   string   $profile       override profile setting
     * @return  void
     * @see     TNT::Shutdown(), TNT::Abort(), TNT::OnShutdown()
     */
    /*static*/ function Initialise($name = TNT_SITE_NAME, $profile = false) {
        global $tnt;

        TNT::Timestamp('TNT::Initialise: '.$name);

        if ($profile) {
            $tnt['_profile'] = 1;
        }

        /* Support xdebug profiling */
        if ($tnt['_profile'] > 1 && function_exists('xdebug_start_profiling')) {
            @xdebug_start_profiling();
        }

        $tnt['name'] = preg_replace('/[^\w]/', '_', $name);

        if (!file_exists(TNT_TEMP_DIR)) {
            @mkdir(TNT_TEMP_DIR);
        }

        if (!file_exists(TNT_CACHE_DIR)) {
            @mkdir(TNT_CACHE_DIR);
        }

        /* Maintain profiling state for this session */
        @setcookie('_tnt_profile', $tnt['_profile'], 0, '/');

        // Make sure Shutdown() is called even if its after the browser
        // connection has closed if not called before.
        register_shutdown_function(array('TNT', 'Shutdown'));
    }

    /** 
     * Shutdown the script.
     *
     * Call this method at the end of the script, or at any point in the script
     * where you want the script to exit. Do not just let the script exit or
     * return without calling this method or the framework will not be able to
     * output any terminating data.
     *
     * Always call {@link TNT::Initialise()} on entering the script to ensure
     * the framework is correctly initialised.
     *
     * For abnormal shutdown call the {@link TNT::Abort()} method.
     *
     * The normal shutdown sequence is as follows;
     *   + All callback functions registered with {@link TNT::OnShutdown()} are
     *     called. Any output from them will be sent to the browser as normal.
     *
     *   + All TNTObject derived objects are destroyed is the reverse order to
     *     which they were created.
     *
     *   + If profiling is enabled then all profiling timestamps are output,
     *     along with the TNTLoader profiling and a dump of the superglobals.
     *     If XDebug profiling supported then it is asked to dump its profile.
     *
     * WARNING: Never use the PHP function {@link exit()} or {@link die()}
     * to exit the script. If you do then the this method will get run after
     * the client connection has closed and not be able to produce output.
     *
     * @access  public
     * @param   mixed    $status        status code to return to caller
     * @return  void
     * @see     TNT::Initialise(), TNT::Abort(), TNT::OnShutdown()
     */
    /*static*/ function Shutdown($status = 0) {
        global $tnt;

        if (!empty($tnt['_shutdown'])) {
            return;
        }

        $tnt['_shutdown'] = true;
        
        TNT::Timestamp("TNT::Shutdown($status)");
    
        if (!empty($tnt['_onshutdown'])) {
            foreach ($tnt['_onshutdown'] as $callback) {
                call_user_func($callback);
            }
        }

        TNT::Timestamp('destroying objects');
    
        _TNTObject_CallDestructors();
    
        if ($tnt['_profile']) {
            TNT::Timestamp('profiling');

            echo '<hr />';
            echo '<pre>'.TNT::Timestamp(null).'</pre>';
            echo '<pre>'.TNTLoader::DumpProfile(true).'</pre>';
            echo '<pre>$_GET: '.var_info($_GET).'</pre>';
            echo '<pre>$_POST: '.var_info($_POST).'</pre>';
            echo '<pre>$_FILES: '.var_info($_FILES).'</pre>';
            echo '<pre>$_COOKIE: '.var_info($_COOKIE).'</pre>';
            echo '<pre>$_REQUEST: '.var_info($_REQUEST).'</pre>';
            echo '<pre>$_SERVER: '.var_info($_SERVER).'</pre>';
    
            if ($tnt['_profile'] > 1 && function_exists('xdebug_dump_function_profile')) {
                xdebug_dump_function_profile($tnt['_profile'] - 1);
            } 
        }
    
        exit($status);
    }
    
    /** 
     * Abort the script.
     *
     * The script is aborted immediately. No {@link TNT::OnShutdown()} callbacks
     * are called and no profiling dumped.
     *
     * @access  public
     * @param   string   $msh           error message to display
     * @return  void
     * @see     TNT::Initialise(), TNT::Shutdown(), TNT::OnShutdown()
     */
    /*static*/ function Abort($msg, $status = 255) {
        require_once(dirname(__FILE__).'/tntdebug.class.inc');
        echo TNTDebug::Backtrace();
        trigger_error($msg, E_USER_ERROR);
    }

    /** 
     * Register a method to be called on a clean exit of the script.
     *
     * Unlike the PHP {@link register_shutdown_function()} function these
     * callback functions are run before the client connection is closed
     * and therefore can produce output.
     *
     * WARNING: The {@link TNT::Shutdown()} method must be used to exit the
     * script and not the PHP {@link exit()} function. The shutdown method
     * must even be called at natural end of the script before the client
     * connection is closed.
     *
     * If a script terminates without the {@link TNT::Shutdown()} function
     * being called then it will be automatically called but in this case
     * the client connection will be closed and no output will be seen.
     *
     * @access  public
     * @return  void
     * @see     TNT::Initialise(), TNT::Shutdown(). TNT::Abort()
     */
    /*static*/ function OnShutdown($callback) {
        global $tnt;
        if (!is_callable($callback)) {
            return false;
        }
        $tnt['_onshutdown'][] = $callback;
        return true;
    }

    /**
     * Translate some text.
     *
     * Short cut to the translator - all static strings should be passed through
     * this function. If the translator is known and loaded then it will attempt
     * to translate them, otherwise they will be return unmodified.
     *
     * @access  public
     * @param   string  $text       text to translate
     * @return  string              the translated text
     */
    /*static*/ function Translate($text) {
        global $tnt;
        if (class_exists('tnttranslator') &&
            is_a($tnt['_tnttranslator'], 'tnttranslator')) {
            return $tnt['_tnttranslator']->Translate($text);
        }
        return $text;
    }

    /**
     * Return reference to the global registry.
     *
     * Use this reference to load registry files or bulk set variables etc.
     *
     * The convenience methods {@link RegHasVar()}, {@link RegSetVar()}
     * and {@link RegGetVar()} are provided to prevent having to keep
     * calling this method as PHP 4 does not understand the usefull syntax
     * 'TNT::Registry()->GetVar()' etc.
     *
     * The global registry is not created until it is first required by calling
     * one of these registry methods. This just-in-time creation saves time
     * when the registry variables are not required by a script.
     *
     * When created the follow files will automatically be read;
     *   + <TNT_ROOT_DIR>/tnt.ini
     *   + <TNT_ROOT_DIR>/<name>.ini
     *   + <TNT_ROOT_DIR>/<script>.ini
     *
     * where <name> is the name passed to the {@link TNT::Initialise()} method,
     * and <script> is the relative path to the current script excluding the
     * PHP_EXT and relative to the installation root TNT_ROOT_DIR.
     *
     * If a 'Site' section is found in the loaded initialisation data then the
     * multi-site mode is invoked and the following files will be read;
     *   + <TNT_ROOT_DIR>/<TNT_SITE_PATH>/tnt.ini
     *   + <TNT_ROOT_DIR>/<TNT_SITE_PATH>/<name>.ini
     *   + <TNT_ROOT_DIR>/<TNT_SITE_PATH>/<script>.ini
     *
     * where TNT_SITE_PATH is the relative path to the directory holding the
     * site data (if set in the sites registry).
     *
     * @access  public
     * @return  TNTRegistry&            reference to initialisation store
     * @see     TNT::RegHasVar(), TNT::RegSetVar(), TNT::RegGetVar()
     */
    /*static*/ function &Registry() {
        global $tnt;

        if (empty($tnt['_registry']) || !is_a($tnt['_registry'], 'tntregistry')) {
            TNTLoader::RequireClass('TNTRegistry');

            $tnt['_registry'] =& new TNTRegistry();

            $files = array(
                TNT_ROOT_DIR.'/tnt.ini',
                TNT_ROOT_DIR.'/'.$tnt['name'].'.ini',
                TNT_ROOT_DIR.'/'.TNT_SCRIPT_PATH.'/'.TNT_SCRIPT_NAME.'.ini');

            TNT::RegLoad($files);

            TNT::Timestamp('site ? = '.($tnt['_registry']->HasVar('site') ? 'true' : 'false'));

            if ($tnt['_registry']->HasVar('site')) {
                // Multi-site support - need to load correct site for request
                TNT::_SiteInit();
            }

        }

        return $tnt['_registry'];
    }

    /**
     * Load a file, array of files or list of files into the global
     * initialisation store.
     *
     * The global ini variable store is not created until it is first required
     * by calling one of these Ini methods. This just-in-time creation saves
     * time when the variables are not required by a script.
     *
     * Ini files are optional, any file listed that does't exist is silently
     * skipped. Failure to load an INI file will result in the script aborting.
     *
     * The $file  argument can be one of the following;
     *  + a string containing a single file name
     *  + a string containing multiple file names separated by PATH_SEPARATOR
     *  + an array of strings containing a single name
     *
     *
     * @access  public
     * @access  mixed       $file       name of, array of or list of files
     * @return  void
     * @see     TNT::Registry()
     */
    /*static*/ function RegLoad($file) {

        if (is_string($file)) {
            $files = explode(PATH_SEPARATOR, $file);
        } else if (is_array($file)) {
            $files = $file;
        } else {
            return;
        }

        $registry =& TNT::Registry();

        foreach (array_unique($files) as $file) {
            TNT::TimeStamp('TNT::RegLoad() - checking file: '.$file);
            if (file_exists($file) && !$registry->LoadFile($file, 'ini')) {
                TNT::Abort("TNT::RegLoad() - failed to load ini file '{$file}': ".$registry->Error());
            }
        }
    }

    /**
     * Does a global initialisation variable exist.
     *
     * Equivalent to the syntax 'TNT::Registry()->HasVar()'.
     *
     * @access  public
     * @param   string      $key        key of registry value to check
     * @param   string      $name       name of ini variable
     * @return  bool                    true if option is stored
     * @see     Ini(), RegSetVar(), RegGetVar(), TNTRegister::HasVar()
     */
    /*static*/ function RegHasVar($key) {
        $registry =& TNT::Registry();
        return $registry->HasVar($key);
     }
        
    /**
     * Set the value of a global initialisation variable
     *
     * Equivalent to the syntax 'TNT::Registry()->SetVar()'.
     *
     * @access  public
     * @param   string      $key        key of registry value to set
     * @param   mixed       $val        value to store at the given key
     * @return  mixed                   original value stored at the given key
     * @see     RegHasVar(), RegSetVar(), TNTRegistry::SetVar()
     */
    /*static*/ function RegSetVar($key, $val) {
        $registry =& TNT::Registry();
        return $registry->SetVar($key, $val);
    }
    
    /**
     * Get the value of a global initialisation variable
     *
     * Equivalent to the syntax 'TNT::Registry()->GetVar()'.
     *
     * @access  public
     * @param   string      $key        key of registry value to get
     * @param   mixed       $def        default to return if not set
     * @return  mixed                   value stored at the given key
     * @see     RegHasVar(), RegSetVar(), TNTRegistry::GetVar()
     */
    /*static*/ function RegGetVar($key, $def = null) {
        $registry =& TNT::Registry();
        return $registry->GetVar($key, $def);
    }

    /*
     * Record/output timestamps for simple profiling
     *
     * Call timestamp('name') - to record a timestamp with the given name,
     * and then call timestamp(null) to return a log of all timestamps records
     * plus load stats..
     *
     */
    /*static*/ function Timestamp($text) {
        global $tnt;

        if (empty($tnt['_profile'])) {
            return;
        }
    
        if (!is_null($text)) {
            if (!isset($tnt['_timestamps'])) {
                $tnt['_timestamps'] = array();
            }
            if (!is_string($text)) {
                $text = var_info($text);
            }
            $tnt['_timestamps'][] = array(microtime(), $text);
            return;
        }

        if (empty($tnt['_timestamps'])) {
            return '';
        }
    
        // Return log of timestamps gathered so far
        $ret = '';
    
        // Convert start microtime string to float
        $m = explode(' ', $tnt['starttime']);
        $startusecs = ((float)$m[0] + (float)$m[1]);
    
        $prev = 0;
        $max_i = count($tnt['_timestamps']);
        for ($i = 0; $i < $max_i; ++$i) {
            // Convert checkpoint microtime string to float
            $m = explode(' ', $tnt['_timestamps'][$i][0]);
            $usecs = (((float)$m[0] + (float)$m[1]) - $startusecs);
            if (!isset($m[1])) {
                $ret .= "<pre>Corrupt timestamp[$i]=".var_export($timestamps[$i], true).'</pre>';
            }
    
            // Printf is slow but faster than number_format()
            tnt_dump('dump=', $tnt['_timestamps'][$i][1]);
            $ret .= sprintf("%02.04f (+%02.04f) - %s\n",
                $usecs, $usecs - $prev, var_info($tnt['_timestamps'][$i][1]));
            $prev = $usecs;
        }
        
        return $ret;
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
