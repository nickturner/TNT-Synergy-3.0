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
 * File Class
 * ~~~~~~~~~~
 *
 * This class allows you to manipulate a file in the file system.
 *
 * @package TNT.System
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
define('TNT_FILE_CREATE',   0x0010);
define('TNT_FILE_TRUNCATE', 0x0020);
define('TNT_FILE_BINARY',   0x0100);
define('TNT_FILE_LOCK',     0x0200);
define('TNT_FILE_MKDIR',    0x0400);

/**
 * Class to manipulate a file
 *
 * Due to PHPs internal caching you can use the PHP functions to access a
 * files properties with no speed implications by using the code, for example,
 * "is_readable($file->Path());" or "filemtime($file->Path());" etc.
 *
 * At present this class only supports the reading and writing of whole files
 * at a time as we rarely ever read or update parts of a file.
 *
 * File locking on reading and writing is supported.
 *
 * @package TNT.System
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTFile extends TNTObject {

    /**
     * File name
     * @var     string                  file name as specified
     * @access  private
     */
    var $_name = null;

    /**
     * File absolute path
     * @var     string                  real file path after expansion
     * @access  private
     */
    var $_path = null;

    /**
     * File options
     * @var     integer                 integer consisting of TNT_FILE_OPTS
     * @access  private
     */
    var $_opts = null;

    /**
     * File permissions
     * @var     integer                 permissions to use if creating a file
     * @access  private
     */
    var $_perm = 0777;

    /**
     * File path components
     * @var     array                   path components
     * @access  private
     */
    var $_pi = null;

    /**
     * File statistics
     * @var     array                  file stats
     * @access  private
     */
    var $_st = null;

    /**
     * Get file name
     *
     * File name should be specified as either an absolute path or a path
     * relative to the installation directory.
     *
     * (use $this->Path() if you want absolute file path)
     *
     * @access  public
     * @return  string                  file name
     */
    function GetName() {
        return $this->_name;
    }

    /**
     * Set file name
     *
     * File name should be specified as either an absolute path or a path
     * relative to the installation directory.
     *
     * This sets the name of the file to operate on - it does not try to
     * rename any existing file being operated on.
     *
     * @access  public
     * @param   string      $name       file name
     * @return  void
     */
    function SetName($name) {
        $this->_name = $name;
        $this->Reset();
    }

    /**
     * File path
     *
     * @access  public
     * @return  string                  file path
     */
    function Path() {
        return $this->_path;
    }

    /**
     * Get file path component or components
     *
     * Elements are:
     *    null     - associative array of all components
     *    'path'   - the complete path
     *    'dir'    - the directory path
     *    'base'   - the file name (including extension)
     *    'name'   - the file name (excluding extension)
     *    'ext'    - the extension
     *
     * NB: We currently don't support WIN32 drives etc. they will be retained
     * in the 'dir' component.
     *
     * @access  public
     * @param   string      $elm       component element to get
     * @return  array                  file path component(s) (or null if error)
     */
    function PathInfo($elm) {
        $ret = null;

        // Start of error section
        do {
            if (!$this->_path) {
                $this->_SetError('no file name specified');
                break;
            }
    
            if (!$this->_pi) {
                if (!($this->_pi = TNTFileSystem::SplitPath($this->_path))) {
                    $this->_SetError('can\'t split path: '.$this->_path);
                    break;
                }
            }

            if ($elm && !isset($this->_pi[$elm])) {
                $this->_SetError('no element named: '.$elm);
                break;
            }
    
            $ret = ($elm ? $this->_pi[$elm] : $this->_pi);
        } while (false);
        // End of error section
    
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
     * @param   string      $elm       component element to get
     * @return  array                  file statistic(s) (or null if error)
     */
    function Stats($elm = null) {
        $ret = null;

        // Start of error section
        do {
            if (!$this->_path) {
                $this->_SetError('no file name specified');
                break;
            }

            if (!$this->_st) {
                if (!$this->Exists()) {
                    $this->_SetError('no file named: '.$this->_path);
                    break;
                }
                $this->_st = stat($this->_path);
                if ($this->_st) {
                    $this->_st['perm'] =& $this->_st['mode'];
                }
            }

            if ($elm && !isset($this->_st[$elm])) {
                $this->_SetError('no element named \''.$elm.'\': ', $this->_st);
                break;
            }
    
            $ret = ($elm ? $this->_st[$elm] : $this->_st);
        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Reset any cached information about this file
     *
     *
     * @access  public
     * @return  void
     */
    function Reset() {
        $this->_path = ($this->_name ? TNTFileSystem::ExpandPath($this->_name) : null);
        $this->_pi = null;
        $this->_st = null;
    }

    /**
     * Read the actual file data
     *
     * The method allows any derived class to read the file contents from 
     * the open (and locked?) file via a different mechanism.
     *
     * @access  public
     * @param   resource        $fp     open file handle
     * @return  string                  actual data read (or false or error)
     */
    function _ReadData($fp) {

        $data = null;
        clearstatcache();
        $len = filesize($this->_path);
        $len2 = 0;
        if ((($data = @fread($fp, $len)) === false) ||
            (($len2 = strlen($data)) != $len)) {
            //TNTDebug::Dump("TNTFile::_ReadData() - Only read $len2 out of $len bytes");
             return false;
        }

        return $data;
    }

    /**
     * Write the actual file data
     *
     * The method allows any derived class to write the file contents to 
     * the open (and locked?) file via a different mechanism.
     *
     * @access  public
     * @param   resource        $fp     open file handle
     * @param   string          $data   data to write to file
     * @return  boolean                 true if written ok
     */
    function _WriteData($fp, $data) {

        $len = strlen($data);
        if (($len2 = @fwrite($fp, $data, $len)) !== $len) {
            //TNTDebug::Dump("TNTFile::_WriteData() - Only wrote $len2 out of $len bytes");
            return false;
        }

        return true;
    }

    /**
     * Get the file contents
     *
     * @access  public
     * @param   include     $opts       file reading options
     * @return  string                  data read from file (or false on error)
     */
    function GetContents($opts = 0) {
        $ret = false;

        $track_errors = ini_set('track_errors', 1);
        $php_errormsg = '';

        // Make sure magic quotes are off
        $mqr = get_magic_quotes_runtime();
        set_magic_quotes_runtime(0);

        $fp = null;
        $locked = false;

        // Start of error section
        do {
            if (!$this->_path) {
                $this->_SetError('no file name specified');
                break;
            }

            $ret = false;

            if ($opts & TNT_FILE_BINARY) {
                $omode = 'rb';
            } else {
                $omode = 'rt';
            }

            $fp = @fopen($this->_path, $omode);
            if (!$fp) {
                $this->_SetError('can\'t open file: '.$this->_path." ({$php_errormsg})");
                break;
            }

            if ($opts & TNT_FILE_LOCK) {
               if (!@flock($fp, LOCK_SH)) {
                    $this->_SetError('can\'t lock file: '.$this->_path." ({$php_errormsg})");
                    break;
               }
               $locked = true;
            }

            if (($data = $this->_ReadData($fp)) === false) {
                $this->_SetError('can\'t read from file: '.$this->_path." ({$php_errormsg})");
                break;
            }

            $ret = $data;
    
        } while (false);
        // End of error section

        if ($locked && !@flock($fp, LOCK_UN)) {
            //Debug::Dump("can't unlock file: {$this->_path} ({$php_errormsg})");
        }
        if ($fp) {
            fclose($fp);
        }

        set_magic_quotes_runtime($mqr);

        clearstatcache();
        $this->_st = null;

        ini_set('track_errors', $track_errors);

        return $ret;
    }

    /**
     * Write the data to the given file
     *
     * Overwrites any data already in the file.
     *
     * If creating the file then any parent directories will be created if
     * TNT_FILE_MKDIR is specified.
     *
     * @access  public
     * @param   string      $data       data to write to the file
     * @param   integer     $opts       file writing options
     * @param   integer     $perm       permissions to create file with
     * @return  boolean                 true if file written successfully
     */
    function SetContents($data, $opts = 0, $perm = 0777) {
        $ret = false;

        $track_errors = ini_set('track_errors', 1);
        $php_errormsg = '';

        // Make sure magic quotes are off
        $mqr = get_magic_quotes_runtime();
        set_magic_quotes_runtime(0);

        // Clear the umask
        $oldumask = umask(0);

        $fp = null;
        $locked = false;

        // Start of error section
        do {
            if (!$this->_path) {
                $this->_SetError('no file name specified');
                break;
            }

            if ($opts & (TNT_FILE_CREATE|TNT_FILE_MKDIR)) {
                // Ensure parent directory exists

                $dir = $this->PathInfo('dir');
                if (!TNTFileSystem::MkDir($dir, 0777)) {
                    $this->_SetError('can\'t make parent directory: '.$dir);
                    break;
                }
            }

            if ($opts & TNT_FILE_CREATE) {
                $omode = 'w';
            } else {
                $omode = 'r+';
            }
            if ($opts & TNT_FILE_BINARY) {
                $omode .= 'b';
            } else {
                $omode .= 't';
            }

            $fp = @fopen($this->_path, $omode);
            if (!$fp) {
                $this->_SetError('can\'t open file: '.$this->_path." ({$php_errormsg})");
                break;
            }
            
            if ($opts & TNT_FILE_LOCK) {
               if (!@flock($fp, LOCK_EX)) {
                    $this->_SetError('can\'t lock file: '.$this->_path." ({$php_errormsg})");
                    break;
               }
               $locked = true;
            }

            if (!$this->_WriteData($fp, $data)) {
                $this->_SetError('can\'t write file: '.$this->_path." ({$php_errormsg})");
                break;
            }

            $ret = true;

        } while (false);
        // End of error section

        if ($locked && !@flock($fp, LOCK_UN)) {
            //Debug::Dump("can't unlock file: {$this->_path} ({$php_errormsg})");
        }

        if ($fp) {
            fclose($fp);
        }

        set_magic_quotes_runtime($mqr);

        umask($oldumask);

        @chmod($this->_path, $perm);

        if ($ret) {
            // Check the file has the minimum mode we require
            // (we don't flag as error)
            if (!($st = @stat($this->_path))) {
                //Debug::Dump("can not get file stats: {$this->_path} ({$php_errormsg})");
            } else if (!isset($st['mode']) || ($st['mode'] & 0777) == ($perm & 0777)) {
                //Debug::Dump(sprintf('file "%s" has perm %o and not %o as required', $this->_path, ($st['mode'] & 0777), ($perm & 0777)));
    	    }
        }
    
        clearstatcache();
        $this->_st = null;

        ini_set('track_errors', $track_errors);

        return $ret;
    }

    /**
     * Delete the file
     *
     * @access  public
     * @return  boolean                 true if file delete successfully
     */
    function Delete() {
        $track_errors = ini_set('track_errors', 1);
        $php_errormsg = '';

        // Start of error section
        do {
            if (!$this->_path) {
                $this->_SetError('no file name specified');
                break;
            }

            if (!@unlink($this->_path)) {
                //Debug::Dump("can't unlock file: {$this->_path} ({$php_errormsg})");
            }

            $ret = true;

        } while (false);
        // End of error section

        ini_set('track_errors', $track_errors);

        return $ret;
    }

    /**
     * Test if the file exists
     *
     * @access  public
     * @return  boolean                 true if file exists
     */
    function Exists() {
        $ret = false;

        // Start of error section
        do {
            if (!$this->_path) {
                $this->_SetError('no file name specified');
                break;
            }

            $ret = TNTFileSystem::Exists($this->_path, 'file');

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Construct the object
     *
     * File name should be specified as either an absolute path or a path
     * relative to the installation directory. Alternatively the path can
     * be specified in the virtual '/synergy' filesystem.
     *
     * @access  public
     * @param   string      $name       file name
     * @return  void
     */
    function TNTFile($name = null) {

        TNTObject::TNTObject();

        TNTLoader::RequireClass('TNTFileSystem');

        $this->SetName($name);
    }

    /**
     * Destruct the object.
     *
     * This method is hookable.
     *
     * @access  private
     */
    function _TNTFile() {
    }

}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
