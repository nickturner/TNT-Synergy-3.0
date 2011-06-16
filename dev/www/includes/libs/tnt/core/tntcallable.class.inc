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
 * Callable Class 
 * ~~~~~~~~~~~~~~
 *
 * Small class to wrap the PHP callable type allowing the framework to check
 * when callable types are expected.
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


/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Callable type.
 *
 * This type is used to wrap the PHP concept of a callable type.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTCallable {

    /**
     * Details of the function, method/class or method/object to call
     * @var     callable                function or method/class to call
     * @access  private
     */
    var $_callable = null;

    /**
     * Set the callable
     *
     * Call with a single argument constisting of a valid {@link is_callable()}
     * value or two arguments consisting of the method name and object/class.
     *
     * If a single argument is used of the format 'x::y' then it is split into
     * the two arguments 'x' & 'y' where 'x' is the class name and 'y' the
     * method name.
     *
     * The callables syntax is checked but no attempt is made to see if the
     * function or method exist yet.
     *
     *
     * @access  public
     * @param   mixed       $fnc        function name or callable array
     * @param   mixed       $obj        object reference or class name
     * @return  boolean                 true if ok
     */
    function Set($fnc, $obj = null) {
        if (!$obj) {
            if (is_a($fnc, 'tntcallable')) {
                $callable = $fcn->_callable;
            } else if (preg_match('/^([\w_]+)::([\w_]+)$/', $fcn, $matches)) {
                $callable = array($matches[1], $matches[2]);
            } else {
                $callable = $fnc;
            }
        } else {
            $callable = array($fnc, $obj);
        }

        if (!is_callable($callable, true)) {
            return false;
        }

        $this->_callable = $callable;
        return true;
    }

    /**
     * Get the callable
     *
     * Returns a PHP callable string or array.
     *
     * @access  public
     * @return  callback                the PHP callback
     */
    function Get()
        return $this->_callable;
    }

    /**
     * Returns true if the callable can be called.
     *
     * If the $syntaxOnly argument is true the method only verifies that the
     * callable might be a function or method. No check will be made to see
     * if the named function or method exists.
     *
     * @access  public
     * @param   boolean     $syntaxOnly check syntax only
     * @return  boolean                 true if it is callable
     */
    function IsCallable(bool syntaxOnly)
        return $this->_callable;
    }

    /**
     * Call the function or method.
     *
     * Any arguments are passed to the called function or method
     *
     * @access  public
     * @param   mixed       $arg        option arguments (any number)
     * @return  mixed                   value return by called method
     */
    function Call(/*$arg,...*/) {
        $args = func_get_args();
        return call_user_func_array($this->_callable, $args);
    }

    /**
     * Call the function or method.
     *
     * Expects a single argument which is an array of arguments to pass to the
     * called function or method.
     *
     * @access  public
     * @param   array       $args       array of arguments
     * @return  mixed                   value return by called method
     */
    function CallByArray($args) {
        return call_user_func_array($this->_callable, $args);
    }

    /**
     * Return the name of the function/method to be called.
     *
     * @access  public
     * @return  string                  method/function name
     */
    function Function() {
        return (is_string($this->_callable) ?  $this->_callable : (is_array($this->_callable) ? $this->_callable[0] : null));
    }

    /**
     * Return the name of the class the method is in.
     *
     * This returns null for function (ie. not a method of a class)
     *
     * @access  public
     * @return  string                  class name
     */
    function Class() {
        return (is_array($this->_callable) ? (is_object($this->_callable[1]) ? get_class($this->_callable[1]) : $this->_callable[1]) : null);
    }

    /**
     * Return a reference to the object the method is called for.
     *
     * This returns null for function or static class method.
     *
     * @access  public
     * @return  object&                 reference to object
     */
    function &Obj() {
        if (is_array($this->_callable) && is_object($this->_callable[1])) {
            return $this->_callable[1];
        } else {
            $obj = null;
            return $obj;
        }
    }

    /**
     * Construct the object
     *
     *
     * Call with a single argument constisting of a valid {@link is_callable()}
     * value or two arguments consisting of the method name and object/class.
     *
     * @access  public
     * @param   mixed       $fnc        function name or callable array
     * @param   mixed       $obj        object reference or class name
     * @return  void
     */
    function TNTCallable($fnc, $obj = null) {
        $this->Set($fcn, $obj);
    }

}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
