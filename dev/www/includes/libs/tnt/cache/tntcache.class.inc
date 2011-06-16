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
 * Cache Class
 * ~~~~~~~~~~~
 *
 * This is the base caching class. Its adds caching support to any datastore.
 *
 * @package TNT.Cache
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
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

/*
 * Defines
 */

/**
 * Class to manage the caching of data in a datastore.
 *
 * @package TNT.Cache
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @extends TNTObject
 */
class TNTCache extends TNTObject {

    /**
     * Cache life time in seconds (defaults to 1 hour)
     * @var     integer                 cache lifetime
     * @access  public
     */
    var $cacheLife = 3600;

    /**
     * Cache integritity checking hash
     * @var     callback                integretity checking hash function
     * @access  public
     */
    var $cacheHash = null;

    /**
     * The datastore in which the data is cached
     * @var     TNTObject               object that implements TNTDataStore
     * @access  private
     */
    var $_datastore = null;

    /**
     * Get cache name (datastore name)
     *
     * @access  public
     * @return  string                  datastore name
     */
    function GetName() {
        $ret = null;

        /* Start error section */
        do {
            if (!$this->_datastore || !implements($this->_datastore, 'TNTDataStore')) {
                $this->_SetError('no datastore');
                break;
            }

            $ret = $this->_datastore->GetName($name);

        } while (false);
        /* End error section */

        return $ret;
    }

    /**
     * Set cache name (datastore name)
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function SetName($name) {
        $ret = null;

        /* Start error section */
        do {
            if (!$this->_datastore || !implements($this->_datastore, 'TNTDataStore')) {
                $this->_SetError('no datastore');
                break;
            }

            $ret = $this->_datastore->SetName($name);

        } while (false);
        /* End error section */

        return $ret;
    }

    /**
     * Get data store stats
     *
     * Elements are:
     *    null     - associative array of all components
     *    'size'   - size in bytes
     *    'atime'  - time of last access
     *    'mtime'  - time of last modification
     *    'ctime'  - time of last change
     *
     * If a datastore can not provide a given statistic then it should ommit
     * it from the returned structure or return a null value.
     *
     * If the datastore does not exist then the method should return null.
     *
     * @access  public
     * @param   string      $elm       component element to get
     * @return  array                  file statistic(s) (or null if error)
     */
    function Stats($elm = null) {
        $ret = null;

        /* Start error section */
        do {
            if (!$this->_datastore || !implements($this->_datastore, 'TNTDataStore')) {
                $this->_SetError('no datastore');
                break;
            }

            $ret = $this->_datastore->Stats($elm);

        } while (false);
        /* End error section */

        return $ret;
    }

    /**
     * Get the data from the cache
     *
     * Fails if the data is not cached or if the cache has expired
     *
     * Optionally a timestamp can be passed, if the cache is older than this
     * timestamp then it is assumed to have expired.
     *
     * @access  public
     * @param   integer     $exptime    cache expiry time
     * @return  string                  the cached data, or null
     * @see SetData()
     */
    function GetData($exptime = null) {
        $ret = null;

        /* Start error section */
        do {
            if (!$this->_datastore || !implements($this->_datastore, 'TNTDatastore')) {
                $this->_SetError('no datastore');
                break;
            }

            // Get the time the cache was last updated
            $mtime = $this->_datastore->Stats('mtime');

            if (!$mtime) {
                // cache does not yet exist
                $this->_SetError('cache does not exist');
                break;
            }

            if ($mtime < 0) {
                $this->_SetError('failed to get cache update time');
                break;
            }

            if ($exptime && $mtime < $exptime) {
                $this->_SetError('cache has expired');
                break;
            }
    
            if ($this->cacheLife && (($mtime + $this->cacheLife) < time())) {
                $this->_SetError('cache has exceeded its lifespan');
                break;
            }

            // Read the data from the cache file
            if (($_data = $this->_datastore->GetData()) === false) {
                $this->_SetError('failed to read cache');
                break;
            }

            if (!isset($_data['hash']) || !isset($_data['data'])) {
                // Delete the cache
                $this->SetData(null);
                $this->_SetError('invalid data in the cache');
                TNTDebug::Error('Invalid cache data = ', $_data);
                break;
            }

            if (($func = $this->cacheHash) && function_exists($func)) {
                $hash = (is_string($_data['data']) ? $func($_data['data']) : $func(serialize($_data['data'])));
                if ($_data['hash'] != $hash) {
                    // Delete the cache
                    $this->SetData(null);
                    $this->_SetError('hash is incorrect');
                    break;
                }
            }

            // Return the data
            $ret = $_data['data'];

        } while (false);
        /* End error section */

        return $ret;
    }

    /**
     * Set the contents for the cache.
     *
     * @access  public
     * @param   mixed       $data       the data to cache
     * @return  boolean                 true if successfull
     * @see GetData()
     */
    function SetData($data) {
        $ret = false;

        /* Start error section */
        do {
            if (!$this->_datastore || !implements($this->_datastore, 'TNTDatastore')) {
                $this->_SetError('no datastore');
                break;
            }

            $_data['data'] = $data;
    
            if (($func = $this->cacheHash) && function_exists($func)) {
                // The first 32 bytes are the hash the rest are the data
                $_data['hash'] = (is_string($_data['data']) ? $func($_data['data']) : $func(serialize($_data['data'])));
            } else {
                $_data['hash'] = 'none';
            }

            // Write the data to the cache
            if (!$this->_datastore->SetData($_data)) {
                $this->_SetError('failed to write cache');
                break;
            }

            $ret = true;

        } while (false);
        /* End error section */
        
        return $ret;
    }

    /**
     * Construct the object
     *
     * Supported construction options are;
     * @element integer 'cacheLife' life of the cached data in seconds
     * @element string  'cacheHash' name of function used as integrity hash
     *
     * @access  public
     * @param   TNTObject   $datastore  reference to datastore
     * @param   array                   array of construction options
     * @return  void                    constructor
     */
    function TNTCache(&$datastore, $options = null) {

        TNTObject::TNTObject();

        $allowed_options = array('cacheLife', 'cacheHash');

        $this->_datastore =& $datastore;

        if ($this->_datastore && !implements($this->_datastore, 'tntdatastore')) {
            $this->_SetError('not a valid datastore');
        }

        foreach ($allowed_options as $allowed_option) {
            if (isset($options[$allowed_option])) {
                $this->$allowed_option = $options[$allowed_option];
            }
        }

        $this->_implements[] = 'tntdatastore';

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTCache() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
