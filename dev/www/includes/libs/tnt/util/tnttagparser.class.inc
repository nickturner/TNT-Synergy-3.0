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
 * Tag Parser Class
 * ~~~~~~~~~~~~~~~~
 *
 * This is an abstract base class used by the event based tag parsers to build
 * a hierachical data structure from the parsed data. Different styles of output
 * are supported, from ordered trees to grouping trees. Various post production
 * processing is supported as well such as node condensing etc.
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

/*
 * Globals
 */

/*
 * Defines
 */
define('TNT_TAGPARSER_OPTS_CASEUPPER',          0x0001);
define('TNT_TAGPARSER_OPTS_CASELOWER',          0x0002);

define('TNT_TAGPARSER_OPTS_KEEPCLOSE',          0x0004);

define('TNT_TAGPARSER_OPTS_SKIPCOMMENTS',       0x0010);
define('TNT_TAGPARSER_OPTS_SKIPPI',             0x0020);
define('TNT_TAGPARSER_OPTS_SKIPCDATA',          0x0040);
define('TNT_TAGPARSER_OPTS_SKIPWHITE',          0x0080);

define('TNT_TAGPARSER_OPTS_TRIM',               0x0100);
define('TNT_TAGPARSER_OPTS_REDUCE',             0x0200);
define('TNT_TAGPARSER_OPTS_CONDENSE',           0x0400);
define('TNT_TAGPARSER_OPTS_GROUP',              0x0800);
define('TNT_TAGPARSER_OPTS_MERGE',              0x1000);

define('TNT_TAGPARSER_OPTS_CASEMASK',           0x0003);

define('TNT_TAGPARSER_OPTS_SKIPALL',            0x00F0);

// Return a simple content centric view of the data
define('TNT_TAGPARSER_OPTS_SIMPLE',             0xFFF2);

define('TNT_TAGPARSER_ERROR_NONE',              0);
define('TNT_TAGPARSER_ERROR_BAD_FILE',          1);
define('TNT_TAGPARSER_ERROR_EMPTY_FILE',        2);
define('TNT_TAGPARSER_ERROR_NO_MEMORY',         3); 
define('TNT_TAGPARSER_ERROR_SYNTAX',            4); 
define('TNT_TAGPARSER_ERROR_NO_ELEMENTS',       5); 
define('TNT_TAGPARSER_ERROR_INVALID_TOKEN',     6); 
define('TNT_TAGPARSER_ERROR_UNCLOSED_TOKEN',    7); 
define('TNT_TAGPARSER_ERROR_TAG_MISMATCH',      8); 
define('TNT_TAGPARSER_ERROR_DUP_ATTRIBUTE',     9);
define('TNT_TAGPARSER_ERROR_JUNK_AFTER_ROOT',   10);
define('TNT_TAGPARSER_ERROR_UNDEFINED_ENTITY',  11); 
define('TNT_TAGPARSER_ERROR_INTERNAL',          12); 

/**
 * Structure to represent a parsed tag node
 *
 * <pre>
 * struct TNTTagParserNode {
 *   'type'        - tag type
 *   'tag'         - element tag name
 *   'attributes'  - associative array of attribute name=>value pairs
 *   'content'     - array of child tags or reduced child CDATA content
 *   'values'      - associative array of condensed child content
 *   'position'    - tag position (associative array)
 *   'pdata'       - associative array of optional parser data
 * }
 * </pre>
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTTagParserNode {
    var $type;          // type (opened|closed|empty|block|etc.)
    var $tag;           // name
    var $attributes;    // associative array of attribute name=>value pairs
    var $values;        // associative array of children tag=>content pairs
    var $content;       // content (string CDATA or TNTTagParserNode[] children)
    var $position;      // associative array of tag positions ('open' & 'close')
    var $pdata;         // optional parser information
};

/**
 * Abstract class to handle the parsing of tag tree formated data such as XML
 * and HTML.
 *
 * The parser produces a tag tree, and then can optional process that tag tree
 * to gain different output styles.
 *
 * In its default state the tag tree, contains an hierarchical tree of
 * TNTTagParserNode structures, each tag is contains the following members,
 *        'type'        - tag type
 *        'tag'         - element tag name
 *        'attributes'  - associative array of attribute name=>value pairs
 *        'content'     - array of child tags or reduced child CDATA content
 *        'values'      - associative array of condensed child content
 *        'position'    - tag position (associative array)
 *        'pdata'       - associative array of optional parser data
 *
 *    Tag types are;
 *        'open'        - open tag (eg. <tag>)
 *        'close'       - close tag (eg. </tag>)
 *        'empty'       - empty tag (eg. <tag />)
 *        'block'       - block tag (eg. <tag></tag>)
 *        'cdata'       - cdata section (eg. "this is cdata")
 *        'comment'     - comment (eg. "<!-- this is a comment -->")
 *        'pi'          - processing instruction (eg. "<?...?>");
 *        'notation'    - notation declaration (eg. "<!...>");
 *
 *    'close' tags are only stored in the tree in 'raw' mode.
 *    'empty' tags the content is null.
 *
 *    A further notation of 'simple' tag is used and this referes to a tag
 *    which has no 'attributes' or 'values' but may have 'content'.
 *
 *    The handler will allow a tag type of 'close_empty' to close an 'open' tag
 *    and convert it to a 'empty' tag. This is used when the underlying parser
 *    doesn't distinquish 'empty' tags separately. A tag in the tree will never
 *    have the 'close_empty' type.
 *
 *    'close' tags are not normally stored in the tag tree, they are used by
 *    the handler to close an open tag. An option can be set to cause these
 *    'close' tags to remain in the tree as the last child of the 'open' tag.
 *
 * Optional post processing;
 *   a) Reduction
 *      Tags with a single 'cdata' child node can be optionally reduced to
 *      hold the 'cdata' nodes content as its content, deleting the child.
 *
 *   b) Condensing
 *      'simple' child tags containing no attributes or values are removed and
 *      their content is added to the parent tags 'values' array as
 *      'tagname'=>'content'.
 *      
 *   c) Grouping
 *      Child tags are grouped together as arrays of tags of a given name
 *      indexed by the tag name.
 *
 *   d) Merging
 *      This must be the last stage as it destroys the internal TNTTagParserNode
 *      structures and replaces them with simple associative values which
 *      are either the tags content for 'simple' tags or a merger of the
 *      tags 'attributes', 'values' and 'content'. If the content is a string
 *      it is merged in to the array with the index '_content'.
 *      
 * Output is governed by the following options;
 *    TNT_TAGPARSER_OPTS_CASEUPPER      - convert tag & attr names to uppercase
 *    TNT_TAGPARSER_OPTS_CASELOWER      - convert tag & attr names to uppercase
 *
 *    TNT_TAGPARSER_OPTS_SKIPCOMMENTS   - skip comments
 *    TNT_TAGPARSER_OPTS_SKIPPI         - skip processing instructions
 *    TNT_TAGPARSER_OPTS_SKIPCDATA      - skip cdata (unless reduced)
 *    TNT_TAGPARSER_OPTS_SKIPWHITE      - skip cdata containing only whitespace
 * 
 *    TNT_TAGPARSER_OPTS_TRIM           - trim leading/trailing CDATA whitespace
 *    TNT_TAGPARSER_OPTS_REDUCE         - single child CDATA tags become content
 *    TNT_TAGPARSER_OPTS_CONDENSE       - simple children become 'values'
 *    TNT_TAGPARSER_OPTS_GROUP          - group child tags by tag name
 * 
 * 
 * Example;
 *
 * <moldb> 
 *   <molecule code="A" symbol="ala"> 
 *     <name>Alanine</name> 
 *     <type>hydrophobic</type> 
 *     <description language="en">
 *       Alanine is a hydrophobic molecule
 *     </description>
 *   </molecule> 
 *   <molecule code="K" symbol="lys"> 
 *     <name>Lysine</name> 
 *     <type>charged</type> 
 *     <description language="en">
 *       Lysine is a charged molecule
 *     </description>
 *   </molecule> 
 * </moldb>
 *
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTTagParser extends TNTObject {
    /**
     * Processing tag stack
     * @var     TNTTagParserNode[]                 document root
     * @access  public
     */
    var $_stack = null;

    /**
     * Error code given by TAG parser
     * @var     integer                 Parser error code
     * @access  public
     */
    var $errorcode = 0;

    /**
     * Number of line that the parser failed on
     * @var     integer                 Parser error line
     * @access  public
     */
    var $errorline = -1;

    /**
     * Position on the line that the parser failed on
     * @var     integer                 Parser error column
     * @access  public
     */
    var $errorchar = -1;

    /**
     * Position in the file that the parser failed on
     * @var     integer                 Parser error byte
     * @access  public
     */
    var $errorbyte = -1;

    /**
     * Textual description of parser error
     * @var     integer                 Parser error text
     * @access  public
     */
    var $errortext = '';

    /**
     * Parse options
     * @var     integer                 bitwise or of TNT_TAGPARSER_xxx defines
     * @access  private
     */
    var $options = 0;

    /**
     * Data being parsed
     * @var     integer                 data being parsed
     * @access  private
     */
    var $_data = null;

    /** 
     * Convert the case of a string
     *
     * @access  private
     * @param   string      $str        string to convert case of
     * @return  string                  modified string
     */
    function _ConvertCase($str) {
        if ($this->options & TNT_TAGPARSER_OPTS_CASEUPPER) {
            return strtoupper($str);
        } else if ($this->options & TNT_TAGPARSER_OPTS_CASELOWER) {
            return strtolower($str);
        } else {
           return $str;
        }
    }

    /** 
     * Convert the case of a attribute names
     *
     * @access  private
     * @param   string      $str        string to convert case of
     * @return  string                  modified string
     */
    function _ConvertAttrs($attributes) {

        if (!is_array($attributes)) {
            return $attributes;
        }
        if (!($this->options & (TNT_TAGPARSER_OPTS_CASEUPPER|TNT_TAGPARSER_OPTS_CASELOWER))) {
            return $attributes;
        }

        $nattributes = array();
        if (function_exists('array_change_key_case')) {
            // Internal function will be quicker if available :-)
            if ($this->options & TNT_TAGPARSER_OPTS_CASEUPPER) {
                $nattributes = array_change_key_case($attributes, CASE_UPPER);
            } else {
                $nattributes = array_change_key_case($attributes, CASE_LOWER);
            } 
        } else {
            foreach($attributes as $n => $v) {
                $nattributes[$this->_ConvertCase($n)] = $v;
            }
        }

        return $nattributes;
    }

    /** 
     * Trim data
     *
     * @access  private
     * @param   string      $str        string to trim
     * @return  string                  modified string
     */
    function _Trim($str) {
        if ($this->options & TNT_TAGPARSER_OPTS_TRIM) {
            return trim($str);
        } else {
            return $str;
        }
    }

    /** 
     * Adds a tag to the child list of the top tag and optionally pushes it
     * onto the stack depending on the tag type.
     *
     * The parser event call backs should call this with the correct arguments
     * depending on the parsing state.
     *
     * When parsing a 'complete' tag (eg '<tag/>') the parse may either call
     * the handler once with a 'complete' type tag or may call the handler
     * twice first with an 'open' and then with a 'close_complete' (as opposed
     * to a 'close'). If the parser calls 'open' then 'close' then a 'block'
     * type tag is assumed.
     *
     * Where possible the parser should provide the $info parameter as an
     * associative array containing at least the following items;
     *   'line' - line number at which the tag was found
     *   'char' - character number on line at which the tag was found
     *   'byte' - byte index at which the tag was found
     * The parser can return other information if required as this structure
     * is stored in the tag unaltered.
     *
     * @access  private
     * @param   string      $type       type
     * @param   string      $tag        tag name
     * @param   array       $pos        tag position ('byte', 'line', 'char')
     * @param   array       $attributes associative array of element attributes
     * @param   string      $content    tag content where known
     * @return  boolean                 true if successfull
     */
    function _HandleTag($type, $tag, $position = null, $attributes = null, $content = null, $pdata = null) {

        static $types = array('open', 'close', 'block', 'complete',
                              'cdata', 'comment', 'pi', 'notation',
                              'close_complete');

        $ret = true;

        if (($this->options & TNT_TAGPARSER_OPTS_SKIPCOMMENTS) &&
            $type == 'comment') {
            return true;
        }

        if (($this->options & TNT_TAGPARSER_OPTS_SKIPWHITE) &&
            $type == 'cdata' && trim($content) == '') {
            return true;
        }

        if (empty($tag)) {
            $tag = "<$type>";
        }

        $open = ($type == 'open');
        $close = ($open ? false : (strncmp($type, 'close', 5) == 0));

        // If not open or close then must be a complete tag

        if ($close && !is_null($attributes)) {
            $this->_Error(TNT_TAGPARSER_ERROR_SYNTAX,
                          'close tag "'.$tag.'" can not have attributes (tag syntax)',
                          $position);
            return false;
        }

        if (($close || $open) && !is_null($content)) {
            $this->_Error(TNT_TAGPARSER_ERROR_INTERNAL,
                          'open or close tags "'.$tag.'" can not have content (tag internal)',
                          $position);
            return false;
        }

        if (!in_array($type, $types)) {
            $this->_Error(TNT_TAGPARSER_ERROR_INTERNAL,
                          'invalid tag type "'.$type.'" (internal error)',
                          $position);
            return false;
        }

        $tag = $this->_ConvertCase($tag);

        // How deep is the stack
        $depth = count($this->_stack);

        // Get reference to node at top of stack
        $top =& $this->_stack[$depth - 1];

        if ($close) {
            // If we are not closing the current open tag then stop with error
            if ($top->tag != $tag) {
                $this->_Error(TNT_TAGPARSER_ERROR_TAG_MISMATCH,
                              'closing wrong tag (tag mismatch)',
                              $position);
                return false;
            }
            if ($top->type != 'open') {
                $this->_Error(TNT_TAGPARSER_ERROR_TAG_MISMATCH,
                              'close tag with no open tag (tag mismatch)',
                              $position);
                return false;
            }

            // An open tag when closed becomes a block tag
            $top->type = (($type == 'close_empty') ? 'empty' : 'block');

            $top->position['close'] = $position;

            $top->pdata += $pdata;

            // Optimise processing of the tag now - saves time later

            // Reduce the tags children ?
            if ($this->options & TNT_TAGPARSER_OPTS_REDUCE) {
                $this->_ReduceTag($top);
            }
            // Condense the tag children ?
            if ($this->options & TNT_TAGPARSER_OPTS_CONDENSE) {
                $this->_CondenseTag($top);
            }
            // Group the tags children ?
            if ($this->options & TNT_TAGPARSER_OPTS_GROUP) {
                $this->_GroupTag($top);
            }
            // Merge the tags children ?
            if ($this->options & TNT_TAGPARSER_OPTS_MERGE) {
                // We can only merge children - not the tag its self
                $this->_RecurseChildTags($top, '_MergeTag');
            }

            // pop back down the stack
            array_pop($this->_stack);
        } 

        if (!$close || ($this->options & TNT_TAGPARSER_OPTS_KEEPCLOSE)) {

            if ($attributes) {
                $attributes = $this->_ConvertAttrs($attributes);
            }

            if ($type == 'cdata') {
                if ($this->options & TNT_TAGPARSER_OPTS_TRIM) {
                    $content = preg_replace('/\s+/', ' ', $content);
                }

                // If previous child was 'cdata' then append it and return
                if (is_array($top->content) &&
                    ($children = count($top->content)) &&
                    $top[$children - 1]->type == 'cdata') {
                    $child->content .= $content;
                    return;
                }
            }

            $o = new TNTTagParserNode();
            $o->type = $type;
            $o->tag = $tag;
            $o->depth = $depth;
            $o->attributes = $attributes;
            $o->content = $content;
            $o->position = array('open' => $position);
            $o->pdata = $pdata;

            if (!$top->content) {
                $top->content = array();
            }

            // Add tag to children of top node
            $top->content[] = $o;
            $c = count($top->content);

            if ($type == 'open') {
                // Push this node on to the stack - any more nodes are children
                $this->_stack[] =& $top->content[$c - 1];
            }
        }

        return true;
    }

    /** 
     * Recurse through all the descendants of the given tag and call the given
     * method for each child tag, passing a reference to the tag as the only
     * parameter.
     *
     * @access  private
     * @param   TNTTagParserNode      $tag        reference to tag to recurse from
     * @param   string      $method     name of method to call
     * @return  void
     */
    function _RecurseChildTags(&$tag, $method) {

        if (is_array($tag->content)) {
            // Scan all children 
            foreach (array_keys($tag->content) as $ci) {
                // Children will be arrays if Group'ed before hand
                if (is_array($tag->content[$ci])) {
                    foreach (array_keys($tag->content[$ci]) as $cci) {
                        // Call method on child tag
                        $this->$method($tag->content[$ci][$cci]);
                    }
                } else {
                    // Call method on child tag
                    $this->$method($tag->content[$ci]);
                }
            }
        }

    }

    /** 
     * Reduce the tag 
     *
     * @access  private
     * @param   array       &$tag       tag to reduce
     * @return  void
     */
    function _ReduceTag(&$tag) {

        if ($tag->content && is_array($tag->content) &&
            count($tag->content) == 1 && is_a($tag->content[0], 'tnttagparsernode') &&
            $tag->content[0]->type == 'cdata') {

            $tag->content = $tag->content[0]->content;
        }

    }

    /** 
     * Reduce the tag tree starting at the given tag.
     *
     * This function works recursively, reducing all a tags children before
     * attempting to reduce the tag itself.
     *
     * @access  private
     * @param   array       &$tag       tag to reduce
     * @return  void
     */
    function _ReduceTree(&$tag) {

        // Reduce all descendants of this tag
        $this->_RecurseChildTags($tag, __FUNCTION__);

        // Reduce this tag
        $this->_ReduceTag($tag);

    }

    /** 
     * Reduce parsed tag tree
     *
     * This function reduces the tag tree produced by the parser. That is if
     * a tag contains a single cdata child it will remove the child and copy
     * its 'content' to the tags 'content'.
     *
     * This function works recursively, reducing all a tags children before
     * attempting to reduce the tag itself.
     *
     * This function is called automatically after parsing the tags if the
     * TNT_TAGPARSER_OPTS_REDUCE flag is set. However it is available as
     * a separate function to allow the tree to be reduced at a later time
     * or so the post processing order can be user defined.
     *
     * @access  private
     * @return  TNTTagParserNode           reference to root tag (or null)
     */
    function &Reduce() {
        if (is_array($this->_stack) && is_a($this->_stack[0], 'tnttagparsernode')) {
            $this->_ReduceTree($this->_stack[0]);
        }

        $ret =& $this->Root();

        return $ret;
    }

    /** 
     * Condense the given tag.
     *
     * @access  private
     * @param   array       &$tag       tag to condense
     * @return  void
     */
    function _CondenseTag(&$tag) {

        if (is_array($tag->content)) {
            // Scan all children 
            foreach (array_keys($tag->content) as $ci) {
               if (is_a($tag->content[$ci], 'tnttagparsernode')) {
                    // Condense this child
                    $ct = $tag->content[$ci];
                    if (!$ct->attributes && !$ct->values &&
                        !is_array($ct->content)) {
                        if (!isset($tag->values)) {
                            $tag->values = array();
                        }
                        if (isset($tag->values[$ct->tag])) {
                            settype($tag->values[$ct->tag], 'array');
                            $tag->values[$ct->tag][] = $ct->content;
                        } else {
                            $tag->values[$ct->tag] = $ct->content;
                        }
                        unset($tag->content[$ci]);
                    }
                }
            }

            // Did we remove all the children?
            if (count($tag->content) == 0) {
                $tag->content = null;
            }
        }

    }

    /** 
     * Condense the tag tree starting at the given tag.
     *
     * This function works recursively, condensing all a tags children before
     * attempting to condense the tag itself.
     *
     * @access  private
     * @param   array       &$tag       tag to condense
     * @return  void
     */
    function _CondenseTree(&$tag) {

        // Condense all descendants of this tag
        $this->_RecurseChildTags($tag, __FUNCTION__);

        // Condense this tag
        $this->_CondenseTag($tag);

    }

    /** 
     * Condense parsed tag tree
     *
     * This function condenses a tag. That is all children that contain no
     * 'attributes' or 'values' have their 'content' added to their parents
     * 'values' indexed by child tag name. They are then removed as children.
     * If multiple children use the same tag name then the 'values' value will
     * be an array of the each of the child 'content'.
     *
     * This function works recursively, condensing all a tags children before
     * attempting to condense the tag itself.
     *
     * This function is called automatically after parsing the tags if the
     * TNT_TAGPARSER_OPTS_CONDENSE flag is set. However it is available as
     * a separate function to allow the tree to be condensed at a later time
     * or so the post processing order can be user defined.
     *
     * @access  private
     * @return  TNTTagParserNode           reference to root tag (or null)
     */
    function &Condense() {

        if (is_array($this->_stack) && is_a($this->_stack[0], 'tnttagparsernode')) {
            $this->_CondenseTree($this->_stack[0]);
        }

        $ret =& $this->Root();

        return $ret;
    }

    /** 
     * Group the tag
     *
     * @access  private
     * @param   array       &$tag       tag to group
     * @return  void
     */
    function _GroupTag(&$tag) {

        if (is_array($tag->content)) {
            $content = array();
            foreach ($tag->content as $ci => $ct) {
                if (is_a($tag->content[$ci], 'tnttagparsernode')) {
                    // Group this child
                    $ct = $tag->content[$ci];
                    if (isset($content[$ct->tag])) {
                        if (!is_array($content[$ct->tag])) {
                            $content[$ct->tag] = array($content[$ct->tag]);
                        }
                        $content[$ct->tag][] = $tag->content[$ci];
                    } else {
                        $content[$ct->tag] = $tag->content[$ci];
                    }
                } else {
                    $content[$ci] = $tag->content[$ci];
                }
            }
            $tag->content = $content;
        }

    }

    /** 
     * Group the tag tree starting at the given tag.
     *
     * This function works recursively, grouping all a tags children before
     * attempting to group the tag itself.
     *
     * @access  private
     * @param   array       &$tag       tag to group
     * @return  void
     */
    function _GroupTree(&$tag) {

        // Group all descendants of this tag
        $this->_RecurseChildTags($tag, __FUNCTION__);

        // Group this tag
        $this->_GroupTag($tag);

    }

    /** 
     * Group parsed tag tree
     *
     * This function groups the tag tree produced by the parser. That is all a
     * tags children are grouped together indexed by the childs tag name.
     *
     * This function is called automatically after parsing the tags if the
     * TNT_TAGPARSER_OPTS_GROUP flag is set. However it is available as
     * a separate function to allow the tree to be groupd at a later time
     * or so the post processing order can be user defined.
     *
     * @access  private
     * @return  TNTTagParserNode           reference to root tag (or null)
     */
    function &Group() {

        if (is_array($this->_stack) && is_a($this->_stack[0], 'tnttagparsernode')) {
            $this->_GroupTree($this->_stack[0]);
        }

        $ret =& $this->Root();

        return $ret;
    }

    /** 
     * Merge the tag
     *
     * @access  private
     * @param   array       &$tag       tag to group
     * @return  void
     */
    function _MergeTag(&$tag) {

        $v = null;
        if (is_array($tag->attributes)) {
            $v = (is_array($v) ? array_merge($v, $tag->attributes) : $tag->attributes);
        }
        if (is_array($tag->content)) {
            $v = (is_array($v) ? array_merge($v, $tag->content) : $tag->content);
        } else if ($tag->content) {
            $v = (is_array($v) ? array_merge($v, array('_content' => $tag->content)) : $tag->content);
        }
        if (is_array($tag->values)) {
            $v = (is_array($v) ? array_merge($v, $tag->values) : $tag->values);
        }
        $tag = $v;

    }

    /** 
     * Merge the tag tree starting at the given tag.
     *
     * This function works recursively, merging all a tags children before
     * attempting to merging the tag itself.
     *
     * @access  private
     * @param   array       &$tag       tag to group
     * @return  void
     * @see     Merge()
     */
    function _MergeTree(&$tag) {

        // Merge all descendants of this tag
        $this->_RecurseChildTags($tag, __FUNCTION__);

        // Merge this tag
        $this->_MergeTag($tag);

    }

    /** 
     * Merge parsed tag tree
     *
     * This function merges a tags properties. That is if a tag contains just
     * 'content' and no 'attributes' or 'values' then it is replaced by the
     * content. Otherwise it is replace by an array consisting of the merger of
     * the 'attributes', 'content', 'values' - in that order.
     *
     * Once a tree has been merged it is impossible to do any more processing
     * on it as all the TNTTagParserNode objects no longer exist and the tree is
     * now a hierachical tree of structures representing the parsed data with
     * no parsing structure information.
     *
     * This function is called automatically after parsing the tags if the
     * TNT_TAGPARSER_OPTS_MERGE flag is set. However it is available as
     * a separate function to allow the tree to be merged at a later time
     * or so the post processing order can be user defined.
     *
     * @access  private
     * @return  TNTTagParserNode           reference to root data (or null)
     */
    function &Merge() {

        if (is_array($this->_stack) && is_a($this->_stack[0], 'tnttagparsernode')) {
            $this->_MergeTree($this->_stack[0]);
        }

        $ret =& $this->Root();

        return $ret;
    }

    /** 
     * Intialize the parsing
     *
     * This performs any initialisation required before the _Parse() method is
     * called.
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function _Init() {

        $ret = true;

        $this->errorcode = 0;
        $this->errorline = $this->errorchar = $this->errorbyte = 0;
        $this->errortext = null;

        $tag = new TNTTagParserNode();
        $tag->type = 'root';
        $tag->tag = '<root>';
        $tag->info = null;
        $tag->attributes = null;
        $tag->content = null;

        $this->_stack = array();
        $this->_stack[] = $tag;

        return $ret;
    }

    /** 
     * Terminate the parsing
     *
     * This performs any final reduction, condensing and grouping in that order
     * if the appropriate option flags are set.
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function _Term() {

        $ret = true;

        if (is_array($this->_stack) && is_a($this->_stack[0], 'tnttagparsernode')) {
            // Finally process the root node
            $top =& $this->_stack[0];
    
            // Reduce the tags children ?
            if ($this->options & TNT_TAGPARSER_OPTS_REDUCE) {
                $this->_ReduceTag($top);
            }
            // Condense the tag children ?
            if ($this->options & TNT_TAGPARSER_OPTS_CONDENSE) {
                $this->_CondenseTag($top);
            }
            // Group the tags children ?
            if ($this->options & TNT_TAGPARSER_OPTS_GROUP) {
                $this->_GroupTag($top);
            }
            // Merge the tags children ?
            if ($this->options & TNT_TAGPARSER_OPTS_MERGE) {
                $this->_RecurseChildTags($top, '_MergeTag');
                $this->_MergeTag($top);
            }
        }

        return $ret;
    }

    /** 
     * Parse the data, calling the event handlers on each event
     *
     * The default event handlers will build the tag tree rooted at $this->root.
     *
     * This method should be overridden by the derived class to call the
     * _HandleTag() handler at the appropriate times to build the tag tree.
     *
     * This base method does nothing.
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function &_Parse() {
        return null;
    }

    /** 
     * Call this to flag up an error
     *
     * Parsing should stop if the $this->errorcode is set.
     *
     * The byte, line and char parameters are optional. If byte is given
     * then the context of the error in the file can be dumped to the debug
     * log.
     *
     *
     * @access  private
     * @param   integer     $code       data to be parsed
     * @param   string      $text       explanation of error
     * @param   array       $position   associative array holding error position
     * @return  void
     */
    function _Error($code, $text, $position = null) {

        $this->errorcode = $code;
        $this->errortext = $text;
        $this->errorbyte = (isset($position['byte']) ? $position['byte'] : -1);
        $this->errorline = (isset($position['line']) ? $position['line'] : -1);
        $this->errorchar = (isset($position['char']) ? $position['char'] : -1);

        if (!$this->errorcode) {
            $txt = 'no error';
        } else {
            $txt =  'Parser error '.$this->errorcode.
                    (($this->errorline >= 0) ?
                        ' ['.$this->errorline.':'.$this->errorchar.'] - ' :
                        ' - ').
                    $this->errortext;
        }

        $this->_SetError($txt);

        if (($byte = $this->errorbyte) != -1) {
            $l = strlen($this->_data);
            $s = ($byte - 20);
            $e = ($byte + 20);

            if ($s < 0) { $s = 0; }
            if ($e > $l) { $e = $l; }

            // When calculating the position we have to measure the length of
            // the initial portion of the data as displayed by var_info() as
            // that is how the data is displayed by the debugger.
            $context = substr($this->_data, $s, $e - $s);
            $position = (($byte - $s) ? strlen(var_info(substr($context, 0, $byte - $s))) : 0);

        }

    }

    /** 
     * Parsed Tag Tree
     *
     * Returns a reference to the parsed tag tree.
     * file names.
     *
     * @access  public
     * @return  TNTTagParserNode                  reference to root tag (or null)
     */
    function &Root() {
        if ($this->_stack) {
            return $this->_stack[0];
        } else {
            $ret = null;
            return $ret;
        }
    }

    /** 
     * Parse some Data
     *
     * @access  public
     * @param   string      $data       data to be parsed
     * @param   integer     $options    options to use when parsing
     * @return  TNTTagParserNode        reference to root tag (or null)
     */
    function &ParseData($data, $options = null) {

        $ret = null;

        // Start error section
        do {
            $this->_data = $data;

            if (!is_null($options)) {
                $this->options = $options;
            }
    
            if (!$this->_Init() || !$this->_Parse() || !$this->_Term()) {
                $this->_stack = null;
            }

            $ret =& $this->Root();

        } while (false);
        // end error section

        return $ret;
    }

    /** 
     * Parse a file or files
     *
     * Filename can be an array of filenames or a comma separated list of
     * file names.
     *
     * @access  public
     * @param   mixed       $path       path to file to parse
     * @param   integer     $options    options to use when parsing
     * @param   boolean     $useincpath use PHP include path to find the files
     * @return  TNTTagParserNode        reference to root tag (or null)
     */
    function &ParseFile($path, $options = null, $useincpath = true) {

        $ret = null;

        // Start error section
        do {
            if (($data = file_get_contents($path, $useincpath)) === false) {
                $this->_Error(TNT_TAGPARSER_ERROR_BAD_FILE,
                              'failed to read from file: '.$path);
                break;
            }
    
            if ($data == null || strlen($data) == 0) {
                $this->_Error(TNT_TAGPARSER_ERROR_BAD_FILE,
                              'file is empty: '.$path);
                return null;
            }
    
            $this->_data = $data;

            if (!is_null($options)) {
                $this->options = $options;
            }
    
            if (!$this->_Init() || !$this->_Parse() || !$this->_Term()) {
                $this->_stack = null;
            }

            $ret =& $this->Root();

        } while (false);
        // end error section

        return $ret;
    }

    /** 
     * Construct the object
     *
     * @access  public
     * @return  void
     */
    function TNTTagParser() {

        TNTObject::TNTObject();

    }

    /** 
     * Destruct the object
     *
     * @access  public
     * @return  void
     */
    function _TNTTagParser() {

    }
}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
