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
 * Generic Group Class
 * ~~~~~~~~~~~~~~~~~~~
 *
 * This class represents a generic trustee group.
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
 * Class to represent a generic Group type trustee.
 *
 * It is intended that any application built on this framework would either
 * derive from this class or reimplement it.
 *
 * The application INI uses the variable 'ClassNames.TNTGenericGroup' to know
 * the name of the class to instatiate for a GenericGroup when required.
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
class TNTGenericGroup extends TNTGenericTrustee {

    /*
     * Return the type of the trustee.
     *
     * @access  public
     * @return  string                  type string
     */
    function Type() {
        return 'group';
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
        return true;
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
     * Method always returns NULL as Group type trustees can not be
     * stored.
     *
     * @access  public
     * @param   string      $reason     reason for storing the trustee
     * @return  mixed[]                 always NULL
     * @see     Restore()
     */
    function Store($reason) {
        return null;
    }

    /*
     * Method always returns NULL as Group type trustees can not be
     * stored.
     *
     * @access  public
     * @param   string      $reason     reason for storing the trustee
     * @return  mixed       $data       data required to restore this trustee
     * @return  TNTObject               always NULL
     * @see     Store()
     */
    function &Restore($reason, $data) {
        return null;
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
     * Method always returns false as Group type trustees can not be
     * authenticated.
     *
     * @access  public
     * @return  boolean                 always false
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
     *   'id' - id of group to intially obtain
     *
     * @access  public
     * @param   array   $options    construction options
     * @return  void
     */
    function TNTGenericGroup($options = null) {

        TNTGenericTrustee::TNTGenericTrustee($options);

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTGenericGroup() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
