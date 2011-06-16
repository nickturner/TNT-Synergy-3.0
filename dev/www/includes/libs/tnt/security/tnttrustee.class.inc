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
 * Trustee Interface Class
 * ~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class defines the TNTTrustee interface.
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
define('TNT_TRUSTEE_LOGIN',     'login');
define('TNT_TRUSTEE_REQUEST',   'autologin');
define('TNT_TRUSTEE_SESSION',   'session');
define('TNT_TRUSTEE_LOGOUT',    'logout');

/**
 * Class to define the basic TNTTrustee interface. Classes can either derive
 * from this class or implement this interface.
 *
 * A Trustee object represents the security context of a trusted user on whose
 * behalf the code is running, including that user's identity and any parent
 * Trustees to which they belong.
 *
 * For an example of an object that implements this interface refer to
 * {@link TNTUser} or {@link TNTGroup}.
 *
 * The trustee model is heirachical in nature. Trustee's fall into two types,
 * 'users' and 'groups'. A 'group' trustee forms the branches of the heirachy
 * and can have children and a 'user' trustee forms the leaves and can not
 * have children.
 *
 * To further add complication 'user' type trustees can be a member of many
 * groups. That is the same can leaf can occur on many branches. 'group' type
 * trustee can only be a member of a single group.
 *
 * <pre>
 * For example;
 *
 *                (group:staff)
 *                      |
 *        +-------------+-------------+
 *        |                           |
 *  (group:admin)               (group:development)
 *        |                           |
 *   (user:jane)        +-------------+-------------+
 *                      |                           |
 *                (group:support)             (group:coders)
 *                      |                           |
 *              +-------+-------+           +-------+-------+
 *         (user:bob)      (user:joe)  (user:guy)      (user:zak)
 *
 * </pre>
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
class TNTTrustee extends TNTObject {

    /*
     * Return the Distinquished Name of the trustee.
     *
     * The DN is unique amongst all trustees. It has the format '<type>:<name>'.
     *
     * @access  public
     * @return  string                  unique DN
     */
    function DN() {
        return $this->Type().':'.$this->Name();
    }

    /*
     * Return the type of the trustee.
     *
     * This method should be overriden by the deriving class.
     *
     * @access  public
     * @return  string                  type string
     */
    function Type() {
        return 'unknown';
    }

    /*
     * Return the ID of the trustee.
     *
     * The ID is unique amongst all trustees of the same type and must be
     * a signed integer greater than zero.
     *
     * This method should be overriden by the deriving class.
     *
     * @access  public
     * @return  string                  unique id
     */
    function ID() {
        return 0;
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
     * This method should be overriden by the deriving class.
     *
     * @access  public
     * @return  string                  unique name
     */
    function Name() {
        return $this->ID();
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
     * This can either return a list of immediate groups or a complete list
     * of all the groups by traceing the trustee back to the root of the tree.
     *
     * The groups are returned as an array of DN strings.
     *
     * This method should be overriden by the deriving class.
     *
     * @access  public
     * @param   boolean     $complete   complete list required
     * @return  array                   array of parent group DNs
     */
    function Groups($complete = true) {
        return null;
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
        return array(
            '_className' => get_class($this),
            '_reason' => $reason,
            '_dn' => $this->DN());
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
        if ($this && !is_a($this, 'tnttrustee')) {
            return $this;
        }

        if (empty($data['_className']) ||
            empty($data['_reason']) ||
            empty($data['_dn'])) {
            TNT::Abort('TNTTrustee::Restore() - invalid data');
        }

        if ($data['_reason'] != $reason) {
            TNT::Abort('TNTTrustee::Restore() - trustee was not intended to be stored for this reason - it may not be possible to restore it.');
        }

        $className = $data['className'];

        $trustee =& new $className();

        $trustee->Restore();

        if ($trustee->DN() != $data['_dn']) {
            TNT::Abort('TNTTrustee::Restore() - trustee restored has a different DN to that stored.');
        }

        return $trustee;
    }

    /*
     * Authenticate the trustee using the data supplied.
     *
     * Different authentication reasons supported;
     *   + TNT_TRUSTEE_LOGIN - authenticate using data sent from a login form.
     *   + TNT_TRUSTEE_REQUEST - authenticate using data stored in the request.
     *   + TNT_TRUSTEE_SESSION - authenticate using data stored in the session.
     *   + TNT_TRUSTEE_LOGOUT - deauthenticate.
     *
     * Normally 'LOGIN' and 'REQUEST' do full login while 'SESSION' only need
     * do a quick integrety check. 'LOGOUT' is usually just for notification
     * if required.
     *
     * @access  public
     * @param   string      $reason     authentication reason
     * @param   mixed       $data       credentials
     * @return  boolean                 true if has been authenticated
     */
    function Authenticate($reason, $data) {
        return false;
    }

    /*
     * Check if the trustee is in a given group.
     *
     * @access  public
     * @param   string      $dn         DN of group to check for
     * @return  boolean                 true if in given group
     */
    function IsInGroup($dn) {
        return false;
    }

    /*
     * Check if the trustee has been authenticated.
     *
     * @access  public
     * @return  boolean                 true if has been authenticated
     */
    function IsAuthenticated() {
        return false;
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
        $perm =& TNTPerm::Singleton();
        return $perm->CheckPerm($this, $resource, $perm);
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
        $perm =& TNTPerm::Singleton();
        return $perm->CheckPriv($this, $priv);
    }

    /**
     * Construct the object
     *
     * @access  public
     * @return  void
     */
    function TNTTrustee() {

        TNTObject::TNTObject();
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTTrustee() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
