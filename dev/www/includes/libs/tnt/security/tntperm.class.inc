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
 * Permissions Class
 * ~~~~~~~~~~~~~~~~~
 *
 * This is a set of classes to handle permission allocation via
 * access control lists. 
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
$tnt['_tntperm'] = null;

/*
 * Defines
 */

// Generic object permissions
define('TNT_PERM_READ',                 0x00000001);
define('TNT_PERM_MODIFY',               0x00000002);
define('TNT_PERM_DELETE',               0x00000004);
define('TNT_PERM_CREATE',               0x00000008);

define('TNT_PERM_COMMENT',              0x00000100);
define('TNT_PERM_MODERATE',             0x00000200);

define('TNT_PERM_CHACL',                0x00000010);
define('TNT_PERM_CHOWN',                0x00000020);

define('TNT_PERM_VIEW',                 TNT_PERM_READ);

// Mapping of above to Applications
define('TNT_PERM_APP_EXECUTE',          TNT_PERM_READ);
define('TNT_PERM_APP_ADMINISTER',       TNT_PERM_MODIFY);
define('TNT_PERM_APP_UNINSTALL',        TNT_PERM_DELETE);
define('TNT_PERM_APP_INSTALL',          TNT_PERM_CREATE);

// Masks between Generic and Specific parts of the permissions
define('TNT_PERM_NONE',                 0x00000000);
define('TNT_PERM_ALL',                  0xFFFFFFFF);
define('TNT_PERM_GENERICMASK',          0x0000FFFF);
define('TNT_PERM_SPECIFICMASK',         0xFFFF0000);

// Size (in bits) of the Generic and Specific parts of the permissions
define('TNT_PERM_ALLSIZE',              32);
define('TNT_PERM_GENERICSIZE',          16);
define('TNT_PERM_SPECIFICSIZE',         16);

// Generic Privileges
define('TNT_PERM_PRIV_PROUSER',         0x40000000);
define('TNT_PERM_PRIV_SUPERUSER',       0x80000000);


/**
 * Class to handle permissions (Access Control Lists)
 *
 * It is also a singleton class and can be accessed via the static
 * method {@link TNTPerm::Singleton()}.
 *
 * Permissions are specified using a method derived from the
 *   "Linux trustees (ACL) project" (http://trustees.sourceforge.net/,
 *   http://www.linuxsecurity.com/resource_files/host_security/trustees-quickstart.html)
 *
 * The permissions model knows nothing about resource ownership, access to a
 * resource is independant of ownership.
 *
 * How the permissions are calculated
 * In short: 
 *   + Deny if the access is explicitly denied 
 *   + Grant if the access is explicitly granted 
 *   + Deny by default 
 *
 * The permission to access a resource is calculated using the following
 * algorithm: 
 *
 * Set initial deny mask, inherit mask and allow mask to 0. 
 * Starting from root of the resource heirachy check all the parents of the
 * resource and the resource itself and find trustee objects applicable, and
 * set the rights in trustee mask in the permission mask applicable. 
 * At each level the inherit mask states what allow permissions are inheritable.
 *
 * After the masks for file name is calculated: 
 * If at least one of the access modes requested is denied, deny the access. 
 * If all the access flags requested set in allow mask, grant the access 
 * Otherwise, deny 
 *
 * Trustees are normally specified in the format
 *    <type>:<name>
 *
 * Trustees can be wildcard by specifying '*' for the name. When this happens
 * the ACL entry applies to all trustees of the given type.
 *
 * The following special trustee DN names can be used when specifying the DN
 * the permissions are for;
 *     + 'anonymous' applies when no trustee in logged in
 *     + 'everyone' applies to all trustees
 *
 * Resources are normally specified in the format
 *   <type>:[<path>]
 *
 * Resources can be grouped by types in the same was as trustees.
 *
 * Permissions is a 32 bit field, the first 16 bits are reserved for the
 * following permissions. The next 16 bits are resource specific.
 *
 * There are different permissions depending on the type of resource.
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTPerm extends TNTObject {

    /**
     * Reference to open database to use
     * @var     TNTDatabase&            reference to database object
     * @access  private
     */
    var $_db = null;

    /**
     * Cached ACLs
     * @var     array                   array of cached ACL's indexed by SID
     * @access  private
     */
    var $_acls = null;

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
     * @return  TNTSession&         reference to global TNTSession object
     * @static
     */
    /*static*/ function &Singleton() {
        return TNTObject::Singleton(__CLASS__);
    }

    /**
     * Tidy up expired permissions
     *
     * @access  public
     * @return  void
     */
    function Tidy() {
    
        static $last_done = 0;
    
        $now = time();
    
        if ($now > $last_done + 60) {
            // Disable any expired permissions (they are not deleted)
            $sql = "UPDATE %perms% SET disabled=1 WHERE (etime > 0 AND etime <= $now)";
            $this->_db->Query($sql);
    
            $last_done = $now;
        }

    }

    /*
     * Return the ACL for a given trustee.
     *
     * A trustee is an object implementing the trustee interface.
     *
     * If a null trustee is given then the ACL list for 'anonymous' is
     * returned.
     *
     * A null value returned means there was an error, whilst an empty array
     * means no ACLs are defined for this trustee.
     *
     * @access  public
     * @param   TNTObject   $trustee    trustee details (null for current user)
     * @return  array                   permissions principle has to resource
     */
    function TrusteeACL(&$trustee) {

        $ret = null;

        // Start error section
        do {
            $this->Tidy();

            if (is_null($trustee)) {
                $dn = 'anonymous';
            } else {
                if (!implements($trustee, 'tnttrustee')) {
                    $this->_SetError('unsupported trustee: ', $trustee);
                    break;
               }
    
               // Trustees is an array of 'type','name' pairs
               $dn = $trustee->DN();
            }

            // Can we use a cached version
            if ($refresh || !isset($this->_acls[$dn])) {
                $sql_in = array();
                if (is_null($trustee)) {
                    $sql_in[] = $this->_db->QuoteString($dn);
                } else {
                    $dns = (array)$trustee->Groups();
                    array_push($dns, $dn);

                    $sql_in = null;
                    foreach ($dns as $dn) {
                        list($type,$name) = explode(':',$dn);
                        $types[$type]++;
                        if (!isset($sql_in[$type])) {
                            $sql_in[$type] = $this->_db->QuoteString("{$type}:*"); 
                        }
                        $sql_in[$dn] = $this->_db->QuoteString($dn); 
                    }
                }

                $sql_in[] = 'everyone';

                $sql_in = join(',', $sql_in);
                $now = (int)time();

                $result = $this->_db->Query("SELECT * FROM %perms% WHERE !disabled AND (stime = 0 OR stime >= $now) AND ((invert = 0 AND trustee IN ($sql_in)) OR (invert = 1 AND trustee NOT IN ($sql_in)))");

                if (!$result) {
                    // Database error - deny access
                    $this->_SetError('can\'t read ACL from database', $this->_db->_GetError());
                    break;
                }

                $acl = array();
                while ($data = $result->FetchArray(TNT_FETCH_ASSOC)) {
                    $acl[] = $data;
                }
    
                $this->_acls[$dn] = $acl;
            }

            $ret = &$this->_acls[$dn];

        } while (false);
        // End error section
    
        return $ret;
    }

    /*
     * Return the permissions a given user has to a resource.
     *
     * If a null trustee is given then the ACL list for 'anonymous' is
     * returned.
     *
     * @access  public
     * @param   TNTObject   $trustee    reference to trustee object
     * @param   string      $resource   absolute path to resource
     * @return  int                     permissions principle has to resource
     */
    function Permissions(&$trustee, $resource) {

        $ret = 0;

        // Start error section
        do {
            $this->Tidy();

            if (!is_string($resource)) {
                $this->_SetError('unsupported resource: ', $resource);
                break;
            }

            if (!implements($trustee, 'tnttrustee')) {
                $this->_SetError('unsupported trustee: ', $trustee);
                break;
            }

            if (strncasecmp($resource, 'priv:', 5) != 0) {
                // If not calculating privileges then a superuser can do
                // anything effectively by passing all permissions checking
                if ($this->CheckPriv(TNT_PERM_PRIV_SUPERUSER, $trustee)) {
                    return TNT_PERM_ALL;
                }
            }

            $acl =& $this->TrusteeACL($trustee);

            // If a user has no ACLs then effectively he has no permissions
            // to any resource.
            if (($ma = count($acl)) == 0) {
                // No permissions
                $ret = TNT_PERM_NONE;
                break;
            }
    
            // We build up the permissions for the specified resource by
            // traversing the resource heirachy adding together all the allows
            // and then removing the denies. A deny anywhere in the tree will
            // deny that permission even if allowed elsewhere (an allow can not
            // override a deny).
            $p = array('allow' => 0, 'deny' => 0);
            $c = explode('/', $resource);
            $mc = count($c);
            for ($i = 0, $t = ''; $i < $mc; $i++) {
                $t .= $c[$i].(($i < ($mc - 1)) ? '/' : '');
                $sp = array('allow' => 0, 'mask' => 0, 'deny' => 0);
                for ($a = 0; $a < $ma; $a++) {
                    $e = &$acl[$a];
                    if (empty($e['resource']) && $i != 0) {
                        continue;
                    }
                    if (empty($e['resource']) ||
                        ($e['resource'][0] == '?' && preg_match($e['resource'], $t)) ||
                        (strcasecmp($e['resource'], $t) == 0)) {
                        $sp[$e['type']] |= (int)$e['perms'];
                    }
                }
        
                $p['allow'] = (($p['allow'] & ~$sp['mask']) | $sp['allow']);
                $p['deny'] |= $sp['deny'];
            }
    
            $ret = ($p['allow'] & ~$p['deny']);

        } while (false);
        // End error section
    
        return $ret;
    }
    
    /*
     * Check if the trustee has the permissions on the resource.
     *
     * If a null trustee is given then the ACL list for 'anonymous' is
     * returned.
     *
     * @access  public
     * @param   TNTObject   $trustee    reference to trustee object
     * @param   string      $resource   absolute path to resource
     * @param   int         $perm       desired permissions
     * @return  boolean                 true if all permissions granted
     */
    function CheckPerm(&$trustee, $resource, $perm) {
        $permissions = $this->Permissions($trustee, $resource);
        return (($permissions & $perm) == $perm);
    }
    
    /*
     * Return the privileges a given trustee has.
     *
     * This is a short cut to the {@link Permissions()} function with the
     * resource string 'priv:/<module>'.
     *
     * If a null trustee is given then the ACL list for 'anonymous' is
     * returned.
     *
     * @access  public
     * @param   TNTObject   $trustee    reference to trustee object
     * @return  int                     permissions security id has to $resource
     */
    function Privileges(&$trustee) {
        $resource = 'priv:/'.($tnt['module'] ? $tnt['module']->code : '');
        return $this->Permissions($trustee, $resource);
    }
    
    /*
     * Check if the trustee has the privileges.
     *
     * If a null trustee is given then the ACL list for 'anonymous' is
     * returned.
     *
     * @access  public
     * @param   TNTObject   $trustee    reference to trustee object
     * @param   int         $priv       desired privileges
     * @return  boolean                 true if all privileges granted
     */
    function CheckPriv(&$trustee, $priv) {
        $privileges = $this->Privileges($trustee);
        return (($privileges & $priv) == $priv);
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
    function TNTPerm($options = null) {

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
    function _TNTPerm() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
