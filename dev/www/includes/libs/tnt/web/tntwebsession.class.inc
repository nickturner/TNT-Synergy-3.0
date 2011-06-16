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
 * Session Class
 * ~~~~~~~~~~~~~
 *
 * This class handles a HTTP session. It is a wrapper around the basic PHP
 * session code which inherently handles HTTP sessions.
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');
TNTLoader::RequireClass('TNTTrustee');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to provides access to session-state values as well as session-level
 * settings and lifetime management methods.
 *
 * @package TNT.Web
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTWebSession extends TNTVarMap {

    /**
     * Session is regarded as active if has been accessed within this
     * number of seconds.
     * @var     integer                 active period in seconds
     * @access  public
     */
    var $activeTime = 600;

    /**
     * Session Identifier
     * @var     integer                 session identifier
     * @access  private
     */
    var $_id = null;

    /**
     * Session stats
     * @var     array                   internal stats
     * @access  private
     */
    var $_stats = null;

    /**
     * Database record handling object
     * @var     TNTDbRec&               reference to db record object
     * @access  private
     */
    var $_rec = null;
    
    /**
     * Internal flag to show this is singleton object
     * @var     string                  name of singleton class
     * @access  private
     * @see     Singleton()
     */
    var $__singleton = __CLASS__;

    /** 
     * Get a reference to this object - only a single instance should exist
     *
     * @access  public
     * @return  TNTWebSession&         reference to global TNTWebSession object
     * @static
     */
    /*static*/ function &Singleton() {
        return TNTObject::Singleton(__CLASS__);
    }

    /**
     * Gets the unique session ID used to identify the session.
     *
     * @access  public
     * @return  string                  the PHP session ID
     */
    function SessionID() {
        return session_id();
    }

    /**
     * Gets a value indicating whether the session ID is embedded in the URL
     * or stored in an HTTP cookie.
     *
     * There are two methods to propagate a session id: 
     *   + Cookies 
     *   + URL parameter 
     * PHP session module supports both methods. Cookies are optimal, but
     * because they are not always available, the alternative way is also
     * provided. The second method embeds the session id directly into URLs. 
     *
     * @access  public
     * @return  boolean                 true if the session is embedded in URL
     */
    function IsCookieless() {
        return (ini_get(session.use_cookies) ? true : false);
    }

    /**
     * Gets a value indicating whether the session was created with the
     * current request.
     *
     * @access  public
     * @return  boolean                 true if the session is new
     */
    function IsNewSession() {
        return false; // TODO
    }

    /**
     * Get session stats
     *
     * Elements are:
     *    null     - associative array of all components
     *    'atime'  - time of last access
     *    'mtime'  - time of last modification
     *    'ctime'  - time of last change
     *
     * If a session can not provide a given statistic then it should ommit
     * it from the returned structure or return a null value.
     *
     * @access  public
     * @param   string      $elm       component element to get
     * @return  array                  file statistic(s) (or null if error)
     */
    function Stats($elm = null) {
        $ret = null;

        // Start of error section
        do {
            if (!$this->_id) {
                $this->_SetError('no session open');
                break;
            }

            $ret = ($elm ? $this->stats[$elm] : $stats);

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Tidy up expired sessions
     *
     * @access  public
     * @return  void
     */
    function Tidy() {
        return $this->_Tidy(0);
    }

    /**
     * Delete the current session
     *
     * @access  public
     * @return  void
     */
    function Delete() {
        if (!$this->_id) {
            $this->_SetError('no session open');
            $ret = false;
        } else {
            $ret = $this->_Delete($this->_id);
        }
        return $ret;
    }

    /**
     * Is there an active session for a given user.
     *
     * If trustee is given as an integer it is assume to be the id of a
     * trustee of type 'user'. If given as a string then it is assumed to
     * be a valid trustee DN ('<type>:<name|id>'). If given as an object
     * implementing the TNTTrustee interface the the TrusteeDN() method
     * is used.
     *
     * @access  public
     * @param   string  $user       user (trustee DN) to check
     * @return  bool                true if trustee has active session
     */
    function IsActiveUser($user) {
        if (is_numeric($user)) {
            $user = "user:{$user}";
        } else if (is_object($user) && implements($user, 'tnttrustee')) {
            $user = $user->TrusteeDN();
        }
    
        $active = $this->_Active();
        if ($active) {
            foreach ($active as $sid => $session) {
                if ($session['owner'] == $user) {
                    $ret = true;
                    break;
                }
            }
        }

        return $ret;
    }
    
    /**
     * Return number of active sessions
     *
     * @access  public
     * @return  array               array of two counters [0]=guests [1]=members
     */
    function ActiveCount() {

        $ret = array(0, 0);

        $active = $this->_Active();
        if ($active) {
            foreach ($active as $sid => $session) {
                if ($session['owner']) {
                    $ret[0]++;
                } else {
                    $ret[1]++;
                }
            }
        }
    
        return $ret;
    }
    
    /**
     * Returns details of the sessions in use.
     *
     * Session details should be returned as an associative array with the
     * following elements;
     * @element integer 'id'    session id (unique integer)
     * @element integer 'ctime' session creation time (seconds)
     * @element integer 'mtime' session last modification time (seconds)
     * @element integer 'atime' session last access time (seconds)
     * @element string  'owner' session owner (trustee DN)
     *
     * @access  public
     * @param   boolean $active         return only active sessions
     * @return  array                   array of sessions indexed by id
     */
    function _Sessions($active = true) {

        $ret = null;

        $sql = 'SELECT DISTINCT * FROM %sessions%';

        if ($active) {
            $period = (int)$this->activeTime;
            $sql .= "WHERE atime + period > UNIX_TIMESTAMP()";
        }

        TNTLoader::RequireClass('TNTDatabase');
        $db =& TNTDatabase::Singleton();

        $rows = $db->QueryFetch($sql);

        if ($rows) {
            $ret = array();
            foreach ($rows as $row) {
                $ret[$row['id']] = $row;
            }
        }
    
        return $ret;
    }

    /*
     * Open a session - PHP callback function
     *
     * @access  public
     * @param   string  $path       session save path
     * @param   string  $name       session name
     * @return  boolean             true if successfull
     */
    function _Open($path, $name) {

        if (!$this->_rec) {
            TNTLoader::RequireClass('TNTDbRec');
            $this->_rec =& new TNTDbRec('sessions', 'sid');
        }

        return true;
    }

    /*
     * Close a session - PHP callback function
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function _Close() {

        if ($this->_rec) {
            destroy($this->_rec);
            $this->_rec = null;
        }

        return true;
    }

    /*
     * Read the session data
     *
     * @access  public
     * @param   string  $id         session id
     * @return  string              session data or empty string
     */
    function _Read($id) {

        $data = null;

        $this->_id = 0;
        $this->_owner = null;
        $this->_stats = null;

        // Start error section
        do {
            if (!$this->_rec) {
                break;
            }

            if (!($rec = $this->_rec->Obtain($id))) {
                // No session for this id - so create one
                $rec = $this->_rec->Define();

                $rec['id'] = $id;
                $rec['ip'] = @$_SERVER['REMOTE_ADDR'];
                $rec['agent'] = @$_SERVER['HTTP_USER_AGENT'];

                if (!$this->_rec->Create($rec)) {
                    $this->_SetError($this->_rec->Error());
                    break;
                }
            }

            $this->_owner = $rec['owner'];

            $this->_stats = array(
                'mtime' => $rec['mtime'],
                'atime' => $rec['atime'],
                'ctime' => $rec['ctime'],
                );

            $data = $rec['data'];

        } while (false);
        // End error section

        return $data;
    }

    /*
     * Write the session data
     *
     * @access  public
     * @param   string  $id         session id
     * @param   string  $data       session data
     * @return  boolean             true if successfull
     */
    function _Write($id, $data) {

        $ret = false;

        // Start error section
        do {
            if (!$this->_rec) {
                break;
            }

            if (!($rec = $this->_rec->Obtain($id))) {
                break;
            }

            $data['data'] = $data;

            $rec['id'] = $id;
            $rec['owner'] = $this->_owner;
            $rec['data'] = $data;

            $ret = $this->_rec->Modify($rec);

        } while (false);
        // End error section

        return $ret;
    }

    /*
     * Delete a session
     *
     * @access  public
     * @param   string  $id         session id
     * @return  boolean             true if successfull
     */
    function _Delete($id) {

        $ret = false;

        // Start error section
        do {
            if (!$this->_rec) {
                break;
            }

            $ret = $this->_rec->Delete($id);

        } while (false);
        // End error section

        return $ret;
    }

    /*
     * Tidy sessions
     *
     * @access  public
     * @param   integer $maxlife    max life time in seconds for a session
     * @return  boolean             true if successfull
     */
    function _Tidy($maxlifetime) {

        $ret = false;

        // Start error section
        do {
            if (!$this->_rec) {
                break;
            }

            if ($maxlifetime == 0) {
                $maxlifetime = $this->activeTime;
            }

            $etime = (int)(time() - $maxlifetime);

            $ret = $this->_rec->Delete("WHERE atime < $etime");

        } while (false);
        // End error section

        return $ret;
    }

    /*
     * Open the session
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function Open() {
        //$this->Tidy();

        // Session start - this should call _Read() if there is a valid session
        return @session_start();
    }

    /*
     * Close the session
     *
     * @access  public
     * @return  boolean             true if successfull
     */
    function Close() {

        // Write and close the session before we destroy the session object
        session_write_close();

        return true;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   array       $options    construction options
     * @return  void
     */
    function TNTWebSession() {
        global $tnt;

        TNTVarMap::TNTVarMap();

        $this->_rec = null;

        // Set the PHP call backs
        session_set_save_handler(
            array(&$this, '_Open'),
            array(&$this, '_Close'),
            array(&$this, '_Read'),
            array(&$this, '_Write'),
            array(&$this, '_Delete'),
            array(&$this, '_Tidy'));

        $this->_id = 0;
        $this->_stats = null;

        // Our variable map is really the PHP $_SESSION map
        $this->_vars =& $_SESSION;

        // Open the session
        $this->Open();

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTWebSession() {

        // Close the session
        $this->Close();

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
