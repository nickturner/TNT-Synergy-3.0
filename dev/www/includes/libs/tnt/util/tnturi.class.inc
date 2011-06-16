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
 * URI Manipulation Class
 * ~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class represents uniform resource identifier as objects which can be
 * manipulated.
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
TNTLoader::RequireClass('TNTFmtStr');

/*
 * Globals
 */
global $tnt;
$tnt['tnturi']['_elementNames'] = array(
    'scheme', 'host', 'port', 'user', 'pass', 'path', 'query', 'fragment');

/*
 * Defines
 */

/**
 * Class to represent a Uniform Resource Locator
 *
 * This is a formatted string consisting of the following named elements;
 *     'scheme' - Scheme name (ie. http, https, ftp, mailto etc.)
 *     'host'   - Host name of machine contain resource
 *     'port'   - Port number on host
 *     'user'   - User name to authenticate to host as
 *     'pass'   - Password for user
 *     'path'   - Path to the resource on host machine
 *     'Query'  - string (after the question mark '?')
 *     'fragment' - string (after the hash mark '#')
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTUri extends TNTFmtStr {

    /**
     * Explode the URI into the elements
     *
     * @access  private
     * @return  boolean                 true if successfull
     * @see TNTFmtStr
     */
    function _Explode() {

        $ret = false;

        $this->_elements = null;
    
        // We let the PHP parse_url() function do the work
        if (empty($this->_string)) {
            $ret = true;
        } else if (!($ui = parse_url($this->_string))) {
            $this->_SetError('can\'t split: '.$url);
        } else {
            $ret = $this->Set($ui);
        }
    
        return $ret;
    }
    
    /**
     * Implode the elements into the URI
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function _Implode() {
    
        $url = null;
        if (!empty($this->_elements['scheme'])) {
            $url .= $this->_elements['scheme'].':';
        }
        if (!empty($this->_elements['host'])) {
            $url .= '//';
            if (!empty($this->_elements['user'])) {
                $url .= $this->_elements['user'];
                if (!empty($this->_elements['pass'])) {
                    $url .= ':'.$this->_elements['pass'];
                }
                $url .= '@';
            }
            $url .= $this->_elements['host'];
            if (!empty($this->_elements['port'])) {
                $url .= ':'.$this->_elements['port'];
            }
            if (!empty($this->_elements['path']) && $this->_elements['path'][0] != '/') {
                $url .= '/';
            }
        }
        if (!empty($this->_elements['path'])) {
            $url .= $this->_elements['path'];
        }
        if (!empty($this->_elements['query'])) {
            if ($this->_elements['query'][0] == '?' || $this->_elements['query'][0] == '&') {
                $this->_elements['query'] = substr($this->_elements['query'], 1);
            }
            $url .= '?'.$this->_elements['query'];
        }
        if (!empty($this->_elements['fragment'])) {
            if ($this->_elements['fragment'][0] == '#') {
                $this->_elements['fragment'] = substr($this->_elements['fragment'], 1);
            }
            $url .= '#'.$this->_elements['fragment'];
        }

        $this->_string = $url;
    
        return true;
    }

    /**
     * Expand a possible virtual URL to a real URL
     *
     * @access  public
     * @return  void
     */
    function Expand() {

        if (!$this->_elements) {
            $this->_Explode();
        }

        $url = null;
        switch ($this->_elements['scheme'] ? $this->_elements['scheme'] : 'http') {
            case 'nul':
            case 'null':
                break;
    
            case 'about':
            case 'page':
            case 'exec':
            case 'sql':
                $link = &new TNTUri();
                $link->path = $this->_elements['path'];
                $link->query = $this->_elements['query'];
                $link->fragment = $this->_elements['fragment'];

                $root = TNT_ROOT_URI;

                $this->_elements['scheme'] = 'http:';
                $this->_elements['path'] = $root.'/index.'.PHP_EXT;
                $this->_elements['query'] = 'url='.urlencode($link->Build());
                break;

            default:
                // Address could be a virtual filesystem path - so expand it
                if ($this->IsFilePath() && !empty($this->_elements['path'])) {
                    TNTLoader::RequireClass('TNTFileSystem');
                    $this->_elements['path'] = TNTFileSystem::ExpandPath($this->_elements['path'], TNT_ROOT_URI);
                }
                break;
    
        }
    
    }

    /**
     * Returns true if the URI address a file system type path.
     *
     * If this is true then the path can be manipluated using the
     * {@link TNTPath} class or via the shortcut methods in this class.
     *
     * @access  public
     * @return  boolean                 true if the URI uses a file path
     */
    function IsFilePath() {

        static $schemes = array('http', 'https', 'ftp', 'ftps', 'file');

        $scheme = $this->GetElement('scheme');

        $ret = (!$scheme || in_array($scheme, $schemes));

        return $ret;
    }

    /**
     * Append Query
     *
     * @access  public
     * @param   string      $query       query to append
     * @return  void
     */
    function AppendQuery($query) {

        if ($query[0] == '?' || $query[0] == '&') {
            $query = substr($query, 1);
        }


        if (!empty($query)) {
            $q = $this->GetElement('query');

            if ($q) {
                $q .= '&';
            }
            $q .= $query;

            $this->SetElement('query', $q);
        }

        return $ret;
    }

    /**
     * Equal to
     *
     * Returns true if $that is equal to current URL object.
     * I'm hoping this method reflects RFC 2616 Section 3.2.3
     *
     * Note: this method will not compare the following:
     *  - user info (username and password)
     *  - fragment (#fragment)
     *
     * @access   public
     * @param    mixed       $url        string URL or instance of URL class
     * @return   boolean                 true if the items match
     */
    function IsEqual($that) {

        $ret = false;

        // Start error section
        do {
            if (!is_object($that) || !is_a($that, 'tnturl')) {
                $that = &new TNTUri($that);
            }

            $_this = $this->Get('array');
            $_that = $that->Get('array');

    
            // Check scheme:
            //   - Comparisons of scheme names MUST be case-insensitive
            if (strcasecmp($_this['scheme'], $_that['scheme']) !== 0) {
                $this->_SetError('schemes do not match');
                break;
            }
            // Check host:
            //   - Comparisons of host names MUST be case-insensitive
            if (strcasecmp($_this['host'], $_that['host']) !== 0) {
                $this->_SetError('hosts do not match');
                break;
            }
    
            // Check port:
            // both ports must be identical, and (from RFC 2616):
            //   - A port that is empty or not given is equivalent to the default
            //     port for that URI-reference.
            // passing true to get_port() will result in the default port for 
            // HTTP and HTTPS schemes to be returned.
            $this_tmp = $_this['port'];
            $that_tmp = $_that['port'];
    
            if (!$this_tmp) $this_tmp = $this->DefaultPort();
            if (!$that_tmp) $that_tmp = $that->DefaultPort();
    
            if ($this_tmp != $that_tmp) {
                $this->_SetError('ports do not match');
                break;
            }
    
            // Check path:
            //   - An empty abs_path is equivalent to an abs_path of "/".
            $this_tmp = urldecode($_this['path']);
            $that_tmp = urldecode($_that['path']);
    
            if ($this_tmp == '') $this_tmp = '/';
            if ($that_tmp == '') $that_tmp = '/';
    
            if (strcmp($this_tmp, $that_tmp) != 0) {
                $this->_SetError('paths do not match');
                break;
            }
    
            // Check query
            $this_tmp = urldecode($_this['query']);
            $that_tmp = urldecode($_that['query']);
            if (strcmp($this_tmp, $that_tmp) != 0) {
                $this->_SetError('queries do not match');
                break;
            }

            $ret = true;

        } while (false);
        // End error section
    
        return $ret;
    }

    /**
     * Default Port
     *
     * Returns the default port for the URL scheme
     *
     * @access   public
     * @return   integer                default port number
     */
    function DefaultPort() {

        $scheme = $this->GetElement('scheme');

        switch ($scheme) {
            case 'http':
                $port = 80;
                break;
            case 'https':
                $port = 443;
                break;
            default:
                $port = null;
                break;
        }

        return $port;
    }

    /**
     * Make relative URL absolute given the specified base URL
     *
     * Base URL can be specified as either a TNTUri object or any value
     * that a TNTUri object can understand as a valid URI via its {@link Set()}
     * method.
     *
     * If no base URL is specified then the current response URL will be
     * used.
     *
     * An absolute URL passed to this method will overwrite all existing URL
     * parts stored.
     *
     * I'm hoping this method reflects RFC 2396 Section 5.2
     *
     * @access  public
     * @param   TNTUri      $url        base to be relative to (or null)
     * @return  boolean                 true if successfull
     */
    function MakeAbsolute($base) {

        $ret = false;

        // Start error section
        do {
            if (is_null($base)) {
                // The URL requested. (Should use the same base URI as was
                // requested - this may be different to the script path if
                // directory translation was enabled.)
                $base = 'http:';
                if (!empty($_SERVER['HTTP_HOST'])) {
                    $base .= '//'.$_SERVER['HTTP_HOST'];
                } else if (!empty($_SERVER['SERVER_ADDR'])) {
                    $base .= '//'.$_SERVER['SERVER_ADDR'];
                    if (!empty($_SERVER['SERVER_PORT'])) {
                        $base .= ':'.$_SERVER['SERVER_PORT'];
                    }
                }
                if (!empty($_SERVER['REQUEST_URI'])) {
                    $base .= $_SERVER['REQUEST_URI'];
                } else if (isset($_SERVER['PHP_SELF'])) {
                    $base .= $_SERVER['PHP_SELF'];
                    if (!empty($_SERVER['QUERY_STRING'])) {
                        $base .= '?'.$_SERVER['QUERY_STRING'];
                    }
                }
            }

            if (!is_object($base) || !is_a($base, 'tnturl')) {
                $base = &new TNTUri($base);
            }

            if (!$base->IsAbsolute()) {
                $this->_SetError('base must be absolute URL: '.$base->ToString());
                break;
            }

            $_this = $this->Get('array');
            $_base = $base->Get('array');

            // if path is empty, and scheme, host, and query are undefined,
            // the URL is referring to the base URL
            if (empty($_this['path']) && empty($_this['scheme']) &&
                empty($_this['host']) && empty($_this['query'])) {
                $this->Set($_base);
                $this->SetElement('query', @$_this['query']);
                $this->SetElement('fragment', @$_this['fragment']);
                $ret = true;
                break;
            }
    
            // if scheme is set and is different to base scheme then
            // URI is assumed to be absolute. We allow the scheme name to be
            // present in the relative URI if it is the same as the base URI
            // scheme.
            if (isset($_this['scheme']) && $_this['scheme'] != $_base['scheme']) {
                $ret = true;
                break;
            }

            // if the authority component is defined, then the reference is a
            // network-path. Otherwise, the reference URI's authority is
            // inherited from the base URI's authority component, which will
            // also be undefined if the URI scheme does not use an authority
            // component.
            if (!empty($_this['host']) ||
                !empty($_this['port']) ||
                !empty($_this['user']) ||
                !empty($_this['pass'])) {
                $ret = true;
                break;
            }
    
            $this->SetElement('scheme', @$_base['scheme']);
            $this->SetElement('host', @$_base['host']);
            $this->SetElement('port', @$_base['port']);
            $this->SetElement('user', @$_base['user']);
            $this->SetElement('pass', @$_base['pass']);
    
            $this->SetElement('path', TNTFileSystem::MakeAbsolutePath($_this['path'], $_base['path']));

            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Test if the URI is absolute.
     *
     * To be an absolute URI it must include a scheme.
     *
     * NOTE: This does not check that any contained file path is absolute.
     *
     * @access  public
     * @return  void
     */
    function IsAbsolute() {

        $scheme = $this->GetElement('scheme');

        $ret = (!is_null($scheme) && $scheme != '');

        return $ret;
    }

    /**
     * Remove all multiple slashes and relative paths from a path
     * It will take into account symbolic directory links if possible
     * where the path is a local path which exists.
     *
     * Calls {@link TNTFileSystem::NormalisePath()} on 'dirname'.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @return  void
     */
    function NormalisePath() {

        if ($this->IsFilePath()) {
            TNTLoader::RequireClass('TNTFileSystem');

            $this->SetElement('path', TNTFileSystem::NormalisePath($this->GetElement('path')));
        }

    }

    /**
     * Test if the file referenced by the URI exists.
     *
     * It attempts to connect to the given URI, if connection is accepted then
     * it assumes it exists.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @param   string      $type       type to test for (optional)
     * @return  boolean                 true if path exists
     */
    function Exists() {

        $ret = false;

        if ($this->IsFilePath()) {
            if (($fp = fopen($this->Get('string')))) {
                $ret = true;
                fclose($fp);
            }
        }

        return $ret;
    }
    
    /**
     * Test if the path component is a child path to another.
     *
     * Accepts paths in the virtual file system.
     *
     * Calls {@link TNTFileSystem::IsChildPath()} on 'dirname'.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @param   string      $parentpath directory path should be within
     * @param   boolean     $exists     check the file exists
     * @return  boolean                 true if path is in dir (and exists?)
     */
    function IsChildPath($parentpath, $exists = true) {

        if ($this->IsFilePath()) {
            TNTLoader::RequireClass('TNTFileSystem');

            $ret = TNTFileSystem::NormalisePath($this->GetElement('path'));
        }

        return $ret;
    }
    
    /**
     * Test if the path component is in the virtual file system
     *
     * Calls {@link TNTFileSystem::IsVirtualPath()} on 'dirname'.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @return  boolean                 true if path is absolute
     */
    function IsVirtualPath() {

        if ($this->IsFilePath()) {
            TNTLoader::RequireClass('TNTFileSystem');

            $ret = TNTFileSystem::IsVirtualPath($this->GetElement('path'));
        }

        return $ret;
    }
    
    /**
     * Test if the path component is absolute
     *
     * Calls {@link TNTFileSystem::IsAbsolutelPath()} on 'dirname'.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @return  boolean                 true if path is absolute
     */
    function IsAbsolutePath() {

        if ($this->IsFilePath()) {
            TNTLoader::RequireClass('TNTFileSystem');

            $ret = TNTFileSystem::IsAbsolutePath($this->GetElement('path'));
        }

        return $ret;
    }
    
    /**
     * Make the path component absolute to given $root.
     *
     * No alteration is made to already absolute paths, otherwise the given root
     * path is added. No check is made to ensure that the addition of the root
     * path makes the given path absolute.
     *
     * For example, if the $path="includes/synergy.inc" and
     * $root="/home/synergy" then the result is
     * "/home/synergy/includes/synergy.inc".
     *
     * If no $root is specified then the code will use the default web server
     * root as specified by {@link TNTSystem::$serverPath}.
     *
     * Calls {@link TNTFileSystem::MakeAbsolutePath()} on 'dirname'.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @return  void
     */
    function MakeAbsolutePath($root = null) {

        if ($this->IsFilePath()) {
            TNTLoader::RequireClass('TNTFileSystem');

            if (is_null($root)) {
                $root = TNTFileSystem::ServerRoot();
            }

            $this->SetElement('path', TNTFileSystem::MakeAbsolutePath($this->GetElement('path'), $root));
        }

    }
    
    /**
     * Make the path component relative to given $root.
     *
     * If the given $path is a child path of the given $root, then the
     * $root prefix is removed to make $path a relative path to $root.
     *
     * For example, if the $path="/home/synergy/includes/synergy.inc" and
     * $root="/home/synergy" then the result is "includes/synergy.inc".
     *
     * Calls {@link TNTFileSystem::MakeRelativePath()} on 'path'.
     *
     * If no $root is specified then the code will use the default web server
     * root as specified by {@link TNTSystem::$serverPath}.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @return  void
     */
    function MakeRelativePath($root = TNT_ROOT_URI) {
    
        if ($this->IsFilePath()) {
            TNTLoader::RequireClass('TNTFileSystem');
            $this->SetElement('path', TNTFileSystem::MakeRelativePath($this->GetElement('path'), $root));
        }

    }
    
    /**
     * Fixup path element.
     *
     * Calls {@link TNTFileSystem::FixupPath()} on 'path' element.
     *
     * This only works for URI's that address files
     * (ie. {@link TNTUri::IsFilePath()} returns true).
     *
     * @access  public
     * @param   string      $base       base path for relative paths
     * @param   integer     $mode       fixup mode to use
     * @return  void
     */
    function FixupPath($base, $mode = 0) {
        if ($this->IsFilePath()) {
            TNTLoader::RequireClass('TNTFileSystem');
            $this->SetElement('path', TNTFileSystem::FixupPath($this->GetElement('path'), $base, $mode));
        }
    }
    
    /**
     * Helper function to get a element in a URL string
     *
     * @access  public
     * @param   mixed       $data       initial URL data
     * @param   string      $elm        element name
     * @return  string                  element value (or null)
     * @static
     */
    /*static*/ function GetUriElement($data, $elm) {
        return parent::GetDataElement(__CLASS__, $data, $elm);
    }

    /**
     * Helper function to set a element in a URL string
     *
     * @access  public
     * @param   mixed       $data       initial URL data
     * @param   string      $elm        element name
     * @param   string      $val        element value
     * @return  string                  data with element set
     * @static
     */
    /*static*/ function SetUriElement($data, $elm, $val) {
        return parent::SetDataElement(__CLASS__, $data, $elm, $val);
    }
    
    /**
     * Construct the object
     *
     * Any initial value is passed to {@link TNTUri::Set()}
     *
     * @access  public
     * @param   mixed       $data       initial value for object
     * @return  void
     */
    function TNTUri($data = null) {
        TNTFmtStr::TNTFmtStr($data, $GLOBALS['tnt']['tnturi']['_elementNames']);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTUri() {
    }
    
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
