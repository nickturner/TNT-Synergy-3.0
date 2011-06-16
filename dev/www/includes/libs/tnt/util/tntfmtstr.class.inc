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
 * Formatted String Manipulation Class
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class is the basis for manipulating formatted strings such as file
 * fmtstrs, uniform resource indicators, comma separated value records etc.
 *
 * It defines a common interface to manipluating such strings.
 *
 * The class can be used as an object allowing the string to be manipulated
 * or as a set of static functions for one off calls. When used statically
 * a static global object is used to perform the manipluation.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */
global $tnt;
$tnt['_fmtstrObjs'] = null;

/*
 * Defines
 */

/**
 * Class to represent a formatted string
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTFmtStr extends TNTObject {
    /**
     * The formatted string
     * @var     string                  formatted string
     * @access  private
     */
    var $_string = null;

    /**
     * The elements of the string
     * @var     string[]                elements of formatted string
     * @access  private
     */
    var $_elements = null;

    /**
     * The names of the elements of the string
     * @var     string[]                element names
     * @access  private
     */
    var $_elementNames = null;

    /**
     * Set the object using the given data.
     *
     * To clear the object pass the data as null or empty string.
     *
     * @access  public
     * @param   mixed       $data       data to set object with
     * @return  boolean                 true if successfull
     * @see Get(), SetElement(), GetElement()
     */
    function Set($data) {

        if (is_null($data) || is_string($data)) {
            $this->_string = $data;
            $this->_elements = null;
            $ret = true;
        } else if (is_object($data) && is_a($data, get_class($this))) {
            $this->SetMembers($data->GetMembers());
            $ret = true;
        } else if (is_array($data)) {
            foreach ($this->_elementNames as $n) {
                $this->_elements[$n] = (isset($data[$n]) ? $data[$n] : null);
            }
            $this->_string = null;
            $ret = true;
        } else {
            $ret = false;
        }

        return $ret;
    }

    /**
     * Get the value of this object as the given type
     *
     * Alias for {@link TNTFmtStr::Cast()}
     *
     * @access  public
     * @param   string      $type       PHP type or class name to cast to
     * @return  mixed                   object in given type
     * @see Set(), SetElement(), GetElement(), Cast()
     */
    function Get($type = 'string') {
        return $this->Cast($type);
    }

    /**
     * Set the object using the given data.
     *
     * [Depreciated] Alias to {@link TNTFmtStr::Set()}
     *
     * @access  public
     * @param   mixed       $data       data to set object with
     * @return  boolean                 true if successfull
     * @depreciated
     * @see Set()
     */
    function SetValue($data) {
        return $this->Set($data);
    }

    /**
     * Get the value of this object as the given type
     *
     * [Depreciated] Alias to {@link TNTFmtStr::Get()}
     *
     * @access  public
     * @param   string      $type       PHP type or class name to cast to
     * @return  mixed                   reference to object in given type
     * @depreciated
     * @see Get()
     */
    function GetValue($type = 'string') {
        return $this->Get($type);
    }

    /**
     * Get a named element of the string
     *
     * @access  public
     * @param   string      $element    name of element to get
     * @return  mixed                   value of named element (or null)
     * @see SetElement(), Set(), Get()
     */
    function GetElement($name) {
        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if (isset($this->_elements[$name])) {
            $ret = $this->_elements[$name];
        }

        return $ret;
    }

    /**
     * Set a named element of the string
     *
     * @access  public
     * @param   string      $element    name of element to get
     * @param   mixed       $value      value of named element
     * @return  boolean                 true if successful
     * @see GetElement(), Set(), Get()
     */
    function SetElement($name, $value) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if (isset($this->_elements[$name]) ||
            in_array($name, $this->_elementNames)) {
            $this->_elements[$name] = $value;
            $this->_string = null; // So it is rebuilt from new elements
            $ret = true;
        }

        return $ret;
    }

    /**
     * Cast this object to another type
     *
     * @access  public
     * @param   string      $type       PHP type or class name to cast to
     * @return  mixed                   object in given type
     * @see Get
     */
    function Cast($type) {

        switch ($type) {
            case 'string':
                if (!$this->_string) {
                    $this->_Implode();
                }
                $ret = $this->_string;
                break;
            case 'array':
                if (!$this->_elements) {
                    $this->_Explode();
                }
                $ret = $this->_elements;
                break;
            default:
                $ret = parent::Cast('type');
                break;
        }

        return $ret;
    }

    /**
     * Explode the string into the elements
     *
     * @access  private
     * @return  boolean                 true if successfull
     * @see TNTFmtStr
     */
    function _Explode() {
        TNTDebug::Error('TNTFmtStr::_Expode() - abstract method');
        return false;
    }
    
    /**
     * Implode the elements into the string
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function _Implode() {
        TNTDebug::Error('TNTFmtStr::_Implode() - abstract method');
        return true;
    }

    /**
     * Get a global object of the given format class, which is used for
     * the static method calls.
     *
     * The state of this object should never be assumed, the methods using
     * it should initialise it as required.
     *
     * If there is no static object of the given class then one is created,
     * if the created object isn't derived from TNTFmtStr the code aborts.
     *
     * @access  public
     * @param   string      $class      class of object required
     * @return  TNTFmtStr&              object of correct class
     * @static
     * @see SetElement()
     */
    /*static*/function &_GetStaticObject($class) {
        global $tnt;
        if (!isset($tnt['_fmtstrObjs'][$class])) {
            $tnt['_fmtstrObjs'][$class] =& new $class();

            if (!is_a($tnt['_fmtstrObjs'][$class], 'tntfmtstr')) {
                TNT::Abort("class '$class' is not derived from 'TNTFmtStr'");
            }
        }
        return $tnt['_fmtstrObjs'][$class];
    }

    /**
     * Get a named element from the data.
     *
     * Any value valid to {@link Set()} can be used for '$data'.
     *
     * @access  public
     * @param   string      $class      class to use to manipulate data
     * @param   mixed       $data       data to set element in
     * @param   string      $element    name of element to get
     * @return  mixed                   value of named element (or null)
     * @static
     * @see SetElement()
     */
    /*static*/function GetDataElement($class, $data, $name) {

        $ret = null;

        $obj =& TNTFmtStr::_GetStaticObject($class);

        if ($obj->Set($data)) {
            $ret = $obj->GetElement($name);
        }

        return $ret;
    }

    /**
     * Set a named element in the data.
     *
     * Any value valid to {@link Set()} can be used for '$data'.
     *
     * The return value is '$data' with the named element set.
     *
     * @access  public
     * @param   string      $class      class to use to manipulate data
     * @param   mixed       $data       data to set element in
     * @param   string      $element    name of element to get
     * @param   mixed       $value      value of named element
     * @return  mixed                   the altered data
     * @static
     * @see GetElement()
     */
    /*static*/function SetDataElement($class, $name, $value) {

        $ret = null;

        $obj =& TNTFmtStr::_GetStaticObject($class);

        if ($obj->Set($data) && $obj->SetElement($name)) {
            $ret = $obj->Get(gettype($value));
        }

        return $ret;
    }

    /**
     * Construct the object
     *
     * Any initial value is passed to {@link TNTFmtStr::Set()}
     *
     * @access  public
     * @param   mixed       $data       initial value for object
     * @param   string[]    $elementNames names of the elements
     * @return  void
     */
    function TNTFmtStr($data = null, $elementNames = null) {

        TNTObject::TNTObject();

        $this->_string = null;
        $this->_elements = null;
        $this->_elementNames = $elementNames;

        $this->Set($data);

    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTFmtStr() {
    }
}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
