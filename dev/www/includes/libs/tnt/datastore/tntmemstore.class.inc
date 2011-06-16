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
 * Memory DataStore Class
 * ~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class defines an implementation of the TNTDataStore interface which
 * reads and writes the data to global memory. As such the data is only
 * available for the lifetime of the script.
 *
 * @package TNT.DataStore
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject'); 
TNTLoader::RequireClass('TNTDataStore'); 

/*
 * Globals
 */
global $tnt;
$tnt['_tntmemstore'] = array();

/*
 * Defines
 */

/**
 * Class to define a memory based implementation of the TNTDataStore interface.
 *
 * This implementation of the TNTDataStore interface uses global memory to
 * store the data and as such the data is only available for the lifetime of
 * the script.
 *
 * @package TNT.DataStore
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @extends TNTObject
 * @implements TNTDataStore
 */
class TNTMemStore extends TNTObject /* implements TNTDataStore */ {

    /**
     * DataStore name (used to identify one store from another)
     * @var     string                  datastore name
     * @access  private
     */
    var $_name = null;

    /**
     * Get datastore name
     *
     * @access  public
     * @return  string                  datastore name
     */
    function GetName() {
        return $this->_name;
    }

    /**
     * Set datastore name
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function SetName($name) {
        $this->_name = $name;
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
     * If the store does not exist then the method should return null.
     *
     * @access  public
     * @param   string      $elm       component element to get
     * @return  array                  file statistic(s) (or null if error)
     */
    function Stats($elm = null) {
        $ret = null;

        // Start of error section
        do {
            if (!$this->_name) {
                $this->_SetError('no store name specified');
                break;
            }

            if (!isset($GLOBALS['tnt']['_tntmemstore'][$this->_name])) {
                $this->_SetError('no store with name:'.$this->_name);
                break;
            }

            $store =& $GLOBALS['tnt']['_tntmemstore'][$this->_name];

            if ($elm && !isset($store['stats'][$elm])) {
                $this->_SetError('no element named: '.$elm);
                break;
            }
    
            $ret = ($elm ? $store['stats'][$elm] : $store['stats']);

        } while (false);
        // End of error section

        return $ret;
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
        $ret = null;

        // Start of error section
        do {
            if (!$this->_name) {
                $this->_SetError('no store name specified');
                break;
            }

            if (!isset($GLOBALS['tnt']['_tntmemstore'][$this->_name])) {
                $this->_SetError('no store with name:'.$this->_name);
                break;
            }

            $GLOBALS['tnt']['_tntmemstore'][$this->_name]['stats']['atime'] = time();

            $ret = $GLOBALS['tnt']['_tntmemstore'][$this->_name]['data'];

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Set the data in the datastore and update last modified timestamp.
     *
     * If the data is not null and the datastore does not exist then it
     * should be created.
     *
     * If the data is set to NULL then the datastore is deleted.
     *
     * Returns false if it can't set the data.
     *
     * @access  public
     * @param   mixed       $data       data to hold in datastore
     * @return  boolean                 true if set ok.
     */
    function SetData($data) {
        $ret = false;

        // Start of error section
        do {
            if (!$this->_name) {
                $this->_SetError('no store name specified');
                break;
            }

            if (is_null($data)) {
                if (isset($GLOBALS['tnt']['_tntmemstore'][$this->_name])) {
                    // Delete the datastore
                    unset($GLOBALS['tnt']['_tntmemstore'][$this->_name]);
                }

                $ret = true;
            
            } else {
                if (!isset($GLOBALS['tnt']['_tntmemstore'][$this->_name])) {
                    // Create the datastore
                    $GLOBALS['tnt']['_tntmemstore'][$this->_name] = array(
                        'stats' => array(
                            'size' => 0,
                            'mtime' => 0,
                            'ctime' => time(),
                            'atime' => 0),
                        'data' => '');
                }
                
                $store =& $GLOBALS['tnt']['_tntmemstore'][$this->_name];

                $store['data'] = $data;
                $store['stats']['mtime'] = time();
                $store['stats']['size'] = strlen(is_string($data) ? $data : serialize($data));

                $ret = true;
            }

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function TNTMemStore($name = null) {

        TNTObject::TNTObject();

        $this->_implements[] = 'tntdatastore';

        $this->SetName($name);

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTMemStore() {

    }

}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
