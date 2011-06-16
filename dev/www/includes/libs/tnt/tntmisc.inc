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
 * Core functions
 * ~~~~~~~~~~~~~~
 *
 * Some miscellaneous functions or comptibility functions that may or may
 * not exist depending on PHP version in use.
 *
 * @package TNT
 * @author  Nick Turner <nick@tntdisco.co.uk>
 * @since   06/07/06
 * @version 1.0
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
 * Return a reference to the value.
 *
 * This can be used to force a reference when one would not normally be
 * allowed according to the new PHP 4 rules.
 *
 * ie.
 * Invalid functions:
 *   function &example() { return null; }
 *   function &example() { return 1+2; }
 *   function &example() { return func(); }
 *
 * Valid functions:
 *   function &example() { return ref(null); }
 *   function &example() { return ref(1+2); }
 *   function &example() { return ref(func()); }
 *
 * NB: If $value was a reference then it will be lost. So if in the last
 * example func() returned a reference then it would be lost and the only
 * way of preserving it is:
 *   function &example() { $a =& func(); return $a; }
 *
 * @access public
 * @param  mixed        $value      value to return reference to
 * @return mixed&                   reference to given value
 */
function &ref($value) {
    return $value;
}

if (!function_exists('realpath')) {
    /**
     * Remove all '/./', '/../' etc. from the path
     *
     * This function exists in PHP version >= 4
     *
     * @access public
     * @param  string       $path       path to normalize
     * @return string                   normalized path
     */
    function realpath($path) {
        $oldpath = getcwd();
        if ($oldpath && chdir($path)) {
            $path = getcwd();
            if (!chdir($oldpath)) {
                die("Fatal: error - can't change to directory: $oldpath");
            }
        }    
        return $path;
    }
}

if (!function_exists('file_get_contents')) {
    /**
     * Read the contents of a file in a binary safe way
     *
     * This function exists in PHP version >= 4.3
     *
     * @access public
     * @param  string       $filename   name of file to read
     * @param  bool         $use_include_path search for file on include path
     * @return string                   contents of the file
     */
    function file_get_contents($filename, $use_include_path = false) {
        $ret = null;
        if (($fp = fopen($filename, 'rb', $use_include_path))) {
            $ret = fread($fp, filesize($filename));
            fclose($fp);
        }
        return $ret;
    }
}

if (!function_exists('file_set_contents')) {
    /**
     * Write the contents of a file in a binary safe way
     *
     * This function is yet to exist in PHP
     *
     * @access public
     * @param  string       $filename   name of file to read
     * @param  string       $contents   contents of the file
     * @param  bool         $use_include_path search for file on include path
     * @return bool                     true if successfull
     */
    function file_set_contents($filename, $contents, $use_include_path = false){
        $ret = false;
        if  (($fp = fopen($filename, 'wb', $use_include_path))) {
            $len = strlen($contents);
            $ret = (fwrite($fp, $contents, $len) === $len);
            fclose($fp);
        }
        return $ret;
    }
}

if (!function_exists('is_a')) {
    /**
     * Is an object of a particular class or derived from a particular class
     *
     * This function exists in PHP version >= 4.2.0
     * 
     * @access public
     * @param  object&      $obj        object to test class type
     * @param  string       $name       class name to test for
     * @return bool                     true if object is derived from class
     */
    function is_a(&$obj, $name) {
         if (!is_string($name)) {
             trigger_error('is_a() - \'name\' is not a string');
             return false;
         }
         if (!is_object($obj)) {
             return false;
         }
         if (strcasecmp(get_class($obj), $name) == 0) {
             return true;
         } else {
             return is_subclass_of($obj, $name);
         }
    }
}

/**
 * Returns the name of the class of an object.
 *
 * This function returns the name of the class of which the object $obj
 * is an instance. Returns FALSE if $obj is not an object. 
 *
 * On PHP 4.3.0+, $obj is optional if called from an objects method where
 * it will use the object/class the method was called from to determine
 * the value for $obj. If not called from a method then this functions
 * returns FALSE. 
 *
 * This is an implementation similar to the PHP5 {@link get_class()}
 * function where the parameter is optional.
 *
 * 
 *
 * <pre>
 * E.g.
 *     class test1 {
 *       function whoami() { echo get_class_name(); }
 *     }
 *
 *     class test2 extends test1 {
 *     }
 *
 *     $a =& new test1();
 *     $b =& new test2();
 *     echo get_class_name($a);  // displays "test1"
 *     echo $a->whoami();        // displays "test1"    [Object call]
 *     echo test1::whoami();     // displays "test1"    [Static call]
 *     echo get_class_name($b);  // displays "test2"
 *     echo $b->whoami();        // displays "test2"    [Object call]
 *     echo test2::whoami();     // displays "test2"    [Static call]
 * </pre>
 *
 * @access public
 * @param  mixed        $obj        object to test (optional in 4.3.0+)
 * @return string                   class name of object or method
 */
function get_class_name($obj = null) {
    if (is_object($obj)) {
        return get_class($obj);
    } else if (function_exists('debug_backtrace')) {
        $bt = debug_backtrace();
        if (empty($bt[1]['type'])) {
            trigger_error('No object passed and not called from class method');
            return FALSE;
        }
        return $bt[1]['class'];
    } else {
        trigger_error('Requires object parameter in this version of PHP');
        return FALSE;
    }
    
    return false;
}

/**
 * Retrieves the parent class name for object or class.
 *
 * If $obj is an object, returns the name of the parent class of the
 * class of which obj is an instance. 
 *
 * If $obj is a string, returns the name of the parent class of the class
 * with that name. This functionality was added in PHP 4.0.5. 
 *
 * On PHP 4.3.0+, $obj is optional if called from an objects method where
 * it will use the object/class the method was called from to determine
 * the value for $obj. If not called from a method then this functions
 * returns FALSE. 
 *
 * This is an implementation similar to the PHP5 {@link get_parent_class()}
 * function where the parameter is optional.
 *
 * <pre>
 * E.g.
 *     class test1 {
 *       function whoami() { echo get_parent_class_name(); }
 *     }
 *
 *     class test2 extends test1 {
 *     }
 *
 *     $a =& new test1();
 *     $b =& new test2();
 *     echo get_class_name($a);  // displays ""
 *     echo $a->whoami();        // displays ""         [Object call]
 *     echo test1::whoami();     // displays ""         [Static call]
 *     echo get_class_name($b);  // displays "test1"
 *     echo $b->whoami();        // displays "test1"    [Object call]
 *     echo test2::whoami();     // displays "test1"    [Static call]
 * </pre>
 *
 * @access public
 * @param  mixed        $obj        object to test (optional in 4.3.0+)
 * @return string                   parent class name of object or method
 */
function get_parent_class_name($obj = null) {
    return get_parent_class(get_class_name($obj));
}

/**
 * Retrieves the hierarchy of class names for object or class.
 *
 * If $obj is an object, returns an array with the names of the classes
 * that make up the class of which obj is an instance. This functionality
 * was added in PHP 5 in the method {@link class_parents()}.
 *
 * If $obj is a string, returns an array with the names of the classes
 * that make up the class with that name. This functionality was added
 * in PHP 5.1.0 in the method {@link class_parents()}.
 *
 * On PHP 4.3.0+, $obj is optional if called from an objects method where
 * it will use the object/class the method was called from to determine
 * the value for $obj. If not called from a method then this functions
 * returns FALSE. 
 *
 * <pre>
 * E.g.
 *     class test1 {
 *       function whoami() { echo join(',', get_parent_class_names()); }
 *     }
 *
 *     class test2 extends test1 {
 *     }
 *
 *     $a =& new test1();
 *     $b =& new test2();
 *     echo $a->whoami();        // displays "test1"        [Object call]
 *     echo test1::whoami();     // displays "test1"        [Static call]
 *     echo $b->whoami();        // displays "test2,test1"  [Object call]
 *     echo test2::whoami();     // displays "test2,test1"  [Static call]
 * </pre>
 *
 * @access public
 * @param  mixed        $obj        object to test (optional in 4.3.0+)
 * @return string[]                 array of class names
 */
function get_class_hierarchy($obj = null) {
    $ret = array();
    for ($n = get_class_name($obj); $n; $n = get_parent_class($n)) {
        $ret[] = $n;
    }
    return (count($ret) ? $ret : false);
}

/**
 * Is an object of a particular class or derived from a particular class
 *
 * If $obj is an object, returns true if the object is derived from one of
 * the given class names. 
 *
 * If $obj is a string, returns true if the class is derived from one of
 * the given class names. 
 *
 * $name can either be a class name or an array of class names.
 * 
 * @access public
 * @param  object       $obj        object to test class type
 * @param  string       $name       class name to test for (or array)
 * @return bool                     true if object derived from class
 */
function is_kindof($obj, $name) {
    if (is_object($obj)) {
        foreach ((array)$name as $n) {
            if (is_a($obj, $n)) {
                return true;
            }
        }
    } else if (is_string($obj)) {
        if (($hierarchy = get_class_hierarchy($obj))) {
            // lowercase all the classnames
            $n = explode('|', strtolower(implode('|', (array)$name)));
            $h = explode('|', strtolower(implode('|', $h)));
            return (count(array_intersect($n, $h)) > 0);
        }
    }

    return false;
}

if (!function_exists('array_search')) {
    /**
     * Searches array for a given value and returns the key if it is found
     *
     * This function exists in PHP version >= 4.0.5
     * 
     * @access public
     * @param  mixed        $needle     value to search for
     * @param  array        $haystack   array to search
     * @return mixed                    array key of found item or false
     */
    function array_search($needle, $haystack, $strict=false) {
        reset($haystack);
        while (list($key, $val) = each($haystack)) {
            if ($needle == $val &&
                (!$strict || gettype($needle) == gettype($val))) {
                return $key;
            }
        }
        return false;
    }
}

/**
 * Removes the values from array1 that are in array2.
 *
 * @access public
 * @param  array&       $array1     array to remove values from
 * @param  array&       $array2     array of values to remove
 * @param  boolean      $preserve   preserve the keys (else array is reindexed)
 * @return integer                  number of values removed
 */
function array_remove(&$array1, $array2, $preserve = true) {

    $diff = array_diff($array1, $array2);

    $ret = count($array1) - count($diff);

    $array1 = ($preserve ? $diff : array_values($diff));

    return $ret;
}

if (!function_exists('fnmatch')) {
    /**
     * Checks if the passed string would match the given shell wildcard pattern.
     *
     * This function exists in PHP version >= 4.3.0 (apart from on Windows)
     *
     * @access public
     * @param  string       $pattern    pattern to check against
     * @param  string       $string     string to check
     * @return boolean                  true if matched
     */
    function fnmatch($pattern, $string) {
        // Convert pattern to a regex pattern
        $regex = '^';
        $l = strlen($pattern);
        for ($i = 0; $i < $l; ++$i) {
            $c = $pattern{$i};
            if ($c == '\\') {
                $i++; // Skip escaped character
            } else if ($c == '?') {
                $regex .= '.'; // any character
            } else if ($c == '*') {
                $regex .= '.*'; // 0 or more any characters    
            } else if ($c == '[' || $c == ']') {
                $regex .= $c;  // one of characters within []
            } else {
                $regex .= '\\' . $c;
            }
        }
        $regex .= '$';

        // trim redundant ^ or $
        // e.g. ^.*\.txt$ matches exactly the same as \.txt$
        if (substr($regex, 0, 3) == '^.*') {
            $regex = substr($regex, 3);
        }
        if (substr($regex, -3, 3) == '.*$') {
            $regex = substr($regex, 0, -3);
        }

        $ret = ereg($regex, $string);

        return $ret;
    }
}

/**
 * Compare two variables for reference.
 *
 * Returns true if one variable is a reference to the other.
 *
 * @access  public
 * @param   mixed       $arg1       reference to first value
 * @param   mixed       $arg2       reference to second value
 * @return  boolean                 true if $arg1 and $arg2 reference each other
 */
function is_reference(&$arg1, &$arg2)
{
    $ret = false;
    // We test for reference by altering the value of one of them and seeing
    // if the other alters, then restoring the original value
    if (gettype($arg1) != gettype($arg2)) {
        // If $arg1 and $arg2 are of different types then not references
        $ret = false;
    } else if (is_object($arg1) && get_class($arg1) != get_class($arg2)) {
        // If $arg1 and $arg2 are of different classes then not references
        $ret = false;
    } else if (is_array($arg1)) {
        $value = '__isref__'.rand();
        $arg1['__isref__'] = $value;
        $ret = (isset($arg2['__isref__']) && $arg2['__isref__'] === $value);
        unset($arg1['__isref__']);
    } else if (is_object($arg1)) {
        $value = '__isref__'.rand();
        $arg1->__isref__ = $value;
        $ret = (isset($arg2->__isref__) && $arg2->__isref__ === $value);
        unset($arg1->__isref__);
    } else {
        $s = $arg1;
        $arg1 = '__isref__';
        $ret = ($arg2 === '__isref__');
        $arg1 = $s;
    }
    return $ret;
}

/**
 * Compare two variables for equality.
 *
 * If the variables are not scalers then this does a recursive
 * scan through the variables checking for equality. If any child
 * variable is an object that implements the IsEquals() interface
 * then that will be called to do the comparison. Otherwise it will
 * recursively call is_equals().
 *
 * @access  public
 * @param   mixed       $arg1       reference to first value
 * @param   mixed       $arg2       reference to second value
 * @return  boolean                 true if $arg1 and $arg2 are equal
 */
function is_equal(&$arg1, &$arg2)
{
    $ret = false;
    if (is_scalar($arg1)) {
        // Scalar values are compared using native PHP comparison
        $ret = ($arg1 === $arg2);
    } else if (gettype($arg1) != gettype($arg2)) {
        // If $arg1 and $arg2 are of different types then not equal
        $ret = false;
    } else if (is_object($arg1) && get_class($arg1) != get_class($arg2)) {
        // If $arg1 and $arg2 are of different classes then not equal
        $ret = false;
    } else if (is_reference($arg1, $arg2)) {
        // If $arg1 is a reference to $arg2 then they must be equal
        $ret = true;
    } else if (is_array($arg1)) {
        // If $arg1 is an array then recursively check all its members
        $ret = true;
        reset($arg1);
        while(list($n) = each($arg1)) {
           if (!is_equal($arg1[$n], $arg2[$n])) {
               $ret = false;
               break;
           }
        }
    } else if (is_object($arg1)) {
        if (!isset($arg1->__isequal__) && method_exists($arg1, 'IsEqual')) {
            $ret = $arg1->IsEqual($arg2);
        } else {
            $members = get_object_vars($arg1);
            reset($members);
            while(list($n) = each($members)) {
               if (!is_equal($arg1->$n, $arg2->$n)) {
                   $ret = false;
                   break;
               }
           }
        }
    } else {
        // Other values are compared using native PHP comparison
        $ret = ($arg1 === $arg2);
    }
    return $ret;
}

/**
 * Test whether the value is boolean string
 *
 * Numbers and numeric strings are taken as being boolean, where 0 is false and
 * non zero is true.
 *
 * Strings are taken as being boolean is they are one of the following keywords;
 *   'true', 'false', 'on', 'off', 'yes', 'no'
 *
 * This function does NOT exist in PHP.
 * 
 * @access public
 * @param  mixed        $x          value to test if its a boolean
 * @return boolean                  true if string specifies a boolean value
 */
function is_boolean($x) {
    return  (is_bool($x) || is_numeric($x) || 
             (is_string($x) && (strcasecmp($x, 'true') == 0 ||
                                strcasecmp($x, 'on') == 0 ||
                                strcasecmp($x, 'yes') == 0 ||
                                strcasecmp($x, 'false') == 0 ||
                                strcasecmp($x, 'off') == 0 ||
                                strcasecmp($x, 'no') == 0)));
}

/**
 * Convert a value to a boolean.
 *
 * Numbers and numeric strings are taken as being boolean, where 0 is false and
 * non zero is true.
 *
 * Strings are taken as being boolean is they are one of the following keywords;
 *   'true', 'false', 'on', 'off', 'yes', 'no'
 *
 * This function does NOT exist in PHP.
 * 
 * @access public
 * @param  mixed        $x          value to convert to a boolean
 * @return boolean                  boolean value
 */
function to_boolean($x) {
    if (is_null($x)) {
        return false;
    } else if (is_bool($x)) {
        return $x;
    } else if (is_numeric($x)) {
        return ($x ? true : false);
    } else if (is_string($x)) {
        return ((strcasecmp($x, 'true') == 0) || (strcasecmp($x, 'on') == 0) || (strcasecmp($x, 'yes') == 0));
    } else {
        return false;
    }
}

/**
 * Alias for to_boolean
 *
 * @access public
 * @param  string       $x          string to test for boolean
 * @return bool                     true if string specifies a boolean value
 */
function strtoboolean($x) {
    return to_boolean($x);
}

/**
 * Boolean to string
 *
 * This function does NOT exist in PHP.
 * 
 * @access public
 * @param  boolean      $bool       boolean to convert to string
 * @return string                   'true' or 'false'
 */
function booleantostr($x) {
    return ($x ? 'true' : 'false');
}

/**
 * Treats array as a stack and returns the current value without popping it off.
 *
 * Fits with PHP4 functions array_push() and array_pop().
 *
 * This function does NOT exist in PHP.
 * 
 * @access public
 * @param  array        $stack      array to peep at last value
 * @return mixed                    the last value in the array (or null if empty)
 */
function array_peek(&$stack) {
    if (($cnt = count($stack) - 1) < 0)
        return null;
    return $stack[$cnt];
}

/**
 * Gets the given value from the array if exists else returns default
 * value.
 *
 * This function does NOT exist in PHP.
 * 
 * @access public
 * @param  array        $array      array to peep at last value
 * @param  string       $index      index to return value of
 * @param  mixed        $default    default value to return if not set
 * @return mixed                    the array[index] item or default
 */
function array_value(&$array, $index, $default = null) {
    return (is_array($array) && (isset($array[$index]) || array_key_exists($index, $array)) ? $array[$index] : $default);
}

/**
 * Gets a reference to the given value from the array if exists
 * else returns a reference to the default value.
 *
 * This function does NOT exist in PHP.
 * 
 * @access public
 * @param  array        $array      array to peep at last value
 * @param  string       $index      index to return value of
 * @param  mixed        $default    default value to return if not set
 * @return mixed                    the array[index] item or default
 */
function &array_ref(&$array, $index, &$default /*=& $null*/) {
    return (is_array($array) && (isset($array[$index]) || @is_null($array[$index])) ? $array[$index] : $default);
}

/**
 * Return is a path name is absolute
 *
 * Handles windows drive letters. Assumes if the path starts with a '/' or '\'
 * then it is an absolute path.
 *
 * @access public
 * @param  string       $path       path to check
 * @return boolean                  true if path is absolute
 */
function is_abspath($path) {
    //echo "is_abspath($path)<br />";

    $ret = false;
    if (($l = strlen((string)$path)) >= 1) {
        $c = ((($l > 2) && ($path[1] == ':')) ? $path[2] : $path[0]);
        $ret = ($c == '/' || $c == '\\');
    }

    //echo "is_abspath($path) - return: ".($ret ? 'true' : 'false')."<br />";
    return $ret;
}

/**
 * Dumps information about a variable in a way that's readable by humans. If
 * given a string, integer or float, the value itself will be printed. If given
 * an array, values will be presented in a format that shows keys and elements.
 * Similar notation is used for objects. 
 * 
 * This is similar to the PHP functions, var_dump() and var_export() but it
 * handles reference recursion.
 *
 * Maximum depth to dump to is given using '$max_depth', if this is a number
 * between 0 and 50 then arrays and objects will be recursed into to that
 * depth, at the maximum depth they will just be displayed as 'Array' or
 * 'Object' and not expanded.
 *
 * If the max_depth is not in this range then recursion will stop at a depth of
 * 50 and an indication that this 'absolute maximum depth' will be displayed 
 * along with the 'Array' or 'Object' text.
 *
 * @access public
 * @param  mixed        $var        variable to dump
 * @param  integer      $max_depth  maximum depth to expand.
 * @return string                   textual description of the variable
 */
function var_info(&$var, $max_depth = -1) {
    $dumped = array();

    $txt = _var_info($var, 0, $max_depth, $dumped);

    $max_i = count($dumped);
    for ($i = 0; $i < $max_i; ++$i) {
        if (is_object($dumped[$i])) {
            unset($dumped[$i]->__var_info__);
        } else {
            unset($dumped[$i]['__var_info__']);
        }
    }
    $dumped = null;

    return $txt;
}

/**
 * Private interface to {@link var_info()}
 *
 * @access private
 * @param  mixed        $var        variable to dump
 * @param  integer      $depth      nesting depth
 * @param  integer      $max_depth  maximum depth to expand.
 * @return string                   textual description of the variable
 */
function _var_info(&$var, $depth, $max_depth, &$dumped) {
    $text = '';
    $pad = str_repeat('  ', $depth);
    set_time_limit(40);

    $type = gettype($var);

    switch ($type) {
        case 'array':
            $c = count($var);
            if (isset($var['__var_info__'])) {
                $c--;
            }
            $text .= "Array[$c] ";
            if ($max_depth >= 0 && $depth >= $max_depth) {
                /*do nothing*/ ;
            } else if ($depth > 50) {
                $text .= '{ !max depth exceeded! }';
            } else if (isset($var['__var_info__'])) {
                $text .= "{ Reference to array<{$var['__var_info__']}> } ";
            } else {
                $c = count($dumped);
                $var['__var_info__'] = $c;
                $dumped[$c] =& $var;
    
                $text .= "<$c> {\n";
                reset($var);
                while(list($n) = each($var)) {
                    if ($n === '__var_info__') {
                        continue;
                    }
                    $text .= "$pad  ".trim(var_info($n, $depth + 1, $max_depth, $dumped))." => "._var_info($var[$n], $depth + 1, $max_depth, $dumped);
                }
                $text .= $pad.'}';
            }
            break;

        case 'object':
            $class = get_class($var);
            if (!class_exists($class)) {
                // Prevent PHP complaining about trying to access an unknow class
                TNTLoader::RequireClass($class);
            }
            $text .= 'Object ('.$class.') ';
            if ($max_depth >= 0 && $depth >= $max_depth) {
                /*do nothing*/ ;
            } else if ($depth > 50) {
                $text .= '{ !max depth exceeded! }';
            } else if (isset($var->__var_info__)) {
                $text .= "{ Reference to object<{$var->__var_info__}> } ";
            } else {
                $c = count($dumped);
                $var->__var_info__ = $c;
                $dumped[$c] =& $var;
                $text .= "<$c> {\n";
    
                $methods = get_class_methods(get_class($var));
                $text .= "$pad  methods {".join(', ', $methods)."}\n";
                $members = get_object_vars($var);
                reset($members);
                while(list($n) = each($members)) {
                    if ($n === '__var_info__') {
                        continue;
                    }
                    $text .= "$pad  $n -> "._var_info($var->$n, $depth + 1, $max_depth, $dumped);
                }
                $text .= $pad.'}';
            }
            break;

        case 'boolean':
            $text .= ($var ? 'true' : 'false');
            break;

        case 'resource':
            $text .= $var.' '.get_resource_type($var);
            break;

        case 'NULL':
            $text .= 'NULL';
            break;

        case 'string':
            $text .= '"'.addcslashes($var, "\0..\37\"\\'\177..\377").'"';
            break;

        case 'float':
        case 'double':
            $text .= "{$var}F";
            break;

        case 'integer':
            $text .= "{$var}";
            break;

        case 'resource':
            $text .= (string)$var;
            break;

        default:
            if (is_scalar($var)) {
                $text .= '('.gettype($var).')'.$var;
            } else {
                $text .= "!  Unsupported Type: '.$type !";
            }
    }

    $text .= "\n";
    return $text;
}

/**
 * Applies the user-defined function function to the given variable. If given an
 * array it will call the function for each element of the array array. If
 * given an object it will call it for each member variable of the object. This
 * is done recursively down through the values.
 *
 * Typically, function takes on four parameters. The array/object parameter's
 * value being first, and the key/index/member second, the nesting depth third,
 * and finally the optional userdata parameter is supplied forth.
 *
 * If function function requires more parameters than given to it, an error of
 * level E_WARNING will be generated each time array_walk() calls function.
 * These warnings may be suppressed by prepending the PHP error operator @ to
 * the array_walk() call, or by using error_reporting(). 
 *
 * If the callback function returns true and the value is itself an array or
 * object then the function will recursively walk the value as well.
 *
 * Recursive references will only be walked once.
 *
 * This will reset any walked arrays internal pointers.
 * 
 * This is similar to the PHP functions, array_walk() but it supports both
 * arrays and objects.
 *
 * @access public
 * @param  mixed        $var        reference to array or object to walk
 * @param  callback     $callback   function to callback
 * @param  mixed        $userdata   optional user data to pass to callback
 * @return void
 */
function var_walk(&$var, $callback, $userdata = null) {
    if (!is_callable($callback)) {
        trigger_error('var_walk(): Second argument is expected to be a valid callback');
    }
    if (!is_array($var) && !is_object($var)) {
        trigger_error('var_walk(): First argument is expected to be a valid array or object');
    }
    return _var_walk($var, $callback, $userdata, 0);
}

/**
 * Private interface to {@link var_walk()}
 *
 * @access private
 * @param  mixed        $var        reference to array or object to walk
 * @param  callback     $callback   function to callback
 * @param  mixed        $userdata   optional user data to pass to callback
 * @param  integer      $depth      nesting depth
 * @return void
 */
function _var_walk(&$var, $callback, $userdata = null, $depth = 0) {
    if (is_array($var)) {
        if (!isset($var['__tntwalked__'])) {
            $var['__tntwalked__'] = true;
            reset($var);
            while (list($n) = each($var)) {
                if ($n === '__tntwalked__') {
                    continue;
                }
                $ret = call_user_func($callback, $var[$n], $n, $depth, $userdata);
                if ($ret && (is_array($var[$n]) || is_object($var[$n]))) {
                    _var_walk($var[$n], $callback, $userdata, $depth + 1);
                }
            }
            unset($var['__tntwalked__']);
            reset($var);
        }
    } else if (is_object($var)) {
        if (!isset($var->__tntwalked__)) {
            $var->__tntwalked__ = true;
            $members = get_object_vars($var);
            reset($members);
            while(list($n) = each($members)) {
                if ($n === '__tntwalked__') {
                    continue;
                }
                $ret = call_user_func($callback, $var->$n, $n, $depth, $userdata);
                if ($ret && (is_array($var->$n) || is_object($var->$n))) {
                    _var_walk($var->$n, $callback, $userdata, $depth + 1);
                }
            }
            unset($var->__tntwalked__);
        }
    }
}

/**
 * Calls the object method for the given object and any contained objects.
 * 
 * Contained objects can be contained as members or values in member arrays.
 * This drills down through all member variables and array values recursively
 * so that the method is called on all child objects. 
 *
 * If the object or subobject does not support the given method then it will
 * not be called and no feedback will be given that the method was not called.
 *
 * If function function requires more parameters than given to it, an error of
 * level E_WARNING will be generated each time array_walk() calls function.
 * These warnings may be suppressed by prepending the PHP error operator @ to
 * the array_walk() call, or by using error_reporting(). 
 *
 * Recursive references will only be called once.
 *
 * This will reset any called arrays internal pointers.
 * 
 * @access public
 * @param  mixed        $var        reference to object to call method on
 * @param  callback     $callback   method to call
 * @param  array        $args       array of arguments to pass to called method
 * @return void
 */
function call_method_recursive(&$var, $method, $args = null) {
    if (!is_object($var)) {
        trigger_error('call_method_recursive(): First argument is expected to be a valid object');
    } else if (!is_string($method)) {
        trigger_error('call_method_recursive(): Second argument is expected to be a method name');
    } else {
        // First call the function on the passed in object
        if (method_exists($var, $method)) {
             call_user_func_array(array(&$var, $method), $args);
        }

        // Then walk its members to call the function on all sub objects.
        $_cb = create_function('$val,$var,$depth,$args','if (is_object($val) && !isset($val->__tntwalked__) && method_exists($val, \''.$method.'\')) { call_user_func_array(array(&$val, \''.$method.'\'), $args); } return true;');
        _var_walk($var, $_cb, $args, 0);
    }
}

/**
 * Recursive version of stripslashes
 *
 * @access public
 * @param  mixed        $val        value to strip slashes from
 * @return mixed                    value with slashes stripped
 */
function stripslashes_r($val) {
    if (is_array($val)) {
        $ret = array();
        reset($val);
        while (list($k, $v) = each($val)) {
            $ret[$k] = stripslashes_r($v);
        }
        return $ret;
    } else if (is_string($val)) {
        return stripslashes($val);
    } else {
       return $val;
    }
}

/**
 * Flatten an array to an single dimension array of name => values where
 * the name is a valid PHP name in script notation.
 *
 * For example the array ['a' => '1', 'b' => array(0 => '3', 'c' => '4')]
 * would flatten to ['a' => 1, 'b[0]' => '3', 'b['c']' => 4]
 *
 * If the second argument is set to true then the quotes are removed from
 * arround string keys in the name, this is then suitable for using in PHP
 * request urls, cookie names, html hidden variables, etc. where the PHP
 * automatically 'unflattens' the names on receiving the request.
 *
 * @access public
 * @param  array        $vars       assoc. array of values
 * @param  string       $base       base name for values
 * @return array                    flattened array
 */
function array_flatten($vars, $base = null) {
    $ret = null;
    foreach ($vars as $var => $val) {
        $name = ($base ? ($base.'['.$var.']') : $var);
        if (is_array($val)) {
            $ret = array_merge((array)$ret, array_flatten($val, $name));
        } else {
            $ret[$name] = $val;
        }
    }
    return $ret;
}

/**
 * array_merge_recursive2()
 *
 * Similar to array_merge_recursive but keyed-valued are always overwritten.
 * Priority goes to the 2nd array.
 *
 * @access public
 * @param  array        $a1         first array
 * @param  array        $a2         second array
 * @return array                    resultant merged array
 */
function array_merge_recursive2($a1, $a2) {
    foreach (array_keys($a2) as $k2) {
        if (is_array($a1[$k2]) && is_array($a2[$k2])) {
            $a1[$k2] = array_merge_recursive2(@$a1[$k2], $a2[$k2]);
        } else {
            $a1[$k2] = $a2[$k2];
        }
    }
    return $a1;
}


/**
 * Test if a string is empty.
 *
 * This method will test if the given variable is null or if its string
 * representation is a zero length string.
 *
 * @access public
 * @param  string       $value      value to test if empty
 * @return boolean                  true if empty else false
 */
function is_empty_str($value) {
    return (is_null($value) || (string)$value == '');
}

/**
 * Test if a string is blank.
 *
 * This method will test if the given variable is null or if its string
 * representation contains nothing but whitespace.
 *
 * @access public
 * @param  string       $value      value to test if blank
 * @return boolean                  true if blank else false
 */
function is_blank_str($value) {
    return (is_null($value) || $value === '' || ctype_space((string)$value));
}

/**
 * Decode a URL query string.
 *
 * This is a wrapper around the default PHP function 'parse_str'.
 *
 * @access public
 * @param  string       $str        the URL query string
 * @return array                    assoc. array of values
 * @see urlquery_encode
 */
function urlquery_decode($str) {
    $ret = null;
    @parse_str($str, $ret);
    return $ret;
}

/**
 * Encode a URL query string.
 *
 * This is the opposite of the PHP 'parse_str' function.
 *
 * @access public
 * @param  array        $vars       assoc. array of values
 * @return string                   the URL query string
 * @see urlquery_decode
 */
function urlquery_encode($value) {
    // Flatten the array
    $sep = '';
    $str = '';
    foreach (array_flatten($vars) as $n => $v) {
        $str .= $sep.urlencode($n).'='.urlencode($v);
        $sep = '&';
    }
    return $str;
}

/**
 * Check if the name is a valid variable name.
 *
 * @access public
 * @param  string       $s          string to check
 * @return boolean                  true if name is valid
 */
function is_valid_varname($s) {
    return ($s && is_string($s) && strlen($s) > 0 && strspn($s, 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789') == strlen($s));
}

/**
 * Check if the array is a numerically indexed array as opposed to an
 * associatively index array.
 *
 * @access public
 * @param  array&       $a          array to check
 * @return boolean                  true if array is numerically indexed
 */
function is_index_array(&$a) {
    if (!is_array($a)) {
        return false;
    }
    $i = 0;
    foreach (array_keys($a) as $_i) {
        if ($_i !== $i) {
            return false;
        }
        $i++;
    }
    return true;
}

/**
 * Check if the array is an associatively indexed array as opposed to an
 * numerically indexed array.
 *
 * @access public
 * @param  array&       $a          array to check
 * @return boolean                  true if array is associatively indexed
 */
function is_assoc_array(&$a) {
    if (!is_array($a)) {
        return false;
    }
    return !is_index_array($a);
}

/**
 * Get the time in microseconds
 *
 * There are various ways shown below to convert microtime() to a float;
 *
 * a) $u = (float)eval('return '.str_replace(' ', '+', microtime()).';');
 *
 * b) $m = microtime(); $u = ((float)substr($m, 0, 9) + (float)substr($m, -10));
 *
 * c) $u = (float)array_sum(explode(' ', microtime())); 
 *
 * d) $m = explode(' ', microtime()); $u = $m[1].substr($m[0], 1);
 *
 * e) $m = explode(' ', microtime()); $u = ((float)$m[0] + (float)$m[1]);
 *
 * Method e) is the fasted :-)
 *
 * @access public
 * @return float                    current time in microseconds
 */
function microsecs() {
    $_usecs = explode(' ', microtime());
    return ((float)$_usecs[0] + (float)$_usecs[1]);
}

/**
 * Cast the given variable to the given type.
 *
 * Same as PHP {@link settype()} only supports casting to user defined
 * classes.
 *
 * If the cast can not be made then null is returned otherwise a reference
 * to the cast variable is returned.
 *
 * Casting from an object or an array to a user defined class is allowed.
 *
 * @access public
 * @param  mixed&       $var        reference to variable to cast
 * @param  string       $type       type or class name
 * @return mixed&                   reference to variable after casting
 */
function &cast(&$var, $type) {
    if (!settype($var, $type)) {
        if (!class_exists($type)) {
            TNTLoader::RequireClass($type, false);
        }
        if (!class_exists($type)) {
            trigger_error("cast() - \'$type\' is not valid");
            $ret = null;
            return $ret;
        }
        $obj = (object)$var;
        $var =& new $type();
        foreach (array_keys((array)$obj) as $m) {
            $var->$m =& $obj->$m;
        }
    }
    return $var;
}

/**
 * Return the extension component of a path.
 *
 * Complements the PHP methods {@link dirname()} and {@link basename()}.
 *
 * Returns the characters after the final '.' in a path.
 *
 * This function is quicker than calling the PHP function
 * "pathinfo($path, PATHINFO_EXTENSION)".
 *
 * @access public
 * @param  string       $path       path to return extension from
 * @return string                   path extension or empty string
 */
function extname($path) {
    return (string)substr((string)strrchr($path, '.'), 1);
}

/**
 * Escape the string for use inside lanquage quotes.
 *
 * Makes the string suitable for use in PHP/SQL/C type language contexts
 * where it can be enclosed in single quotes by escape characters non
 * ascii characters and quotes with a backslash. Control characters can be
 * represented as '\n' etc. or '\000' where 000 is the octal value.
 *
 * This is a shortcut to calling the PHP function {@link addcslashes()} with
 * a list of none ascii characters and quotes to escape.
 *
 * @access public
 * @param  string       $str        string to escape
 * @return string                   escaped string
 */
function stresc($str) {
  return addcslashes($str, "\0..\37\"\\'\177..\377");
}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
