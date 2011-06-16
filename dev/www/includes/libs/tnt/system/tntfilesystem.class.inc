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
 * File System Class
 * ~~~~~~~~~~~~~~~~~
 *
 * This class allows you to manipulate the file system and and its paths.
 *
 * @package TNT.System
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTClass');

if (isset($tnt['tntfilesystem'])) {
    die('TNT File System already included');
}

/*
 * Globals
 */
global $tnt;
$tnt['tntfilesystem']['vfs'] = array('cache' => array(), 'dirs' => array());
$tnt['tntfilesystem']['findcount'] = 0;

global $usecs;
$usecs = 0;

/*
 * Defines
 */
define('TNT_FILESYSTEM_SYSTEMTOSERVER',   1);   // system to server paths
define('TNT_FILESYSTEM_SYSTEMTOVIRTUAL',  2);   // system to virtual paths
define('TNT_FILESYSTEM_SYSTEMTORELATIVE', 3);   // system to relative paths

define('TNT_FILESYSTEM_SERVERTOSYSTEM',   4);   // server to system paths
define('TNT_FILESYSTEM_SERVERTOVIRTUAL',  5);   // server to virtual paths
define('TNT_FILESYSTEM_SERVERTORELATIVE', 6);   // server to relative paths

define('TNT_FILESYSTEM_VIRTUALTOSYSTEM',  7);   // virtual to system paths
define('TNT_FILESYSTEM_VIRTUALTOSERVER',  8);   // virtual to server paths
define('TNT_FILESYSTEM_VIRTUALTORELATIVE',9);   // virtual to relative paths

define('TNT_FILESYSTEM_IO_READ',    0x0001);
define('TNT_FILESYSTEM_IO_WRITE',   0x0002);
define('TNT_FILESYSTEM_IO_CREATE',  0x0010);
define('TNT_FILESYSTEM_IO_TRUNCATE',0x0020);
define('TNT_FILESYSTEM_IO_APPEND',  0x0040);
define('TNT_FILESYSTEM_IO_BINARY',  0x0100);
define('TNT_FILESYSTEM_IO_LOCK',    0x0200);
define('TNT_FILESYSTEM_IO_MKDIR',   0x0400);

/**
 * Static class to manipluate the file system.
 *
 * Path names are treated as case insensitive but any case is preserved.
 * Therefore the paths '/home/TNT' and '/home/tnt' are regarded as the same.
 *
 * Methods accept either the directory separator '/' or that defined by the
 * DIRECTORY_SEPARATOR definition. All paths are returned using the '/'
 * separator only for consistancy.
 *
 * @package TNT.System
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @static
 */
class TNTFileSystem extends TNTClass {

    /**
     * Expand a possible virtual path to a real path
     *
     * We understand a virtual file system, these virtual paths are mapped to
     * real paths at runtime by supporting functions. This allows us to move
     * and rename directories without actually having to know the new names in
     * the code and templates.
     *
     * When comparing path names case is ignored but preserved. Therefore the
     * paths '/home/TNT' and '/home/tnt' are regarded as the same.
     *
     * The TNT Framework supports these virtual paths.
     *
     * The virtual filesystem is specified under the 'vfs' key in the global
     * registry as follows;
     *  + 'tnt.vfs.dirs' - an array of virtual to real path mappings. Each sub key
     *    is assumed to map a virtual root to its real path. The value can one
     *    of the following;
     *      - str:<string>
     *      - php:<php_code>|<function_name>|<class_name>::<method_name>
     *  
     *    If no 'str:' or 'php:' prefix is given then 'str:' is assumed. For the
     *    'php:' prefix any php code should modify the given $path variable. Any
     *    function or static class method will be passed a reference to $path to
     *    modify.
     *
     *    If no mapping for a virtual root is found then the root is assumed to
     *    be relative. (ie. '~unknown/fred.txt' => './unknown/fred.txt')
     *  
     *    A virtual path is assumed to be an absolute path but is expected to
     *    map to a path relative to the installation root. It can however be
     *    mapped directly to a another virtual path or to an absolute path.
     *    When one virtual path map to another the replacement recurses until
     *    no further matches can be made.
     *  
     *  + 'tnt.vfs.vars' - an array of replacement variables to search for and
     *    replace in any virtual path. Each subkey is a variable to replace and
     *    its value is the what to replace the variable with.
     *    As in 'tnt.vfs.dirs' the value can be either a plain string or php code
     *    where the code is passed the variable and is expected to return the
     *    replacement string value.
     *
     * Mappings are cached for efficiency, to ensure a mapping is not taken
     * from the cache then an optional '$refresh' parameter can be specified
     * with a true value.
     *
     * To clear the whole cache set the global registry key 'tnt.vfs.refresh' to
     * true, the first call to this method that sees this registry value will
     * delete the cache and delete the registry value.
     *
     * @access  public
     * @param   string      $virtual    virtual path
     * @param   string      $root       return absolute to this root path
     * @param   boolean     $refresh    refresh any cached value
     * @return  string                  real path (relative if no root given)
     * @static
     */
    /*static*/ function ExpandPath($virtual, $root = TNT_ROOT_DIR, $refresh = false) {
        global $tnt;

        $ret = null;

        // Start error section
        do {
            // We only translate if its a virtual path
            if ($virtual{0} != '~') {
                // Return a none virtual path 
                $ret = TNTFileSystem::MakePath($virtual, $root);
                TNT::Timestamp("TNTFileSystem::ExpandPath('$virtual') - return none virtual path: '$ret'");
                break;
            }

            if ($virtual == '~' || $virtual == '~/') {
                // Return a virtual root path
                $ret = preg_replace('/\/+/', '/', $root.'/'.substr($virtual, 1));
                TNT::Timestamp("TNTFileSystem::ExpandPath('$virtual') - return virtual root path: '$ret'");
                break;
            }

            if (TNT::RegGetVar('tnt.vfs.refresh')) {
                $tnt['tntfilesystem']['vfs']['cache'] = array();
                $refresh = true;
                TNT::RegSetVar('tnt.vfs.refresh', null);
            }

            $cache_key = strtolower($virtual);

            if (!$refresh && isset($tnt['tntfilesystem']['vfs']['cache'][$cache_key])) {
                $p = $tnt['tntfilesystem']['vfs']['cache'][$cache_key][1];
                $ret = TNTFileSystem::MakePath($p, $root);
                TNT::Timestamp("TNTFileSystem::ExpandPath('$virtual') - return cached path: '$ret'");
                break;
            }

            // Replace all DIRECTORY_SEPARATOR characters with '/'
            // and condense any multiple separators
            $p = preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $virtual);

            $dirs = TNT::RegGetVar('tnt.vfs.dirs', null);

            if ($dirs) {
                $c = 0;
                while ($p{0} == '~' && ($c++ < 99)) {
    
                    if ($p == '~' || strncmp($p, '~/', 2) == 0) {
                        break;
                    }
    
                    if (($n = strpos($p, '/')) === false) {
                        $n = strlen($p);
                    }
    
                    $s1 = substr($p, 1, $n - 1);
                    $s2 = substr($p, $n + 1);

                    if (!isset($dirs[$s1])) {
                        break;
                    }

                    $r = $dirs[$s1];

                    if (isset($tnt['tntfilesystem']['vfs']['dirs'][$s1]) &&
                        $tnt['tntfilesystem']['vfs']['dirs'][$s1][0] == $r) {
                        // Use our decoded and value
                        $cb = $tnt['tntfilesystem']['vfs']['dirs'][$s1][1];
                    } else {
                        // Decode and cache the value
                        if (!is_string($r)) {
                            trigger_error('TNTFileSystem::ExpandVirtual() - invalid string in VFS directory map');
                            break;
                        }
    
                        $a = explode(':', $r, 2);
                        if (count($a) == 1) {
                            $t = 'str';
                            $v = $r;
                        } else {
                            list($t, $v) = $a;
                        }
                        switch ($t) {
                            case 'str':
                                $cb = create_function('&$path, $pfx', '$path = \''.stresc($v).'\'.substr($path, '.(strlen($s1) + 1).');');
                                break;

                            case 'php':
                                if (preg_match('/^([\w_]+)(::([\w_]+))?$/', $v, $m)) {
                                    if (count($m == 4)) {
                                        $cb = array($m[1], $m[3]);
                                    } else {
                                        $cb = $m[1];
                                    }
                                } else {
                                    $cb = create_function('&$path, $pfx', $v);
                                }
                                break;

                            default:
                                $cb = create_function('&$path, $pfx', '$path = \''.stresc($r).'\'.substr($path, '.(strlen($s1) + 1).');');
                                break;

                        }
    
                        $tnt['tntfilesystem']['vfs']['dirs'][$s1] = array($r, $cb);
                    }
    
                    call_user_func_array($cb, array(&$p, $s1));
                }
            }

            // if path still virtual then strip the virtual prefix and make
            // a relative path
            $p = ltrim($p, '~/');

            TNT::Timestamp("caching [$cache_key]=".htmlentities(var_export(array($dirs, $virtual, $p), true)));
            $tnt['tntfilesystem']['vfs']['cache'][$cache_key] = array($virtual, $p);

            $ret = TNTFileSystem::MakePath($p, $root);

            TNT::Timestamp("TNTFileSystem::ExpandPath('$virtual') - return virtual path: '$ret'");

        } while (false);
        // End error section

        return $ret;
    }
    
    /**
     * Contract a possible real path to a virtual path.
     *
     * See {@link TNTFileSystem::Expand()} for details on the virtual file
     * system.
     *
     * Expanding from real paths to virtual paths removes the root directory
     * and then attempts to reverse the expansion by looking in the mapping
     * cache. This means a path can only be converted back if it was expanded
     * since the last time the virtual mapping cache was cleared.
     *
     * @access  public
     * @static
     * @param   string      $path       absolute path (relative if no root)
     * @param   string      $root       path is absolute to this root path
     * @return  string                  virtual path
     */
    /*static*/ function ContractPath($path, $root = TNT_ROOT_DIR) {
        
        // Only contract it if its not a virtual path already
        if ($path{0} != '~') {
            // Replace all DIRECTORY_SEPARATOR characters with '/'
            // and condense any multiple separators and then strip the root
            $ret = preg_replace(array('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/^'.preg_quote($root, '/').'/'), array('/', ''), $path);
    
            global $_tnt;
            // Look up the path in the VFS cache
            if (isset($tnt['tntfilesystem']['vfs']['cache'][$key = strtolower($p)])) {
                $ret = $tnt['tntfilesystem']['vfs']['cache'][$key][0];
            } else {
                $ret = '~/'.$ret;
            }
        }

        return $ret;
    }

    /**
     * Recursion function called from {@link MakePath()}
     *
     * @access  private
     * @param   string&     $path       reference to path to build
     * @param   array       $dirs       array of paths to add
     * @return  void
     * @static
     */
    /*static*/ function _MakePath(&$path, $dirs) {

        foreach ($dirs as $d) {
           if (TNTFileSystem::IsAbsolutePath($path)) {
               break;
           }
           if (is_array($d)) {
               $path = TNTFileSystem::_MakePath($path, $d);
           } else {
               $path = $d.'/'.$path;
           }
        }

    }
    
    /**
     * Concatenates paths together to make a single path
     *
     * Recursively expands all arguments prefixing each one to the preceeding
     * one until an absolute path is made or all the paths are prepended.
     *
     * The returned path will be neatly formed with a single '/' between
     * each directory and no trailing '/'.
     *
     * @static
     * @access  public
     * @param   mixed       $dirs,...   string paths or array of paths
     * @return  string                  path made up of concatenated paths
     */
    /*static*/ function MakePath($dirs) {

        $argv = func_get_args();

        TNTFileSystem::_MakePath($path, $argv);

        // Replace all DIRECTORY_SEPARATOR characters with '/'
        // and condense any multiple separators
        // and strip any trailing separator
        $path = rtrim(preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $path), '/');

        return $path;
    }
    
    /**
     * Remove all multiple slashes and relative paths from a path.
     *
     * For files that exist this expands all symbolic links and resolves
     * references to '/./', '/../' and extra '/' characters in the path and
     * returns the canonicalized absolute pathname. The resulting path will
     * have no symbolic link, '/./' or '/../' components. The PHP function
     * {@link realpath()} is used to do this.
     *
     * For paths that do not exist the method will attempt to manually resolve
     * references to '/./', '/../' and extra '/' characters in the path. If
     * the result of a '/../' causes the file to exist then the PHP function
     * {@link realpath()} will be used to resolve any remaining references.
     * If the path can not be fully resolved as in the case of a relative path
     * with references to '/../' that take it to a undocumented parent then
     * these references will remain.
     *
     * Any trailing '/' will be removed.
     *
     * @access  public
     * @param   string      $path       path to normalise
     * @return  string                  normalised path
     * @static
     */
    /*static*/ function NormalisePath($path) {

        if (@file_exists($path)) {
            // Expand out the path
            $path = realpath($path);

            // Replace all DIRECTORY_SEPARATOR characters with '/'
            // and condense any multiple separators
            $path = rtrim(preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $path), '/');
        } else {
            // Replace all DIRECTORY_SEPARATOR characters with '/'
            // and condense any multiple separators
            // and remove any '/./' references
            $path = rtrim(preg_replace(array('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/\/\.\//'), array('/', ''), $path), '/');

            $sdirs = explode('/', $path);
            $ddirs = array();
            $m = count($sdirs);
            $d = 0;
            for ($s = 0; $s < $m; $s++) {
                if ($d && $sdirs[$s] == '.') {
                    // skip embeded '.'
                    continue;
                }
    
                if ($d && $sdirs[$s] == '..') {
                    // Back up over embeded '..' characters
                    $d--;
                    continue;
                }
    
                // Add this directory to the list
                $ddirs[$d] = $sdirs[$s];
                $d++;
            }
    
            // Rejoin all the directories
            $path = implode('/', $ddirs);
        }

        return $path;
    }

    /**
     * Finds file(s) on a given path (file is matched using an regex pattern)
     * If $depth then this indicates maximum tree depth to traverse (-1 is all)
     * If $multiple then all matching files found are returned in an array
     * If $hidden then hidden files are found - otherwise they are skipped.
     *
     * Accepts a path in the virtual file system. Relative paths are assumed to
     * be relative to the installation root.
     *
     * Pattern can either be specified as follows;
     *   '/regex/'  - a regex pattern {@see preg_match()}
     *   '~glob~'   - a glob pattern (@see fnmatch()}
     *   '+substr+' - a case insensitive sub string match (@see stristr()}
     *   'string'   - a case insensitive exact match
     *
     * If no pattern is given then all files are matched.
     *
     * If $hidden is true the hidden files (those starting with a period)
     * are returned as well
     *
     * If $detail controls the level of detail returned for each file as
     * follows;
     *
     * If 'detail=0' then each array item is a string containing the path name.
     *
     * If 'detail=0' and 'tree=1' then each item is an associative array
     * containing the following items;
     *   'path'     - relative path to file
     *   '_children' - list of child directories which match or have matches
     *   '_numchildren' - number of children of this item (count(_children))
     *   '_numdescendants' - number of descendants of this item
     *
     * If 'detail=1' then each array item is an associative array containing
     * the following items;
     *   'path'     - relative path to file
     *   'relpath'  - relative path to file (as '<path>')
     *   'abspath'  - absolute path to file
     *   'dir'      - directory part of relpath "<path>=<dir>/<file>"
     *   'reldir'   - directory part of relpath (as '<dir>')
     *   'absdir'   - directory part of abspath "<abspath>=<absdir>/<file>"
     *   'file'     - file part of path "<path>=<dir>/<file>"
     *   'base'     - base part of file "<file>=<base>.<ext>"
     *   'ext'      - extension part of file "<file>=<base>.<ext>"
     *   '_depth'   - depth in the hierachy
     *
     * If 'tree=1' then the following item is also added;
     *   '_children' - list of child directories which match or have matches
     *   '_numchildren' - number of children of this item (count(_children))
     *   '_numdescendants' - number of descendants of this item
     *
     * If 'detail=2' then each array item is an associative array containing
     * the following items;
     *   'path'     - relative path to file
     *   'relpath'  - relative path to file (as '<path>')
     *   'abspath'  - absolute path to file
     *   'dir'      - directory part of path "<path>=<dir>/<file>"
     *   'reldir'   - directory part of relpath (as '<dir>')
     *   'absdir'   - directory part of abspath "<abspath>=<absdir>/<file>"
     *   'file'     - file part of path "<path>=<dir>/<file>"
     *   'base'     - base part of file "<file>=<base>.<ext>"
     *   'ext'      - extension part of file "<file>=<base>.<ext>"
     *   'dev'      - device number 
     *   'ino'      - inode number 
     *   'mode'     - inode protection mode 
     *   'nlink'    - number of links 
     *   'uid'      - userid of owner 
     *   'gid'      - groupid of owner 
     *   'rdev'     - device type, if inode device * 
     *   'size'     - size in bytes 
     *   'atime'    - time of last access (unix timestamp) 
     *   'mtime'    - time of last modification (unix timestamp) 
     *   'ctime'    - time of last change (unix timestamp) 
     *   'blksize'  - blocksize of filesystem IO * 
     *   'blocks'   - number of blocks allocated 
     *
     * If 'tree=1' then the following item is also added;
     *   '_children' - list of child directories which match or have matches
     *   '_numchildren' - number of children of this item (count(_children))
     *   '_numdescendants' - number of descendants of this item
     *
     * File system perusal is cached to speed up file searches. Each time a
     * directory is searches it checked against any previous cache of that
     * directory search. The caches last 5 minutes or until {@link ClearCache()}
     * is called.
     *
     * Normally the array returned is a flat array of matching directory
     * entries. However if '$tree' is specified then the array is returned
     * matching the directory hierachy. Each directory with matching descendants
     * is added.
     *
     * @static
     * @access  public
     * @param   string      $relpath    path to search from 
     * @param   string      $pattern    regex pattern to select matching files
     * @param   integer     $maxdepth   depth of sub-directories to search
     * @param   boolean     $multiple   return all files or just first found
     * @param   array       $hidden     match hidden files (first char '.')
     * @param   boolean     $detail     level of detail to return
     * @param   boolean     $tree       return in hierachical format array
     * @param   boolean     $nocache    don't use caching
     * @param   integer     $depth      current depth (internal use)
     * @return  mixed                   matched path or array of matched paths
     */
    /*static*/ function Find($relpath, $pattern = null, $maxdepth = -1, $multiple = false, $hidden = false, $detail = 0, $tree = false, $nocache = false, $depth = 0) {

        global $tnt;
    
        $ret = null;

        if (!$nocache) {
            TNT::Timestamp("TNTFileSystem::Find($relpath)");

            $hash = md5(serialize(func_get_args()));

            $cachePath = TNTFileSystem::MakeAbsolutePath("tntfilesystem/find/$hash.dat", TNTFileSystem::CachePath());

            if (file_exists($cachePath) &&
                (filemtime($cachePath) > (time() - (5*60))) &&
                ($data = file_get_contents($cachePath)) &&
                ($ret = unserialize($data))) {
                {
                }
            }

            $tnt['tntfilesystem']['findcount'] = 0;
        }

        if (!$ret) {
            // Expand the path and make absolute to the installation path
            $abspath = TNTFileSystem::MakeAbsolutePath($relpath);

            // Then contract the path to a relative path
            $relpath = TNTFileSystem::MakeRelativePath($abspath);
    
            $found = array();
        
            if (!($handle = @opendir($abspath))) {
                $found = null;
            } else {
                $matchfn = null;
                if (is_callable($pattern)) {
                    $matchfn = $pattern;
                } else if ($pattern) {
                    $l = strlen($pattern);
                    if ($l && ($pattern{0} == $pattern{$l - 1})) {
                        $p = addcslashes(substr($pattern, 1, -1), '\0..\37\177..\377"\'');
                        switch ($pattern[0]) {
                            case '/': // regex
                                $matchfn = create_function('$s', "return preg_match(\"$p\", \$s);");
                                break;
                            case '~': // glob
                                $matchfn = create_function('$s', "return fnmatch(\"$p\", \$s);");
                                break;
                            case '+': // sub string
                                $matchfn = create_function('$s', "return stristr(\$s, \"$p\");");
                                break;
                            default:
                                $p = addcslashes($pattern, '\0..\37\177..\377"\'');
                                $matchfn = create_function('$s', "return (\$s == \"$p\");");
                                break;
                        }
                    } else {
                        $p = addcslashes($pattern, '\0..\37\177..\377"\'');
                        $matchfn = create_function('$s', "return (\$s == \"$p\");");
                    }
                }
            }
    
            while ($handle && $file = readdir($handle)) {
                // We allow hidden files to be indicated via the UNIX '.' notation &
                // also a '_' notation for WIN32 whos filemanager won't allow you to
                // rename a file to start with a '.' even though it can be done from
                // the command line.
                if ($file == '.' || $file == '..') {
                    continue; // Skip special entries
                }
                if (!$hidden && ($file[0] == '.' || $file[0] == '_')) {
                    continue; // Skip hidden files and directories
                }
                $relfile = ($relpath ? "$relpath/$file" : $file);
                $absfile = ($abspath ? "$abspath/$file" : $file);
                $lnkpath = realpath($absfile);
    
                $match = ($matchfn ? $matchfn($file) : true);

                // If its a directory then scan it as well if required
                $children = null; // null means we didn't recurse it (not that it has no children)
                $numchildren = 0;
                $numdescendants = 0;
                if ((!$match || $multiple) && ($maxdepth == -1 || $depth < $maxdepth) && is_dir($absfile)) {
                    // If the abspath is a parent of the current path then don't
                    // recurse into it - this occurs if the file was a symbolic link
                    // to a parent directory
                    if (is_link($absfile) && strncasecmp($lnkpath, $abspath, strlen($lnkpath)) == 0) {
                    } else if (($children = TNTFileSystem::Find($relfile, $matchfn, $maxdepth, $multiple, $hidden, $detail, $tree, true, $depth + 1))) {
                        if ($tree) {
                            $numchildren = count($children);
                            $numdescendants = $numchildren;
                            foreach ($children as $child) {
                                $numdescendants += $child['_numdescendants'];
                            }
                        }
                            
                        if (!$multiple) {
                            $found = $children;
                            break;
                        } else if ($tree) {
                            $match = true;
                        }
                    }
                }
    
                if ($match) {
                    if ($detail > 0) {
                        $details = array('path' => $relfile,
                                         'relpath' => $relfile,
                                         'abspath' => $absfile,
                                         'dir' => $relpath,
                                         'reldir' => $relpath,
                                         'absdir' => $abspath,
                                         'file' => $file);
    
                        $details += TNTFileSystem::SplitPath($relfile);
    
                        if ($detail > 1 && $stats = stat($absfile)) {
                            foreach ($stats as $i => $v) {
                                if (!is_int($i)) {
                                    $details[$i] = $v;
                                }
                            }
                        }

                        $details['_depth'] = $depth;
    
                        if ($tree) {
                            $details['_numchildren'] = $numchildren;
                            $details['_numdescendants'] = $numdescendants;
                            $details['_children'] = $children;
                        }
    
                        $found[] = $details;
                        ++$tnt['tntfilesystem']['findcount'];
                    } else if ($tree) {
                        $details['path'] = $relfile;
                        $details['_numchildren'] = $numchildren;
                        $details['_numdescendants'] = $numdescendants;
                        $details['_children'] = $children;
                        $found[] = $details;
                        ++$tnt['tntfilesystem']['findcount'];
                    } else {
                        $found[] = $relfile;
                        ++$tnt['tntfilesystem']['findcount'];
                    }
                    if (!$multiple) {
                        break;
                    }
                }
                if (!$tree && $children) {
                    if ($found) {
                        $found = array_merge($found, $children);
                    } else {
                        $found = $children;
                    }
                }
            }
        
            if ($handle) {
                closedir($handle);
            }
    
            if ($found && !$multiple) {
                $found = $found[0];
            }

            $ret = $found;

            if (!$nocache) {
                // Update the cache
                TNTFileSystem::Mkdir(dirname($cachePath));
                file_set_contents($cachePath, serialize($ret));
                TNT::Timestamp('TNTFileSystem::Find() - wrote cache: total found='.$tnt['tntfilesystem']['findcount']);
            }
        }

        return $ret;
    }
    
    /**
     * Switch the root path of a given file.
     *
     * Strips $oldroot from the start of the path and replaces it with $newroot.
     *
     * If the path is not in $oldroot then it is returned unaltered.
     *
     * Will not expand virtual paths before use.
     *
     * @static
     * @access  public
     * @param   string      $path       path to switch
     * @param   string      $oldroot    old root of path
     * @param   string      $newroot    new root of path
     * @return  string                  switched path
     */
    /*static*/ function SwitchRootPath($path, $oldroot, $newroot) {

        $path = rtrim(preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $path), '/');
        $oldroot = rtrim(preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $oldroot), '/');

        $oldrootlen = strlen($oldroot);
        if (strncasecmp($path, $oldroot, $oldrootlen) == 0) {
            $newroot = rtrim(preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $newroot), '/');
            $ret = $newroot.substr($path, $oldrootlen);
        } else {
            $ret = $path;
        }

        if (!$newroot || $newroot == '') {
            // Ensure its a relative path
            $ret = ltrim($ret, '/');
        }

        return $ret;
    }

    /**
     * Switch the path between the server and system roots.
     *
     * Shortcut to {@link SwitchRootPath()} for know root paths.
     *
     * Can be used to switch to and from the filesystem root, or webserver root
     * using TNT_ROOT_DIR and TNT_ROOT_URL.
     *
     * Can also be used to switch to and from the virtual file system, which
     * actually called {@link ContractPath()} and {@link ExpandPath()} to do
     * the conversion.
     *
     * Switching to relative paths is also supported - in this case the path is
     * returned as a relative path to the give root. (ie. the root path is
     * removed from the start of the given path).
     *
     * If the path can not be switched then it is returned unaltered.
     *
     * The $switch parameter should be one of;
     *   + TNT_FILESYSTEM_SYSTEMTOSERVER - switch from system to server
     *   + TNT_FILESYSTEM_SYSTEMTOVIRTUAL - switch from system to virtual
     *   + TNT_FILESYSTEM_SYSTEMTORELATIVE - switch from system to relative
     *   + TNT_FILESYSTEM_SERVERTOSYSTEM - switch from server to system
     *   + TNT_FILESYSTEM_SERVERTOVIRTUAL - switch from server to virtual
     *   + TNT_FILESYSTEM_SERVERTORELATIVE - switch from server to relative
     *   + TNT_FILESYSTEM_VIRTUALTOSERVER - switch from virtual to server
     *   + TNT_FILESYSTEM_VIRTUALTOSYSTEM - switch from virtual to system
     *   + TNT_FILESYSTEM_VIRTUALTORELATIVE - switch from virtual to relative
     *
     * If the $switch parameter is ommitted or zero then the method will guess
     * the switch by examining the current root of the path as follows;
     *   + currently server - switch to system
     *   + currently system - switch to server
     *   + currently virtual - switch to system
     *
     * @access  public
     * @param   string      $path       path to switch
     * @param   integer     $switch     direction in which to switch
     * @return  string                  switched path
     * @static
     */
    /*static*/ function SwitchPath($path, $switch = 0) {

        $ret = $path;

        $path = rtrim(preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $path), '/');

        if (empty($switch)) {
            if (strncasecmp($path, TNT_ROOT_DIR, strlen(TNT_ROOT_DIR)) == 0) {
                // currently a system path so switch to a server path
                $switch = TNT_FILESYSTEM_SYSTEMTOSERVER;
            } else if (strncasecmp($path, TNT_ROOT_URI, strlen(TNT_ROOT_URI)) == 0) {
                // currently a server path so switch to a system path
                $switch = TNT_FILESYSTEM_SERVERTOSYSTEM;
            } else if ($path{0} ==  '~') {
                // currently a virtual path so switch to a system path
                $switch = TNT_FILESYSTEM_VIRTUALTOSYSTEM;
            } else {
                $switch = 0;
            }
        }

        switch ($switch) {
            case TNT_FILESYSTEM_SYSTEMTOSERVER:
                // switch from system to server paths
                $ret = TNTFileSystem::SwitchRootPath($ret, TNT_ROOT_DIR, TNT_ROOT_URI);
                break;

            case TNT_FILESYSTEM_SYSTEMTOVIRTUAL:
                // switch from system to virtual paths
                $ret = TNTFileSystem::ContractPath($path, TNT_ROOT_DIR);
                break;

            case TNT_FILESYSTEM_SYSTEMTORELATIVE:
                // switch from system to relative paths
                $ret = TNTFileSystem::SwitchRootPath($ret, TNT_ROOT_DIR, '');
                break;

            case TNT_FILESYSTEM_SERVERTOSYSTEM:
                // switch from server to system paths
                $ret = TNTFileSystem::SwitchRootPath($ret, TNT_ROOT_URI, TNT_ROOT_DIR);
                break;

            case TNT_FILESYSTEM_SERVERTOVIRTUAL:
                // switch from server to virtual paths
                $ret = TNTFileSystem::ContractPath($path, TNT_ROOT_URI);
                break;

            case TNT_FILESYSTEM_SERVERTORELATIVE:
                // switch from server to relative paths
                $ret = TNTFileSystem::SwitchRootPath($ret, TNT_ROOT_URI, '');
                break;

            case TNT_FILESYSTEM_VIRTUALTOSYSTEM:
                // switch from virtual to system paths
                $ret = TNTFileSystem::ExpandPath($path, TNT_ROOT_DIR);
                break;

            case TNT_FILESYSTEM_VIRTUALTOSERVER:
                // switch from virtual to server paths
                $ret = TNTFileSystem::ExpandPath($path, TNT_ROOT_DIR);
                break;

            case TNT_FILESYSTEM_VIRTUALTORELATIVE:
                // switch from virtual to relative paths
                $ret = TNTFileSystem::ExpandPath($path, '');
                break;

            default:
                break;
        }

        return $ret;
    }
    
    /**
     * Split a path into component parts
     *
     * Returns associative array with keys 'dir', 'file', 'base', 'ext'
     *
     * @access  public
     * @param   string      $path       path to split
     * @return  array                   associative array of path components
     * @see TNTPath
     * @static
     */
    /*static*/ function SplitPath($path) {
    
        $pi['dir'] = preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', dirname($path));
        $pi['file'] = basename($path);
        $dot = strrpos($pi['file'], '.');
        if ($dot) {
            $pi['base'] = substr($pi['file'], 0, $dot);
            $pi['ext'] = substr($pi['file'], $dot + 1);
        } else {
            $pi['base'] = $pi['file'];
            $pi['ext'] = '';
        }
    
        return $pi;
    }
    
    /**
     * Build a path from component parts
     *
     * Accepts associative array with keys 'dir', 'file', 'base', 'ext'
     *
     * @access  public
     * @param   array       $pi         associative array of path components
     * @return  string                  built path
     * @static
     */
    /*static*/ function BuildPath($pi) {
    
        $path = '';
    
        if (!empty($pi['base'])) {
            $path .= $pi['base'];
        }
    
        if (!empty($pi['ext'])) {
            if ($pi['ext'][0] != '.') {
                $path .= '.';
            }
            $path .= $pi['ext'];
        }
    
        if (!empty($pi['dir'])) {
            $path = preg_replace('/(\/|'.preg_quote(DIRECTORY_SEPARATOR, '/').')+/', '/', $pi['dir'].'/'.$path);
        }
    
        return $path;
    }
    
    /**
     * Test if the path exists as one of the given types.
     *
     * Accepts paths in the virtual file system.
     *
     * Type can be one of: null, 'file', 'dir', 'link'
     *
     * @access  public
     * @param   string      $path       path to test
     * @param   string      $type       type to test for (optional)
     * @return  boolean                 true if path exists
     * @static
     */
    /*static*/ function Exists($path, $type = null) {
    
        $path = TNTFileSystem::ExpandPath($path);

        $ret = (@file_exists($path));

        if ($ret && $type) {
            switch ($type) {
                case 'dir':
                    $ret = is_dir($path);
                    break;
                case 'file':
                    $ret = is_file($path);
                    break;
                case 'link':
                    $ret = is_link($path);
                    break;
                default:
                    trigger_error('TNTFileSystem::Exists() - unknown type specified');
            }
        }

        return $ret;
    }
    
    /**
     * Get file stat or stats
     *
     * Elements are:
     *    null     - associative array of all components
     *    'dev'    - device
     *    'ino'    - inode number
     *    'mode'   - inode protection mode
     *    'perm'   - inode protection mode (same as mode)
     *    'nlink'  - number of links
     *    'uid'    - user id of owner
     *    'gid'    - group id owner
     *    'rdev'   - device type if inode device *
     *    'size'   - size in bytes
     *    'atime'  - time of last access
     *    'mtime'  - time of last modification
     *    'ctime'  - time of last change
     *    'blksize'- blocksize for filesystem I/O *
     *    'blocks' - number of blocks allocated
     *
     * NB: Items marked '*' may not be supported on all file systems.
     *
     * @access  public
     * @param   string      $path       path to get stats for
     * @param   string      $elm        component element to get
     * @return  array                   file statistic(s) (or null if error)
     */
    /*static*/ function Stats($path, $elm = null) {
    
        $path = TNTFileSystem::ExpandPath($path);

        $ret = (@stat($path));

        if ($ret) {
           $ret['perm'] = $ret['mode'];
        }

        if ($elm) {
           $ret = (isset($ret[$elm]) ? $ret[$elm] : null);
        }

        return $ret;
    }
    
    /**
     * Test if the path is a child path to another.
     *
     * Accepts paths in the virtual file system.
     *
     * @access  public
     * @param   string      $path       path to check
     * @param   string      $dir        directory path should be within
     * @param   boolean     $exists     check the file exists
     * @return  boolean                 true if path is in dir (and exists?)
     * @static
     */
    /*static*/ function IsChildPath($path, $parentpath, $exists = true) {

        $ret = false;
    
        $path = TNTFileSystem::ExpandPath($path);
        $parentpath = TNTFileSystem::ExpandPath($parentpath);

        $pl = strlen($parentpath);
        $l = strlen($path);

        if ($pl > $l) {
            // parent path is longer than child path
            $ret = false;
        } else if ($exists && !@file_exists($path)) {
            // child path does not exist
            $ret = false;
        } else if (strncasecmp($path, $parentpath.'/', $pl + 1) != 0) {
            // file is not in directory
            $ret = false;
        } else {
            $ret = true;
        }

        return $ret;
    }
    
    /**
     * Test if the path is in the virtual file system
     *
     * The virtual path is one starting with a tilde (~) character.
     *
     * @access  public
     * @param   string      $path       path to check
     * @return  boolean                 true if path is absolute
     * @static
     */
    /*static*/ function IsVirtualPath($path) {
    
        return ($path{0} == '~');
    }
    
    /**
     * Test if the path is absolute
     *
     * Handles Windows style drive name prefixes
     *
     * A virtual path is always assumed to be absolute.
     *
     * @access  public
     * @param   string      $path       path to check
     * @return  boolean                 true if path is absolute
     * @static
     */
    /*static*/ function IsAbsolutePath($path) {
        // @ error suppression is used in case path isn't long enough
        @$c = (($path{1} == ':') ? $path{2} : $path{0});
        return ($c == '/' || $c == '\\' || $c == '~');
    }
    
    /**
     * Make the path absolute to given $root.
     *
     * No alteration is made to already absolute paths, otherwise the given
     * $root path is prepended. No check is made to ensure that the resultant
     * path makes the given path absolute. 
     *
     * Examples,
     *  + MakeAbsolutePath('includes/code.inc', '/home/tnt/') =>
     *      '/home/tnt/includes/code.inc'.
     *  + MakeAbsolutePath('/tmp/access.log', '/home/tnt/') =>
     *      '/tmp/access.log'.
     *  + MakeAbsolutePath('includes/code.inc', 'tnt/') =>
     *      'tnt/includes/code.inc'. *** NOTE: result is not absolute ***
     *
     * This method is a shortcut to {@link MakePath()} as follows
     *  MakePath($path, $root).
     *
     * @access  public
     * @param   string      $path       path to check
     * @param   string      $root       return absolute to this root path
     * @return  string                  modified path
     * @static
     */
    /*static*/ function MakeAbsolutePath($path, $root = TNT_ROOT_DIR) {
    
        if (!TNTFileSystem::IsAbsolutePath($path)) {
            $path = TNTFileSystem::MakePath($path, $root);
        }

        return $path;
    }
    
    /**
     * Make the path relative to given $root.
     *
     * If the given $path is a child path of the given $root, then the
     * $root prefix is removed to make $path a relative path to $root.
     *
     * For example,
     *  + MakeRelativePath('/home/tnt/includes/code.inc', '/home/tnt/') =>
     *      'includes/code.inc'.
     *  + MakeAbsolutePath('/tmp/access.log', '/home/tnt/') =>
     *      '/tmp/access.log'.
     *
     * This method is a shortcut to {@link SwitchRootPath()} as follows
     * SwitchRootPath($path, $root, '');
     *
     * @access  public
     * @param   string      $path       path to check
     * @param   string      $root       return relative to this root path
     * @return  string                  modified path
     * @static
     */
    /*static*/ function MakeRelativePath($path, $root = TNT_ROOT_DIR) {

        $ret = TNTFileSystem::SwitchRootPath($path, $root, '');
    
        return $path;
    }

    /**
     * Convert a path to path relative to the installation root depending on a
     * given fixup criteria.
     *
     * The main use for this is when embedding paths in templates, ini files
     * database etc. The documentation for the embededed path will say whether
     * it is subjected to a 'fixup'.
     *
     * Two fixup modes are commonly used. In Mode One all absolute paths are
     * treated as relative to the installation root path and all relative
     * paths are treated as relative to the given $base path. In Mode Two all
     * absolute paths are left alone and all relative paths are treated as
     * relative to the installation root with the exception of relative paths
     * starting './' which are treated as relative to the given $base path.
     *
     * Absolute paths are assumed to be those starting with '/'. This method
     * does not use {@link TNTFileSystem::IsAbsolute()} and thus does not
     * support Windows drive letters.
     *
     * @access  public
     * @param   string      $path       path to fix up
     * @param   string      $base       base path (relative to SYNERGY_DIR)
     * @param   integer     $mode       fixup mode to use (0 or 1)
     * @return  string                  modified path (relative to SYNERGY_DIR)
     * @static
     */
    /*static*/ function FixupPath($path, $base, $mode = 1) {

        if ($path) {
            if ($mode == 0) {
                if ($path{0} == '/') {
                    // Absolute paths are relative to installation root
                    $path = ltrim($path, '/');
                } else if ($base) {
                    // Relative paths are relative to given base path
                    $path = rtrim($base, '/').'/'.$path;
                }
            } else if ($mode == 1) {
                if ($base && strncmp($path, './', 2) == 0) {
                    // Path relative to $base
                    $path = rtrim($base, '/').substr($path, 1);
                }
            }
        }

        return $path;
    }
    
    /**
     * Make directory and optionally all missing parent directories
     *
     * Accepts paths in the virtual file system.
     *
     * @access  public
     * @param   string      $dir        directory to make
     * @param   integer     $perm       permission to assign to the directory
     * @param   boolean     $parents    create all parents as required
     * @return  boolean                 true if directory made successfully
     * @static
     */
    /*static*/ function MkDir($dir, $perm = 0777, $parents = true) {

        $ret = true;
        $cwd = null;
    
        $dir = TNTFileSystem::ExpandPath($dir);

        $ret = true;

        if (!is_dir($dir)) {
            $track_errors = ini_set('track_errors', 1);
        
            $oldumask = umask(0);
        
            if (!$parents) {
                $ret = @mkdir($cwd, $perm);
            } else {
                // Only the $ret from the final call to mkdir() is relevant :-)
                // We ignore errors while making the parent directories as the
                // is_dir($cwd) may access a directory we don't have permissions
                // to see and so fail - however it may still exist and be possible
                // to create child directories within it.
    
                $dirs = explode('/', $dir);
                $m = count($dirs);
                if ($m != 0) {
                    $cwd = $dirs[0];
                    $ret = true;
                    if ($cwd != '' && !is_dir($cwd)) {
                        $ret = @mkdir($cwd, $perm);
                    }
        
                    for ($i = 1; ($i < $m); $i++) {
                        $cwd .= '/' . $dirs[$i];
                        if (!is_dir($cwd)) {
                            $ret = @mkdir($cwd, $perm);
                        }
                    }
                }
            }
        
            if (!$ret) {
                //TNTDebug::Dump("TNTFileSystem::MkDir() - failed to make directory: $cwd ($php_errormsg)");
            }
        
            umask($oldumask);
        
            ini_set('track_errors', $track_errors);
        }
    
        // Check the directory has the mode we require (we don't flag as error)
        if ($ret) {
            if (!($st = @stat($dir))) {
                //TNTDebug::Dump('TNTFileSystem::MkDir() - can not get dir stats: '.$dir);
            } else if (!isset($st['mode']) || ($st['mode'] & 0777) != ($perm & 0777)) {
                //TNTDebug::Dump('TNTFileSystem::MkDir() - '.sprintf('file "%s" has perm %o and not %o as required', $dir, $st['mode'], $perm));
    	    }
        }
    
        return $ret;
    }

    /**
     * Get the contents of a file
     *
     * Possible $open flags are:
     *    TNT_FILESYSTEM_IO_BINARY      - treat as binary data
     *    TNT_FILESYSTEM_IO_LOCK        - lock the file for reading
     *
     * @access  public
     * @param   string      $path       path to file
     * @param   integer     $opts       flags to use to open the file
     * @return  string                  $data if file read correctly (else null)
     * @static
     */
    /*static*/ function GetFileContents($path, $opts) {

        $ret = false;

        $path = TNTFileSystem::ExpandPath($path);

        if ($opts & TNT_FILESYSTEM_IO_BINARY) {
            $omode = 'rb';
        } else {
            $omode = 'rt';
        }

        $fp = fopen($path, $omode);
        if ($fp) {
            // Make sure magic quotes are off
            $mqr = get_magic_quotes_runtime();
            set_magic_quotes_runtime(0);

            // Clear cached details for this file and then get its size
            clearstatcache();
            $len = filesize($path);
        
            if ($opts & TNT_FILESYSTEM_IO_LOCK) {
                $ret = (flock($fp, LOCK_SH) &&
                        (($data = fread($fp, $data, $len)) !== false) &&
                        (strlen($data) == $len) &&
                        flock($fp, LOCK_UN));
            } else {
                $ret = ((($data = fread($fp, $data, $len)) !== false) &&
                        (strlen($data) == $len));
            }

            set_magic_quotes_runtime($mqr);

            fclose($fp);
        }

        if ($ret) {
            $ret = $data;
        }

        return $ret;
    }

    /**
     * Write the data to the given file
     *
     * Overwrites any data already in the file.
     *
     * Possible $open flags are:
     *    TNT_FILESYSTEM_IO_CREATE      - create the file is doesn't exist
     *    TNT_FILESYSTEM_IO_MKDIR       - make parent directories if required
     *    TNT_FILESYSTEM_IO_BINARY      - treat as binary data
     *    TNT_FILESYSTEM_IO_LOCK        - lock the file for writing
     *
     *
     * @access  public
     * @param   string      $path       path to file
     * @param   integer     $opts       flags to use to open the file
     * @param   string      $data       data to write to the file
     * @param   integer     $perm       permissions to create file with
     * @return  boolean                 true if file written successfully
     * @static
     */
    /*static*/ function SetFileContents($path, $opts, $data, $perm = 0666) {

        $ret = false;

        $path = TNTFileSystem::ExpandPath($path);

        if ($opts & TNT_FILESYSTEM_IO_CREATE) {
            $omode = 'w';
        } else {
            $omode = 'r+';
        }
        if ($opts & TNT_FILESYSTEM_IO_BINARY) {
            $omode .= 'b';
        } else {
            $omode .= 't';
        }

        $oldumask = umask(0);

        $mqr = get_magic_quotes_runtime();
        set_magic_quotes_runtime(0);

        if (($opts & TNT_FILESYSTEM_IO_CREATE) &&
            ($opts & TNT_FILESYSTEM_IO_MKDIR) &&
            (!file_exists($dir = dirname($path)))) {
            // Make parent directories
            // (failure to do so will be trapped by fopen() which will fail)
            TNTFileSystem::MkDir($dir, 0777);
        }

        $fp = fopen($path, $omode);
        if ($fp) {
            // Make sure magic quotes are off
            $mqr = get_magic_quotes_runtime();
            set_magic_quotes_runtime(0);

            $len = strlen($data);
            if ($opts & TNT_FILESYSTEM_IO_LOCK) {
                $ret = (flock($fp, LOCK_EX) &&
                        (fwrite($fp, $data, $len) === $len) &&
                        flock($fp, LOCK_UN));
            } else {
                $ret = (fwrite($fp, $data, $len) === $len);
            }

            set_magic_quotes_runtime($mqr);

            fclose($fp);
        }

        @chmod($path, $perm);

        umask($oldumask);

        if ($ret) {
            // Check the file has the minimum mode we require
            // (we don't flag as error)
            if (!($st = stat($path))) {
                //TNTDebug::Dump('TNTFileSystem::SetFileContents() - can not get file stats: '.$path);
            } else if (!isset($st['mode']) || ($st['mode'] & 0777) != ($perm & 0777)) {
                //TNTDebug::Dump('TNTFileSystem::SetFileContents() - '.sprintf('file "%s" has perm %o and not %o as required', $path, ($st['mode'] & 0777), ($perm & 0777)));
    	    }
        }
    
        return $ret;
    }

    /** 
     * Read some data from the filesystem cache.
     *
     * This is a quick and dirty cache, for a more secure cache use the
     * {@link TNTFileCache} class. This method is optimised for speed and
     * doesn't support cache verification etc.
     *
     * Any variable value can be cached - apart from the null value.
     *
     * Returns null if no cached data.
     *
     * @access  public
     * @param   string      $path       path to cache data in
     * @param   integer     $exptime    cache expiry time
     * @param   boolean     $useLock    use file locking
     * @return  mixed                   cached data (or null)
     */
    function GetCachedData($path, $exptime = null, $useLock = true) {

        $ret = null;

        $path = TNTFileSystem::ExpandPath($path, TNTFileSystem::CachePath());

        if (file_exists($path) &&
            ($exptime ? true : filemtime($path) < $exptime)) {

            $opts = TNT_FILESYSTEM_IO_BINARY |
                    ($useLock ? TNT_FILESYSTEM_IO_LOCK : 0);

            $data = FileSystem::GetFileContents($path, $opts);

            $ret = @unserialize($data);
        }

        return $ret;
    }

    /** 
     * Save some data to the filesystem cache.
     *
     * This is a quick and dirty cache, for a more secure cache use the
     * {@link TNTFileCache} class. This method is optimised for speed and
     * doesn't support cache verification etc.
     *
     * Any variable value can be cached - apart from the null value.
     *
     * Returns false if can't store the value or delete the cache.
     *
     * @access  public
     * @param   string      $path       path to cache data in
     * @param   mixed       $data       data to store in cache
     * @param   boolean     $useLock    use file locking
     * @return  boolean                 true if successfull
     */
    function SetCachedData($path, $data = null, $useLock = true) {

        $ret = false;

        $path = TNTFileSystem::ExpandPath($path, TNTFileSystem::CachePath());

        if (is_null($data)) {
            $ret = (file_exists($path) ? unlink($path) : true);
        } else {
            $opts = TNT_FILESYSTEM_IO_CREATE | TNT_FILESYSTEM_IO_MKDIR |
                    ($useLock ? TNT_FILESYSTEM_IO_LOCK : 0);

            $ret = TNTFileSystem::SetFileContents($path, $opts, serialize($data));
        }

        return $ret;
    }

   /**
     * Return the system path to the directory to to use for temporary files.
     *
     * This is a alias for ExpandPath('~/temp');
     *
     * @access  public
     * @return  string                  server path
     * @static
     */
    /*static*/ function TempPath() {
        return TNTFileSystem::ExpandPath('~/temp');
    }

    /**
     * Return the system path to the directory to to use for cache files.
     *
     * This is a alias for ExpandPath('~/cache');
     *
     * @access  public
     * @return  string                  server path
     * @static
     */
    /*static*/ function CachePath() {
        return TNTFileSystem::ExpandPath('~/cache');
    }

    /**
     * Return the system path to the directory to to use for data files.
     *
     * This is a alias for ExpandPath('~/data');
     *
     * @access  public
     * @return  string                  server path
     * @static
     */
    /*static*/ function DataPath() {
        return TNTFileSystem::ExpandPath('~/data');
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
