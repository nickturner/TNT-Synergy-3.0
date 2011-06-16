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
 * Objects Class
 * ~~~~~~~~~~~~~
 *
 * This is a small class that manages a collection of objects.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
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
 * Class to manage a collection of objects.
 *
 * This class is used to manage a collection of objects, it derives from
 * TNTObject and contains an array of managed TNTObject derived objects.
 *
 * The collection can be readonly in which case the Add and Remove calls
 * will fail.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @extends TNTObject
 * @iface   TNTICollection
 */
class TNTObjects extends TNTObject {
    /**
     * Objects is read only
     * @var     boolean                 true if collection is readonly
     * @access  public
     */
    var $readOnly = false;

    /**
     * Items removed from the collection should be destroyed
     *
     * As PHP does not have destructors we have to call an objects destructor
     * when needed. As we don't know if removing a child in the collection
     * would effectively remove the last reference and should effectively call
     * the destructor we have to be told to do it.
     *
     * A normal case is that if this collection is the only place where the
     * objects are held then they should be destroyed when removed.
     *
     * If an object that is still referenced elsewhere is destroyed then the
     * other references to this object should be regarded as invalid, this can
     * be detected as the {@link TNTObject::InstanceId()} for the object will
     * return null. This is analogous to using a pointer to a deleted object in
     * 'C++' etc.
     *
     * @var     boolean                 true if items should be destroyed
     * @access  public
     */
    var $autoDestroy = true;

    /**
     * If set then all objects in the array must be derived from one of the
     * class names given in this list.
     * @var     string[]                array of class name(s) (optional)
     * @access  public
     */
    var $reqClasses = array('TNTObject');

    /**
     * Private array of objects
     * @var     TNTObject[]             array of references to objects
     * @access  protected
     */
    var $_objects = array();

    /**
     * Called after an object is added to the collection
     *
     * Base implementation does nothing
     *
     * @access  protected
     * @abstract
     * @param   TNTObject   $object     reference to the object added
     * @param   integer     $index      index at which the object was added
     * @return  void
     * @see     Add(), AddAt(), AddAll(), AddAllAt()
     */
    function _Added(&$object, $index) {
    }

    /**
     * Called after an object is removed from the collection
     *
     * Base implementation destroys the object if $this->autodestroy is true.
     *
     * @access  protected
     * @abstract
     * @param   TNTObject   $object     reference to the object removed
     * @param   integer     $index      index from which the object was removed
     * @return  void
     * @see     Remove(), RemoveAt(), RemoveAll(), RemoveAllAt()
     */
    function _Removed(&$object, $index) {
        if ($this->autoDestroy) {
            destroy($object);
        }
    }

    /**
     * Tests to see if the given object is one of the required classes.
     *
     * @access  public
     * @param   TNTObject&  $object     reference to the object
     * @return  boolean                 true if object is of required class
     */
    function IsReqClass(&$object) {
        return ($this->reqClasses ? is_kindof($object, $this->reqClasses) : true);
    }

    /**
     * Returns a reference to the current array of objects in the collection
     *
     * @access  public
     * @return  TNTObject[]             reference to an array of objects
     */
    function &Items() {
        return $this->_objects;
    }

    /**
     * Returns a reference to a specific object in the collection
     *
     * @access  public
     * @param   integer     $index      index of object to obtain
     * @return  TNTObject               reference to object at given index
     */
    function &Item($index) {

        $ret =& $this->Get($index);

        return $ret;
    }

    /**
     * Sets the object at the specified position
     *
     * NB: This will overwrite any existing object and not call RemoveAt() on it
     *
     * @access  public
     * @param   integer     $index      index of object to replace
     * @param   TNTObject   $object     reference to object to replace with
     * @return  TNTObject               reference to original object
     */
    function &Set($index, &$object) {

        $ret = null;

        if ($this->readOnly) {
            $this->_SetError('collection is read only');
        } else if ($index < 0 || $index >= count($this->_objects)) {
            $this->_SetError('index is out of range');
        } else {
            $ret =& $this->_objects[$index];

            $this->_objects[$index] =& $object;
        }

        return $ret;
    }

    /**
     * Gets the object at the specified position
     *
     * @access  public
     * @param   integer     $index      index of object to get
     * @return  TNTObject               reference to object, or null
     */
    function &Get($index) {

        $ret = null;

        if ($index < 0 || $index >= count($this->_objects)) {
            $this->_SetError('index is out of range');
        } else {
            $ret =& $this->_objects[$index];
        }

        return $ret;
    }

    /**
     * Adds an object to the array of child objects.
     *
     * Verifies that the object is of the required type and then add it to
     * the end of the internal array of objects.
     *
     * This calls the AddAll() method to actually do the work passing an array
     * with the object to be added as the only element.
     *
     * @access  public
     * @param   TNTObject   $object     reference to an object to add
     * @return  integer                 index of added object, or -1
     * @see     AddAt(), AddAll(), AddAllAt()
     */
    function Add(&$object) {

        $ret = null;

        // Start error section
        do {
            if ($this->readOnly) {
                $this->_SetError('collection is read only');
                break;
            }

            if (!$this->IsReqClass($object)) {
                $this->_SetError('object is not of the correct class: '.get_class($object));
                break;
            }

            $index = count($this->_objects);
            $this->_objects[$index] =& $object;
            $this->_Added($this->_objects[$index], $index);
            ++$index;

            $ret = $index;

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Adds an object to the array of child objects at the given index.
     *
     * Verifies that the object is of the required type and then inserts it
     * into the internal array of objects at the given index, shifting objects
     * at that index and above up by one.
     *
     * This calls the AddAllAt() method to actually do the work passing an array
     * with the object to be added as the only element.
     *
     * @access  public
     * @param   integer     $index      index at which to insert the object
     * @param   TNTObject   $object     reference to an object to add
     * @return  integer                 index of added object, or -1
     * @see     Add(), AddAll(), AddAllAt()
     */
    function AddAt($index, &$object) {

        $objects = array($object);

        $ret = AddAllAt($index, $objects);

        return $ret;
    }

    /**
     * Adds multiple objects to the array of child objects.
     *
     * Verifies that the objects are of the required type and then adds them to
     * the end of the internal array of objects.
     *
     * @access  public
     * @param   TNTObject[] $objects    array of references to objects to add
     * @return  integer                 index of first added object, or -1
     * @see     Add(), AddAt(), AddAllAt()
     */
    function AddAll(&$objects) {

        $ret = null;

        // Start error section
        do {
            if ($this->readOnly) {
                $this->_SetError('collection is read only');
                break;
            }

            if (!$objects || !is_array($objects) ||
                ($max = count($objects)) < 1) {
                $this->_SetError('invalid or empty array');
                break;
            }

            for ($i = 0; $i < $max; ++$i) {
                if (!$this->IsReqClass($objects[$i])) {
                    $this->_SetError('object is not of the correct class: '.get_class($objects[$i]));
                    break;
                }
            }
            if ($i < $max) {
                break;
            }

            $index = count($this->_objects);
            for ($i = 0; $i < $max; ++$i) {
                $this->_objects[$index] =& $objects[$i];
                $this->_Added($this->_objects[$index], $index);
                ++$index;
            }

            $ret = $index;

        } while (false);
        // End of error section

        return $ret;
    }

    /**
     * Adds multiple objects to the array of child objects starting at the
     * given index.
     *
     * Verifies that the objects are of the required type and then inserts them
     * into the internal array of objects starting at the given index, shifting
     * objects at that index and above up by one.
     *
     * @access  public
     * @param   integer     $index      index at which to insert the object
     * @param   TNTObject[] $objects    array of references to objects to add
     * @return  integer                 index of first added object, or -1
     * @see     Add(), AddAt(), AddAll()
     */
    function AddAllAt($index, &$objects) {

        $ret = null;

        // Start error section
        do {
            if ($this->readOnly) {
                $this->_SetError('collection is read only');
                break;
            }

            if (!$objects || !is_array($objects) ||
                ($max = count($objects)) < 1) {
                $this->_SetError('invalid or empty array');
                break;
            }

            $cnt = count($this->_objects);
            if ($index < 0 || $index > $cnt) {
                $this->_SetError('index is out of range');
                break;
            }

            for ($i = 0; $i < $max; ++$i) {
                if (!$this->IsReqClass($objects[$i])) {
                    $this->_SetError('object is not of the correct class: '.get_class($objects[$i]));
                    break;
                }
            }
            if ($i < $max) {
                break;
            }
    
            array_splice($this->_objects, $index, 0, $objects);
    
            for ($i = 0; $i < $max; ++$i) {
                $this->_Added($this->_objects[$index], $index);
                ++$index;
            }

            $ret = $index;

        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Removes an object to the array of child objects.
     *
     * Verifies that the object is of the required type and then add it to
     * the end of the internal array of objects.
     *
     * This calls the RemoveAll() method to actually do the work passing an
     * array with the object to be added as the only element.
     *
     * @access  public
     * @param   TNTObject   $object     reference to an object to remove
     * @return  integer                 index of removed object, or -1
     * @see     RemoveAt(), RemoveAll(), RemoveAllAt()
     */
    function Remove(&$object) {

        $objects = array($object);

        $ret = $this->RemoveAll($objects);

        return $ret;
    }

    /**
     * Removes an object from the array of child objects at the given index.
     *
     * Objects with higher indices will shift down to fill the gap left by
     * the removed object.
     *
     * @access  public
     * @param   integer     $index      index at which to remove the object
     * @return  integer                 index of removed object, or -1
     * @see     Remove(), RemoveAll(), RemoveAllAt()
     */
    function RemoveAt($index) {

        $ret = $this->RemoveAllAt($index, 1);

        return $ret;
    }

    /**
     * Removes multiple objects from the array of child objects.
     *
     * Objects with higher indices will shift down to fill the gap left by
     * the removed object.
     *
     * @access  public
     * @param   TNTObject[] $objects    array of references to objects to remove
     * @return  integer                 index of last object removed, or -1
     * @see     Remove(), RemoveAt(), RemoveAllAt()
     */
    function RemoveAll(&$objects) {

        $ret = null;

        // Start error section
        do {
            if ($this->readOnly) {
                $this->_SetError('collection is read only');
                break;
            }

            if (!$objects || !is_array($objects) ||
                ($max = count($objects)) < 1) {
                $this->_SetError('invalid or empty array');
                break;
            }

            for ($i = 0; $i < $max; ++$i) {
                if (!$this->IsReqClass($objects[$i])) {
                    $this->_SetError('object is not of the correct class: '.get_class($objects[$i]));
                    break;
                }
            }
            if ($i < $max) {
                break;
            }

            $index = -1;
            for ($i = 0; $i < $max; ++$i) {
                if (($index = $this->IndexOf($objects[$i])) == -1) {
                    $this->_SetError('object is not in this collection');
                    break;
                } else if (!$this->RemoveAt($index)) {
                    $this->_SetError('failed to remove object at index: '.$index);
                    break;
                }
            }
            if ($i < $max) {
                break;
            }

            $ret = $index;

        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Removes multiple objects from the array of child objects starting at the
     * given index.
     *
     * Objects with higher indices will shift down to fill the gap left by
     * the removed object.
     *
     * @access  public
     * @param   integer     $index      index at which to remove the objects
     * @param   integer     $count      number of objects to remove
     * @return  integer                 index of first removed object, or -1
     * @see     Remove(), RemoveAt(), RemoveAll()
     */
    function RemoveAllAt($index, $count) {

        $ret = null;

        // Start error section
        do {
            if ($this->readOnly) {
                $this->_SetError('collection is read only');
                break;
            }

            $cnt = count($this->_objects);
            $max = $index + $count;
            if ($index < 0 || $max >= $cnt) {
                $this->_SetError('index is out of range');
                break;
            }

            $removed_objs = array_splice($this->_objects, $index, $count, null);

            $max = count($removed_objs);
            for ($i = $index; $i < $max; ++$i) {
                $this->_Removed($removed_objs[$i], $i);
            }

            $ret = $index;

        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Retains only objects that are in the given array
     *
     * The specified array may contain other objects which are not in this
     * collection. The collection will be altered to contain only those objects
     * which exist in both lists.
     *
     * Calls RemoveAt() to remove unrequired objects.
     *
     * @access  public
     * @param   mixed[]     $objects    array of references to objects to retain
     * @return  boolean                 true if the collection was altered
     */
    function RetainAll(&$objects) {

        $ret = null;

        // Start error section
        do {
            if ($this->readOnly) {
                $this->_SetError('collection is read only');
                break;
            }

            if (!$objects || !is_array($objects) ||
                ($max = count($objects)) < 1) {
                $this->_SetError('invalid or empty array');
                break;
            }

            $ret = true;
            $altered = false;
            $removed = false;
            do {
                $cnt = count($this->_objects);
                $removed = false;
                for ($i = 0; $ret && !$removed && $i < $cnt; ++$i) {
                    $contained = false;
                    for ($j = 0; !$contained && $j < $max; $j++) {
                        if (is_equal($this->_objects[$i], $objects[$j])) {
                            $contained = true;
                        }
                    }
                    if (!$contained) {
                        $removed = true;
                        $altered = true;
                        if (!$this->RemoveAt($i)) {
                            $this->_SetError('object could not be removed from the collection, index: '.$i);
                            $ret = false;
                            break;
                        }
                    }
                }
            } while ($removed && $ret);

            $ret = $altered;
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Check the collection contains the given object
     *
     * Calls ContainsAll() to do the actual work
     *
     * @access  public
     * @param   TNTObject   $object     reference to object to check
     * @return  boolean                 true if object is in the collection
     * @see     ContainsAll()
     */
    function Contains(&$object) {

        $objects = array($object);

        $ret = $this->ContainsAll($objects);

        return $ret;
    }

    /**
     * Check the collection contains the given objects
     *
     * @access  public
     * @param   TNTObject[] $objects    array of references to objects to add
     * @return  integer                 index of last object removed, or -1
     * @see     Contains()
     */
    function ContainsAll(&$objects) {

        $ret = null;

        // Start error section
        do {
            if (!$objects || !is_array($objects) ||
                ($max = count($objects)) < 1) {
                $this->_SetError('invalid or empty array');
                break;
            }

            for ($i = 0; $i < $max; ++$i) {
                if (!$this->IsReqClass($objects[$i])) {
                    $this->_SetError('object is not of the correct class: '.get_class($objects[$i]));
                    break;
                }
            }
            if ($i < $max) {
                break;
            }

            for ($i = 0; $i < $max; ++$i) {
                if (($index = $this->IndexOf($objects[$i])) == -1) {
                    $this->_SetError('object is not in this collection');
                    break;
                }
            }
            if ($i < $max) {
                break;
            }

            $ret = true;
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Returns the number of objects in the collection
     *
     * @access  public
     * @return  integer                 number of objects in the collection
     */
    function Count() {

        $ret = count($this->_objects);

        return $ret;
    }

    /**
     * Checks if the collection is empty
     *
     * @access  public
     * @return  boolean                 true if the collection is empty
     */
    function IsEmpty() {

        $ret = (count($this->_objects) == 0);

        return $ret;
    }

    /**
     * Clears the collection
     *
     * @access  public
     * @return  void
     */
    function Clear() {

        $this->_objects = array();

    }

    /**
     * Returns the indices in the collection of the given object.
     *
     * Normally an object occurrs only once in a collection but this allows
     * for collections where the object can occur multiple times.
     *
     * As objects are stored by reference we use {@link is_reference()} to
     * test if two objects are the same object.
     *
     * @access  public
     * @param   TNTObject   $object     reference to object to find indices of
     * @return  integer[]               array of indices of object (or null)
     */
    function IndicesOf(&$object) {

        $ret = null;

        // Start error section
        do {
           if (!$object || !$this->IsReqClass($objects[$i])) {
                $this->_SetError('object is not of the correct class: '.get_class($objects[$i]));
                break;
            }


            $indices = null;
            $max = count($this->_objects);
            for ($i = 0; $i < $max; ++$i) {
                if (is_reference($this->_objects[$i], $object)) {
                    if (is_null($indices)) {
                        $indices = array();
                    }
                    $indices[] = $i;
                }
            }

            $ret = $indices;
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Returns the index in the collection of the first occurence of the
     * given object, or -1 if object does not exist.
     *
     * As objects are stored by reference we use {@link is_reference()} to
     * test if two objects are the same object.
     *
     * @access  public
     * @param   TNTObject   $object     reference to object to find index of
     * @return  integer                 index of object, or -1
     */
    function IndexOf(&$object) {

        $ret = null;

        // Start error section
        do {
           if (!$object || !$this->IsReqClass($object)) {
                $this->_SetError('object is not of the correct class: '.get_class($object));
                break;
            }


            $index = -1;
            $max = count($this->_objects);
            for ($i = 0; $i < $max; ++$i) {
                if (is_reference($this->_objects[$i], $object)) {
                    $index = $i;
                    break;
                }
            }

            $ret = $index;
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Returns the index in the collection of the last occurence of the
     * given object, or -1 if object does not exist.
     *
     * As objects are stored by reference we use {@link is_reference()} to
     * test if two objects are the same object.
     *
     * @access  public
     * @param   TNTObject  $object     reference to object to find index of
     * @return  integer                index of object, or -1
     */
    function LastIndexOf(&$object) {

        $ret = null;

        // Start error section
        do {
           if (!$object || !$this->IsReqClass($objects[$i])) {
                $this->_SetError('object is not of the correct class: '.get_class($objects[$i]));
                break;
            }


            $index = -1;
            $max = count($this->_objects);
            for ($i = ($max_i - 1); $i >= 0; $i--) {
                if (is_reference($this->_objects[$i], $object)) {
                    $index = $i;
                    break;
                }
            }

            $ret = $index;
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Find indices of objects in the collection given the value of one of
     * their members.
     *
     * The property '!class' represents the class name of the object. This
     * allows you to find objects of a given class.
     *
     * '$strict' cause strict value and type matching. When checking class
     * names '$strict' can be used to restrict the class match to exact classes
     * and not derived classes.
     *
     * @access  public
     * @param   mixed       $value      value the member should have
     * @param   string      $member     name of member to check
     * @param   bool        $strict     value must type match
     * @return  integer[]               array of indices of objects, or null
     */
    function &FindIndicesOf($value, $member = 'id', $strict = false) {

        $ret = null;

        // Start error section
        do {
            $m = count($this->_objects);
            for ($i = 0; $i < $m; ++$i) {
                $o =& $this->_objects[$i];
                if ((($member === '!class') &&
                     ($strict && $o->IsClassOf($value)) ||
                     (!$strict && $o->IsSubClassOf($value))) ||
                    (($o->MemberExists($member) &&
                     ($strict && $o->$member == $value) ||
                     (!$strict && $o->$member == $value)))) {
                    if (is_null($ret)) {
                        $ret = array();
                    }
                    $ret[] =& $i;
                    break;
                }
            }
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Find objects in the collection given the value of one of their members.
     *
     * The property '!class' represents the class name of the object. This
     * allows you to find objects of a given class.
     *
     * '$strict' cause strict value and type matching. When checking class
     * names '$strict' can be used to restrict the class match to exact classes
     * and not derived classes.
     *
     * @access  public
     * @param   mixed       $value      value the member should have
     * @param   string      $member     name of member to check
     * @param   bool        $strict     value must type match
     * @return  TNTObject[]             array of references to objects, or null
     */
    function &FindItems($value, $member = 'id', $strict = false) {

        $ret = null;

        // Start error section
        do {
            $indices = $this->FindIndicesOf($value, $member, $strict);

            if ($indices) {
               $ret = array();
               $max_i = count($indices);
               for ($i = 0; $i < $max_i; ++$i) {
                   $ret[] =& $this->_objects[$indices[$i]];
               }
            }
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Call the given boolean method on all the child objects that support the
     * method, stopping at first one that fails.
     *
     * The variable number of arguments are passed to the called method which
     * is assumed to return a boolean.
     *
     * @access  public
     * @param   string      $method     name of method to call
     * @param   mixed       ...         further params to pass to called method
     * @return  boolean                 true if successfully called method
     */
    function Call($method /*...*/) {
        $vargs = func_get_args();

        $ret = null;

        // Start error section
        do {
            if (!$this->_objects) {
                $ret = true;
                break;
            }

            $ret = true;

            $max = count($this->_objects);
            reset($this->_objects);
            while (list($key, $val) = each($this->_objects)) {
                $ret = call_user_method_array($method, $this->_objects[$key], $vargs);
                if (!$ret) {
                    break;
                }
            }

        
        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Call the given method on all the child objects that support the method.
     *
     * The variable number of arguments are passed to the called method whose
     * return value is returned.
     *
     * @access  public
     * @param   string      $method     name of method to call
     * @param   mixed       ...         further params to pass to called method
     * @return  mixed[]                 array of return values by object index.
     */
    function CallEx($method /*...*/) {
        $vargs = func_get_args();

        $ret = null;

        // Start error section
        do {
            if (!$this->_objects) {
                $ret = array();
                break;
            }

            $max = count($this->_objects);
            reset($this->_objects);
            while (list($key, $val) = each($this->_objects)) {
                $ret[$key] =& call_user_method_array($method, $this->_objects[$key], $vargs);
            }

        } while (false);
        // End of error scope

        return $ret;
    }

    /**
     * Return the collection as an array
     *
     * This returns a copy of the internal object array
     *
     * @access  public
     * @return  TNTObject[]         an array holding the collection
     */
    function ToArray() {

        $ret =& $this->_objects;

        return $ret;
    }

    /**
     * Construct the collection object.
     *
     * This will construct the base TNTObject class.
     *
     * @access  public
     */
    function TNTObjects() {

        TNTObject::TNTObject();

        $this->readOnly = false;
        $this->autoDestroy = true;
        $this->reqClasses = array('TNTObject');
        $this->_objects = array();
    }

    /**
     * Destruct the collection object.
     *
     * This will destruct the collection object.
     *
     * It will destroy each child in the collection if $this->_destroy is
     * true.
     *
     * @access  private
     */
    function _TNTObjects() {

        if ($this->autoDestroy) {
            $max = count($this->_objects);
            for ($i = 0; $i < $max; ++$i) {
                destroy($this->_objects[$i]);
            }
            //$this->_objects = array();
        }

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
