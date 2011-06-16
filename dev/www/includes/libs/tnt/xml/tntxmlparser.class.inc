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
 * XML Parser Class
 * ~~~~~~~~~~~~~~~~
 *
 * This is an implementation of the TNTXmlParser class that retreives
 * its tags from an XML source file.
 *
 * @package TNT.Xml
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTTagParser');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Handle the parsing of XML tags.
 *
 * @package TNT.Xml
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTXmlParser extends TNTTagParser {

    /**
     * Handle to PHP XML Parser
     * @var resource                    handle to PHP XML expat parser
     */
    var $_parser = null;

    /**
     * Buffered character data
     * @var array                       associated array of cdata details
     */
    var $_cdata = null;

    /**
     * Get parser position returned which each tag
     *
     * @access  private
     * @return  array                   associative array of parser info
     */
    function _GetPos() {
        if (!$this->_parser) {
            return null;
        }
        return array(
            'byte' => xml_get_current_byte_index($this->_parser),
            'line' => xml_get_current_line_number($this->_parser),
            'char' => xml_get_current_column_number($this->_parser));
    }

    function _CDataFlush() {
        if ($this->_cdata) {
            $this->_HandleTag('cdata', '', $this->_cdata['info'], null, $this->_cdata['content']);
            $this->_cdata = null;
        }
    }

    /** 
     * Callbacks
     */
    function __OpenTagHandler($parser, $tag, $attrs) {
        if (!$this->errorcode) {
            $this->_CDataFlush();
            $this->_HandleTag('open', $tag, $this->_GetPos(), $attrs, null);
        }
    }
    
    function __CloseTagHandler($parser, $tag) {
        if (!$this->errorcode) {
            $this->_CDataFlush();
            $info = $this->_GetPos();
            if ($this->_data{$info['byte']} != '<') {
                $this->_HandleTag('close_complete', $tag, $info, null, null);
            } else {
                $this->_HandleTag('close', $tag, $info, null, null);
            }
        }
    }
    
    function __PIHandler($parser, $target, $data) {
        if (!$this->errorcode) {
            $this->_CDataFlush();
            $attrs = array('target' => $target);
            $this->_HandleTag('pi', '', $this->_GetPos(), $attrs, $data);
        }
    }

    function __NotationHandler($parser, $notation, $base, $system_id, $public_id) {
        if (!$this->errorcode) {
            $this->_CDataFlush();
            $attrs = array('notation' => $notation, 'base' => $base, 'system_id' => $system_id, 'public_id' => $public_id);
            $this->_HandleTag('notation', '', $this->_GetPos(), $attrs, null);
        }
    } 

    function __DefHandler($parser, $data) {
        if (!$this->errorcode) {
            $this->_CDataFlush();
            if (preg_match('/<\?([\w_:-]*)\s+(.*)\?>/Umsi', $data, $matches)) {
                $this->__PIHandler($parser, $matches[1], $matches[2]);
            } else if (preg_match('/<!--.*-->/Umsi', $data)) {
                $type = 'comment';
                $this->_HandleTag('comment', '', $this->_GetPos(), null, $data);
            } else {
                $this->_HandleTag('cdata', '', $this->_GetPos(), null, $data);
            }
        }
    }

    function __CDataHandler($parser, $data) {
        // We buffer up the character data as the XML parser fragments it
        if (!$this->_cdata) {
            $this->_cdata['info'] = $this->_GetPos();
            $this->_cdata['content'] = '';
        }
        $this->_cdata['content'] .= $data;
    }

    /** 
     * Parse the data, calling the event handlers on each event
     *
     * The default event handlers will build the tag tree rooted at $this->root.
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function &_Parse() {

        $ret = false;

        if (($this->_parser = xml_parser_create())) {
            xml_set_object($this->_parser, $this);
            xml_parser_set_option($this->_parser, XML_OPTION_CASE_FOLDING, false);
            if ($this->options & TNT_TAGPARSER_OPTS_TRIM) {
                xml_parser_set_option($this->_parser, XML_OPTION_SKIP_WHITE, true);
            }
            xml_set_element_handler($this->_parser, '__OpenTagHandler', '__CloseTagHandler');
            xml_set_character_data_handler($this->_parser, '__CDataHandler');
            xml_set_processing_instruction_handler($this->_parser, '__PIHandler');
            xml_set_notation_decl_handler($this->_parser, '__NotationHandler');
            xml_set_default_handler($this->_parser, '__DefHandler');
    
            $startline = xml_get_current_line_number($this->_parser);

            $ret = xml_parse($this->_parser, $this->_data, true);
            if ($ret) {
                $this->_CDataFlush();
            } else if (!$this->errorcode) {
                static $_errmap = array(
					XML_ERROR_NONE =>
                                TNT_TAGPARSER_ERROR_NONE,
					XML_ERROR_NO_MEMORY =>
                                TNT_TAGPARSER_ERROR_NO_MEMORY,
					XML_ERROR_SYNTAX =>
                                TNT_TAGPARSER_ERROR_SYNTAX,
					XML_ERROR_NO_ELEMENTS =>
                                TNT_TAGPARSER_ERROR_NO_ELEMENTS,
					XML_ERROR_INVALID_TOKEN =>
                                TNT_TAGPARSER_ERROR_INVALID_TOKEN,
					XML_ERROR_UNCLOSED_TOKEN =>
                                TNT_TAGPARSER_ERROR_UNCLOSED_TOKEN,
					XML_ERROR_TAG_MISMATCH =>
                                TNT_TAGPARSER_ERROR_TAG_MISMATCH,
					XML_ERROR_DUPLICATE_ATTRIBUTE =>
                                TNT_TAGPARSER_ERROR_DUP_ATTRIBUTE,
					XML_ERROR_JUNK_AFTER_DOC_ELEMENT =>
                                TNT_TAGPARSER_ERROR_JUNK_AFTER_ROOT,
					XML_ERROR_UNDEFINED_ENTITY =>
                                TNT_TAGPARSER_ERROR_UNDEFINED_ENTITY,
                    );

                $ec = xml_get_error_code($this->_parser);

                $this->_Error((isset($_errmap[$ec]) ? $_errmap[$ec] :
                                TNT_TAGPARSER_ERROR_INTERNAL),
                              xml_error_string($ec),
                              array('byte' => xml_get_current_byte_index($this->_parser),
                                    'line' => xml_get_current_line_number($this->_parser),
                                    'char' => xml_get_current_column_number($this->_parser))
                              );
            }
    
            xml_parser_free($this->_parser);
        } else {
            $this->_SetError('failed to create xml parser');
            $this->errorcode = TNT_TAGPARSER_ERROR_INTERNAL;
        }
    
        return $ret;
    }

    /** 
     * Construct the object
     *
     * @access  public
     * @return  void
     */
    function TNTXmlParser() {
        TNTTagParser::TNTTagParser();
    }

    /** 
     * Destruct the object
     *
     * @access  public
     * @return  void
     */
    function _TNTXmlParser() {
    }
}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
