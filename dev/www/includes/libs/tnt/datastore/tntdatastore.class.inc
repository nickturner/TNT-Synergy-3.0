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
 * DataStore Interface Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class defines the TNTDataStore interface.
 *
 * @package TNT.DataStore
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

/*
 * Defines
 */

/**
 * Class to define the TNTDataStore interface. Classes which implement this
 * interface must implement these methods.
 *
 * A datastore is an opaque chunk of data. The datastore knows nothing about
 * how that data is formatted, it just allows access to read and write it.
 *
 * @package TNT.DataStore
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTDataStore {

    /**
     * Get datastore name (implementation specific)
     *
     * @access  public
     * @return  string                  datastore name
     */
    function GetName() {
    }

    /**
     * Set datastore name (implementation specific)
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function SetName($name) {
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
    }

    /**
     * Get the data from the data store and update last accessed timestamp.
     *
     * Returns null if it can't get the data.
     *
     * @access  public
     * @return  mixed                   data held in store (or null).
     */
    function GetData() {
    }

    /**
     * Set the data in the datastore and update last modified timestamp.
     *
     * If the data is not null and the datastore does not exist then it
     * should be created.
     *
     * If the data is set to NULL then the implementation can delete the
     * datastore if require. This is not the same as storing an empty
     * string.
     *
     * Returns false if it can't set the data.
     *
     * @access  public
     * @param   mixed       $data       data to hold in datastore
     * @return  boolean                 true if set ok.
     */
    function SetData($data) {
    }

    /**
     * Constructor to prevent direct instanciation
     *
     * @access  private
     */
    function TNTDataStore() {
        if (get_class($this) == __CLASS__) {
            TNT::Abort(__CLASS__.'::'.__FUNCTION__.' - can\'t construct interface class directly');
        }
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
