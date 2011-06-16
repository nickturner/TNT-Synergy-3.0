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
 * Template class
 * ~~~~~~~~~~~~~~
 *
 * This file holds the basic template class that is used to parse templates.
 * The standard PHP templating engine Smarty (http://smarty.php.net) is used
 * as the core template parsing engine.
 *
 * @package TNT.View
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
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
 * Template parsing class.
 *
 * The class is responsible for parsing all site templates.
 *
 * A template resource is specified as follows;
 *   <type>:<name>
 * where the common types are;
 *   'file:<path>'  - load template from filesystem path (relative to root dirs)
 *   'db:<dsn>'     - load template from a TNTDbStore() object
 *   'mem:<dsn>'    - load template from a TNTMemoryStore() object
 *   'data:<str>'   - use <str> as the actual template
 *
 * @package TNT.View
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
class TNTTemplate extends TNTObject {

    /**
     * Name of Smarty template resource
     * @access  private
     * @var     string                  smarty template resource name
     */
    var $_tpl = null;

    /**
     * Cache identifier for this template
     * @access  private
     * @var     string                  smarty cache identifier
     */
    var $_cacheId = null;

    /**
     * Compile identifier for this template
     * @access  private
     * @var     string                  smarty compile identifier
     */
    var $_compileId = null;

    /**
     * Smarty object
     * @access  private
     * @var     object                  smarty template parser
     */
    var $_smarty = null;

    /**
     * Timestamp all parsed templates with an HTML comment showing how
     * long the template took to parse and whether is was cached or not.
     * @access  private
     * @var     boolean                 true if templates are timestamped
     */
    var $_timestamp = true;

    /**
     * Return if the template object is valid (ie. contains a good template)
     *
     * @access  public
     * @return  bool                    true if the template is valid for use
     */
    function IsValid() {
        return (!is_null($this->_tpl) &&
                $this->_smarty->template_exists($this->_tpl));
    }

    /**
     * Check the named template exists
     *
     * Check the given Smarty template resource exists.
     *
     * @access  public
     * @return  bool                    true if the named template exists
     */
    function TemplateExists($tpl) {
        return $this->_smarty->template_exists($tpl);
    }

    /**
     * Set the default template details
     *
     * This will not check the template exist, it will just check the
     * data is well formed. Is mainly used to set the defaults to be
     * used in the other method.
     *
     * @access  public
     * @param   string      $tpl       name of template resource
     * @return  bool                   true if set ok
     */
    function SetTemplate($tpl = null) {
        $this->_tpl = $tpl;
        return true;
    }

    /**
     * Get the default template details
     *
     * @access  public
     * @return  string                  name of template resource (or null)
     */
    function GetTemplate() {
        return $this->_tpl;
    }

    /**
     * Set the default caching details
     *
     * @access  public
     * @param   string      $cache_id  cache identifier (optional)
     * @param   string      $compile_id compile identifier (optional)
     * @return  bool                   true if set ok
     */
    function SetCache($cache_id = null, $compile_id = null) {
        $this->_cacheId = $cache_id;
        $this->_compileId = $compile_id;
        return true;
    }

    /**
     * Get the default caching details
     *
     * @access  public
     * @return  array               array containing cache id and compile id
     */
    function GetCache() {
        return array($this->_cacheId, $this->_compileId);
    }

    /**
     * Get the template delimiters
     *
     * These are the tag markers, by default we use the standard Smarty
     * markers of '{' and '}'.
     *
     * @access  public
     * @param   string      $ldelim    left delimiter
     * @param   string      $rdelim    right delimiter
     * @return  void
     */
    function SetDelimiters($ldelim = '{', $rdelim = '}') {
        $this->_smarty->left_delimiter = $ldelim;
        $this->_smarty->right_delimiter = $rdelim;
    }

    /**
     * Get the template delimiters
     *
     * @access  public
     * @return  array               array containing left & right delimiter
     */
    function GetDelimiters() {
        return array($this->_smarty->left_delimiter, $this->_smarty->right_delimiter);
    }

    /**
     * Assign value to variable, (alternatively $variable can be associative
     * array of 'variable' => 'value')
     *
     * @access  public
     * @param   mixed       $variable   name of variable (or associative array)
     * @param   mixed       $value      value of variable (or null)
     * @return  void
     */
    function Assign($variable, $value = null) {
        $this->_smarty->assign($variable, $value);
    }

    /**
     * Assign variable to a another by reference
     *
     * Prevents need to copy large variable arrays or alternatively allows
     * variable to be 'live' and keep the current value of the '$value'
     * variable.
     *
     * Alternatively $variable can be associative array of
     * 'variable' =>& 'value' elements. In this case $value is not used but
     * must be passed as references can't have default values.
     *
     * @access  public
     * @param   string      $variable   name of variable
     * @param   mixed       $value      reference to variable
     * @return  void
     */
    function AssignByRef($variable, &$value) {
        $this->_smarty->assign_by_ref($variable, $value);
    }

    /**
     * Appends values to template variables
     *
     * @access  public
     * @param   mixed       $variable   name of variable (or associative array)
     * @param   mixed       $value      the  value to append
     * @return  void
     */
    function Append($variable, $value = null, $merge = false) {
        $this->_smarty->append($variable, $value, $merge);
    }

    /**
     * Appends values to template variables by reference
     *
     * @access  public
     * @param   string      $variable   the template variable name
     * @param   mixed       $value      the referenced value to append
     * @return  void
     */
    function AppendByRef($variable, &$value, $merge = false) {
        $this->_smarty->append_by_ref($variable, $value, $merge);
    }

    /**
     * Unassign the given template variable.
     *
     * If no variable name is specified then all variables are unassigned.
     *
     * @access  public
     * @param   string      $variable   the template variable to clear
     * @return  void
     */
    function Unassign($variable) {
        if (is_null($variable)) {
            $this->_smarty->clear_all_assign();
        } else {
            $this->_smarty->clear_assign($variable);
        }
    }

    /**
     * Get the array of assigned variables
     *
     * If a variable name is passed then the value of that variable is
     * returned and not an array of all variables.
     *
     * The return is a reference to the variable or array of variables.
     *
     * @access  public
     * @param   string      $variable   the name of a variable to return
     * @return  array                   reference to array of variables
     */
    function &Variables($variable = null) {
        return $this->_smarty->get_template_vars($variable);
    }

    /**
     * Registers object to be used in templates
     *
     * [Objects are currently only supported by Smarty type templates]
     *
     * This allows access to PHP objects through the templates using a syntax
     * similar to custom functions. This method is prefered over just assigning
     * the object as a variable via {@link AssignByRef()}.
     *
     * Using this method allows for a much nicer template syntax. It is also
     * more secure, as a registered object can be restricted to certain methods
     * and properties. However, a registered object cannot be looped over or
     * assigned in arrays of objects, etc.
     *
     * You can restrict the methods and properties that can be accessed by
     * listing them in an array as the $allowed parameter. 
     *
     * By default, parameters passed to objects through the templates are
     * passed the same way custom functions get them. An associative array is
     * passed as the first parameter, and the underlying template parseing
     * engine as the second. If you want the parameters passed one at a time
     * for each argument like traditional PHP parameter passing, set the
     * $tradition parameter to true.
     *
     * The optional $block_methods parameter has only effect with $phpargs
     * being false and contains a list object methods that should be treated
     * as blocks. That means these methods have a closing tag in the template
     * ({foobar->meth2}...{/foobar->meth2}) and the parameters to the methods
     * have the same synopsis as the parameters for block-function-plugins:
     * They get 4 parameters $params, $content, &$smarty and &$repeat and they
     * also behave like block-function-plugins. 
     *
     * @param   string      $name       name of template object
     * @param   object&     $obj        referenced to object being registered
     * @param   array       $allowed    list of allowed methods (empty = all)
     * @param   boolean     $smarty     use smarty calling method, else PHP
     * @param   array       $block_methods list of methods that are block format
     * @return  void
     */
    function RegisterObject($name, &$obj, $allowed = null, $smarty = true, $block_methods = null) {
        $this->_smarty->register_object($name, $obj, $allowed, $smarty, $block_methods);
    }

    /**
     * Unregisters an object registered with {@link RegisterObject}
     *
     * If no object name is specified then all objects are unregistered.
     *
     * @access  public
     * @param   string      $name       name of template object
     * @return  void
     */
    function UnregisterObject($name) {
        if (is_null($name)) {
            $this->_smarty->unregister_all_objects();
        } else {
            $this->_smarty->unregister_object($name);
        }
    }

    /**
     * Registers a custom plugin to be used in templates.
     *
     * The '$name' parameter is the name used in the templates.
     *
     * The '$type' parameter can be one of the following values;
     *   + 'function'
     *   + 'block'
     *   + 'compiler' ('cache_attrs' parameter ignored)
     *   + 'modifier' ('cacheable' & 'cache_attrs' parameter ignored)
     *
     * The '$func' parameter can be either: 
     *   + A string containing the function name 
     *   + An array of the form array(&$object, $method) with &$object being a
     *     reference to an object and $method being a string containing the
     *     name of a method of the object.
     *   + An array of the form array($class, $method) with $class being a
     *     string containg a static class name and $method being a string
     *     containing the name of a method of the class. 
     *
     * When registering a custom plugin other that of type 'modifier' then
     * the '$cacheable' and '$cache_attrs' parameters can be used to control
     * the cacheability of the plugin. If '$cacheable' is equal to false then
     * the plugin is called everytime the template is fetched, even if the
     * data comes from the cache. The plugin function behaves a little like
     * the standard Smarty '{insert}' function. 
     *
     * In contrast to the '{insert}' function the attributes to the plugin
     * are not cached by default. For function and block plugins they can be
     * declared to be cached with the '$cache_attrs' parameter. This is an
     * array of attribute names that should be cached, so the plugin gets the
     * value as it was the time the page was written to cache everytime it is
     * fetched from the cache.
     *
     * Trying to register a previously registered name will overwrite the
     * previous registration.
     *
     * @access  public
     * @param   string      $name       the name of the template function
     * @param   string      $type       the function type
     * @param   callable    $func       PHP function/method to call
     * @param   boolean     $cachable   true if the function should cached
     * @param   array       $cache_attrs array of attributes to cache
     * @return  boolean                 true if registered ok
     */
    function RegisterPlugin($name, $type, $func, $cacheable = true, $cache_attrs = null) {

        if (!is_string($name)) {
            $this->_SetError('Invalid plugin name');
            return false;
        }

        if (!is_callable($func, true, $cn)) {
            $this->_SetError('Invalid callable: '.$cn);
            return false;
        } 

        switch ($type) {
            case 'function':
                $this->_smarty->register_function($name, $func, $cacheable, $cache_attrs);
                break;
            case 'block':
                $this->_smarty->register_block($name, $func, $cacheable, $cache_attrs);
                break;
            case 'compiler':
                $this->_smarty->register_compiler_function($name, $func, $cacheable);
                break;
            case 'modifier':
                $this->_smarty->register_modifier($name, $func);
                break;
            case 'tag':
                if (count($t = explode(':', $name, 2)) < 2) {
                    array_unshift($t, 'tnt');
                }
                $n = "{$t[0]}_{$t[1]}";
                $this->_smarty->register_block($n, $func, $cacheable, $cache_attrs);
                $this->_tags[$name] = array($name, $t, $n, $func, $cacheable, $cache_attrs);
                break;
            default:
                $this->_SetError('Invalid plugin type: '.$type);
                return false;
        }

        return true;
    }

    /**
     * Unregisters a plugin registered with {@link RegisterPlugin}
     *
     * The '$name' parameter is the name used in the templates.
     *
     * The '$type' parameter can be one of the following values;
     *   + 'function'
     *   + 'block'
     *   + 'compiler'
     *   + 'modifier'
     *   + 'tag'
     *
     * Trying to unregister a non registered plugin does not result in
     * failure, the method will return true.
     *
     * @access  public
     * @param   string      $name       the name of the template function
     * @param   string      $type       the function type
     * @return  boolean                 true if unregistered ok
     */
    function UnregisterPlugin($name, $type) {

        switch ($type) {
            case 'function':
                $this->_smarty->unregister_function($name);
                break;
            case 'compiler':
                $this->_smarty->unregister_compiler_function($name);
                break;
            case 'block':
                $this->_smarty->unregister_block($name);
                break;
            case 'modifier':
                $this->_smarty->unregister_modifier($name);
                break;
            case 'tag':
                if (isset($this->_tags[$name])) {
                    $this->_smarty->unregister_function($this->_tags[$name][2]);
                    unset($this->_tags[$name]);
                }
                break;
            default:
                $this->_SetError('Invalid plugin type: '.$type);
                return false;
        }

        return true;
    }

    /**
     * Test to see if the template has been cached
     *
     * Call with no arguments to used defaults set using {@link SetTemplate()}
     * and {@link SetCache()}.
     *
     * @access  public
     * @param   string      $tpl       name of template resource (optional)
     * @param   string      $cache_id  cache identifier (optional)
     * @param   string      $compile_id compile identifier (optional)
     * @return  bool                true if this template has been cached
     */
    function IsCached($tpl = null, $cache_id = null, $compile_id = null) {
        if (func_num_args() == 0) {
            if (is_null($this->_tpl)) {
                return false;
            }
            $tpl = $this->_tpl;
            $cache_id = $this->_cacheId;
            $compile_id = $this->_compileId;
        }
        return $this->_smarty->is_cached($tpl, $cache_id, $compile_id);
    }

    /**
     * Clear cached copy of template
     *
     * Call with no arguments to used defaults set using {@link SetTemplate()}
     * and {@link SetCache()}.
     *
     * @access  public
     * @param   string      $tpl       name of template resource (optional)
     * @param   string      $cache_id  cache identifier (optional)
     * @param   string      $compile_id compile identifier (optional)
     * @return  bool                   true if cleared
     */
    function ClearCached($tpl = null, $cache_id = null, $compile_id = null) {
        if (func_num_args() == 0) {
            if (is_null($this->_tpl)) {
                return false;
            }
            $tpl = $this->_tpl;
            $cache_id = $this->_cacheId;
            $compile_id = $this->_compileId;
        }
        return $this->_smarty->clear_cache($tpl, $cache_id, $compile_id);
    }

    /**
     * Clear all cached templates.
     *
     * @access  public
     * @param   string      $exp_time  expire time (optional)
     * @return  bool                   true if cleared
     */
    function ClearAllCached($exp_time = null) {
        return $this->_smarty->clear_all_cache($exp_time);
    }

    /**
     * Clear compiled copy of template
     *
     * Call with no arguments to used defaults set using {@link SetTemplate()}
     * and {@link SetCache()}.
     *
     * @access  public
     * @param   string      $tpl       name of template resource (optional)
     * @param   string      $cache_id  cache identifier (optional)
     * @param   string      $exp_time  expire time (optional)
     * @return  bool                   true if cleared
     */
    function ClearCompiled($tpl = null, $compile_id = null, $exp_time = null) {
        if (func_num_args() == 0) {
            if (is_null($this->_tpl)) {
                return false;
            }
            $tpl = $this->_tpl;
            $compile_id = $this->_compileId;
        }
        return $this->_smarty->clear_compiled_tpl($tpl, $compile_id, $exp_time);
    }

    /**
     * Clear all compiled templates.
     *
     * @access  public
     * @param   string      $exp_time  expire time (optional)
     * @return  bool                   true if cleared
     */
    function ClearAllCompiled($exp_time = null) {
        return $this->_smarty->clear_compiled_tpl(null, null, $exp_time);
    }

    /**
     * Parse the template
     *
     * Parse the Smarty template resource set via the {@link SetTemplate()}
     * method returning the result.
     *
     * Any errors produced by the parsing can be seen by calling the
     * {@link Error()} method.
     *
     * Call with no arguments to used defaults set using {@link SetTemplate()}
     * and {@link SetCache()}.
     *
     * @access  public
     * @param   string      $tpl       name of template resource (optional)
     * @param   string      $cache_id  cache identifier (optional)
     * @param   string      $compile_id compile identifier (optional)
     * @return  string              the parsed content (or null)
     */
    function Parse($tpl = null, $cache_id = null, $compile_id = null) {

        $ret = null;

        if (func_num_args() == 0) {
            if (is_null($this->_tpl)) {
                return null;
            }
            $tpl = $this->_tpl;
            $cache_id = $this->_cacheId;
            $compile_id = $this->_compileId;
        }

        if ($this->_timestamp) {
            $st = microsecs();
        }

        // Makesure errors are easily detected. This will trap more errors than
        // a custom error handler could detect. It will trap parser and compile
        // errors etc. The custom error handler is still needed to prevent the
        // code from treating E_USER_ERROR etc. as a fatal error and stopping
        // execution
        /*
        $ini_eh = set_error_handler('_TNTTemplate_ErrorHandler');
        $ini_el = error_reporting(E_ALL);
        $ini_de = ini_set('display_errors', '1');
        $ini_he = ini_set('html_errors', '0');
        $ini_ps = ini_set('error_prepend_string', '<template_error>');
        $ini_as = ini_set('error_append_string', '</template_error>');
        */

        // anything output by the parser is assume to be an error
        ob_start();
        $content = $this->_smarty->Fetch($tpl, $cache_id, $compile_id);
        $output = ob_get_contents();
        ob_end_clean();

        // Revert to original error settings
        /*
        if ($ini_eh) {
            set_error_handler($ini_eh);
        } else {
            restore_error_handler();
        }
        error_reporting($ini_el);
        ini_set('display_errors', $ini_de);
        ini_set('html_errors', $ini_he);
        ini_set('error_prepend_string', $ini_ps);
        ini_set('error_append_string', $ini_as);
        */

        if ($this->_timestamp) {
            $iscached = $this->_smarty->is_cached($tpl, $cache_id, $compile_id);
            $content = '<!-- Template: name="'.htmlentities($tpl).'", generated="'.htmlentities(date('r')).'" cached='.($iscached ? 'true' : 'false').' -->'.$content.'<!-- Template parsed in '.sprintf('%.03f', microsecs() - $st).' seconds'.($iscached ? ' (from cache)' : '').' -->';
        }

        //TRACE $this->Trace("Template Output '{$this->name}':", explode("\n", $output));

        // Scan the content and output for errors
        $errors = null;
        if (preg_match_all('/<template_error>(.*)<\\/template_error>/Uis', $content.$output, $matches)) {
            foreach ($matches[1] as $match) {
                $errors[] = strip_tags($match);
            }
        }

        if ($errors) {
            $this->_SetError($errors);
            $ret = null;
        } else {
            $ret = $content;
        }

        return $ret;
    }

    /** 
     * Contruct object
     *
     * @access  public
     * @param   string      $tpl       name of template resource (optional)
     * @param   string      $cache_id  cache identifier (optional)
     * @param   string      $compile_id compile identifier (optional)
     */
    function TNTTemplate($tpl = null, $cache_id = null, $compile_id = null) {

        TNTObject::TNTObject();

        TNTLoader::RequireClass('_TNTSmarty');

        $this->_smarty =& new _TNTSmarty($this);

        // Initialise smarty from the registry

        // Template file directories
        $this->_smarty->template_dir = array();
        if (($value = TNT::RegGetVar('templates.smarty.template_dirs', null)) && !is_blank_str($value)) {
            foreach ((array)$value as $dir) {
                $this->_smarty->template_dir[] = TNTFileSystem::ExpandPath($dir);
            }
        }
        $this->_smarty->template_dir[] = TNTFileSystem::ExpandPath('~theme/templates/');
        $this->_smarty->template_dir[] = TNTFileSystem::ExpandPath('~theme/');
        $this->_smarty->template_dir[] = TNTFileSystem::ExpandPath('~/templates');
        $this->_smarty->template_dir[] = TNTFileSystem::ExpandPath('~/');

        // Configuration file directories
        $this->_smarty->config_dir = array();
        if (($value = TNT::RegGetVar('templates.smarty.config_dirs', null)) && !is_blank_str($value)) {
            foreach ((array)$value as $dir) {
                $this->_smarty->config_dir[] = TNTFileSystem::ExpandPath($dir);
            }
        }
        $this->_smarty->config_dir[] = TNTFileSystem::ExpandPath('~theme/templates/config/');
        $this->_smarty->config_dir[] = TNTFileSystem::ExpandPath('~/templates/config/');

        // Plugin file directories
        $this->_smarty->plugins_dir = array();
        if (($value = TNT::RegGetVar('templates.smarty.plugin_dirs', null)) && !is_blank_str($value)) {
            foreach ((array)$value as $dir) {
                $this->_smarty->plugins_dir[] = TNTFileSystem::ExpandPath($dir);
            }
        }

        $this->_smarty->plugins_dir[] = TNTFileSystem::ExpandPath('~theme/templates/plugins/');
        $this->_smarty->plugins_dir[] = TNTFileSystem::ExpandPath('~/templates/plugins/');

        // Auto load filters
        $this->_smarty->autoload_filters = array();
        if (($value = TNT::RegGetVar('templates.smarty.autoload_filters', null)) && !is_blank_str($value)) {
            $this->_smarty->autoload_filters = value;
        }

        $this->_smarty->compile_dir = TNTFileSystem::ExpandPath(TNT::RegGetVar('templates/smarty/compile_dir', '~temp/templates/'));
        $this->_smarty->cache_dir = TNTFileSystem::ExpandPath(TNT::RegGetVar('templates/smarty/cache_dir', '~cache/templates/'));

        $this->_smarty->left_delimiter = TNT::RegGetVar('templates.smarty.left_delimiter', $this->_smarty->left_delimiter);
        $this->_smarty->right_delimiter = TNT::RegGetVar('templates.smarty.right_delimiter', $this->_smarty->right_delimiter);
        $this->_smarty->default_modifiers = TNT::RegGetVar('templates.smarty.default_modifiers', null);
        $this->_smarty->debugging = TNT::RegGetVar('templates.smarty.debugging', $this->_smarty->debugging);
        $this->_smarty->error_reporting = TNT::RegGetVar('templates.smarty.error_reporting', $this->_smarty->error_reporting);
        $this->_smarty->debugging_ctrl = TNT::RegGetVar('templates.smarty.debugging_ctrl', $this->_smarty->debugging_ctrl);
        $this->_smarty->compile_check = TNT::RegGetVar('templates.smarty.compile_check', $this->_smarty->compile_check);
        $this->_smarty->force_compile = TNT::RegGetVar('templates.smarty.force_compile', $this->_smarty->force_compile);
        $this->_smarty->caching = TNT::RegGetVar('templates.smarty.caching', $this->_smarty->caching);
        $this->_smarty->cache_lifetime = TNT::RegGetVar('templates.smarty.cache_lifetime', $this->_smarty->cache_lifetime);
        $this->_smarty->cache_modified_check = TNT::RegGetVar('templates.smarty.cache_modified_check', $this->_smarty->cache_modified_check);
        $this->_smarty->php_handling = TNT::RegGetVar('templates.smarty.php_handling', $this->_smarty->php_handling);
        $this->_smarty->security = TNT::RegGetVar('templates.smarty.security', $this->_smarty->security);

        // Assign all the constants as variables
        $this->_smarty->assign('CONSTANTS', get_defined_constants());

        // Assign ourself as the current template
        $this->_smarty->assign_by_ref('template', $this);
        $this->_smarty->register_object('template', $this, null, false, null);

        // Ensure the compile dir and cache dir exist
        TNTFileSystem::MkDir($this->_smarty->compile_dir);
        TNTFileSystem::MkDir($this->_smarty->cache_dir);

        $this->SetTemplate($tpl);
        $this->SetCache($cache_id, $compile_id);

        TNTDebug::Dump('template=', var_info($this));
    }

    /** 
     * Destruct object
     *
     * @access  public
     */
    function _TNTTemplate() {
    }

}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
