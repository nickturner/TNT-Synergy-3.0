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
 * File DataStore Class
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * This class defines an implementation of the TNTDataStore interface which
 * stores its data in a file. As such the data is available for the lifetime
 * of the file.
 *
 * @package TNT.DataStore
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTFile'); 
TNTLoader::RequireClass('TNTDataStore'); 

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to define a file based implementation of the TNTDataStore interface.
 *
 * This implementation of the TNTDataStore interface uses a file to store the
 * data and as such the data is only available for the lifetime of the script.
 *
 * This is a dummy class as the TNTFile class which it extends currently
 * implements the TNTDataStore interface already but doesn't declare it.
 *
 * @package TNT.DataStore
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @extends TNTObject
 * @implements TNTDataStore
 */
class TNTFileStore extends TNTFile /* implements TNTDataStore */ {

    /**
     * File access options
     * @var     integer                 binary combination of file io flags
     * @access  public
     */
    var $opts = 0;

    /**
     * File creation permissions
     * @var     integer                 binary combination of file perms
     * @access  public
     */
    var $perm = 0777;

    /**
     * Get datastore name
     *
     * @access  public
     * @return  string                  datastore name
     */
    function GetName() {
        return parent::GetName();
    }

    /**
     * Set datastore name
     *
     * @access  public
     * @param   string      $name       datastore name
     * @return  void
     */
    function SetName($name) {
        return parent::SetName($name);
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
        return parent::Stats($elm);
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
        if (($data = $this->GetContents($this->opts))) {
            $ret = unserialize($data);
        }
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
     * @param   string      $data       data to hold in datastore
     * @return  boolean                 true if set ok.
     */
    function SetData($data) {
        if (is_null($data)) {
            $ret = $this->Delete();
        } else {
            $ret = $this->SetContents(serialize($data), $this->opts, $this->perm);
        }
        return $ret;
    }

    /**
     * Construct the object
     *
     * <pre>
     * Allowed options are:
     * @element boolean     'lock'      use file locking
     * @element boolean     'perm'      file creation perm
     * </pre>
     *
     * @access  public
     * @param   string      $name       datastore name
     * @param   array       $options    various options
     * @return  void
     */
    function TNTFileStore($name = null, $options = null) {

        TNTFile::TNTFile($name);

        $this->_implements[] = 'tntdatastore';

        $this->opts = TNT_FILESYSTEM_IO_BINARY|TNT_FILESYSTEM_IO_CREATE;
        if (isset($options['lock']) && $options['lock']) {
           $this->opts |= TNT_FILESYSTEM_IO_LOCK;
        }

        $this->perm = 0777;
        if (isset($options['perm'])) {
           $this->perm = $options['perm'];
        }

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTFileStore() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
