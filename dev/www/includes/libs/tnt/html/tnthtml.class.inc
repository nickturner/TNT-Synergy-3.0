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

/*
 * HTML Utility Class
 * ~~~~~~~~~~~~~~~~~~
 *
 * This is a static class providing some usefull HTML utility methods.
 *
 * @package TNT.Html
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
TNTLoader::RequireClass('TNTTag');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Static class to provide some common HTML utility methods.
 *
 * @package TNT.Html
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTHtml extends TNTClass {

    /** 
     * Split an HTML tag.
     *
     * Uses the {@link TNTTag} class to do the work.
     *
     * Returns an associative array containing the following elements;
     *   + string 'type' - tag type (see {@link TNTTag}).
     *   + string 'name' - tag name.
     *   + array  'attrs' - tag attributes as an array of name => val pairs.
     *
     * @static
     * @access  public
     * @param   string      $tag        tag to be split
     * @return  array                   the split tag
     */
    /*static*/ function SplitTag($tag) {
        $tag = new TNTTag($tag);
        $ret = $tag->Get('array');
        destroy($tag);
        return $ret;
    }

    /** 
     * Builds an HTML tag.
     *
     * Uses the {@link TNTTag} class to do the work.
     *
     * Accepts an associative array containing the following elements;
     *   + string 'type' - tag type (see {@link TNTTag}).
     *   + string 'name' - tag name.
     *   + array  'attrs' - tag attributes as an array of name => val pairs.
     *
     * @static
     * @access  public
     * @param   array       $tag        tag to be built
     * @return  string                  the built tag
     */
    /*static*/ function BuildTag($tag) {
        $tag = new TNTTag($tag);
        $ret = $tag->Get('string');
        destroy($tag);
        return $ret;
    }

    /** 
     * Encode a HTML attribute.
     *
     * Alias for {@link TNTTag::EncodeAttrVal()}.
     *
     * Automatically serializes non scalar values.
     *
     * @static
     * @access  public
     * @param   mixed       $val        value to encode
     * @return  string                  the encoded value
     */
    /*static*/ function EncodeAttrVal($val) {
        return TNTTag::EncodeAttrVal($val);
    }

    /** 
     * Decode a HTML attribute.
     *
     * Alias for {@link TNTTag::DecodeAttrVal()}.
     *
     * Automatically unserializes non scalar values.
     *
     * @static
     * @access  public
     * @param   string      $val        value to decode
     * @return  mixed                   the decoded value
     */
    /*static*/ function DecodeAttrVal($val) {
        return TNTTag::DecodeAttrVal($val);
    }

    /** 
     * Hidden value.
     *
     * Converts a PHP value to a series of HTML hidden control tags for
     * embedding in a form.
     *
     * @static
     * @access  public
     * @param   string      $name       name of variable to hide
     * @param   mixed       $val        value of variable to hide
     * @return  string                  the HTML value
     */
    /*static*/ function HiddenVal($name, $value) {
        $ret = null;
        if (is_object($value) || is_array($value)) {
            $ret = '';
            foreach ((array)$value as $n => $v) {
                $ret .= TNTHtml::HiddenVal($name."[$n]", $v);
            }
        } else {
            $ret .= "<input type=\"hidden\" name=\"".TNTHtml::EncodeAttrVal($name)."\" value=\"".TNTHtml::EncodeAttrVal($value)."\" />\n";
        }
        return $ret;
    }

    /** 
     * Hidden values.
     *
     * Converts an array of values to a series of HTML hidden control tags.
     *
     * @static
     * @access  public
     * @param   array       $vals       array of name => value pairs
     * @return  string                  the HTML value (or null)
     */
    /*static*/ function HiddenVals($values) {

        $ret = null;

        if (is_array($values)) {
            $ret = '';
            foreach ($values as $n => $v) {
                $ret .= TNTHtml::HiddenVal($n, $v);
            }
        }

        return $ret;
    }

    /** 
     * Builds an HTML tag.
     *
     * Uses the {@link TNTTag} class to do the work.
     *
     * Alternative interface to {@link BuildTag()}.
     *
     * @static
     * @access  public
     * @param   string      $name       tag name.
     * @param   array       $attrs      tag attributes as an array of name => val pairs.
     * @param   string      $type       tag type (see {@link TNTTag}).
     * @return  string                  the built tag
     */
    /*static*/ function Tag($name, $attrs = null, $type = TNT_TAG_OPEN) {
        return TNTHtml::BuildTag(array('name' => $name, 'attrs' => $attrs, 'type' => $type));
    }

    /** 
     * Builds an HTML Block tag.
     *
     * Uses the {@link TNTTag} class to do the work.
     *
     * @static
     * @access  public
     * @param   string      $name       tag name.
     * @param   array       $attrs      tag attributes as an array of name => val pairs.
     * @param   string      $content    tag content
     * @return  string                  the built tag
     */
    /*static*/ function BlockTag($name, $attrs = null, $content = '') {
        return TNTHtml::Tag($name, $attrs)."$content</$name>";
    }

    /**
     * Return value of variable in JavaScript.
     *
     * Return the correct JavaScript code to represent the value of a given
     * variable.
     *
     * If the value is a string containing just a valid jscript variable name
     * prefixed with '&' then it is assumed to mean the address of the variable
     * and thus is not enclosed in quotes. Use this to pass variables and
     * method names.
     *
     * Recursive references are not supported !!!
     *
     * @access  public
     * @param   mixed&      $value      reference to variable to convert.
     * @param   boolean     $incPrivate include private members
     * @return  string                  JavaScript representing variable
     */ 
    function ValueToJavaScript(&$value, $incPrivate = true) {

        if (is_null($value)) {
            $ret = 'null';
        } else if (is_bool($value)) {
            $ret = ($value ? 'true' : 'false');
        } else if (is_string($value)) {
            if ($value == '') {
                $ret = '\'\'';
            } else if ($value{0} == '&' && preg_match('/&[\w.]+/', $value)) {
                $ret = substr($value, 1);
            } else {
                $ret = '\''.addcslashes($value, "\0..\37'\"\177..\377").'\'';
            }
        } else if (is_resource($value)) {
            $ret = 'null';
        } else if (is_scalar($value)) {
            $ret = (string)$value;
        } else if (is_object($value)) {
            // This assumes object properties are valid javascript properties
            $ret = null;
            foreach (array_keys((array)$value) as $n) {
                if ($n{0} == '_' && !$incPrivate) {
                    continue;
                }
                if ($ret) {
                    $ret .= ', ';
                }
                $ret .= $n.' : done'; //'.TNTHml::ValueToJavaScript($value->$n, $incPrivate);
            }
            $ret = "{ $ret }";
        } else if (is_array($value) && is_index_array($value)) {
            // Assumes array is sequential from zero upwards
            $ret = null;
            foreach (array_keys($value) as $n) {
                if ($ret) {
                    $ret .= ', ';
                }
                $ret .= TNTHtml::ValueToJavaScript($value[$n], $incPrivate);
            }
            $ret = "[ $ret ]";
        } else if (is_array($value)) {
            // Assumes array indices can only be numbers or strings
            $ret = null;
            foreach (array_keys($value) as $n) {
                if (is_string($n) && $n{0} == '_' && !$incPrivate) {
                    continue;
                }
                if ($ret) {
                    $ret .= ', ';
                }
                $ret .= (is_string($n) ? '\''.addcslashes($n, "\0..\37'\"\177..\377").'\'' : $n).' : '.TNTHtml::ValueToJavaScript($value[$n], $incPrivate);
            }
            $ret = "{ $ret }";
        } else {
            $ret = 'null';
        }

        return $ret;
    }

    /**
     * Return the JavaScript to construct an object of the given class.
     *
     * Return the correct JavaScript code to construct an object of the
     * given class and then initialise its properties as required.
     *
     * The arguments to be passed to the constructor are given as an array of
     * values, and properties to be initialised are given as an associative
     * array of property => value pairs.
     *
     * After assigning all the properties the 'init()' method of the object is
     * called if it exists.
     *
     * @access  public
     * @param   string      $name       name of javascript object
     * @param   string      $class      name of javascript class
     * @param   array       $args       array of constructor arguments
     * @param   array       $props      associative array of extra properties
     * @return  string                  JavaScript to create the object
     */ 
    function JavaScriptObject($name, $class, $args = null, $props = null) {

        $jsargs = null;
        if ($args) {
            foreach ($args as $v) {
                $jsargs[] = TNTHtml::ValueToJavaScript($v);
            }
            $jsargs = implode(',', $jsargs);
        }

        $ret = "$name = new $class($jsargs);\n";

        if ($props) {
            // This assumes object properties are valid javascript properties
            foreach ($props as $n => $v) {
                $ret .= "{$name}['{$n}'] = ".TNTHtml::ValueToJavaScript($v).";\n";
            }
        }

        $ret .= "if ({$name}.init && typeof {$name}.init == 'function') { {$name}.init(); }\n";

        return $ret;
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
