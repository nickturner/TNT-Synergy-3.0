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
 * Memory Cache Class
 * ~~~~~~~~~~~~~~~~~~
 *
 * This class handles the caching of data to local memory.
 *
 * @package TNT.Cache
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTCache');

/*
 * Globals
 */
global $tnt;
$tnt['_tntcachemem'] = array();

/*
 * Defines
 */

/**
 * Class to manage the caching of data to memory
 *
 * @package TNT.Cache
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @extends TNTFile
 */
class TNTMemCache extends TNTCache {

    /**
     * Get cache name (datastore name)
     *
     * @access  public
     * @return  string                  datastore name
     */
    function GetName() {
        return $this->_datastore->GetName($name);
    }

    /**
     * Set cache name (datastore name)
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function SetName($name) {
        return $this->_datastore->SetName($name);
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
        return $this->_datastore->Stats($elm);
    }

    /**
     * Get the data from the data store and update last accessed timestamp
     *
     * @access  public
     * @return  string                  data held in cache (or false)
     */
    function GetData() {
        return parent::GetData();
    }

    /**
     * Set the data into the cache and update last modified timestamp.
     *
     * @access  public
     * @param   string      $data       data to hold in cache
     * @return  boolean                 true if set ok.
     */
    function SetData($data) {
        return parent::SetData($data);
    }

    /**
     * Construct the object
     *
     * <pre>
     * Allowed options are:
     *    'cacheLife' => life of cache file in seconds (integer),
     *    'useHash' => name function to use as an integrity hash (md5|crc32),
     * </pre>
     *
     * @access  public
     * @param   string      $cacheName  name of cache
     * @param   array                   array of construction options
     * @return  void                    constructor
     */
    function TNTMemCache($cacheName = null, $options = null) {

        TNTLoader::RequireClass('TNTMemStore');

        $_datastore =& new TNTMemStore();

        TNTCache::TNTCache($_datastore, $options);

        $this->SetName($cacheName);

        $this->_implements[] = 'tntdatastore';
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTMemCache() {

        destroy($this->_datastore);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
