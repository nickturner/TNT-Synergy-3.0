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
 * HTML Parser Class
 * ~~~~~~~~~~~~~~~~~
 *
 * This is an implementation of the TNTTagParser abstract class that retreives
 * its tags from an HTML source file.
 *
 * The HTML parser differs from the XML parser in that it can handle badly
 * formatted HTML style documents. It allows tags to be implicitly closed etc.
 *
 * Also unline XML the parser does allow '<' and '>' characters to be used in
 * the CDATA without escaping as '&lt;', '&gt'. This means that some potential
 * tag errors such as '</tag />' will be treated as CDATA and not as an invalid
 * tag - this is not always desirable.
 *
 * It uses the HTMLParser designed by Anton Y. Kalmykov	[anton@concord.ru] to
 * actually parse the HTML, the parsed results are then fed to the TNTTagParser
 * for it to build the tag tree.
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
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Handle the parsing of HTML tags.
 *
 * @package TNT.Html
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTHtmlParser extends TNTTagParser {

    /** 
     * Traverse the tree produced by the native parser, calling the _HandleTag()
     * method at the appropriate times to allow the TagParser to build its
     * tag tree.
     *
     * @access  private
     * @param   string      $data       data to be parsed
     * @return  boolean                 true if successfull
     */
    function _ScanNodes($nodes) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('nodes' =>& $nodes));

        $ret = null;
        foreach ($nodes as $i => $n) {
            $pos = null;
            if (!is_numeric($i)) {
                continue;
            }
            switch ($n['type']) {
                case 'tag':
                    $tag = $n['data']['name'];
                    $attrs = null;
                    $content = null;
                    if (!empty($n['xmlclose'])) {
                        $type = 'complete';
                    } else {
                        $type = $n['data']['type'];
                    }
                    if (!empty($n['pars'])) {
                        $attrs = array();
                        foreach ($n['pars'] as $name => $data) {
                            $attrs[$name] = $data['value'];
                        }
                    }

                    $this->_HandleTag($type, $tag, $pos, $attrs, $content);

                    if (!empty($n['content'])) {
                        $this->_ScanNodes($n['content']);
                    }
                    break;
                case 'text':
                    if (strpos($n['data'], '<') === false) {
                        $this->_HandleTag('cdata', '', $pos, null, $n['data']);
                        break;
                    }

                    $regex = '/(<\?.*\?>)|(<!.*>)/Uims';
                    $chunks = preg_split($regex, $n['data'], -1,
                        PREG_SPLIT_NO_EMPTY|PREG_SPLIT_DELIM_CAPTURE);

                    // Loop through the chunks creating the parse tree
                    $nc = count($chunks);
                    for ($c = 0; $c < $nc; $c++) {
                        if (preg_match('/<\?([\w_:-]*)\s+(.*)\?>/', $chunks[$c], $matches)) {
                            $attrs = array('target' => $matches[1]);
                            $data = $matches[2];
                            $this->_HandleTag('pi', '', $pos, $attrs, $data);
                        } else if (preg_match('/<!.*>/', $chunks[$c])) {
                            // TODO - decode NOTATIONS
                            $attrs = null;
                            $data = $chunks[$c];
                            $this->_HandleTag('notation', '', $pos, $attrs, $data);
                        } else {
                            $this->_HandleTag('cdata', '', $pos, null, $chunks[$c]);
                        }
                    }
                    break;
                case 'comment':
                    $this->_HandleTag('comment', '', $pos, null, $n['data']);
                    break;
            }
            if ($this->errorcode) {
                break;
            }
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /** 
     * Parse the data, calling the event handlers on each event
     *
     * The default event handlers will build the tag tree rooted at $this->root.
     *
     * This method is hookable.
     *
     * @access  private
     * @return  boolean                 true if successfull
     */
    function &_Parse() {
        $args = null;
        $ret = null;

        if (!($this->_Enter(__CLASS__, __FUNCTION__, $args, $ret))) {
            return $ret;
        }

        require_once(SYNERGY_DIR.'includes/libs/contrib/HTMLParser/htmlparser.inc');

        $ret = false;

        static $grammar = null;
        if (!$grammar) {
            $grammarfile = SYNERGY_DIR.'includes/libs/contrib/HTMLParser/htmlgrammar.cmp';
            $grammar = unserialize(file_get_contents($grammarfile));
        }

        $parser =& new HtmlParser($this->_data, $grammar);
        @$parser->Parse();

        if ($parser->content) {
            //TRACE $this->Trace('content=', $parser->content);
            $this->_ScanNodes($parser->content);
            $ret = ($this->errorcode == 0);
        } else {
            $this->_Error(TNT_TAGPARSER_ERROR_INTERNAL,
                          'no nodes (internal error)');
        }

        unset($parser->content);
        unset($parser);

        $this->_Exit(__CLASS__, __FUNCTION__, $args, $ret);
        return $ret;
    }

    /** 
     * Construct the object
     *
     * @access  public
     * @return  void
     */
    function TNTHtmlParser() {
        $args = null;
        $ret = null;

        if (!($this->_Enter(__CLASS__, __FUNCTION__, $args, $ret))) {
            return $ret;
        }

        TNTTagParser::TNTTagParser();

        $this->_Exit(__CLASS__, __FUNCTION__, $args, $ret);
    }

    /** 
     * Destruct the object
     *
     * @access  public
     * @return  void
     */
    function _TNTHtmlParser() {
        $args = null;
        $ret = null;

        if (!($this->_Enter(__CLASS__, __FUNCTION__, $args, $ret))) {
            return $ret;
        }

        $this->_Exit(__CLASS__, __FUNCTION__, $args, $ret);
 
    }
}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
