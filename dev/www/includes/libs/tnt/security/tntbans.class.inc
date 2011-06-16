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
 * Bans Class
 * ~~~~~~~~~~
 *
 * This handles the manipulation of site wide bans, which state who can and
 * who can't handle the site. Bans can be temporay or time restricted, they
 * can effect certain Clients (by IP address) or Trustees (by DN).
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
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
 * Class to handle the manipulation of Bans.
 *
 * This handles the manipulation of site wide bans, which state who can and
 * who can't handle the site. Bans can be temporay or time restricted, they
 * can effect certain Clients (by IP address) or Trustees (by DN).
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTBans extends TNTObject {

    /**
     * Reference to open database to use
     * @var     object                  reference to TNTDatabase object
     * @access  private
     */
    var $_db = null;

    /**
     * Tidy up old bans
     *
     * @access  public
     * @return  void
     */
    function Tidy() {

        static $tidied = false;
    
        if (!$tidied) {
            // Disable any expired permissions (they are not deleted)
            $sql = "UPDATE %bans% SET disabled=1 WHERE (etime > 0 AND etime <= UNIX_TIMESTAMP())";
            $this->_db->Query($sql);
    
            $tidied = true;
        }

    }

    /**
     * Check if given Client is banned from the system.
     *
     * Client is specified by its IP address.
     *
     * @access  public
     * @param   string      $ip         client ip address
     * @return  boolean                 true if the client is banned
     */
    function IsBannedClient($ip = null) {

        $ret = false;

        // Start error section
        do {
            // Normalise the IP address to nnn.nnn.nnn[.nnn] format
            $nip = @vsprintf('%03u.%03u.%03u.%03u', explode('.', $ip));
            if (!$nip) {
                $this->_SetError('Invalid Client IP address: '.$ip);
                break;
            }

            // The ip address must not match any given in the ban list. The
            // list can contain wildcarded IP addresses.
            $_nip = $this->_db->QuoteString($nip);
            $where = "!disabled AND (UNIX_TIMESTAMP() BETWEEN stime AND etime) AND (type = 0 AND $_nip LIKE ban)";
        
            if ($this->_db->CountWhere('bans', $where) > 0) {
                $ret = true;
            }
        } while (false);
        // end error section
           
        return $ret;
    }
  
    /**
     * Ban a client for a duration of time
     *
     * User is specified by their trustee DN.
     *
     * @access  public
     * @param   string      $dn         trustee dn
     * @return  boolean                 true if the user is banned
     */
    function IsBannedUser($dn = null) {

        $ret = false;

        // Start error section
        do {
            $_dn = $this->_db->QuoteString($dn);
            $where = "!disabled AND (UNIX_TIMESTAMP() BETWEEN stime AND etime) AND (type = 1 AND ban = $_dn)";
        
            if ($this->_db->CountWhere('bans', $where) > 0) {
                $ret = true;
                break;
            }

        } while (false);
           
        return $ret;
    }
    
    /**
     * Returns a list of all the current ban records.
     *
     * Can specified page and pageSize to allow the list to be paged through.
     *
     * @access  public
     * @param   integer     $page       page number to return
     * @param   integer     $pageSize   size of each page
     * @return  integer                 unqiue identifier for this ban (or 0)
     */
    function ListBans($page = 0, $pageSize = 20) {

        $where = 0;
        if ($page && $pageSize) {
            $where = "PAGE $page,$pageSize";
        }

        return $this->_db->SelectRec('bans', 'id', $where);
    }

    /**
     * Ban a client or user from the system
     *
     * Returns a unique id number for this ban which can be used to modify
     * or delete this ban.
     *
     * If no start and end time are given then the ban is forever
     * (in practice $stime is set to 0 and $etime to 0xFFFFFFFF seconds).
     *
     * @access  public
     * @param   string      $ban        trustee dn or ip address to ban
     * @param   integer     $type       0 = ip, 1 = dn
     * @param   integer     $stime      time to start the ban (UTC)
     * @param   integer     $etime      time to end the ban (UTC)
     * @return  integer                 unqiue identifier for this ban (or 0)
     */
    function AddBan($ban = null, $type, $stime = 0, $etime = 0xffffffff) {

        $ret = 0;

        // Start error section
        do {
            if ($etime < $stime) {
                $this->_SetError('End time is before start time');
                break;
            }

            settype($stime, 'integer');
            settype($etime, 'integer');
            settype($type, 'integer');
            $_ban = $this->_db->QuoteString($ban);

            $sql = "INSERT INTO %bans% (type, ban, stime, etime) VALUES($type, $ban, $stime, $etime)";

            if (!$this->_db->Query()) {
                $this->_SetError('Failed to insert new ban');
                break;
            }

            $ret = $this->_db->InsertId();

        } while (true);
           
        return $ret;
    }
    
    /**
     * Construct the object
     *
     * Supported construction options are;
     *   'db' - reference to database instance to use
     *
     * @access  public
     * @param   array   $options    construction options
     * @return  void
     */
    function TNTBans($options = null) {

        TNTObject::TNTObject();

        TNTLoader::RequireClass('TNTDatabase');

        if (!empty($options['db'])) {
            $this->_db =& $options['db'];
        } else {
            $this->_db =& TNTDatabase::Singleton();
        }

        $this->Tidy();

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTBans() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
