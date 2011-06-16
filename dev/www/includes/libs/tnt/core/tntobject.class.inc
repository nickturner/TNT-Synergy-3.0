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
 * Object Class 
 * ~~~~~~~~~~~~
 *
 * This is a small class that all objects are derived from.
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
TNTLoader::RequireClass('TNTClass');

/*
 * Globals
 */
global $tnt;
$tnt['_objects'] = array();
$tnt['_singletons'] = array();
$tnt['_objects_idx'] = 0;
$tnt['_class_hooks'] = null;
$tnt['_class_members'] = null;

/*
 * Defines
 */
define('TNT_OBJECT_HOOKS_PRE', 'pre');
define('TNT_OBJECT_HOOKS_PST', 'pst');

/**
 * Class to represent an object
 *
 * This class is used as the basis of all main objects. It contains members to
 * obtain the objects class names and hierachy etc.
 *
 * The object supports,
 *
 *      Conversion
 *          - {@link TNTObject::Cast()}: convert one type of object to another
 *          - {@link TNTObject::ToString()}: equivalent to Cast('string')
 *          - {@link TNTObject::ToArray()}: equivalent to Cast('array')
 *
 *      Cloning
 *          - {@link TNTObject::Clone()}: create a clone of the object
 *
 *      Comparison
 *          - {@link TNTObject::IsReference()}: compare one object with another
 *          - {@link TNTObject::IsEqual()}: compare one object with another
 *
 *      Serialization
 *          - {@link TNTObject::Serialize()}: save object to string
 *          - {@link TNTObject::UnSerialize()}: create an object from string
 *
 *      Error handling
 *         - {@link TNTObject::_SetError()}: set up an error message
 *         - {@link TNTObject::_GetError()}: set up an error message
 *         - {@link TNTObject::Error()}: return last error message
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTObject extends TNTClass {

    /**
     * Unique identification name for this object
     * @var     string                  identification name
     * @access  public
     */
    var $id = null;

    /**
     * Current error stack
     * @var     string[]                description of heirachy of errors
     * @access  private
     */
    var $_error = null;

    /**
     * Array on interface names this call implements. This should be updated
     * by a derived class if it implements a known interface. Any interface
     * class in the objects heirachy need not be included in the array as the
     * heirachy is checked before this array is checked.
     *
     * @access  public
     * @var     string[]                array of interface class names
     * @access  private
     */
    var $_implements = null;

    /**
     * Index to this object in the object table (non serializable)
     * @var     integer                 idx
     * @access  private
     */
    var $__idx = null;

    /**
     * Internally cached array of property names supported by {@link Set()}
     * @var     string[]                property names
     * @access  private
     */
    var $__set = null;

    /**
     * Internally cached array of property names supported by {@link Get()}
     * @var     string[]                property names
     * @access  private
     */
    var $__get = null;

    /**
     * Internal flag used by derived classes to indicate this object is a
     * singleton object.
     * @var     string                  name of singleton class
     * @access  private
     * @see     Singleton()
     */
    var $__singleton = null;

    /** 
     * Return a singleton object of the given class.
     *
     * A singleton object is one where a single instance of the object is
     * available. Calling this method will return that single instance.
     *
     * If the class is marked as a singleton class then the constructor will
     * only allow a single instance of the object to be created. This single
     * instance can either be created using the PHP new operator or by calling
     * this method for the first time. Future calls to this method will always
     * return that single instance.
     *
     * If the object is not marked as a singleton then this method will create
     * a single instance of the object when first called and subsequent calls
     * will return that single instance. In this instance another instance of
     * the object can be created normally using the PHP new operator.
     *
     * If this method has to create the singleton then it calls the constructor
     * with no arguments, therefore the object must support this.
     *
     * To mark an object as a singleton then set the __singleton private member
     * to the name of the singleton class.
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $class      name of class to get names for
     * @return  object&                 reference to singleton object
     * @static
     */
    /*static*/ function &Singleton($class) {
        global $tnt;

        $class = strtolower($class);

        //tnt_dump(debug_backtrace());

        if (empty($tnt['_singletons'][$class])) {
            // Get the real class name we were called by -
            // this is the one to create if required.
            $bt = debug_backtrace();
            if (!isset($bt[1]['class'])) {
                TNT::Abort('TNTObject::Singleton() - can not determine the class of the method that called us');
            }

            $real_class = $bt[1]['class'];

            $obj =& new $real_class();

            if (!is_a($obj, $class)) {
                TNT::Abort("TNTObject::Singleton() - real class ($real_class) is not a subclass of ($class)");
            }
            if (isset($obj->__singleton) && $obj->__singleton != $class) {
                TNT::Abort('TNTObject::Singleton() - singleton class definition mismatch');
            }
            if (empty($tnt['_singletons'][$class])) {
                $tnt['_singletons'][$class] =& $obj;
            }
        }

        // Mark this instance as a singleton
        $tnt['_singletons'][$class]->__singleton = $class;

        return $tnt['_singletons'][$class];
    }

    /** 
     * Get a reference to an object given its instance identifier
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $idx        object instance identifer
     * @return  object                  reference to TNTObject with given id
     * @static
     */
    /*static*/ function &Instance($idx) {
        global $tnt;
        if (isset($tnt['_objects'][$idx])) {
            return $tnt['_objects'][$idx];
        } else {
            return null;
        }
    }

    /** 
     * Get a reference to an object given its id
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $id         object id
     * @return  object                  reference to TNTObject with given id
     * @static
     */
    /*static*/ function &Object($id) {
        global $tnt;
        $m = count($tnt['_objects']);
        for ($i = 0; $i < $m; $i++) {
            if ($tnt['_objects'][$i]->id == $id) {
                return $tnt['_objects'][$i];
            }
        }
        return null;
    }

    /** 
     * Get a reference to the current object.
     *
     * This is useful where an object has been passed by copy and the
     * actual original reference is required.
     *
     * @access  public
     * @return  object                  reference to self TNTObject
     */
    function &Self() {
        return TNTObject::Instance($this->__idx);
    }

    /**
     * Add an error to the error stack (recursively).
     *
     * Helper method for {@link _SetError()}.
     *
     * @access  protected
     * @param   mixed       $e          error message/array/object
     * @return  void
     */
    function _AddError($e) {
        if (is_scalar($e)) {
            $this->_error[] = $e;
        } else if (is_array($e)) {
            foreach ($e as $_e) {
                $this->_AddError($_e);
            }
        } else if (method_exists($e, '_GetError')) {
            $this->_error = array_merge($this->_error, $e->_GetError());
        }
    }

    /**
     * Set the error stack.
     *
     * Any existing error stack is cleared and then the methods arguments are
     * added to the error stack in order depending on their type as follows;
     *      'string'    - add error string to stack
     *      'array[]'   - add each item in the array to stack depending on type
     *      'TNTObject' - add the errors in that object to the stack
     *
     * The errors are added in a first in first out [FIFO] order, that is the
     * first argument is the first to be displayed etc.
     *
     * @access  protected
     * @param   mixed       $errors     error messages/arrays/objects
     * @return  void
     */
    function _SetError() {
        $argc = func_num_args();
        $argv = func_get_args();

        $this->_error = array();

        for ($i = 0; $i < $argc; $i++) {
            $this->_AddError($argv[$i]);
        }

        if (!$this->_error) {
            $this->_error = null;
        }
    }

    /**
     * Get the error stack
     *
     * @access  protected
     * @return  string[]                error stack
     */
    function _GetError() {
        return $this->_error;
    }

    /**
     * PHP Callback function.
     *
     * This is the PHP callback function. It calls the callback method
     * {@link TNTObject::_Serialize()} which can be overriden by derived
     * classes if required.
     *
     * NB: Note the double underscore - this is because this is an internal
     * PHP virtual function.
     *
     * This function will call the {@link TNTObject::_Serialize()} method to
     * get a list of the member variables that need to be serialized along as
     * part of the object. If a null list is provided than all variables are
     * serialized (apart from those starting with a double underscore), if an
     * empty list is provided then no variables are serialized.
     *
     * NB: The default list of members of each class used when a null list is
     * provided is cached for efficiency. If an 'expando' class is used in
     * which the members my differ between objects of that class then they
     * must override the {@link TNTObject::_Serialize()} method to return the
     * actual list of member names to serialize.
     *
     * @access  private
     * @return  array                   array of member names to serialize
     */
    function __Sleep() {
        // Get list of members all non runtime members
        $members = TNTClass::MemberNames(get_class($this),
            TNT_CLASS_PUBLIC|TNT_CLASS_PRIVATE);

        // Get a list of members not to serialize
        $not_members = $this->_Serialize();

        if (!$not_members) {
            $members = array_diff($members, $not_members);
        }

        return $members;
    }

    /**
     * PHP Callback function.
     *
     * This is the PHP callback function. It calls the callback method
     * {@link TNTObject::_UnSerialize()} which can be overriden by derived
     * classes if required.
     *
     * NB: Note the double underscore - this is because this is an internal
     * PHP virtual function.
     *
     * This method will fixup the objects instance id so that it references
     * the now new copy of the object before calling the {@link _UnSerialize()}
     * method. However the objects id will remain the same - this may result
     * in two objects with the same id if the previous object was not deleted
     * after serilization.
     *
     * @access  private
     * @return  void
     */
    function __WakeUp() {
        // Add this object to our global list of objects
        global $tnt;
        $this->__idx = $tnt['_objects_idx']++;
        $tnt['_objects'][$this->__idx] = &$this;

        // Call the _UnSerialize callback method
        $this->_UnSerialize();
    }

    /** 
     * Get the instance id of the object
     *
     * @access  public
     * @return  integer                 instance identifier for the object
     */
    function InstanceId() {
        return $this->__idx;
    }

    /**
     * Get the name of the class (will be in lowercase)
     *
     * @access  public
     * @return  string                  PHP class name
     */
    function ClassName() {
        return get_class($this);
    }

    /**
     * Get the name of the parent class (will be in lowercase)
     *
     * @access  public
     * @return  string                  PHP parent class name
     */
    function ParentClassName() {
        return get_parent_class($this);
    }

    /**
     * Get the array of class names in the class hierachy
     *
     * Returns an array of class names in the current object hierachy with
     * the object class first (index 0) moving up to the superclass last.
     *
     * Requires PHP version >= 4.0.5.
     *
     * @access  public
     * @return  array                   array of class names in the hierachy
     */
    function HierachyClassNames() {
        static $_hierachies = null;
        $class = get_class($this);
        $base = $class;
        if (!isset($_hierachies[$base])) {
            do {
                $_hierachies[$base][] = $class;
            } while ($class = get_parent_class($class));
        }

        return $_hierachies[$base];
    }

    /**
     * Get a name of a class from the hierachy
     *
     * Given a class name and a position the function will return the class
     * in the given position relative to the given class in the class heirachy.
     *
     * For example if the hierachy is 'salmon,fish,animal,life' and then,
     *   HierachyClassName('animal', -1) == 'fish'; 
     *   HierachyClassName('animal', +2) == null; 
     *   HierachyClassName('life', -1) == 'animal';
     *   HierachyClassName('life', +2) == null;
     *
     * If base is not specified (empty or null) then uses the current class name
     * (which is the last in the hierachy).
     *
     * Requires PHP version >= 4.0.5.
     *
     * @access  public
     * @param   string      $base       class name of use as base for offset
     * @param   integer     $offset     return class at this offset to the base
     * @return  string                  name of class in hierachy
     */
    function HierachyClassName($base, $offset) {
        $hierachy = $this->HierachyClassNames();

        $m = count($hierachy);
        $b = (!empty($base) ? array_search($base, $hierachy) : ($m - 1));
        if (!is_numeric($b)) {
            return null;
        }
        $b += $offset;
        if ($b < 0 || $b >= $m) {
            return null;
        }

        return $hierachy[$b];
    }

    /**
     * Test to see if object is of a given class (exact)
     *
     * Returns true if object is of the specified class. Object has to be
     * exactly of that class and not a derivative of it.
     *
     * @access  public
     * @param   string      $classname  name of class to test for
     * @return  bool                    true if object is of given class
     */
    function IsClassOf($classname) {
        return (get_class($this) == strtolower($classname));
    }

    /**
     * Test to see if object is derived from given class (exact)
     *
     * Returns true if object is derived from given class. Also if '$inclusive'
     * is true then returns true if object is the specified class.
     *
     * @access  public
     * @param   string      $classname  name of class to test for
     * @param   bool        $inclusive  include objects of exact class 
     * @return  bool                    true if object is of given class
     */
    function IsSubClassOf($classname, $inclusive = true) {
        $classname = strtolower($classname);
        return (is_subclass_of($this, $classname) ||
            ($inclusive && get_class($this) == $classname));
    }

    /**
     * Test to see if the object implements a given interface.
     *
     * An interface is a set of predefined methods. An interface is defined
     * that is not an actual class is declared using a "xxx.iface.inc" file
     * which is used purely for documentation and contain dummy methods.
     *
     * @access  public
     * @param   string      $iface      name of interface to if implements
     * @return  bool                    true if interface is implemented
     */
    function Implements($iface) {
        return (is_a($this, $iface) || ($this->_implements && in_array(strtolower($iface), $this->_implements)));
    }

    /**
     * Test to see if the object has a specified method.
     *
     * @access  public
     * @param   string      $method     name of method to test for
     * @return  bool                    true if method exists for this object 
     * @see     MemberExists
     */
    function MethodExists($method, $class = null) {
        return method_exists($this, $method);
    }

    /**
     * Return an array of all the objects method names
     *
     * @access  public
     * @param   boolean     $private    true to return private members as well
     * @param   boolean     $runtime    true to return runtime members as well
     * @return  array                   array of method names
     * @static
     * @see     MethodNames, MemberExists
     */
    function MethodNames($private = true, $runtime = false) {
          return TNTClass::MethodNames(null, $private, $runtime);
    }

    /**
     * Return an array of all the objects member names
     *
     * @access  public
     * @param   boolean     $private    true to return private members as well
     * @param   boolean     $runtime    true to return runtime members as well
     * @return  array                   array of method names
     * @static
     * @see     MemberNames, MethodExists
     */
    function MemberNames($private = true, $runtime = false) {
          return TNTClass::MemberNames(null, $private, $runtime);
    }

    /**
     * Test to see if the object has a specified member variable.
     *
     * Unlike the base {@link TNTClass::MemberExists()} this method will
     * check the actual object for the member as it may exist in the object
     * but not in the class definition as PHP objects are 'expando' objects
     * and can have members added at runtime.
     *
     * If a class name is specified then the above doesn't apply and the base
     * {@link TNTClass::MemberExists()} method is used.
     *
     * @access  public
     * @param   string      $member     name of member to test for
     * @param   string      $class      name of class to test member exists for
     * @return  bool                    true if member exists for this object 
     * @see     MethodExists
     */
    function MemberExists($member, $class = null) {
        if ($class) {
            return TNTClass::MemberExists($member, $class);
        }
        return isset($this->$member);
    }

    /**
     * Return an associative array of all the objects member variables.
     *
     * @access  public
     * @param   boolean     $private    true to return private members as well
     * @return  array                   associative array of member variables
     * @see     SetMembers, GetMember, SetMember
     */
    function GetMembers($private = true) {
        $members = array_keys(get_object_vars($this));
        $ret = array();
        foreach ($members as $member) {
            if ($private || $member{0} != '_') {
                $ret[$member] = $this->$member;
            }
        }
        return $ret;
    }

    /**
     * Set member variables from an associative array of variables
     *
     * @access  public
     * @param   array       $vars       associative array of member variables   
     * @param   boolean     $create     create the member if not already exist
     * @param   boolean     $private    set private members as well
     * @return  array                   associative array of member variables
     * @see     GetMembers, GetMember, SetMember
     */
    function SetMembers($vars, $create = false, $private = true) {
        if (!is_array($vars)) {
            TNT::Abort('TNTObject::SetMembers() - vars is not an array');
        }

        foreach ($vars as $member => $value) {
            if ($private || $member{0} != '_') {
                $this->SetMember($member, $value, $create);
            }
        }

        return $this->GetMembers($this);
    }

    /**
     * Return an associative array of references to all the objects member
     * variables.
     *
     * @access  public
     * @param   boolean     $private    true to return private members as well
     * @return  array                   associative array of member variables
     * @see     SetMembers, GetMember, SetMember
     */
    function GetMembersRef($private = true) {
        $members = array_keys(get_object_vars($this));
        $ret = array();
        foreach ($members as $member) {
            if ($private || $member{0} != '_') {
                $ret[$member] =& $this->$member;
            }
        }
        return $ret;
    }

    /**
     * Set member variables by reference from an associative array of variables
     *
     * @access  public
     * @param   array       $vars       associative array of member variables   
     * @param   bool        $create     create the member if not already exist
     * @param   boolean     $private    set private members as well
     * @return  array                   associative array of member variables
     * @see     GetMembers, GetMember, SetMember
     */
    function SetMembersRef($vars, $create = false, $private = true) {
        if (!is_array($vars)) {
            TNT::Abort('TNTObject::SetMembers() - vars is not an array');
        }
        foreach ($vars as $member => $value) {
            if ($private || $member{0} != '_') {
                $this->SetMemberRef($member, $vars[$member]);
            }
        }
        return $this->GetMembersRef($this);
    }

    /**
     * Return value of a specific member variable
     *
     * @access  public
     * @param   string      $member     name of member variable to return
     * @return  mixed                   reference to the member variable
     * @see     SetMember, GetMembers, SetMembers
     */
    function GetMember($member) {
        return (isset($this->$member) ? $this->$member : null);
    }

    /**
     * Return reference to a specific member variable
     *
     * @access  public
     * @param   string      $member     name of member variable to return
     * @return  mixed                   reference to the member variable
     * @see     SetMemberRef, GetMember
     */
    function &GetMemberRef($member) {
        return (isset($this->$member) ? $this->$member : null);
    }

    /**
     * Set a specific member variable
     *
     * @access  public
     * @param   string      $member     name of member variable to return
     * @param   mixed       $value      value to set the specified member to
     * @param   bool        $create     create the member if not already exist
     * @return  mixed                   the previous value (or null)
     * @see     GetMember, GetMembers, SetMembers
     */
    function SetMember($member, $value, $create = false) {
        $ret = @$this->$member;
        if ($create || isset($this->$member)) {
            $this->$member = $value;
        }
        return $ret;
    }

    /**
     * Set a specific member variable by reference
     *
     * @access  public
     * @param   string      $member     name of member variable to return
     * @param   mixed       $value      reference to value to set
     * @param   bool        $create     create the member if not already exist
     * @return  mixed                   the previous value (or null)
     * @see     GetMember, GetMembers, SetMembers
     */
    function &SetMemberRef($member, &$value, $create = false) {
        if (isset($this->$member)) {
            $ret =& $this->$member;
        } else {
            $ret = null;
        }

        if ($create || isset($this->$member)) {
            $this->$member =& $value;
        }

        return $ret;
    }

    /**
     * Set the object.
     *
     * Properties of an object can have Set methods associated with them. For
     * example to the property 'foo' should have a Set method called SetFoo()
     * which is called to set its value. The method should take at least one
     * argument which is the new value of the property and return a boolean
     * where true means the value was set ok. Other arguments can be used but
     * they must have default values.
     *
     * This method normally takes two arguments which are the name of a member
     * and its new value.  Alternatively this method can be called with a single
     * argument which is an associative array (or object) of 'member' => 'value'
     * pairs, in which case the method is called recursively for each pair in
     * the array/object.
     *
     * @access  public
     * @param   string      $name       member name
     * @param   mixed       $value      member value
     * @return  boolean                 true if all members could be set.
     * @see     Get
     */
    function Set() {
        if (func_num_args() == 1) {
            $a = func_get_arg(0);

            if (is_null($a)) {
                return true;
            }

            if (is_array($a)) {
                $ret = true;
                foreach ($a as $n => $v) {
                    if (!$this->Set($n , $a[$n])) {
                        $ret = false;
                    }
                }
                return $ret;
            }
            
            if (is_object($a)) {
                $ret = true;
                foreach ($a as $n => $v) {
                    if (!$this->Set($n , $a->$n)) {
                        $ret = false;
                    }
                }
                return $ret;
            }

            $this->_SetError('Not a valid property map: '.gettype($a));
            return false;
        }

        if (func_num_args() == 2) {
            $name = func_get_arg(0);
            $value = func_get_arg(1);
        
            if (!$this->__set || !in_array(strtolower($name), $this->__set)) {
                $this->_SetError('No such property: '.$name);
                return false;
            }

            $set = 'set'.$name;
            return $this->$set($value);
        }

        $this->_SetError('Invalid number of arguments');
        return false;
    }

    /**
     * Get the object.
     *
     * Properties of an object can have Get methods associated with them. For
     * example to the property 'foo' should have a Get method called GetFoo()
     * which is called to get its value. The method should take no arguments
     * and return the properties value.
     *
     * The method normally takes a single argument which is the name of a
     * property to get. Alternatively the method can be called with no arguments
     * in which case it will return an associative array containing all the
     * properties which have Get methods as 'name' => 'value' pairs (the name
     * will be the property name in lowercase).
     *
     * @access  public
     * @param   string      $name       property name
     * @return  mixed                   value of the property
     * @see     Set
     */
    function Get() {
        
        if (func_num_args() == 0) {
            $ret = array();

            if ($this->__get) {
                foreach ($this->__get as $n) {
                    $m = 'get'.$n;
                    $ret[$n] = $this->$m();
                }
            }

            return $ret;
        }

        if (func_num_args() == 1) {
            $name = func_get_arg(0);

            if (!$this->__get || !in_array(strtolower($name), $this->__get)) {
                $this->_SetError('No such property: '.$name);
                return false;
            }

            $get = 'get'.$name;
            return $this->$get();
        }

        $this->_SetError('Invalid number of arguments');
        return false;
    }

    /**
     * Check the object is valid for use.
     * (This checks the object is valid - doesn't check the data is valid)
     *
     * This method is hookable.
     *
     * @access  public
     * @return  boolean                 true if object is valid
     */
    function IsValid() {
        $ret = false;
        if (!is_a($this, 'tntobject')) {
            $this->_SetError('Not a valid object');
        } else {
            $ret = true;
        }

        return $ret;
    }

    /**
     * Clone this object
     *
     * A shallow close returns a new object which is a shallow copy of the
     * objects properties. Any properties that are references to other
     * variables, will remain references. If a __clone() method is defined,
     * then the newly created object's __clone() method will be called, to
     * allow any necessary properties that need to be changed.  
     *
     * A deep clone returns a new object which is a deep copy of the objects
     * properties. Any properties that are references to other variable, will
     * become references to clones of that variable. In this instance the
     * __clone() method will not be called.
     *
     * Shallow cloning is the same as the PHP 5 clone keyword.
     *
     * This method is hookable.
     *
     * @access  public
     * @param   boolean     $shallow    true to shallow clone, else deep
     * @return  TNTObject               reference to new clone
     */
    function &Clone($shallow = true) {
        if ($shallow) {
            $ret = $this;
            if (method_exists($ret, '__clone')) {
                $ret->__clone();
            }
        } else {
            $ret =& $this->UnSerialize($this->Serialize());
        }

        return $ret;
    }

    /**
     * Cast this object to another type
     *
     * Usually overridden to support more inteligent casting
     *
     * This method is hookable.
     *
     * @access  public
     * @param   string      $type       PHP type or class name to cast to
     * @return  mixed                   reference to object in given type
     */
    function &Cast($type) {
        $ret = null;
        $type = strtolower($type);

        switch ($type) {
            case 'string':
                $ret = var_info($this);;
                break;
            case 'int':
            case 'integer':
                $ret = $this->InstanceId();
                break;
            case 'bool':
            case 'boolean':
                $ret = $this->IsValid();
                break;
            case 'array':
                $ret = $this->GetMembersRef();
                break;
            case 'object':
                $ret =& $this;
                break;
            default:
                if (in_array($type, $this->HierachyClassNames())) {
                    $ret =& $this;
                    break;
                }
                $this->_SetError('can not cast to: ', $type);
                break;
        }

        return $ret;
    }

    /**
     * Return a string representation of this object
     *
     * Short cut to {@link TNTObject::Cast('string')}
     *
     * @access  public
     * @return  string                  string representation of object
     */
    function ToString() {
        return $this->Cast('string');
    }

    /**
     * Return a string representation of this object
     *
     * Short cut to {@link TNTObject::Cast('array')}
     *
     * @access  public
     * @return  string                  string representation of object
     */
    function ToArray() {
        return $this->Cast('array');
    }

    /**
     * Check if an object is a reference to this object.
     *
     * If it is a reference then the given object is a TNTObject and their
     * {@link InstanceId()} are the same
     *
     * @access  public
     * @param   TNTObject&  $object     reference to another object
     * @return  boolean                 true if object is a reference to this
     */
    function IsReference(&$object) {
        return (is_a($object, 'tntobject') &&
                ($object->InstanceId() == $this->InstanceId()));
    }

    /**
     * Check this object is equal to another object
     *
     * This method is hookable.
     *
     * @access  public
     * @param   object      $object     reference to another object
     * @return  boolean                 true if object is valid
     */
    function IsEqual(&$object) {
        $this->__isequal__ = true;

        $ret = is_equal($this, $object);

        unset($this->__isequal__);

        return $ret;
    }

    /**
     * Serialize the object
     *
     * This function returns the object as a character string.
     *
     * By default all non runtime member variables are serialized. Runtime
     * member variable, used mainly for caching method results between
     * successive calls are indicated by a double underscore prefix '__'.
     *
     * This method is hookable.
     *
     * @access  public
     * @return  string                  string of serialized object data
     * @see     UnSerialize()
     */
    function Serialize() {
        // Serialized objects are wrapped in an array to overcome having to
        // pass in a reference to the object to unserialize() when force pass
        // by reference is disabled.
        $ret = serialize(array(&$this));

        return $ret;
    }

    /**
     * _Serialize callback
     *
     * Occurs when the object is about to be put to be serialized.
     *
     * The function can return a list of member variable names that should NOT
     * be serialized with this object. This is useful if you have very large
     * objects which need not be saved completely.
     *
     * This base implementation returns NULL indicating that their are no
     * members that should not be serialized.
     *
     * NB: Runtime members (prefixed '__') are never serialized and need not
     * be specified by this method.
     *
     * NB: _Serialize() should not alter the object, close resources or
     * anything of that nature as the object that would invalidate the object
     * from use after serialization.
     *
     * This function is expected to be overridden by derived classes.
     *
     * @access  protected
     * @return  array                   names of members to serialise, or null
     */
    function _Serialize() {
        return TNTClass::MemberNames(get_class($this), TNT_CLASS_RUNTIME);
    }

    /**
     * Return a new object from the serialized data
     *
     * This function returns a new object constructed from the serialized data.
     *
     * It can be called statically in which case it constructs a temporary
     * object to do the unserialisation which is discarded at the end of
     * the method call.
     *
     * This method is hookable.
     *
     * @access  public
     * @static  (optional - will create temporary object to do work)
     * @param   string      $data       string of serialized object data
     * @return                          reference to unserialized object
     * @see     Serialize()
     */
    /*static*/ function &UnSerialize($data) {

        if (!isset($this) || !is_a($this, 'tntobject')) {
            $obj =& new TNTObject();
            $ret =  $obj->UnSerialize($data);
            destroy($obj);
            return $ret;
        }

        $a = unserialize($data);

        // Serialized objects are wrapped in an array to overcome having to
        // pass in a reference to the object to unserialize() when force pass
        // by reference is disabled.
        if (is_array($a) && count($a) == 1 && is_a($a[0], 'tntobject')) {
            $ret =& $a[0];
        } else {
            trigger_error('TNTObject::UnSerialize() - did not unserialize ok');
        }

        return $ret;
    }

    /**
     * _UnSerialize callback
     *
     * Occurs when the object is unserialized.
     *
     * The intended use of _UnSerialize() is to reconnect any resource handles
     * etc. used by the object.
     *
     * This function is expected to be overridden by derived classes.
     *
     * @access  protected
     * @return  void
     */
    function _UnSerialize() {
    }

    /**
     * Return the current errors generated.
     *
     * An error is not reset on successful calls, so testing this method for
     * an error string is not the way to test if an error occurred. The return
     * code from a method should indicate an error.
     *
     * The current error stack is returned with each message separated by
     * the '$sep' character which defaults to "\n". If $sep is set to null
     * then an error of errors is returned. The first item in the error is
     * the last error to be reported, etc.
     *
     * You can optionally clear the error stack after retrieving it.
     *
     * @access  public
     * @param   boolean     $clr        clear error after retrieving it
     * @param   string      $sep        error separation character
     * @return  string                  error message (or array if $sep is null)
     */
    function Error($clr = false, $sep = "\n") {

        $ret = (is_null($sep) ? $this->_error : ($this->_error ? join($sep, $this->_error) : ''));

        if ($clr) {
            $this->_error = null;
        }

        return $ret;
    }

    /**
     * Define a static class member
     *
     * PHP does not allow true static class members so these are created
     * external to the class using this method.
     *
     * When the object is constructed any members declared using this
     * method appear as genuine static object member variables. 
     *
     * Such member variables are shared amongst all instances of the object.
     *
     * NB: This sharing is done by assigning each member variable to be
     * a reference to the same shared value. Thus assigning the member
     * variable to a new reference will break this 'static' reference.
     *
     * Warning: Static class members are not assigned until the base
     * TNTObject constructor is called and thus can not be used in any
     * derived constructor. To use in a derived constructor call
     * {@link _InitStaticMembers()} before use.
     *
     * This is a static method.
     *
     * @access  public
     * @param   string      $class      name of class this member is for
     * @param   string      $name       name of member
     * @param   mixed       $value      initial value of the static member
     * @see     _InitStaticMember()
     * @static
     */
    /*static*/ function StaticMember($class, $name, $value = null) {
        global $tnt;
        unset($tnt['_static_class_members'][$class][$name]);
        $tnt['_static_class_members'][$class][$name] = $value;
    }

    /**
     * Initialise static class members.
     *
     * PHP does not allow true static class members so these are created
     * external to the class using the {@link StaticMember()} method.
     *
     * When the object is constructed this method makes any members declared
     * by {@link StaticMember()} appear as genuine object member variable.
     *
     * Such member variables are shared amongst all instances of the object.
     *
     * @access  public
     * @param   array       $data       assoc. array of data for the object
     * @see     StaticMember
     */
    function _InitStaticMembers() {
        if (!isset($this->__initStaticMembers)) {
            global $tnt;
            $class = get_class($this);
            if (isset($tnt['_static_class_members'][$class]) &&
                is_array($tnt['_static_class_members'][$class])) {
                foreach ($tnt['_static_class_members'][$class] as $n => $v) {
                    unset($this->$n);
                    $this->$n =& $tnt['_static_class_members'][$class][$n];
                }
            }
            $this->__initStaticMembers = true;
        }
    }

    /**
     * Construct the object
     *
     * This will initialise all member variables etc.
     *
     * By default we count the objects of each class type and set the id to
     * indicate the objects number. Derived objects that require the id to
     * remain constant across pages should set the id to a unique known value
     * and not rely on the default id as it may change depending on object
     * creation order.
     *
     * Note: When deriving from a TNTObject class the derived class should
     * call its immediate base (parent) classes constructor. This effectively
     * create a construction order which is reverse to other languages such as
     * C++, to minimise possible construction problems the class should call
     * the base classes constructor immediately before it constructs itself.
     *
     * This method is hookable.
     *
     * @access  public
     * @param   array       $data       assoc. array of data for the object
     * @see     SetMembers
     */
    function TNTObject() {
        TNT::Timestamp('TNTObject::TNTObject() - creating object');

        // If this object is a singleton then add to list of singletons
        if ($this->__singleton) {
            if (!is_a($this, $this->__singleton)) {
                TNT::Abort('TNTObject::TNTObject() - singleton flag must be the name of a class in this objects class hierachy');
            }
            if (!empty($tnt['_singletons'][$this->__singleton])) {
                TNT::Abort('TNTObject::TNTObject() - possible duplicate singleton creation detected');
            }

            $tnt['_singletons'][$this->__singleton] =& $this;
        }

        $this->_InitStaticMembers();

        static $object_counters = null;

        $classname = get_class($this);
        if (!isset($object_counters[$classname])) {
            $object_counters[$classname] = 0;
        }

        $this->id = $classname.'_'.$object_counters[$classname]++;

        // Finally push this object onto the end of the global list of objects.
        global $tnt;
        $this->__idx = $tnt['_objects_idx']++;
        $tnt['_objects'][$this->__idx] = &$this;

        // All methods that are not in this base class
        if (!isset($tnt['_object'][$classname]['methods'])) {

            $_m =& $tnt['_object'][$classname]['methods'];
            $_m = array('get' => array(), 'set' => array());

            //TNT::Timestamp('Calculating get/set methods');

            $methods = array_diff(get_class_methods(get_class($this)), get_class_methods(__CLASS__));

            // List of methods that can be used to get/set properties
            // Derived classes can remove method from this list if required.
            foreach ($methods as $method) {
                if (strlen($method) > 3) {
                    if (strncasecmp($method, 'get', 3) == 0) {
                        $_m['get'][] = strtolower(substr($method, 3));
                    } else if (strncasecmp($method, 'set', 3) == 0) {
                        $_m['set'][] = strtolower(substr($method, 3));
                    }
                }
            }

            //TNT::Timestamp('__get='.join(',', (array)$_m['get']));
            //TNT::Timestamp('__set='.join(',', (array)$_m['set']));
        }

        $this->__set = $tnt['_object'][$classname]['methods']['get'];
        $this->__get = $tnt['_object'][$classname]['methods']['set'];

        TNT::Timestamp("TNTObject::TNTObject() - created object number {$this->__idx} of class $classname. (total objects=".count($tnt['_objects']).')');
    }

    /**
     * Destruct the object.
     *
     * This will destruct the object and remove it from the internal list of
     * objects.
     *
     * Note: When deriving from a TNTObject class the derived class need not
     * call its immediate base (parent) classes destructor as this will be done
     * by the destroy() function when destroying the object. The order in which
     * such destructors are called is similar to other languages such as C++,
     * being from the top most derived class down to the bottom most base class.
     *
     * This method is hookable.
     *
     * @access  private
     * @hooks   pre,pst
     */
    function _TNTObject() {

        // If this object is a singleton then remove from list of singletons
        if ($this->__singleton) {
            if (!empty($tnt['_singletons'][$this->__singleton]) &&
                $tnt['_singletons'][$this->__is_singleton] == $this) {

                unset($tnt['_singletons'][$this->__singleton]);
                $tnt['_singletons'][$this->__singleton] = null;
            }
        }

        // Remove this object from our global list of objects. The __idx value
        // is cleared to indicate that the object is now invalid and should not
        // be used incase references to it still exist else where.
        global $tnt;
        if (!is_null($this->__idx) && isset($tnt['_objects'][$this->__idx])) {
            $idx = $this->__idx;
            $this->__idx = null;
            unset($tnt['_objects'][$idx]);
        }

    }
}

/**
 * Helper function to wrap the {@link TNTObject::Implements()} method.
 *
 * The function will check the passed value is a valid TNTObject and then
 * call the {@link TNTObject::Implements()} method.
 *
 * @access public
 * @param  object       $obj            reference to the object to check
 * @param  string       $iface          name of interface to check for
 * @return boolean                      true if the interface is implemented
 */
function implements(&$obj, $iface) {
    return (($obj && is_a($obj, 'tntobject')) ? $obj->Implements($iface) : false);
}

/**
 * Helper function to destruct an object.
 *
 * The function will call all the objects destructors from the superclass to
 * the base class.
 *
 * @access public
 * @param  object                       reference to the object to destruct
 */
function destroy(&$obj) {
    if (!$obj || !is_object($obj)) {
        return;
    }

    static $recursion = 0;
    $recursion++;
    $classname = get_class($obj);

    if (is_a($obj, 'tntobject')) {
        TNT::Timestamp("Destroying object number {$obj->__idx} of class $classname total objects = ".count($GLOBALS['tnt']['_objects']));
    }

    while ($classname) {
        $destructor = "_$classname";
        if (method_exists($obj, $destructor)) {
            $obj->$destructor();
        }
        $classname = get_parent_class($classname);
    }
    $obj = null;
    unset($obj);
    $recursion--;
}

/**
 * Helper function to call the destructors on all current objects when the
 * PHP application exits.
 *
 * @access  private
 */
function _TNTObject_CallDestructors() {
    
    global $tnt;
    if (isset($tnt['_objects']) && is_array($tnt['_objects']) &&
        count($tnt['_objects']))
    {
        while (count($tnt['_objects'])) {
            reset($tnt['_objects']);
            $idx = key($tnt['_objects']);

            destroy($tnt['_objects'][$idx]);

            // In-case the destructor didn't remove object from list
            unset($tnt['_objects'][$idx]);
        }
    }
}

// Ensure the destructors are called when the program exits
register_shutdown_function('_TNTObject_CallDestructors');

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
