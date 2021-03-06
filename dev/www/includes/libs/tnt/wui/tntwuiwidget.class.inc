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
 * WUI Widget Class 
 * ~~~~~~~~~~~~~~~~
 *
 * This is the base class for a Widget in the Web User Interface library.
 *
 * @package TNT.Wui
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTNode');

/*
 * Globals
 */
global $tnt;
$tnt['_tntwui_nodes'] = array();

/*
 * Defines
 */

/**
 * Class to represent a Widget in the Web User Interface.
 *
 * This class is used as the basis of all visible objects. It supports
 * initialisation from a template and rendering via a template.
 *
 * On rendering if both static content and a template are provided then the
 * template will be used and the static content made available to it as the
 * 'content' variable.
 *
 * A widget can have properties which control how it is displayed, these can
 * be set using the {@link SetXXX()} and {@link GetXXX()} methods, and the
 * generic {@link TNTObject::Set()} and {@link TNTObject::Get()} methods.
 *
 * Derived classes should add new custom Get/Set method for each property and
 * the generic methods will call the custom methods if they use the 'GetXXX()',
 * 'SetXXX()' syntax (where 'XXX' is the property name).
 *
 * All properties that can be set from a template should accept a string value
 * to their 'Set<name>()' method. This is a string is usually the value
 * specified in any the template used to create the widget.
 *
 * Widgets can be constructed in heirachies where a parent widget can contain
 * child widgets etc.
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiWidget extends TNTNode {

    /**
     * CSS class name
     * @access private
     * @var string                      a valid alignment name
     */
    var $_class = null;

    /**
     * CSS styles
     * @access private
     * @var TNTCssStyle                 reference to CSS style class
     */
    var $_style = null;

    /**
     * The widgets inner contents as stored by {@link SetContent()}.
     * @access private
     * @var string                      inner contents of block tag
     */
    var $_content = null;

    /**
     * Optional template that should be used to render the widget
     * @access private
     * @var TNTTemplate&                reference to a data map
     */
    var $_template = null;

    /**
     * Display a widget in a template.
     *
     * This is a static method which is called to create and display a widget
     * from a template function plugin.
     *
     * It will create the widget, initialise it from the given parameters and
     * then return its rendering.
     *
     * A widget hierachy is built as the widgets are created, this widget is
     * added to the child list of the current parent widget.
     *
     * @access  public
     * @param   mixed       $params     assoc. array of parameters
     * @param   object&     $caller     reference to the calling template engine
     * @return  boolean                 true if successfull
     */
    function _TemplatePluginFunction($params, &$caller) {
        global $tnt;

        if (!empty($params['__widget'])) {
            // Create a widget of the specified class
            $class = $params['__widget'];
            unset($params['__widget']);

            // Include the correct class definition for this widget
            if (!TNTLoader::RequireClass($class, false)) {
                $caller->ReportError("unknown widget '$class' in widget tag", E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }
        } else {
            // Create widget of the same class as the method was called in
            $class = get_class_name();
        }

        // Create the widget
        $widget =& new $class();

        // If there is a ancester widget then add this widget as its child
        if (($c = count($tnt['_tntwui_nodes']))) {
            $parent =& $tnt['_tntwui_nodes'][$c - 1]['widget'];
            //echo "Adding widget {$params['id']} to {$parent->id}</pre>";
            if (!$parent->children->Add($widget)) {
                $caller->ReportError('Widget can not be a child of this parent: '.$parent->children->Error(), E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }
        }

        //timestamp('Template widget created: '.$class);

        // Initialise the widget
        if (!$widget->Set($params)) {
            $caller->ReportError($widget->Error(), E_USER_ERROR, __FILE__, __LINE__);
            destroy($widget); // This will also remove it from any parent
            return '';
        }

        // Register widget with template
        $widget->TemplateRegister($caller->tnttemplate, true);

        // Render the widget
        $content = $widget->Render();
        if (is_null($content)) {
            $caller->ReportError($widget->Error(), E_USER_ERROR, __FILE__, __LINE__);
            destroy($widget); // This will also remove it from any parent
            return '';
        }

        //timestamp('Template widget finished');

        // Unregister widget with template
        $widget->TemplateRegister($caller->tnttemplate, false);

        if (!$widget->parent) {
            // Finished with the widget now
            destroy($widget);
        }

	    if (isset($params['assign'])) {
		    $caller->Assign($params['assign'], $content);
            return '';
	    }

		return $content;
    }

    /**
     * Display a widget in a template.
     *
     * This is a static method which is called to create and display a widget
     * from a template block plugin.
     *
     * It will create the widget, initialise it from the given parameters and
     * then return its rendering.
     *
     * The method is called on the open and close template tags, on the open
     * tag the $content is null, on the close tag it contains the output
     * created by the template content between the tags.
     *
     * A widget hierachy is built as the widgets are created, on the open tag
     * this widget is added to the child list of the current parent widget and
     * then becomes the current parent widget, on the close tag the previous
     * parent widget is restored.
     *
     * @access  public
     * @param   mixed       $params     assoc. array of parameters
     * @param   string      $content    block content
     * @param   object&     $caller     reference to the calling template engine
     * @return  boolean                 true if successfull
     */
    function _TemplatePluginBlock($params, $content, &$caller) {
        global $tnt;

        if (!empty($params['__widget'])) {
            // Create a widget of the specified class
            $class = $params['__widget'];
            unset($params['__widget']);

            // Include the correct class definition for this widget
            if (!TNTLoader::RequireClass($class, false)) {
                $caller->ReportError("unknown widget '$class' in widget tag", E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }
        } else {
            // Create widget of the same class as the method was called in
            $class = get_class_name();
        }

        if (is_null($content)) {
            // Opening tag - create the widget but don't output any thing yet

            // Create the widget
            $widget =& new $class();

            // If there is a ancester widget then add this widget as its child
            if (($c = count($tnt['_tntwui_nodes']))) {
                $parent =& $tnt['_tntwui_nodes'][$c - 1]['widget'];
                //echo "Adding widget {$params['id']} to {$parent->id}</pre>";
                if (!$parent->children->Add($widget)) {
                    $caller->ReportError('Widget can not be a child of this parent: '.$parent->children->Error(), E_USER_ERROR, __FILE__, __LINE__);
                    return '';
                }
            }

            // Store away the widget on the stack
            array_push($tnt['_tntwui_nodes'], array('widget' =>& $widget, 'params' =>& $params));

            // Initialise the widget
            if ($params && !$widget->Set($params)) {
                $caller->ReportError($widget->Error(), E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }

            // Register widget with the template
            $widget->TemplateRegister($caller->tnttemplate, true);

            return '';

        } else {
            // Closing tag - set widgets content and render it
    
            if (!($w = array_pop($tnt['_tntwui_nodes']))) {
                $caller->ReportError('widget close tag mismatch - no open tag', E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }

            if (!isset($w['widget']) || strcasecmp(get_class($w['widget']), $class) != 0) {
                $caller->ReportError('widget close tag mismatch - close of \''.get_class($w['widget']).'\' doesn\'t match open of \''.$class.'\'', E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }
    
            $widget =& $w['widget'];
    
            // Initialise the widgets content
            if (!$widget->SetContent($content)) {
                $caller->ReportError($widget->Error(), E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }
    
            // Render the widget
            $content = $widget->Render();
            if (is_null($content)) {
                $caller->ReportError($widget->Error(), E_USER_ERROR, __FILE__, __LINE__);
                return '';
            }

            // Unregister widget with template
            $widget->TemplateRegister($caller->tnttemplate, false);

            if (!$widget->parent) {
                // Finished with the widget now
                destroy($widget);
            }

    	    if (isset($params['assign'])) {
		        $caller->Assign($params['assign'], $content);
                return '';
	        }

		    return $content;
        }
    }

    /**
     * Called to allow the object to register itsself with a Template.
     *
     * The widget should always register the following as both template
     * variables and objects (this base method does this);
     *   'self' - reference to itself
     *   'widget' - reference to itself
     *   $id - reference to itself if it has been assigned an id
     *
     * When the widget registers as part of a block at the end of the block
     * the widget should unregister and restore the 'self' and 'widget'
     * variable back to that of any parent widget. To indicate this pass
     * the optional seconds parameter as false.
     *
     * @access  public
     * @param   TNTTemplate&    $template   reference to the template
     * @param   boolean         $register   register or unregister
     * @return  boolean                     true if successfull
     */ 
    function TemplateRegister(&$template, $register = true) {
        if ($register) {
            $template->AssignByRef('widget', $this);
            $template->RegisterObject('widget', $this, null, false);
    
            $template->AssignByRef('self', $this);
            $template->RegisterObject('self', $this, null, false);
    
            if (($id = $this->GetId())) {
                $template->AssignByRef($id, $this);
                $template->RegisterObject($id, $this, null, false);
            }
        } else {
            $template->UnAssign('widget');
            $template->UnRegisterObject('widget');
    
            $template->UnAssign('self', $this);
            $template->UnRegisterObject('self');
    
            if (($id = $this->GetId())) {
                $template->UnAssign($id);
                $template->UnRegisterObject($id);
            }

            if ($this->parent) {
                $this->parent->TemplateRegister($template);
            }
        }
    }

    /**
     * Set widgets identifier.
     *
     * The identifier can contain any valid variable name characters with the
     * addition of '.'.
     *
     *
     * @access  public
     * @param   mixed       $value      value to set property to
     * @return  boolean                 true if successfull
     */
    function SetId($value) {

        $ret = false;
        if (is_null($value)) {
            $this->id = null;
            $ret = true;
        } else if (!is_string($value) || !preg_match('/^[\w_.]*$/', $value)) {
            $this->_SetError('value must be a valid identifier name');
            $ret = false;
        } else {
            $this->id = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get widgets identifier.
     *
     * @access  public
     * @return  string                  widgets identifier
     */
    function GetId() {
        return $this->id;
    }

    /**
     * Sets the Cascading Style Sheet (CSS) class rendered by the widget.
     *
     * Use the Class property to specify the CSS class to render on the client
     * for the widget. This property will render on browsers for all widgets.
     *
     * Note: On browsers that do not support CSS, setting the Class property
     * will have no effect.
     *
     * A CSS class name can only contain alpha numeric, hypen, spaces and
     * underscore characters.
     *
     * @access  public
     * @param   string      $value      CSS class name
     * @return  boolean                 true if successful
     * @see     GetClass();
     */ 
    function SetClass($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_class = null;
            $ret = true;
        } else if (!is_string($value) || !preg_match('/^[\w_- ]*$/', $value)) {
            $this->_SetError('value must be a valid CSS class name or space separated list of class names');
            $ret = false;
        } else {
            $this->_class = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Gets the Cascading Style Sheet (CSS) class rendered by the widget.
     *
     * If there is no class defined then null is returned.
     *
     * @access  public
     * @return  string                  CSS class name
     * @see     SetClass();
     */ 
    function GetClass() {
        return $this->_class;
    }

    /**
     * Sets the Cascading Style Sheet inline styles rendered by the widget.
     *
     * This replaces any existing styles with those parsed from the given
     * string or array. Style names and values are not validated with CSS
     * specs.
     *
     * If called with a single argument, the argument is assume to be a CSS
     * style string to parse or an array of $name=>$value pairs. Each style
     * parsed from the string or array will be set by calling the method
     * recursively with two arguments $name and $value.
     *
     * If called with two arguments then an individual style is set where
     * first argument is th style name and the second argument is the styles
     * value.
     *
     * Setting a style to NULL removes it. Calling the method with NULL removes
     * all styes.
     *
     * @access  public
     * @param   mixed       $style      the CSS style string or array of styles
     * @param   string      $name       name of idividual CSS style
     * @param   string      $value      value of CSS style
     * @return  boolean                 true if successful
     * @see     GetStyle();
     */ 
    function SetStyle(/*$style | $name, $value*/) {
        $ret = false;

        if (!isset($this->_style) || is_a($this->_style, 'TNTCssStyle')) {
            unset($this->_style);

            TNTLoader::RequireClass('TNTCssStyle');

            $this->_style =& new TNTCssStyle();
        }

        switch (func_num_args()) {
            case 2:
                $name = trim(func_get_arg(0));
                $value = trim(func_get_arg(1));

                if (!$this->_style->SetItem($name, $value)) {
                    $this->_SetError(null, $this->_style->_GetError());
                    break;
                }

                $ret = true;

                break;

            case 1:
                $style = func_get_arg(0);

                if (is_string($style)) {
                    if (!$this->_style->Set($style)) {
                        $this->_SetError(null, $this->_style->_GetError());
                        break;
                    }
                } else if (is_array($style)) {
                    if (!$this->_style->SetItems($style)) {
                        $this->_SetError(null, $this->_style->_GetError());
                        break;
                    }
                } else if (is_a($style, 'TNTCssStyle')) {
                    if (!$this->_style->SetItems($style->GetItems())) {
                        $this->_SetError(null, $this->_style->_GetError());
                        break;
                    }
                } else if (is_null($style)) {
                    if (isset($this->_style) && is_a($this->_style, 'TNTCssStyle')) {
                        destroy($this->_style);
                    }
                    $this->_style = null;
                } else {
                    $this->_SetError('style value must be a string or array');
                    break;
                }

                $ret = true;
                break;

            default:
                trigger_error('TNTWuiWidget::SetStyle() - Invalid number of arguments');
                return false;
        }

        return true;
    }

    /**
     * Gets the Cascading Style Sheet (CSS) style rendered by the widget.
     *
     * Gets either a single style or all styles for the widget. If passed
     * with no arguments it will return the CSS styles formatted up as a
     * style string. If passed with a single null argument then it will return
     * the raw array of arguments. If passed with a string argument then it will
     * return the value for the named style.
     *
     * If there are no styles defined then null is returned.
     *
     * @access  public
     * @param   string      $name       name of individual style to return
     * @return  mixed                   CSS style string, array or value
     * @see     SetStyle();
     */ 
    function GetStyle(/*$name*/) {
        $ret = null;

        switch (func_num_args()) {
            case 0:
                if (is_null($this->_style)) {
                    break;
                }

                $ret = $this->_style->Get('string');

                break;

            case 1:
                if (is_null($this->_style)) {
                    break;
                }

                $name = trim(func_get_arg(0));


                $ret = (empty($name) ? $this->_style->GetItems() : $this->_style->GetItem($name));

                break;

            default:
                trigger_error('TNTWuiWidget::GetStyle() - Invalid number of arguments');
                return false;
        }

        return $ret;
    }

    /**
     * Set widgets content.
     *
     * When the widget is created from a template as a 'block' this is called
     * with the blocks content.
     *
     * This base method takes a value stores it in the member variable
     * {@link $_content} (objects are stored by reference, otherwise by copy).
     *
     * How the widget interprets these contents is up to it.
     *
     * @access  public
     * @param   mixed&      $value      value to set property to
     * @return  boolean                 true if successfull
     */
    function SetContent(&$value) {

        if (is_object($value)) {
            $this->_content =& $value;
        } else {
            $this->_content = $value;
        }

        return true;
    }

    /**
     * Get widgets content.
     *
     * This base method returns a reference to the member variable
     * {@link $_content}.
     *
     * @access  public
     * @return  mixed&                  reference to contents (or null)
     */
    function &GetContent() {
        return $this->_content;
    }

    /**
     * Set the widget to use the specified template
     *
     * This will take a TNTWuiTemplate object or template name.
     *
     * @access  public
     * @param   mixed   $tpl    either a template object or template name
     * @return  bool            true if successful
     * @see     GetTemplate();
     */
    function SetTemplate($value) {

        $ret = false;

        // Start error section
        do {

            if (is_null($value)) {
                destroy($this->_template);
                $this->_template = null;
                $ret = true;
                break;
            }

            TNTLoader::RequireClass('TNTWuiTemplate');

            if (!is_a($value, 'tntwuitemplate') && !is_string($value)) {
                $this->_SetError('value must be string, TNTWuiTemplate object');
                break;
            }

            destroy($this->_template);

            if (is_a($value, 'tntwuitemplate')) {
                // Get reference to object which was 'passed by copy'
                $this->_template =& $value->Self();
                if (!$this->_template->IsValid()) {
                    $this->_SetError('invalid template object', $this->_template);
                    break;
                }
            } else {
                // One or more comma separated template names
                $this->_template =& new TNTWuiTemplate();
                foreach (explode(',', $value) as $name) {
                    if ($this->_template->TemplateExists($name)) {
                        $this->_template->SetTemplate($name);
                        break;
                    } else {
                        //echo "<pre>not found: $name</pre>";
                    }
                    
                }
                if (!$this->_template->IsValid()) {
                    $this->_SetError('template resource could not be found: '.$value, $this->_template);
                    break;
                }
            }


            $ret = true;

        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Get the template.
     *
     * @access  public
     * @return  TNTTemplate&            reference to the output template
     * @see     SetTemplate();
     */ 
    function &GetTemplate() {
        return $this->_template;
    }

    /**
     * Render the widget.
     *
     * This will return the XHTML required to render the widget in the users
     * browser. The widget will normally be combined with other widgets to
     * build up a complex XHTML document.
     *
     * If a template has been specified using {@link SetTemplate()} then it
     * will be parsed and the result returned. This widget can be referenced
     * in the template using the 'widget' template variable.
     *
     * If no template has been specified then this returns any string value
     * of the {@link _content} member variable.
     *
     * @access  public
     * @return  string                  the XHTML to render this widget
     */
    function Render() {

        static $header = false;

        $ret = '';

        // Output the standard WUI JS and CSS 
        if (!$header) {
            $ret = "<head>\n<script ctrl=\"text/javascript\" src=\"~/includes/libs/tnt/wui/scripts/wui.js\"></script>\n<link href=\"~/includes/libs/tnt/wui/styles/wui.css\" type=\"text/css\" rel=\"stylesheet\" />\n</head>\n";
            $header = true;
        }

        if ($this->_template && $this->_template->IsValid()) {
            // Render using specified template
            $this->TemplateRegister($this->_template);
            
            /*
            if (($properties = $this->Get())) {
                foreach ($properties as $name => $value) {
                    if (is_a($value, 'tntobject')) {
                        $this->_template->AssignByRef($name, $value->Self());
                        $this->_template->RegisterObject($name, $this, null, false);
                    } else {
                        $this->_template->Assign($name, $value);
                    }
                }
            }
            */

            $content = $this->_template->Parse();
            if (is_null($content)) {
                $this->_SetError('Failed to parse template: '."{$this->_template->path}#{$this->_template->name}", $this->_template->_GetError());
                $ret = null;
            } else {
                $ret .= $content;
            }
        } else if ($this->_content && is_scalar($this->_content)) {
            // Render specified content
            $ret .= $this->_content;
        } else {
            // No output
            $ret .= '';
        }

        return $ret;
    }

    /**
     * Construct the object
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiWidget() {

        TNTNode::TNTNode();

        $this->_style = null;
    }

    /**
     * Destruct the object.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiWidget() {
        destroy($this->_template);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
