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
 * This class represents a generic trustee user.
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTGenericTrustee');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a generic User type trustee.
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
 */
class TNTGenericUser extends TNTGenericTrustee {

    /**
     * A value that indicates whether the user has been authenticated
     * @var     boolean                 true if the user has been authenticated
     * @access  private
     */
    var $_isAuthenticated = null;

    /*
     * Return the type of the trustee.
     *
     * @access  public
     * @return  string                  type string
     */
    function Type() {
        return 'user';
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
        return parent::ID();
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
        return parent::Name();
    }

    /*
     * Return true if the trustee is a 'group' type trustee as opposed to a
     * 'user' type trustee.
     *
     * Trustees of type 'group' can have child trustees but can only exist in
     * one place in the trustee tree.
     *
     * This method should be overriden by the deriving class.
     *
     * @access  public
     * @return  boolean                 true if 'group' type trustee.
     */
    function IsGroup() {
        return false;
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
        return parent::Groups($complete);
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

        $ret = parent::Store();

        $ret['type'] = $this->Type();
        $ret['id'] = $this->ID();

        if ($reason == TNT_TRUSTEE_SESSION) {
            // To reauthenticate a session we just store the PHP session id
            // to ensure the same session restores the user. We don't store
            // any credentials as we assume session data is secure and if it
            // says this user is trusted we believe it.
            $ret['sessid'] = SID;
        } else {
            // We store the users password in the data so we can check it
            // on restoration. If the user changes his password then he will
            // not be allowed back in until the new data is stored.
            $ret['encpwd'] = $this->GetField('encpwd');

            // This doesn't seem very secure as if someone hacks into the
            // database they can get the encrypted password and then set up
            // a fake request containing this password.
        }

        return $ret;
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
        if (!$this || is_a($this, 'tntgenericuser')) {
            return parent::Restore();
        }

        if (!$data['id']) {
            TNT::Abort('TNTGenericUser::Restore() - invalid data');
        }

        $ret = null;

        // Start error section
        do {

            // Obtain the correct user details
            if (!$this->_record->Obtain($data['id'])) {
                $this->_SetError("No such user ({$data['id']}) - user not restored.", $this->_record->_GetError());
                break;
            }

            // Authenticate the user
            if ($reason == TNT_TRUSTEE_SESSION) {
                // Check the session id's match
                if (@$ret['sessid'] != SID) {
                    $this->_SetError('Session ID mismatch - user not restored.');
                    break;
                }
            } else {
                // Check the encrypted passwords match
                if (strcmp(@$ret['encpwd'], $this->GetField('encpwd')) != 0) {
                    $this->_SetError('Password mismatch - user not restored.');
                    break;
                }
            }

            $this->_isAuthenticated = true;

            $ret =& $this;
        } while (false);
        // End error section

        return $ret;
    }

    /*
     * Check if the trustee is in a given group.
     *
     * @access  public
     * @param   string      $dn         DN of group to check for
     * @return  boolean                 true if in given group
     */
    function IsInGroup($dn) {
        return parent::IsInGroup($dn);
    }

    /*
     * Check if the trustee has been authenticated.
     *
     * @access  public
     * @return  boolean                 true if has been authenticated
     */
    function IsAuthenticated() {
        return $this->_isAuthenticated;
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
    function TNTGenericUser($options = null) {
        TNTGenericTrustee::TNTGenericTrustee($options);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTGenericUser() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
