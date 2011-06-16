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
 * Tag Manipulation Class
 * ~~~~~~~~~~~~~~~~~~~~~~
 *
 * This class represents SGML tags as objects which can be manipulated.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version     2.0
 */

/**
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTFmtStr');

/*
 * Globals
 */
global $tnt;
$tnt['tnttag']['_elementNames'] = array('type', 'name', 'attrs');

/*
 * Defines
 */
define('TNT_TAG_OPEN',      'open');    //  open tag (ie. <p>)
define('TNT_TAG_CLOSE',     'close');   // close tag (ie. </p>)
define('TNT_TAG_EMPTY',     'empty');   // empty tag (ie. <p />)

/**
 * Class to represent a SGML Tag
 *
 * SGML Tags are not the same as Elements. Some people refer to elements as
 * tags (e.g., "the P tag"). Remember that the element is one thing, and the
 * tag (be it start or end tag) is another. For instance, the HEAD element is
 * always present in an HTML document, even though both start and end HEAD
 * tags may be missing in the markup.
 *
 * The SGML Element can be considered as the Tag and Content.
 *
 * This is a formatted string consisting of the following named elements;
 *     'type'   - Tag type ('open'|'close'|'empty')
 *     'name'   - Tag name (string)
 *     'attrs'  - Tag attributes (associative array of strings)
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTTag extends TNTFmtStr {

    /**
     * Encode an attributes value
     *
     * Non scalar values are automativally serialized.
     *
     * @static
     * @access  public
     * @param   scalar      $value      value to encode
     * @return  string                  encoded value
     * @see TNTFmtStr
     */
    /*static*/ function EncodeAttrVal($value) {

        if (!is_scalar($value)) {
            $value = 'z:'.serialize($value);
        }

        //$ret = htmlentities($value);
        // We only translate quotes
        return str_replace(array('\'', '"'), array('&#039;', '&quot'), $value);
    }

    /**
     * Decode an attributes value
     *
     * Serialized values are automatically unserialized.
     *
     * @static
     * @access  private
     * @param   string      $value      value to decode
     * @return  scalar                  decoded value
     * @see TNTFmtStr
     */
    /*static*/ function DecodeAttrVal($value) {

        // All standard HTML entities
        $ret = html_entity_decode($value);

        // Anything left ?
        $ret = preg_replace('/\&\#([0-9]+)\;/me', "chr('\\1')", $ret);

        if (is_numeric($ret)) {
            $ret = (int)$ret;
        }
        if (is_string($ret) && strncmp($ret, 'z:', 2) == 0 &&
            ($data = unserialize($ret)) !== false) {
            $ret = $data;
        } 

        return $ret;
    }

    /**
     * Explode the Tag into the elements
     *
     * @access  private
     * @return  boolean                 true if successfull
     * @see TNTFmtStr
     */
    function _Explode() {

        $ret = false;

        $this->_elements = null;

        // Start error section
        do {
            if (empty($this->_string)) {
                $this->_elements = null;
                $ret = true;
                break;
            }

            $matches = array();
            preg_match('/^<\s*(\\/?\w*)\s*(.*)(\\/?)>$/s',
                trim($this->_string), $matches);
    
            if (empty($matches[1])) {
                $this->_SetError('can not split: '.$this->_string);
                return null;
            }
    
            $open = (($matches[1][0] == '/') ? 0 : 1);
            $name = substr($matches[1], $open);
            $data = (isset($matches[2]) ? trim($matches[2]) : '');
            $attrs = null;
    
            // Check this is not an open and shut case
            if (isset($matches[3]) && $matches[3] == '/') {
                if ($open) {
                    $open++;
                } else {
                    $this->_SetError('invalid tag syntax: '.$this->_string);
                    break;
                }
            }
    
            if ($data) {
                if (preg_match_all('/(\w*)\s*=\s*([\'"])?(.*)(\2)/Us',
                    $data, $matches, PREG_SET_ORDER) < 0) {
                    $this->_SetError('invalid attribute syntax: '.$this->_string);
                    break;
                }
                $max_i = count($matches);
                for ($i = 0; $i < $max_i; $i++) {
                    $attrs[trim(strtolower($matches[$i][1]))] =
                        $this->DecodeAttrVal($matches[$i][3]);
                }
            }

            static $types = array(0 => 'close', 1 => 'open', 2 => 'empty');

            $this->_elements['type'] = $types[$open];
            $this->_elements['name'] = $name;
            $this->_elements['attrs'] = $attrs;

            $ret = true;

        } while (false);
        // End error context
    
        return $ret;
    }
    
    /**
     * Implode the elements into the Tag
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function _Implode() {

        $ret = true;

        $this->_string = null;

        $e =& $this->_elements;
        $s =& $this->_string;

        $s = null;

        if (!empty($e['type'])) {
            $s = '<';
            if ($e['type'] == 'close') {
                $s .= '/';
            }
            $s .= $e['name'];
            if ($e['type'] != 'close' && isset($e['attrs'])) {
                foreach ($this->_elements['attrs'] as $n => $v) {
                    if (is_null($v)) {
                        continue;
                    }
                    $s .= ' '.$n.'="'.$this->EncodeAttrVal($v).'"';
                }
            }
            if ($e['type'] == 'empty') {
                $s .= ' /';
            }
            $s .= '>';
        }

        return true;
    }

    /**
     * Get value of a tag attribute
     *
     * Non scalar values are automatically serialized on {@link SetAttr()}
     * and unserialzed on {@link GetAttr()}.
     *
     * @access  public
     * @param   string      $name       attribute name
     * @return  mixed                   attribute value (or null)
     * @see     SetAttr
     */
    function GetAttr($name) {

        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        $name = trim(strtolower($name));

        if (isset($this->_elements[$name])) {
            $ret = $this->_elements[$name];
        }

        return $ret;
    }

    /**
     * Set value of a tag attribute
     *
     * Non scalar values are automatically serialized on {@link SetAttr()}
     * and unserialzed on {@link GetAttr()}.
     *
     * @access  public
     * @param   string      $name       element name
     * @param   mixed       $val        element value
     * @return  string                  data with element set
     * @see     GetAttr
     */
    function SetAttr($name, $val) {

        $ret = true;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if (!is_scalar($val)) {
            $this->_SetError("Non scalar attribute value $n=" , $v);
            $ret = false;
        } else {
            $name = trim(strtolower($name));

            $ret = (isset($this->_elements[$name]) ? $this->_elements[$name] : null);

            $this->_elements[$name] = $val;
        }
        
        return $ret;
    }

    /**
     * Helper function to get a element in a Tag string
     *
     * @access  public
     * @param   mixed       $data       initial Tag data
     * @param   string      $elm        element name
     * @return  string                  element value (or null)
     * @static
     */
    /*static*/ function GetTagElement($data, $elm) {
        return parent::GetDataElement(__CLASS__, $data, $elm);
    }

    /**
     * Helper function to set a element in a Tag string
     *
     * @access  public
     * @param   mixed       $data       initial Tag data
     * @param   string      $elm        element name
     * @param   string      $val        element value
     * @return  string                  data with element set
     * @static
     */
    /*static*/ function SetTagElement($data, $elm, $val) {
        return parent::SetDataElement(__CLASS__, $data, $elm, $val);
    }
    
    /**
     * Construct the object
     *
     * Any initial value is passed to {@link TNTTag::Set()}
     *
     * @access  public
     * @param   mixed       $data       initial value for object
     * @return  void
     */
    function TNTTag($data = null) {
        TNTFmtStr::TNTFmtStr($data, $GLOBALS['tnt']['tnttag']['_elementNames']);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTTag() {
    }
    
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
