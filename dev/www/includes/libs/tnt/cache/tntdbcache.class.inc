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
 * Record Cache Class
 * ~~~~~~~~~~~~~~~~~~
 *
 * This class handles the caching of data to a field in a database record.
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
class TNTDbCache extends TNTCache {

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
    function TNTDbCache($cacheName = null, $options = null) {

        TNTLoader::RequireClass('TNTDbStore');

        $_datastore =& new TNTDbStore();

        TNTCache::TNTCache($_datastore, $options);

        $this->_datastore->SetName($cacheName);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTDbCache() {
        destroy($this->_datastore);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
