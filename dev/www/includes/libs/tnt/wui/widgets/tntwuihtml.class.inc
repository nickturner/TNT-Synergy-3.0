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
 * Html Widget Class 
 * ~~~~~~~~~~~~~~~~~
 *
 * Represents a simple HTML element.
 *
 * @package TNT.Html
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiWidget');
TNTLoader::RequireClass('TNTHtml');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a simple HTML element.
 *
 * @package TNT.Html
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiHtml extends TNTWuiWidget {

    /**
     * Tag name of the HTML element
     * @access public
     * @var string                      tag name
     */
    var $tagName = null;

    /**
     * A value indicating whether the HTML element is complete or has content
     * @access public
     * @var string                      true it the tag is complete (<tag/>)
     */
    var $tagComplete = false;

    /**
     * This is an array of attribute names that will be rendered automatically
     * by the method {@link RenderOpenTag()}.
     *
     * The value of these attributes is obtained by calling the method
     * {@link TagAttribute()}. Any attributes with null values will not
     * be rendered.
     *
     * @access public
     * @var string[]                    array of attribute names
     */
    var $tagAttrNames = array();

    /**
     * Extra attributes to put in the elements opening tag
     * @access public
     * @var array                       associative array of attributes
     */
    var $attributes = array();

    /**
     * The elements tooltip
     * @var string                      a descriptive text string
     */
    var $_toolTip = null;

    /**
     * A value indicating whether the element is disabled.
     * @access private
     * @var boolean                     true if the element is disabled
     */
    var $_disabled = null;

    /**
     * A value indicating whether the control is hidden.
     * @access private
     * @var boolean                     true if the control is hidden
     */
    var $_hidden = null;

    /**
     * Return the value of a html tag attribute.
     *
     * This base method just casts the value returned by Get() to a string
     * unless special string encoding is required. This method can be
     * overridden to provided values for HTML attribute tags that don't have
     * corresponding properties available using Get() or where custom string
     * encoding is required.
     *
     * This base method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'disabled' - custom encoding (true => 'disabled', false => null)
     *   + 'title' - returns value of 'tooltip' property
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {

        switch (strtolower($name)) {
            case 'title':
                $ret = $this->Get('tooltip');
                break;
            case 'disabled':
                $ret = ($this->Get($name) ? 'disabled' : null);
                break;
            default:
                $ret = $this->Get($name);
                break;
        }

        return $ret;
    }

    /**
     * Sets a value indicating whether the element is disabled.
     *
     * Use the Disabled property to specify or determine whether a element is
     * functional. When set to true, the element appears dimmed, preventing any
     * input from being entered in the element.
     *
     * Note: The ability to enable or disable functionality is always available.
     * However, dimming and locking the element only works in Microsoft
     * Internet Explorer version 4 and later.
     *
     * Note: Not all element support this property. See the indivual elements
     * for details.
     *
     * The value can be specified as either a boolean or one of the following
     * strings;
     *   'true', 'on', 'yes', '1', 'disabled' => true
     *   'false', 'off', 'no', '0', 'enabled' => false
     *
     * @access  public
     * @param   mixed       $disabled   true if element is disabled
     * @return  boolean                 true if successful
     * @see     GetDisabled();
     */ 
    function SetDisabled($disabled) {

        $ret = false;
        if (is_null($disabled)) {
            $this->_disabled = false;
            $ret = true;
        } else {
            if (is_string($disabled)) {
                if (strcasecmp($value, 'disabled') == 0) {
                    $disabled = true;
                } else if (strcasecmp($value, 'enabled') == 0) {
                    $disabled = false;
                } else {
                    $disabled = strtoboolean($disabled);
                }
            }
            if (!is_boolean($disabled)) {
                $this->_SetError('value must be a boolean');
                $ret = false;
            } else {
                $this->_disabled = $disabled;
                $ret = true;
            }
        }

        return $ret;
    }

    /**
     * Gets a value indicating whether the element is disabled.
     *
     * If there is no disabled state defined then null is returned.
     *
     * @access  public
     * @return  boolean                 true if element is disabled
     * @see     SetDisabled();
     */ 
    function GetDisabled() {
        return $this->_disabled;
    }

    /**
     * Sets a value indicating whether the control is hidden.
     *
     * Use the Hidden property to specify or determine whether a control is
     * hidden. Hidden controls hold their value but show no user interface
     * to allow the control to be modified.
     *
     * Note: This faciality is normally used by the framework to place a
     * control on a 'hidden' form tab, when client side tabs can't be used.
     *
     * The value can be specified as either a boolean or one of the following
     * strings;
     *   'true', 'on', 'yes', '1', 'hidden' => true
     *   'false', 'off', 'no', '0', 'visible', '' => false
     *
     * @access  public
     * @param   mixed         $hidden   true if control is hidden
     * @return  boolean                 true if successful
     * @see     GetHidden();
     */ 
    function SetHidden($hidden) {

        if (is_null($hidden)) {
            $this->_hidden = false;
            $ret = true;
        } else {
            if (is_string($hidden)) {
                if (strcasecmp($value, 'hidden') == 0) {
                    $hidden = true;
                } else if (strcasecmp($value, 'visible') == 0) {
                    $hidden = false;
                } else if (strcasecmp($value, '') == 0) {
                    $hidden = false;
                } else {
                    $hidden = strtoboolean($hidden);
                }
            }
            if (!is_boolean($hidden)) {
                $this->_SetError('value must be a boolean');
                $ret = false;
            } else {
                $this->_hidden = $hidden;
                $ret = true;
            }
        }

        return $ret;
    }

    /**
     * Gets a value indicating whether the control is hidden.
     *
     * If there is no hidden state defined then null is returned.
     *
     * @access  public
     * @return  boolean                 true if control is hidden
     * @see     SetHidden();
     */ 
    function GetHidden() {
        return $this->_hidden;
    }

    /**
     * Sets the text displayed when the mouse pointer hovers over the element.
     *
     * Use the ToolTip property to specify the custom text that is displayed
     * when the mouse pointer hovers over the element.
     *
     * Note: This property is rendered for all browsers. However, only
     * Microsoft Internet Explorer will display this property as a ToolTip. All
     * other browsers will ignore this property.
     *
     * @access  public
     * @param   string      $toolTip    text to display
     * @return  boolean                 true if successful
     * @see     GetToolTip();
     */ 
    function SetToolTip($toolTip) {

        $ret = false;
        if (is_null($toolTip)) {
            $this->_toolTip = null;
            $ret = true;
        } else if (!is_string($toolTip)) {
            $this->_SetError('value must be a string');
            $ret = false;
        } else {
            $this->_toolTip = $toolTip;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Gets the text displayed when the mouse pointer hovers over the element.
     *
     * If there is no tooltip defined then null is returned.
     *
     * @access  public
     * @return  integer                 the tab index or 0 if not set
     * @see     SetToolTip();
     */ 
    function GetToolTip() {
        return $this->_toolTip;
    }

    /**
     * Renders the HTML opening tag of the element
     *
     * You can override this method to generate content.
     *
     * @access  public
     * @return  string                  HTML required to render the element
     */
    function RenderOpenTag() {

        $ret = '';
        if ($this->tagName) {
            $attrs = (array)$this->attributes;
            if ($this->tagAttrNames) {
                $max_i = count($this->tagAttrNames);
                for ($i = 0; $i < $max_i; $i++) {
                    $name = $this->tagAttrNames[$i];
                    $value = $this->TagAttribute($name);
                    if (!is_null($value) && !is_scalar($value)) {
                    } else if (!is_null($value)) {
                        $attrs[$name] = $value;
                    }
                }
            }

            $ret .= TNTHtml::Tag($this->tagName, $attrs,
                $this->tagComplete ? TNT_TAG_EMPTY : TNT_TAG_OPEN);
        }

        return $ret;
    }

    /**
     * Render this nodes content
     *
     * You can override this method to generate content.
     *
     * By default this calls {$link TNTWuiWidget::Render()} to
     * render the widgets content.
     *
     * If a tag is complete {@link $this->tagComplete} then this method
     * returns an empty string.
     *
     * @access  public
     * @return  string                  HTML required to render the element
     */
    function RenderContents() {

        $ret = '';
        if ($this->tagName && !$this->tagComplete) {
            // Render specified content
            $ret .= parent::Render();
        }

        return $ret;
    }

    /**
     * Renders the HTML closing tag of the element
     *
     * You can override this method to generate content.
     *
     * If a tag is complete {@link $this->tagComplete} then this method
     * returns an empty string.
     *
     * @access  public
     * @return  string                  HTML required to render the element
     */
    function RenderCloseTag() {

        $ret = '';
        if ($this->tagName && !$this->tagComplete) {
            $ret .= TNTHtml::Tag($this->tagName, null, TNT_TAG_CLOSE)."\n";
        }

        return $ret;
    }

    /**
     * Renders the node and all its children
     *
     * @access  public
     * @return  void
     */
    function Render() {

        if ($this->_template) {
            // Render using specified template
            $ret = parent::Render();
        } else {
            $ret =  $this->RenderOpenTag().
                    $this->RenderContents().
                    $this->RenderCloseTag();
        }

        return $ret;
    }
    
    /**
     * Replace attributes embeded in a string.
     *
     * This method scans a string for attributes names enclosed in '{}' and
     * replaces them with the current value of the attribute as returned by
     * {@link TagAttribute()}. If the attribute does not exist then the '{xx}'
     * in not replaced.
     *
     * @access  public
     * @param   string      $str        string to replace attributes in
     * @return  string                  the string with the attributes replaced
     */
    function StrReplaceTagAttributes($str) {

        if ($str && strpos($str, '{') !== false && strpos($str, '}') !== false) {
            $chunks = preg_split('/({\w*})/', $str, -1, PREG_SPLIT_DELIM_CAPTURE);
            if ($chunks) {
                $max_i = count($i);
                for ($i = 0; $i < $max_i; $i++) {
                    $t = $chunks[$i];
                    if (strlen($t) >= 2) {
                        if ($t{0} == '{' && $t{strlen($t) - 1} == '}') {
                            $t = trim($t, '{}');
                            if (($v = $this->TagAttribute($t))) {
                                if (is_null($v)) {
                                    $chunks[$i] = '';
                                } else if (is_scalar($v)) {
                                    $chunks[$i] = urlencode($v);
                                }
                            }
                        }
                    }
                }
                $str = implode('', $chunks);
            }
        }

        return $str;
    }

    /**
     * Construct the element
     *
     * This will construct element, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiHtml() {

        TNTWuiWidget::TNTWuiWidget();

        $this->tagAttrNames += array('id', 'title', 'class', 'style');
    }

    /**
     * Destruct the element.
     *
     * This will destruct the element and any children.
     *
     * @access  private
     */
    function _TNTWuiForm() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
