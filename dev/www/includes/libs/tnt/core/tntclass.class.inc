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
 * Class Class 
 * ~~~~~~~~~~~
 *
 * This is a small class that all classes are derived from.
 * It supports the basic static class methods.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */
require_once('tnt.class.inc');


/*
 * Globals
 */

/*
 * Defines
 */
define('TNT_CLASS_PUBLIC',      0x0001);
define('TNT_CLASS_PRIVATE',     0x0002);
define('TNT_CLASS_RUNTIME',     0x0004);
define('TNT_CLASS_ALL',         0x000F);

/**
 * Class of static helper methods.
 *
 * This class is used as the basis of all main objects. It contains members to
 * obtain the objects class names and hierachy etc.
 *
 * All methods should be able to be called statically or dynamically.
 *
 * When creating classes all Methods and Member names should use Pascal case,
 * that is each word starts with an upper case letter and no underscores are
 * used to separate words. Private methods/members should be prefixed with an
 * underscore to distinguish them, especially as PHP 4 does not understand any
 * public/private notation at compile time. Non serializable, private runtime
 * members should start with a double underscore. These are runtime members that
 * are accessed via methods and usually used just to cache information derivable
 * from other members and methods which can be/should be re-evaluated when an
 * object is unserialized.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTClass {

    /** 
     * Get a null reference (used for example to pass null reference parameter)
     *
     * This is a static method.
     *
     * @access  public
     * @return  null                    reference to NULL value
     * @static
     */
    /*static*/ function &Null() {
        return null;
    }

    /**
     * Test to see if the named class has a specified method.
     *
     * Understands the method name 'constructor' and 'destructor' to
     * mean the constructor and destructor methods with use the class name.
     *
     * The class argument can be an object in which case the class of the
     * object is used. This can be more efficient.
     *
     * @access  public
     * @param   string      $method     name of method to test for
     * @param   string      $class      name of class to test member exists for
     * @return  bool                    true if method exists for this object 
     * @static
     * @see     MemberExists
     */
    /*static*/ function MethodExists($method, $class) {
        if (is_object($class)) {
            // This is faster call than doing the in_array() lookup used below
            if (strcasecmp($method, 'constructor') == 0) {
                $method = get_class($class);
            } else if (strcasecmp($method, 'destructor') == 0) {
                $method = '_'.get_class($class);
            }
            return method_exists($class, $method);
        } else {
            // Get list of members available to an object of this class
            if (strcasecmp($method, 'constructor') == 0) {
                $method = $class;
            } else if (strcasecmp($method, 'destructor') == 0) {
                $method = '_'.$class;
            }
            $members = get_class_methods($class);
            return ($members ? in_array(strtolower($method), $members) : false);
        }
    }

    /**
     * Test to see if the class has a specified member variable.
     *
     * The class argument can be an object in which case the class of the
     * object is used. This can be more efficient.
     *
     * @access  public
     * @param   string      $member     name of member to test for
     * @param   string      $class      name of class to test member exists for
     * @return  bool                    true if member exists for this class 
     * @static
     * @see     MethodExists
     */
    /*static*/ function MemberExists($member, $class) {
        if (is_object($class)) {
            // This is faster call than doing the in_array() lookup used below
            if (isset($class[$member])) {
                return true;
            }
            // Not set just means that - dont mean member does exist
        }
        $members = TNTClass::MemberNames($class);
        return ($members ? in_array(strtolower($member), $members) : false);
    }

    /**
     * Return an array of all the class members and their default values
     *
     * The '$which' parameter can be used to specify which type of member
     * varaibles to include in the result, selecting from 'public', 'private'
     * and 'runtime' classifications.
     *
     * NB: Although 'runtime' members are also 'private' members to return them
     * you must use the TNT_CLASS_RUNTIME flag.
     *
     * The class argument can be an object in which case the class of the
     * object is used. This can be more efficient.
     *
     * @access  public
     * @param   string      $class      name of class to get members for
     * @param   integer     $which      bit mask of which members to return
     * @return  array                   associative array of member variables
     * @static
     */
    /*static*/ function Members($class, $which = TNT_CLASS_ALL) {
        if (is_object($class)) {
            $class = get_class($class);
        }

        // Method result is cached as class definitions can't change
        $ret =& $GLOBALS['tnt']['tntclass'][$class]['schema'][$which];

        if (!isset($ret)) {
            if ($which == TNT_CLASS_ALL) {
                $ret = get_class_vars($class);
            } else {
                $ret = array();
                foreach (get_class_vars($class) as $name => $value) {
                    if ((($which & TNT_CLASS_RUNTIME) && $name{0} == '_' && $name{1} == '_') ||
                        (($which & TNT_CLASS_PRIVATE) && $name{0} == '_' && $name{1} != '_') ||
                        (($which & TNT_CLASS_PUBLIC) && $name{0} != '_')) {
                        $ret[$name] = $value;
                    }
                }
            }
        }
    
        return $ret;
    }

    /**
     * Return an array of all the class members names.
     *
     * The '$which' parameter can be used to specify which type of member
     * varaibles to include in the result, selecting from 'public', 'private'
     * and 'runtime' classifications.
     *
     * NB: Although 'runtime' members are also 'private' members to return them
     * you must use the TNT_CLASS_RUNTIME flag.
     *
     * The class argument can be an object in which case the class of the
     * object is used. This can be more efficient.
     *
     * @access  public
     * @param   string      $class      name of class to get names for
     * @param   integer     $which      bit mask of which members to return
     * @return  array                   array of method names
     * @static
     * @see     MethodNames, MemberExists
     */
    /*static*/ function MemberNames($class, $which = TNT_CLASS_ALL) {
        if (is_object($class)) {
            $class = get_class($class);
        }

        // Method result is cached as class definitions can't change
        $ret =& $GLOBALS['tnt']['tntclass'][$class]['members'][$which];

        if (!isset($ret)) {
            if ($which == TNT_CLASS_ALL) {
                $ret = array_keys(get_class_vars($class));
            } else {
                $ret = array();
                foreach (get_class_vars($class) as $name => $value) {
                    if ((($which & TNT_CLASS_RUNTIME) && $name{0} == '_' && $name{1} == '_') ||
                        (($which & TNT_CLASS_PRIVATE) && $name{0} == '_' && $name{1} != '_') ||
                        (($which & TNT_CLASS_PUBLIC) && $name{0} != '_')) {
                        $ret[] = $name;
                    }
                }
            }
        }

        return $ret;
    }

    /**
     * Return an array of all the class method names.
     *
     * The '$which' parameter can be used to specify which type of methods
     * to include in the result, selecting from 'public', 'private' and
     * 'runtime' classifications.
     *
     * NB: Although 'runtime' methods are also 'private' methods to return them
     * you must use the TNT_CLASS_RUNTIME flag.
     *
     * The class argument can be an object in which case the class of the
     * object is used. This can be more efficient.
     *
     * @access  public
     * @param   string      $class      name of class to get names for
     * @param   integer     $which      bit mask of which members to return
     * @return  array                   array of method names
     * @static
     * @see     MemberNames, MethodExists
     */
    /*static*/ function MethodNames($class, $which = TNT_CLASS_ALL) {
        if (is_object($class)) {
            $class = get_class($class);
        }

        // Method result is cached as class definitions can't change
        $ret =& $GLOBALS['tnt']['tntclass'][$class]['methods'][$which];

        if (!isset($ret)) {
            if ($which == TNT_CLASS_ALL) {
                $ret = get_class_methods($class);
            } else {
                $ret = array();
                foreach (get_class_methods($class) as $name) {
                    if ((($which & TNT_CLASS_RUNTIME) && $name{0} == '_' && $name{1} == '_') ||
                        (($which & TNT_CLASS_PRIVATE) && $name{0} == '_' && $name{1} != '_') ||
                        (($which & TNT_CLASS_PUBLIC) && $name{0} != '_')) {
                        $ret[] = $name;
                    }
                }
            }
        }

        return $ret;
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
