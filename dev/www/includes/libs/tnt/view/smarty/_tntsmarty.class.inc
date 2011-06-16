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
 * Smarty Template Class
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * TNT derivative of the standard PHP Smarty template processor as defined at
 * {@link http://smarty.php.net}.
 *
 * The class is a private class and is created when required by the
 * {@link TNTTemplate} class.
 *
 * @package TNT.View
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('Smarty');
TNTLoader::RequireClass('Smarty_Compiler');

/*
 * Globals
 */

/*
 * Defines
 */

/*
 * Extension of the standard Smarty class
 *
 * @access  private
 * @package TNT.View
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
class _TNTSmarty extends Smarty {

    /**
     * Reference to the template class.
     *
     * @access public
     * @var TNTTemplate&
     */
    var $tnttemplate = null;

    /**
     * The file that contains the compiler class. This can a full
     * pathname, or relative to the php_include path.
     *
     * @access public
     * @var string
     */
    var $compiler_file = __FILE__;

    /**
     * The class used for compiling templates.
     *
     * @access public
     * @var string
     */
    var $compiler_class = '_TNTSmartyCompiler';

    /**
     * Register a plugin
     *
     * This should be called by plugins to register themselves.
     *
     * @access  public
     * @param   string      $name       the name of the template function
     * @param   string      $type       the function type
     * @param   callable    $func       PHP function/method to call
     * @param   boolean     $cachable   true if the function should cached
     * @param   array       $cache_attrs array of attributes to cache
     * @return  void
     */
    function RegisterPlugin($type, $name, $call, $cacheable = true, $cache_attrs = null) {
        $this->_plugins[$type][$name] = 
            array($call, null, null, false, $cacheable, $cache_attrs);
    }

    /**
     * Report an error in a Smarty template.
     *
     * This should be called by the plugins to indicate an error in the code.
     *
     * @access  public
     * @param   string      $error_msg
     * @param   integer     $error_type
     * @return  void
     */
    function ReportError($error_msg, $error_type = E_USER_WARNING) {
        $this->trigger_error($error_msg, $error_type);
    }

    /**
     * Validate the Smarty object is ok
     * Called by Smarty plugins to validate the parent TplEng class
     *
     * @access  public
     * @return  boolean                 true if ok
     */
    function IsValid() {
         if (!is_a($this, '_TNTSmarty')) {
             trigger_error('Invalid _TNTSmarty object type');
             return false;
         }
         if (!is_a($this->tnttemplate, 'TNTTemplate')) {
             trigger_error('Invalid TNTTemplate object type');
             return false;
         }

         return true;
    }

    /**
     * Unregisters all objects
     *
     */
    function unregister_all_objects() {
        $this->_reg_objects = array();
    }

    /**
     * get filepath of requested plugin
     *
     * This is overridden so if the plugin can not be found in any of the
     * registered plugin directories the code will use the TNTLoader to find
     * the plugin. Thus default can use the loader and to override that use
     * a plugin path.
     *
     * It also handles the automatic registering of tag libraries etc. so
     * they are initialised in the compiled/cached templates.
     *
     * @access  private
     * @param   string      $type
     * @param   string      $name
     * @return  string|false
     */
    function _get_plugin_filepath($type, $name) {
        $_params = array('type' => $type, 'name' => $name);
        require_once(SMARTY_CORE_DIR . 'core.assemble_plugin_filepath.php');
        if (!($ret = smarty_core_assemble_plugin_filepath($_params, $this))) {
            // Use the TNT Loader to find the correct file
            $ret = TNTLoader::Source('smarty_'.$type.'_'.$name, 'function');
        }
        return $ret;
    }

    /**
     * trigger Smarty error
     *
     * @param string $error_msg
     * @param integer $error_type
     */
    function trigger_error($error_msg, $error_type = E_USER_WARNING) {
        TNT::Abort("Smarty error: $error_msg", $error_type);
    }


    /**
     * Construct the object.
     *
     * @access  public
     */
    function _TNTSmarty(&$tnttemplate) {

        Smarty::Smarty();

        $this->plugins_dir = array(__FILE__.'/plugins/');

        $this->tnttemplate =& $tnttemplate;
    }

    /**
     * Destruct the object
     *
     * @access  private
     */
    function __TNTSmarty() {

        $this->tnttemplate = null;
    }
}

/*
 * Extension of the standard Smarty Compiler class
 *
 * @access  private
 * @package TNT.View
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
class _TNTSmartyCompiler extends Smarty_Compiler {

    /**
     * Array of directives found in the template
     * @access  private
     * @var     array
     */
    var $_directives = null;

    /**
     * Array of aliases found in the template
     * @access  private
     * @var     array
     */
    var $_aliases = null;

    /**
     * Array of tags registered with taglib directives
     * @access  private
     * @var     array
     */
    var $_tags = null;

    /**
     * Base name of template (ie. directory template is in)
     * @access  private
     * @var     string
     */
    var $_base = null;

    /**
     * Validate the Smarty object is ok
     * Called by Smarty plugins to validate the parent TplEng class
     *
     * @access  public
     * @return  boolean                 true if ok
     */
    function IsValid() {
         if (!is_a($this, '_TNTSmartyCompiler')) {
             trigger_error('Invalid _TNTSmartyCompiler object type');
             return false;
         }
         if (!is_a($this->tnttemplate, 'TNTTemplate')) {
             trigger_error('Invalid TNTTemplate object type');
             return false;
         }

         return true;
    }

    /**
     * Register a plugin
     *
     * This should be called by plugins to register themselves.
     *
     * @access  public
     * @param   string      $name       the name of the template function
     * @param   string      $type       the function type
     * @param   callable    $func       PHP function/method to call
     * @param   boolean     $cachable   true if the function should cached
     * @param   array       $cache_attrs array of attributes to cache
     * @return  void
     */
    function RegisterPlugin($type, $name, $call, $cacheable = true, $cache_attrs = null) {
        $this->_plugins[$type][$name] = 
            array($call, null, null, false, $cacheable, $cache_attrs);
    }

    /**
     * Report an error in a Smarty template.
     *
     * This should be called by the plugins to indicate an error in the code.
     *
     * @access  public
     * @param   string      $error_msg
     * @param   integer     $error_type
     * @return  void
     */
    function ReportError($error_msg, $error_type = E_USER_WARNING) {
        $this->trigger_error($error_msg, $error_type);
    }

    /**
     * Unregisters all objects
     *
     */
    function unregister_all_objects() {
        $this->_reg_objects = array();
    }

    /**
     * compile a resource
     *
     * sets $compiled_content to the compiled source
     *
     * @access  private
     * @param   string      $resource_name
     * @param   string      $source_content
     * @param   string      $compiled_content
     * @return  true
     */
    function _compile_file($resource_name, $source_content, &$compiled_content) {
        TNT::Timestamp('_compile_file()');

        // Initialise our $tnttemplate member variable so that plugins have
        // access to the TNTTemplate. NB. This is a clone of the parent member
        // with the $_smarty member variable being associated with this class.
        if (!is_a($this->_tpl_vars['template'], 'TNTTemplate')) {
            $this->ReportError('Can\'t get handle to parent TNTTemplate object');
            $this->tnttemplate = null;
        } else {
            $this->tnttemplate =& $this->_tpl_vars['template']->Clone();
            $this->tnttemplate->_smarty =& $this;
        }

        $this->_current_file = $resource_name;
        $this->_current_line_no = 1;
        $this->_directives = null;

        $left_delimiter = $this->left_delimiter;
        $right_delimiter = $this->left_delimiter;

        $source_content = $this->_tnt_prepare($source_content);

        $ret = parent::_compile_file($resource_name, $source_content, $compiled_content);

        if ($this->_directives) {
            $php = '_smarty_load_directives('.var_export($this->_directives, true).", \$this);\n";
            $compiled_content = str_replace('smarty_core_load_plugins(', $php.'_smarty_load_plugins(', $compiled_content);
            $this->_plugins_code = str_replace('smarty_core_load_plugins(', $php.'_smarty_load_plugins(', $this->_plugins_code);
        }

        $this->left_delimiter = $left_delimiter;
        $this->right_delimiter = $left_delimiter;

        TNT::Timestamp('_compile_file() - done');
        return $ret;
    }

    /**
     * Allow a plugin to register itself.
     *
     * Normally plugins have to be either registered with the template or
     * to be found in one of plugin directories under a standard file and
     * function name. The later does not allow the plugin to specify its
     * caching options.
     *
     * Allowing a plugin to self register combines the two approaches, we
     * look for a 'registration' function in the standard plugin file and
     * if found we call that. It can then call the 'register_$type' method
     * to declare its callback and cacheability options.
     *
     * The registration function is only ever called once and it is called
     * just prior to the first use of the plugin.
     *
     * @access  private
     * @param   string      $type       type of plugin
     * @param   string      $name       name of plugin
     * @return  void
     */
    function _check_plugin($type, $name) {
        if (isset($this->_plugins[$type][$name])) {
            if (!is_callable($callable = $this->_plugins[$type][$name][0])) {
                // Ensure the code is loaded
                if (is_array($callable) && is_string($callable[0])) {
                    TNTLoader::RequireClass($callable[0]);
                } else if (is_string($callable)) {
                    TNTLoader::RequireFunction($callable);
                }
            }
            return;
        }

        $plugin_func = 'smarty_'.$type.'_'.$name ;
        $register_func = 'smarty_register_'.$type.'_'.$name ;

        $_params = array('type' => $type, 'name' => $name);
        require_once(SMARTY_CORE_DIR . 'core.assemble_plugin_filepath.php');
        if (!($ret = smarty_core_assemble_plugin_filepath($_params, $this))) {
            // Use the TNT Loader to find the correct file
            $ret = TNTLoader::Source($register_func, 'function');
        }

        if ($ret) {
            include_once $ret;

            if (function_exists($register_func)) {

                // Call the register function
                $register_func($this);

                // If it registered but left function blank and the default function
                // now exists then use that.
                if (isset($this->_plugins[$type][$name]) && 
                    !isset($this->_plugins[$type][$name][0]) && 
                    function_exists($plugin_func)) {
                    $this->_plugins[$type][$name][0] = $plugin_func;
                }
            }
        }
            
        return $ret;
    }

    /**
     * Get filepath of requested plugin
     *
     * This is overridden so if the plugin can not be found in any of the
     * registered plugin directories the code will use the TNTLoader to find
     * the plugin. Thus default can use the loader and to override that use
     * a plugin path.
     *
     * It is also used as a way of preloading tag libraries and modules.
     *
     * @access  private
     * @param   string      $type       type of plugin
     * @param   string      $name       name of plugin
     * @return  string                  file to load plugin from (or false)
     */
    function _get_plugin_filepath($type, $name) {
        $_params = array('type' => $type, 'name' => $name);
        require_once(SMARTY_CORE_DIR . 'core.assemble_plugin_filepath.php');
        if (!($ret = smarty_core_assemble_plugin_filepath($_params, $this))) {
            // Use the TNT Loader to find the correct file
            $ret = TNTLoader::Source("smarty_{$type}_{$name}", 'function');
        }
        return $ret;
    }


    /**
     * Compile compiler function tag
     *
     * This is overridden so that a plugin can register its self before being
     * called. It attempts to find the plugin and then if found it will look
     * for a registration function and call it. The registration function can
     * call $smarty->register_xxx to self register.
     *
     * This means a plugin found in a file can set up its cacheable state
     * before being used and even set the callback to use as opposed to the
     * default "smarty_{$type}_{$name}" function.
     *
     * @access  private
     * @param   string      $tag_command        tag command name
     * @param   string      $tag_args           tag args
     * @param   string&     $output             string to store output in
     * @return  boolean
     */
    function _compile_compiler_tag($tag_command, $tag_args, &$output) {
        $this->_check_plugin('compiler', $tag_command);
        return parent::_compile_compiler_tag($tag_command, $tag_args, $output);
    }

    /**
     * Compile block function tag
     *
     * This is overridden so that a plugin can register its self before being
     * called. It attempts to find the plugin and then if found it will look
     * for a registration function and call it. The registration function can
     * call $smarty->register_xxx to self register.
     *
     * This means a plugin found in a file can set up its cacheable state
     * before being used and even set the callback to use as opposed to the
     * default "smarty_{$type}_{$name}" function.
     *
     * @access  private
     * @param   string      $tag_command        tag command name
     * @param   string      $tag_args           tag args
     * @param   string      $tag_modifier       tag modifiers
     * @param   string&     $output             string to store output in
     * @return  boolean
     */
    function _compile_block_tag($tag_command, $tag_args, $tag_modifier, &$output) {
        $this->_check_plugin('block', $tag_command);
        return parent::_compile_block_tag($tag_command, $tag_args, $tag_modifier, $output);
    }


    /**
     * Regex callback function to handle template directives.
     *
     * Template directives take the format '<%@ name attrs %>' and are used to
     * direct information about a template to the template parser. They can be
     * included anywhere on a template, although it is standard to place them
     * at the beginning.
     *
     * Directives can be either built-in or plugin. Plugin directives follow
     * the same loading and nameing methods as normal plugin functions, using
     * the plugin type 'directive'.
     *
     * Directives apply to the current template and all templates included
     * via the {include file=""} command.
     *
     * The standard built-in directives are;
     *
     * <%@register type="compiler|function|block|modifier" name="name"
     *             [cacheable="yes|no"] [cache_attrs="attr,attr,attr..."] %>
     *   Register a <type> plugin called <name> which either calls the normal
     *   'smarty_$type_$name' PHP function or the supplied '$call' PHP function
     *   or static class method. {@see TNTTemplate::RegisterPlugin()}.
     *
     * <%@tagalias from="[prefix:]name" to="[prefix:]name" %>
     *   Alias the given HTML tag as really being the other one.
     *   eg. <%@alias tag="option" as="wui:option" %> will make all standard
     *   html option tags to be treated as if they were "wui:option" tags.
     *
     * <%@taglib prefix="prefix" name="name" %>
     *   Load a tag library called <name>. The library is imported by calling
     *   a method as if it was a plugin of type 'taglib' and name <name>.
     *   eg. <%@taglib prefix="wui" name="tnt_wui" %> will cause all HTML
     *   tags prefixed by 'wui' invoke the template method registered for that
     *   tags name in the given tag library. Eg. '<wui:input />' will call the
     *   registered function for the tag 'input' in the tag library.
     *   This directive applies to the current template only, however the tag
     *   library called <name> will only be loaded once per template invocation
     *   and so a directive to load an already loaded a tag library will cause
     *   no additional overhead and will just cause the HTML tags in the
     *   included template to be parsed as well. The prefix associated with a
     *   library can be different in each template (its even possible to
     *   associate multiple prefixes to the same library in a single template).
     *
     * <%@import name="name" %>
     *   Import a module called <name>. The module is imported by calling
     *   an method as if it was a plugin of type 'import' and name <name>.
     *   The code is imported once per template invocation, it is designed to
     *   allow modules to be imported and register their template commands.
     *   Pre-registration of a modules template commands speads up template
     *   compilation as the compiler doesn't need to search the plugin
     *   directories for the correct plugin code file.
     *
     *
     * <%@delimiters [left="{{"] [right="}}"] %>
     *   Alter the template command delimiters - this will effect the current
     *   template only.
     *
     * <%@cache [lifetime="0"] %>
     *
     * $matches[0] = complete tag
     * $matches[1] = directive name
     * $matches[2] = directive arguments
     *   
     * @access  private
     * @param   string[]    $matches    preg_pattern matches
     * @return  string                  the replacement string for the match
     */
    function _tnt_decode_directives($matches) {

        //echo '<pre>'.htmlentities(var_export($matches, true)).'</pre>';

        $directive = $matches[0];
        $name = $matches[1];
        $attributes = $matches[2];

        if (trim($matches[2]) != '') {
            if (preg_match_all('/(\w*)\s*=\s*([\'"])?(.*)(\2)/Us',
                $attributes, $matches, PREG_SET_ORDER) < 0) {
                $this->trigger_error("[prefilter:tnt] invalid directive attribute syntax: {$attrs}", E_USER_WARNING);
                return $matches[0];
            }
            $attrs = array();
            $max_i = count($matches);
            for ($i = 0; $i < $max_i; $i++) {
                $attrs[trim(strtolower($matches[$i][1]))] =
                        preg_replace('/\&\#([0-9]+)\;/me', "chr('\\1')", html_entity_decode($matches[$i][3]));
            }
        } else {
            $attrs = null;
        }

        $epfx = 'template \''.htmlentities($directive).'\' directive ';

        switch ($name) {
            case 'register':
                static $types = array('compiler', 'function', 'block', 'modifier', 'prefilter', 'postfilter', 'outputfilter', 'resource', 'insert');
                if (!isset($attrs['type']) || !in_array(strtolower($attrs['type']), $types)) {
                    $this->trigger_error($epfx.'directive requires \'type\' attribute to be one of the following types: '.join(',', $types), E_USER_WARNING);
                    break;
                }
                if (!isset($attrs['name']) || !preg_match('/^(\w+:)?\w+$/', $attrs['name'])) {
                    $this->trigger_error($epfx.'requires \'name\' attribute of the format \'[prefix:]name\'', E_USER_WARNING);
                    break;
                }
                if (isset($attrs['call']) && !preg_match('/^(\w+:)?\w+$/', $attrs['call'], $matches)) {
                    $this->trigger_error($epfx.'requires \'call\' attribute of the format \'class:method\' or \'function\'', E_USER_WARNING);
                    break;
                }
                if (isset($attrs['cacheable']) && !in_array(strtolower($attrs['cacheable']), array('yes', 'no'))) {
                    $this->trigger_error($epfx.'\'cacheable\' attribute should be either \'yes\' or \'no\'', E_USER_WARNING);
                    break;
                }
                if (!isset($attrs['cache_attrs']) && !preg_match('/^[\w,]*$/', $attrs['cache_attrs'])) {
                    $this->trigger_error($epfx.'\'cache_attrs\' should be a list of valid attribute names', E_USER_WARNING);
                    break;
                }

                $this->_directives[] = array($name, $attrs);

                break;

            case 'import':
                if (isset($attrs['name']) && isset($attrs['call'])) {
                    $this->trigger_error($epfx."' directive requires 'name' or 'call' attribute not both", E_USER_WARNING);
                    break;
                } else if (!isset($attrs['name']) && !isset($attrs['call'])) {
                    $this->trigger_error($epfx.'requires \'name\' or \'call\' attribute', E_USER_WARNING);
                    break;
                } else if (isset($attrs['name']) && !preg_match('/^\w+$/', $attrs['name'])) {
                    $this->trigger_error($epfx."' directive 'name' attribute is invalid", E_USER_WARNING);
                    break;
                } else if (isset($attrs['call']) && !preg_match('/^(\w+:)?\w+$/', $attrs['call'], $matches)) {
                    $this->trigger_error($epfx.'requires \'call\' attribute of the format \'class:method\' or \'function\'', E_USER_WARNING);
                    break;
                }

                $this->_directives[] = array($name, $attrs);

                break;

            case 'tagalias':
                if (!isset($attrs['from']) || !preg_match('/^(\w+:)?\w+$/', $attrs['from'])) {
                    $this->trigger_error($epfx.'requires \'from\' attribute of the format \'[prefix:]name\'', E_USER_WARNING);
                    break;
                }
                if (!isset($attrs['to']) || !preg_match('/^(\w+:)?\w+$/', $attrs['to'])) {
                    $this->trigger_error($epfx.'requires \'to\' attribute of the format \'[prefix:]name\'', E_USER_WARNING);
                    break;
                }

                $this->_aliases[$attrs['from']] = $attrs['to'];

                break;

            case 'taglib':
                if (!isset($attrs['prefix']) || !preg_match('/^\w+$/', $attrs['prefix'])) {
                    $this->trigger_error($epfx.'\'prefix\' attribute is invalid', E_USER_WARNING);
                    break;
                }
                if (!isset($attrs['name']) || !preg_match('/^\w+$/', $attrs['name'])) {
                    $this->trigger_error($epfx.'\'name\' attribute is invalid', E_USER_WARNING);
                    break;
                }

                $this->_directives[] = array($name, $attrs);

                break;

            case 'delimiters':
                if (!isset($attrs['left']) || is_blank_str($attrs['left']) ||
                    !isset($attrs['right']) || is_blank_str($attrs['right'])) {
                    $this->trigger_error($epfx.'directive requires valid \'left\' and \'right\' attributes', E_USER_WARNING);
                    return '';
                }

                $this->left_delimiter = $attrs['left'];
                $this->right_delimiter = $attrs['right'];

                return '';

            default:

                $this->_directives[] = array($name, $attrs);

                break;
        }

        return '';
    }

    /**
     * Regex callback function to fix up a relative path 
     *
     * $matches[0] = complete tag
     * $matches[1] = 
     * $matches[2] = attribute name (or 'url')
     * $matches[3] = opening quote (' or ")
     * $matches[4] = url
     * $matches[5] = closing quote (' or ")
     *   
     * @access  private
     * @param   string[]    $matches    preg_pattern matches
     * @return  string                  the replacement string for the match
     */
    function _tnt_fixup_paths($matches) {
    
        if (!is_array($matches) || count($matches) != 5) {
            TNT::Abort('_smarty_prefilter_tnt_fixup_path(', $matches, ') - invalid arguments');
        }
    
        $base = $this->_fixup_base;
        $attr = $matches[2];
        $quote = $matches[3];
        $url = $matches[4];
    
        $u = parse_url($url);
    
        // To be a relative URL only the scheme,path and query components
        // can be used and the scheme if present must be a file protocol.
        static $schemes = array('http', 'https', 'ftp', 'ftps', 'file', 'tpl');
    
        if (!empty($u['host']) || !empty($u['port']) || !empty($u['user']) ||
            !empty($u['pass']) ||
            (!empty($u['scheme']) && !in_array($u['scheme'], $schemes)) ||
            strncmp($url, './', 2) != 0) {
            return $matches[0];
        }

        $url = $base.$url;
        if (!empty($u['scheme'])) {
            $url =  "{$u['scheme']}:{$url}";
        }
        if (!empty($u['query'])) {
            $url =  "{$url}?{$u['query']}";
        }
    
        if ($attr == 'url') {
            $ret = $attr.'('.$quote.$url.$quote.')';
        } else {
            $ret = $attr.'='.$quote.$url.$quote;
        }

        //echo "<pre>base=$base, $matches[0] => $ret</pre>";
    
        return $ret;
    }
    
    /**
     * Regex callback function to fix up Smarty HTML tags. A Smarty HTML tag is
     * standard tag with a tag name of the format '<tagprefix>:<tagname>'.
     *
     * Tags are an alternative shorthand way of representing stardard template
     * calls. Use the directive '<%@ tag prefix="w" alias="tnt_wui_" %>' in
     * template to state that HTML tags with the prefix 'w' should be mapped to
     * smarty functions starting with 'tnt_wui_'. For example, '<w:form>' would
     * be mapped to the smarty code '{tnt_wui_form}'.
     * 
     * $matches[0] = complete tag
     * $matches[1] = opening quote ('<' or '</')
     * $matches[2] = complete tag name
     * $matches[3] = tag name prefix
     * $matches[4] = tag name sufix
     * $matches[5] = attributes
     * $matches[6] = closing quote ('>' or '/>')
     *
     *
     * @access  private
     * @param   string[]    $matches    preg_pattern matches
     * @return  string                  the replacement string for the match
     */
    function _tnt_replace_tags($matches) {
    
        //echo '<pre>'.htmlentities(var_export($matches, true)).'</pre>';

        if (!isset($this->_tags[$matches[3]][$matches[4]])) {
            $this->trigger_error(htmlentities("Tag '{$matches[0]}' is not recognised"), E_USER_WARNING);
            return $matches[0];
        }

        $tag = $this->_tags[$matches[3]][$matches[4]];

        $l = $this->left_delimiter;
        $r = $this->right_delimiter;
        $n = $tag[0];
        $a = $matches[5];

        $is_block = $tag[1];

        if ($matches[6] == '/>') {
            // Empty tag
            if ($is_block) {
                // Replace with an empty template block command
                return "$l$n$a$r$l/$n$r";
            } else {
                // Replace with a template function command
                return "$l$n$a$r";
            } 
        } else if ($matches[1] == '</') {
            // Close tag
            if (!$is_block) {
                $this->trigger_error("\'{$matches[0]}\' close tag not allowed, tag must be complete, (ie. \'<tag />\')", E_USER_ERROR);
                return '';
            }
            // Replace with a close block command
            return "$l/$n$r";
        } else {
            // Open tag
            return "$l$n$a$r";
        }
    }
    
    /**
     * A smarty prefilter for TNT templates.
     *
     * The prefilter does the following;
     *  + removes comments in '<!--*' & '*--!>' delimiters
     *  + fixes relative paths to be relative to the template source
     *  + processes any '<%@ ... %>' directives
     *  + replaces any aliased HTML tags with their alias
     *  + replaces any registered HTML tags with smarty commands
     *  + prevents the parsing of the content of '{template}' commands.
     *
     * @access  private
     * @param   string      $source     source to prepare
     * @return  string                  prepared source
     */
    function _tnt_prepare($source) { 
        TNT::Timestamp('_tnt_prepare()');

        $this->_fixup_base = dirname(TNTFileSystem::SwitchPath($this->_current_file, TNT_FILESYSTEM_SYSTEMTOSERVER)).'/';
    
        // Strip all TNT comments
        if (strstr($source, '<!--') !== FALSE) {
            $source = preg_replace('/<!--\*.*\*-->/Us', '', $source);
        }

        // Replace all path starting './' so they are relative to the
        // template path
        $sch = array(
            '/((href|src|background)\s*=\s*([\\\'"])(?!{)(.*)\3)/Uis',
            '/((file)\s*=\s*([\\\'"])(?!{)(tnttpl:.*)\3)/Ui',
            '/((url)\s*\\(\s*([\\\'"]?)(?!{)([^\\\'"]*)\3?\\))/Uis');
        $fcn = array(&$this, '_tnt_fixup_paths');
        $source = preg_replace_callback($sch, $fcn, $source);
    
        // Extract all @directives
        if (strstr($source, '<%@') !== FALSE) {
            $sch = '~<%@\s*(\w+)(.*?)%>~i';
            $fcn = array(&$this, '_tnt_decode_directives');
            $source = preg_replace_callback($sch, $fcn, $source);
        }

        // Process all directives
        _smarty_load_directives($this->_directives, $this);

        // Replace all tag aliases
        if (!empty($this->_aliases)) {
            $sch = $rep = array();
            foreach ($this->_aliases as $from => $to) {
                $sch[] = '~(</?\s*)'.preg_quote($from, '~').'([^\w])~is';
                $rep[] = '\1'.preg_quote($to, '~').'\2';
            }
            //echo '<pre>sch='.htmlentities(var_export($sch, true)).'</pre>';
            //echo '<pre>rep='.htmlentities(var_export($rep, true)).'</pre>';
            $source = preg_replace($sch, $rep, $source);
        }
    

        // Replace all TNT specific HTML style tags with Smarty calls
        if (!empty($this->_tags)) {
            $pfx = array();
            foreach (array_keys($this->_tags) as $t) {
                $pfxs[] = preg_quote($t, '~');
            }
            $sch = '~(</?)\s*(('.implode('|', $pfxs).'):([\w.-]+))(.*?)(/?>)~is';
            $fcn = array(&$this, '_tnt_replace_tags');
            $source = preg_replace_callback($sch, $fcn, $source);
        }

        // Wrap the content of '{template}' directives with '{literal}' tags
        // so the content is not parsed.
        $ldq = preg_quote($this->left_delimiter, '~');
        $rdq = preg_quote($this->right_delimiter, '~');
        $source = preg_replace("~({$ldq}\s*template (?:.*?)\s*{$rdq})(.*?)({$ldq}\s*/literal\s*{$rdq})~is",
                               "\1{$ldq}literal{$rdq}\2{$ldq}/literal{$rdq}\3", $source);

        TNTDebug::Dump('Directives=', $this->_directives);
        TNTDebug::Dump('Tags=', $this->_tags);
        TNTDebug::Dump('Aliases=', $this->_aliases);
        TNTDebug::Dump('Plugins=', $this->_plugins);
        TNTDebug::Dump('Source=', $source);
        TNTDebug::Dump('Variables=', $this->_tpl_vars);

        TNT::Timestamp('_tnt_prepare() - end');
        return $source;
    }

    /**
     * Construct the object.
     *
     * @access  public
     */
    function _TNTSmartyCompiler() {
        Smarty_Compiler::Smarty_Compiler();

    }

    /**
     * Destruct the object
     *
     * @access  private
     */
    function __TNTSmartyCompiler() {
    }
}

/**
 * get calleable for given plugin.
 *
 * This will return the registered value if present otherwise it will call
 * {@link _get_plugin_filepath()} to locate the plugin file and then
 * include it use the default 'smarty_$type_$name' callable function.
 *
 * If the plugin is found but the callable doesn't exist then an error will
 * be triggered and false returned.
 * If the plugin is not found and is required then an error will be
 * triggered and false returned.
 * If the plugin is not found and is not required then no error is triggered
 * and false is returned.
 *
 * @access  private
 * @param   object      $smarty     reference to template
 * @param   string      $type       type of plugin
 * @param   string      $name       name of plugin
 * @param   boolean     $required   plugin is required
 * @return  string|false
 */
function _smarty_get_plugin_call(&$smarty, $type, $name, $required = false) {
    $callable = null;
    if (isset($smarty->_plugins[$type][$name])) {
        $callable = ($smarty->_plugins[$type][$name][0] ? $smarty->_plugins[$type][$name][0] : "smarty_{$type}_{$name}");
    } else if ($file = $smarty->_get_plugin_filepath($type, $name)) {
        require_once $file;
        $callable = "smarty_{$type}_{$name}";
    }

    if (!$callable && $required) {
        $smarty->trigger_error("$type '$name' is not implemented", E_USER_WARNING);
        return null;
    }

    if ($callable) {
        if (!_smarty_check_plugin_call($smarty, $type, $name, $callable)) {
            return null;
        }

        $smarty->_plugins[$type][$name][0] = $callable;
    }

    return $callable;
}

/**
 * check a plugin callable is actually callable.
 *
 * @access  private
 * @param   object      $smarty     reference to template
 * @param   string      $type       type of plugin
 * @param   string      $name       name of plugin
 * @param   callable    $callable   callable to validate
 * @return  callable                the PHP callable
 */
function _smarty_check_plugin_call(&$smarty, $type, $name, $callable) {
    if (is_string($callable) && count($callable = explode(':', $callable)) == 1) {
        $callable = $callable[0];
        TNTLoader::RequireFunction($callable);
    }
    if (is_array($callable)) {
        if (is_string($callable[0])) {
            TNTLoader::RequireClass($callable[0]);
        }
    }

    if (!is_callable($callable)) {
        $smarty->trigger_error("$type '$name' is not implemented", E_USER_WARNING);
        return null;
    }

    return $callable;
}

/**
 * Load all the directives.
 *
 * @access  private
 * @param   array       $directives     array of directives to load
 * @param   object      $smarty         reference to smarty
 * @return  void
 */
function _smarty_load_directives($directives, &$smarty) {
    if (empty($directives)) {
        return;
    }
    foreach ($directives as $directive) {
        $type = $directive[0];
        $attrs =& $directive[1];
        switch ($type) {
            case 'register':
                $type = $attrs['type'];
                $name = str_replace(':', '_', $attrs['name']);
                $call = (isset($attrs['call']) ? (strstr(':', $attrs['call']) ? explode(':', $attrs['call']) : $attrs['call']) : null);
                $cacheable = (isset($attrs['cacheable']) ? (strtolower($attrs['cacheable']) == 'yes') : true);
                $cache_attrs = (isset($attrs['cache_attrs']) ? explode(':', strtolower($attrs['cache_attrs'])) : null);
                if ($call && !($call = _smarty_check_plugin_call($smarty, $type, $name, $call))) {
                    break;
                }

                $smarty->_plugins[$type][$name] = 
                    array($call, null, null, false, $cacheable, $cache_attrs);

                break;

            case 'taglib':
                $name = $attrs['name'];

                // If we haven't already loaded smarty tag library then do so
                if (!isset($smarty->_plugins[$type][$name]['_tags'])) {
                    if (!($callable = _smarty_get_plugin_call($smarty, $type, $name, true))) {
                        break;
                    }

                    if (!($tags = $callable($attrs, $smarty))) {
                        $smarty->trigger_error("taglib '{$name}' can not be loaded", E_USER_WARNING);
                        break;
                    }

                    $smarty->_plugins[$type][$name]['_tags'] = $tags;
                }

                $smarty->_tags[$attrs['prefix']] =& $smarty->_plugins[$type][$name]['_tags'];

                break;

            case 'import':
                if (isset($attrs['name'])) {
                    $name = $attrs['name'];
                } else {
                    $name = (is_array($attrs['call']) ? "{$attrs['call'][0]}::{$attrs['call'][1]}" : $attrs['call']);
                }

                // If we haven't already loaded smarty module then do so
                if (!isset($smarty->_plugins[$type][$name][0])) {
                    if (isset($attrs['name'])) {
                        if (!($callable = _smarty_get_plugin_call($smarty, $type, $name, true))) {
                            break;
                        }
                    } else {
                        if (!($callable = _smarty_check_plugin_call($smarty, $type, $name, $attrs['call'], true))) {
                            break;
                        }
                    }

                    if (!$callable($attrs, $smarty)) {
                        $smarty->trigger_error("module '{$name}' can not be loaded", E_USER_WARNING);
                        break;
                    }

                    $smarty->_plugins[$type][$name][0] = $callable;
                }

                break;

            default:
                // If we haven't already loaded smarty directive then do so
                if (!($callable = _get_plugin_call($smarty, 'directive', $type, false))) {
                    break;
                }

                if (!$callable($attrs, $smarty)) {
                    $smarty->trigger_error("directive '{$type}' can not be loaded", E_USER_WARNING);
                    break;
                }

                break;
        }
    }

}

/**
 * Load all the plugins.
 *
 * Replaces call to smarty_core_load_plugins in templates.
 *
 * Preloads all plugins that are callables.
 *
 * @access  private
 * @param   array       $params         array of parameters
 * @param   object      $smarty         reference to smarty
 * @return  void
 */
function _smarty_load_plugins($params, &$smarty) {
    foreach ($params['plugins'] as $_plugin_info) {
        list($_type, $_name, $_tpl_file, $_tpl_line, $_delayed_loading) = $_plugin_info;
        $_plugin = &$smarty->_plugins[$_type][$_name];

        if (isset($_plugin)) {
            if (empty($_plugin[3])) {
                if (!is_callable($_plugin[0])) {
                    // Ensure the code is loaded
                    if (is_array($_plugin[0]) && is_string($_plugin[0][0])) {
                        TNTLoader::RequireClass($_plugin[0][0]);
                    } else if (is_string($_plugin[0])) {
                        TNTLoader::RequireFunction($_plugin[0]);
                    }
                }
            }
        }
    }

    // Call original smarty core method
    smarty_core_load_plugins($params, $smarty);
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
