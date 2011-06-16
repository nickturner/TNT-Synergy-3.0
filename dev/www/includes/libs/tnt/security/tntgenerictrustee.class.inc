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
 * Generic User Class
 * ~~~~~~~~~~~~~~~~~~
 *
 * This class represents a generic user using simple password authentication.
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTTrustee');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a generic trustee and is the base class for
 * {@link TNTGenericUser()} and {@link TNTGenericGroup()}.
 *
 * The generic trustee is implemented using simple database tables to store
 * the individual trustee types and a table to store the relationship.
 *
 * It is intended that any application built on this framework would either
 * derive from this class or reimplement it.
 *
 * The application INI uses the variable 'ClassNames.TNTGenericUser' to know
 * the name of the class to instatiate for a GenericUser when required.
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @implements TNTTrustee
 */
class TNTGenericTrustee extends TNTTrustee {

    /**
     * The database data stored for this trustee
     * @var     array                   database data
     * @access  private
     */
    var $_data = null;

    /*
     * Return the type of the trustee.
     *
     * @access  public
     * @return  string                  type string
     */
    function Type() {
        return parent::Type();
    }

    /*
     * Return the ID of the trustee.
     *
     * The ID is unique amongst all trustees of the same type and must be
     * a signed integer greater than zero.
     *
     * @access  public
     * @return  string                  unique id
     */
    function ID() {
        return $this->_data['id'];
    }

    /*
     * Return the Name of the trustee.
     *
     * The Name is unique amongst all trustees of the same type and must be
     * a character string of format "[a-z_][a-z0-9_]{0,31}".
     *
     * The name is optional and if not defined then the implementation should
     * return the ID as a string.
     *
     * @access  public
     * @return  string                  unique name
     */
    function Name() {
        return $this->_data['name'];
    }

    /*
     * Return true if the trustee allows child trustees.
     *
     * @access  public
     * @return  boolean                 true if children are allowed
     */
    function IsGroup() {
        return parent::IsGroup();
    }

    /*
     * Return list of groups the given trustee is a member of.
     *
     * The groups are returned as an array of DN strings.
     *
     * @access  public
     * @param   string      $dn         trustee DN
     * @param   TNTDatabase $db         reference to database object
     * @param   boolean     $resurse    recurse up the heirachy
     * @return  array                   array of trustee DNs
     */
    function _Groups($dn, &$db, $recurse = true) {
        $ret = null;

        if (!$db) {
            $db =& TNTDatabase::Singleton();
        }

        $dn = $db->EscapeString($dn);

        $result = $db->Query("SELECT pdn FROM %trustees% WHERE dn='$dn'");

        if ($result) {
            while (($row = $result->FetchRow())) {
                $ret[] = $row['pdn'];
                if ($recurse && ($r = $this->_Groups($row['pdn'], $db, true))) {
                    $ret += $r;
                }
            }
            destroy($result);
        }

        return $ret;
    }

    /*
     * Return list of all Groups the trustee is a member of.
     *
     * The groups are returned as an array of DN strings.
     *
     * @access  public
     * @return  array                   array of parent group DNs
     */
    function Groups($complete = true) {
        $args = array('complete' =>& $complete);
        $ret = null;

        if (!$this->_Enter(__CLASS__, __FUNCTION__, $args, $ret)) {
            return $ret;
        }

        $complete = ($complete ? 1 : 0);

        if (empty($this->_groups[$complete])) {

            $id = $this->ID();

            $this->_groups[$complete] = $this->_Groups($id, null, $complete);
        }

        return $this->_groups[$complete];
    }

    /*
     * Method to return an associative array of data describing this Trustee.
     * The data can be passed to {@link TNTTrustee::Restore()} to
     * recreate the trustee.
     *
     * This is not used to 'persist' the object as it should only save
     * enough data to recreate a class of the correct type and reload it
     * with the Trustee details. (As such it is only normally required to
     * the trustee id and any credentials required to authenticate it on
     * restoration).
     *
     * The returned data can be of any type and the same type will be passed
     * to {@link Restore()} to restore the trustee.
     *
     * The derived class must call this base implementation to create the
     * associative data array, it can then add to it as required. This base
     * method will store the name of the trustee class so it can be created
     * when restored.
     *
     * Different reasons supported;
     *   + TNT_TRUSTEE_REQUEST - data stored in the request (ie. cookie)
     *   + TNT_TRUSTEE_SESSION - data stored in the session.
     *
     * @access  public
     * @param   string      $reason     reason for storing the trustee
     * @return  mixed[]                 data required to restore this trustee
     * @see     Restore()
     */
    function Store($reason) {
        return parent::Store($reason);
    }

    /*
     * Static method to restore a trustee using the data supplied from
     * a call to {@link Store()}.
     *
     * The call to {@link Store()} must use the same 'reason' or the
     * restoration will fail. The restored trustee is authenticated by
     * calling {@link Authenticate()} with the 'reason' specified.
     *
     * Initialially this method is called statically to create the correct
     * trustee object. Once the object is created then the method is called
     * on that object to intialise it with the correct data.
     *
     * Different reasons supported;
     *   + TNT_TRUSTEE_REQUEST - data stored in the request (ie. cookie)
     *   + TNT_TRUSTEE_SESSION - data stored in the session.
     *
     * @access  public
     * @param   string      $reason     reason for storing the trustee
     * @return  mixed       $data       data required to restore this trustee
     * @return  TNTObject               reference to trustree object
     * @see     Store()
     */
    function &Restore($reason, $data) {
        return parent::Restore($reason, $data);
    }

    /*
     * Check if the trustee is in a given group.
     *
     * @access  public
     * @param   string      $dn         DN of group to check for
     * @return  boolean                 true if in given group
     */
    function IsInGroup($dn) {
        $groups = $this->Groups(true);

        return in_array((array)$groups, $dn);
    }

    /*
     * Check if the trustee has been authenticated.
     *
     * @access  public
     * @return  boolean                 true if has been authenticated
     */
    function IsAuthenticated() {
        return parent::IsAuthenticated();
    }

    /*
     * Check if the trustee has been assigned the given permissions to
     * a given resource.
     *
     * This is a short cut to {@link TNTPerm::CheckPerm()}
     *
     * @access  public
     * @param   string      $resource   name of resource to check
     * @param   integer     $perm       binary combinations of permissions
     * @return  boolean                 true if has required permissions
     */
    function IsPermitted($resource, $perm) {
        return parent::IsPermitted($resource, $perm);
    }

    /*
     * Check if the trustee has been assigned the given privileges.
     *
     * This is a short cut to {@link TNTPerm::CheckPriv()}
     *
     * @access  public
     * @param   integer     $priv       binary combinations of privileges
     * @return  boolean                 true if has required priviliges
     */
    function IsPrivileged($priv) {
        return parent::IsPrivileged($priv);
    }

    /**
     * Construct the object
     *
     * Supported construction options are;
     *   'id' - id of user to intially obtain
     *
     * @access  public
     * @param   array   $options    construction options
     * @return  void
     */
    function TNTGenericTrustee($options = null) {

        TNTTrustee::TNTTrustee();

        TNTLoader::RequireClass('TNTDbRec');

        $table = ($this->IsGroup() ? 'groups' : 'users');

        $rec =& new TNTDbRec($table, 'id');

        if (!empty($options['id'])) {
            if (!($this->_data = $rec->Obtain($options['id']))) {
                TNT::Abort('TNTGenericUser::TNTGenericUser() - failed to obtain specified user: '.$options['id'].' - '.$rec->Error());
            }
        } else {
            if (!($this->_data = $rec->Define())) {
                TNT::Abort('TNTGenericUser::TNTGenericUser() - failed to define a blank user: '.$options['id'].' - '.$rec->Error());
            }
        }

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTGenericTrustee() {
        destroy($this->_record);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
