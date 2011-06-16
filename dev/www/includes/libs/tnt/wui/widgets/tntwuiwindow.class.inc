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
 * WUI Window Class 
 * ~~~~~~~~~~~~~~~~
 *
 * The WUI Window class is a widget that draws a window like framework around
 * its content, including a title bar if required.
 *
 * The content can be constrained to a specific size and if the contents do
 * not fit the constraints the window can display scroll bars or the content
 * can be clipped or marqueed.
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */


/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiWidget');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to represent a Window in the Web User Interface.
 *
 * This class is used to display a constraining window through which the
 * contents can be viewed. The window can have a frame, including title bar,
 * scollbars etc.
 *
 * The basic window constraints are performed by the {@link Render()} method,
 * however the widgets template is used to layout the windows framework. The
 * actual style of the frame can be specified using the {@link SetFrame()}
 * method. All frame templates should try and obey the requested frame features
 * as closely as the theme allows.
 *
 * The frame style lets you choose from a number of styles supported by
 * the current template. You can pass a comma separated list of styles
 * and the window will use the first one that exists in the current theme.
 *
 * The default style is used if no alternative style is specified. The
 * default style consists of a fully collapsible frame with title bar.
 *
 * The following common alternative frame styles are implemented as
 * standard.
 *   + Simple - a simple frame with no title bar (not collapsible)
 *   + Dialog - a dialog frame with title bar
 *
 * A frame style of 'none' will cause no frame to be used any template will be
 * ignored.
 *
 * The window can be given a width and/or height dimension, and a content
 * constraint.
 *
 * Dimensions:
 * The frame dimensions can be set using the {@link SetHeight()} and
 * {@link SetWidth()} methods. The values can be specified using any of the CSS
 * units of measurements. When a fixed value is given it specifies the size of
 * the canvas, when a percentage value is given it specifies size of the frame.
 *
 * Dimensions can be given in the following units;
 *   %      a percentage of parent element
 *   in     inch 
 *   cm     centimeter 
 *   mm     millimeter 
 *   em     the height of the elements font 
 *   ex     the height of the letter "x" in the elements font
 *   pt     point (1 pt is the same as 1/72 inch) 
 *   pc     pica (1 pc is the same as 12 points) 
 *   px     pixels (a dot on the computer screen) 
 *
 * We recommend only using pixel widths.
 *
 * Constraint:
 * The overflow constraint determins how the canvas should behave if its
 * content exceeds the size given for the window. It can take one of the
 * following values.
 *
 *   'visible'      The canvas is expanded to fit all the content.
 *
 *   'hidden'       The canvas size is fixed and the content will be clipped
 *                  to fit. Any content falling outside the canvas area will
 *                  not be shown.
 *   'scroll'       This will display scrollbars if the content is clipped
 *                  to allow the content falling outside the canvas area to
 *                  be scrolled in to view. The scrollbars will take up part
 *                  of the canvas dimensions thus making a smaller scrolling
 *                  window.
 *   'marquee-X'    The content will automatically scroll across the canvas
 *                  at a fixed rate. No scroll bars will be displayed. The
 *                  scrolling will pause when the mouse if over the box and
 *                  resume again when the mouse leaves the box.
 *                  The direction of marquee is specified by the value of X
 *                  and can be one of 'up', 'down', 'left' or 'right'.
 *
 * The default when no constraint is given is to expand the canvas width to
 * fit the content. In in this senario any box dimension can be thought of
 * as a minimum dimension.
 * 
 * Normal CSS class and styling rules can be applied to the frame using the
 * {@link TNTWuiWidget::SetClass()} and {@link TNTWuiWidget::SetStyle()}
 * methods.
 *
 * The following inline CSS styles set via the {@link TNTWuiWidget::SetStyle()}
 * can be used;
 *      'height'    - height of the contents canvas {@link SetHeight()},
 *      'width'     - width of the contents canvas {@link SetHeight()},
 *      'overflow'  - constraint on the content {@link SetOverflow()}
 *      'padding..' - padding between the content and the frame,
 *
 * Any height, width, overflow set via {@link TNTWuiWidget::SetStyle()} will take
 * presidence over any set via {@link SetHeight()}, {@link SetWidth()},
 * {@link SetOverflow()}.
 *
 * Implementation: A window is rendered using two nested divs, the window div
 * and the canvas div. The template is called to wrap the window framework
 * around the cavas div before it is placed inside the window div.
 *
 * Three different CSS styles are created, 'window', 'border' and 'canvas' they
 * control the DIVs used to render the three areas. The theme can use a TABLE
 * or DIV to render the border, the CSS style 'border' allows for both.
 *
 * @package TNT.Wui
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiWindow extends TNTWuiWidget {

    /**
     * Frame style (used to select correct frame template)
     * @access  private
     * @var     string                  frame style name
     */
    var $_frame = null;

    /**
     * Indicated if the window can be collapsed (if implemented by theme)
     * @access  private
     * @var     boolean                 true if collapsible
     */
    var $_collapsible = false;

    /**
     * Caption to display in the title bar if present (optional).
     * @access  private
     * @var     string                  HTML format title string
     */
    var $_title = null;

    /**
     * Icon to display in the title bar if present (optional).
     * @access  private
     * @var     TNTIcon&                reference to icon object to use
     */
    var $_icon = null;

    /**
     * Width of the canvas, null allows canvas to grow with contents
     * @access  private
     * @var     string                  width including units (or null)
     */
    var $_width = null;

    /**
     * Height of the canvas, null allows canvas to grow with contents
     * @access  private
     * @var     string                  height including units (or null)
     */
    var $_height = null;

    /**
     * Content overflow contraint, null implies 'visible'
     * @access  private
     * @var     string                  valid overflow name (or null)
     */
    var $_overflow = null;

    /**
     * Window alignment within its containing element
     * @access  private
     * @var     string                  valid HTML align name (or null)
     */
    var $_align = null;

    /**
     * Sets the frame style.
     *
     * The frame style lets you choose from a number of styles supported by
     * the current template. You can pass a comma separated list of styles
     * and the window will use the first one that exists in the current theme.
     *
     * The default style is used if no alternative style is specified. The
     * default style consists of a fully collapsible frame with title bar.
     *
     * The following common alternative frame styles are implemented as
     * standard.
     *   + Simple - a simple frame with no title bar (not collapsible)
     *   + Dialog - a dialog frame with title bar
     *
     * @access  public
     * @param   mixed       $value      alternative style names (or csv list)
     * @return  boolean                 true if successful
     * @see     GetFrame();
     */ 
    function SetFrame($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_frame = null;
            $ret = true;
        } else if (is_array($value)) {
            $this->_frame = $value;
            $ret = true;
        } else if (is_string($value)) {
            $this->_frame = explode(',', preg_replace('[   ]*', '', strtolower($value)));
            $ret = true;
        } else {
            $this->_SetError('value must be an array or CSV string');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the frame style.
     *
     * If there is no style defined then null is returned.
     *
     * @access  public
     * @return  array                   array of style names
     * @see     SetFrame();
     */ 
    function GetFrame() {
        return $this->_frame;
    }

    /**
     * Sets a value indicating whether the window is collapsible.
     *
     * If a window is collapsible then clicking on the title bar will
     * allow the canvas to be hidden. Any icon assigned to the window
     * will be the hot spot on the window. NB: The actual clickable
     * and collapsible areas are determined by the template.
     *
     * This allows the viewer to collapse down windows he is not interested
     * in at that time.
     *
     * Collapsible windows should ignore any window height specified when
     * collapsed and be of minimum height required for the title bar.
     *
     * The value can be specified as either a boolean or one of the following
     * strings;
     *   'true', 'on', 'yes', '1', 'hidden' => true
     *   'false', 'off', 'no', '0', 'visible', '' => false
     *
     * @access  public
     * @param   mixed       $value      true if window is collapsible
     * @return  boolean                 true if successful
     * @see     GetCollapsible();
     */ 
    function SetCollapsible($value) {

        if (is_null($value)) {
            $this->_collapsible = false;
            $ret = true;
        } else {
            if (is_string($value)) {
                if (strcasecmp($value, 'value') == 0) {
                    $value = true;
                } else if (strcasecmp($value, 'visible') == 0) {
                    $value = false;
                } else if (strcasecmp($value, '') == 0) {
                    $value = false;
                } else {
                    $value = strtoboolean($value);
                }
            }
            if (!is_boolean($value)) {
                $this->_SetError('value must be a boolean');
                $ret = false;
            } else {
                $this->_collapsible = $value;
                $ret = true;
            }
        }

        return $ret;
    }

    /**
     * Gets a value indicating whether the window is collapsible.
     *
     * If there is no collapsible state defined then null is returned.
     *
     * @access  public
     * @return  boolean                 true if window is collapsible.
     * @see     SetCollapsible();
     */ 
    function GetCollapsible() {
        return $this->_collapsible;
    }

    /**
     * Sets the windows width.
     *
     * This value is the minimum window width, the actual width is governed
     * by the windows contents and any overflow constraint set using the
     * {@link SetOverflow()} method. If the overflow constraint don't allow
     * the canvas to expand to its contents size then the window will use
     * this width to constrain the content to.
     *
     * The value can be specified using any of the CSS units of measurements.
     * When a fixed value is given it specifies the width of the windows
     * canvas area, when a percentage value is given it specifies width of
     * the overall window including its frame.
     *
     * Values can be given in the following units;
     *   %      a percentage of parent element
     *   in     inch 
     *   cm     centimeter 
     *   mm     millimeter 
     *   em     the width of the elements font 
     *   ex     the width of the letter "x" in the elements font
     *   pt     point (1 pt is the same as 1/72 inch) 
     *   pc     pica (1 pc is the same as 12 points) 
     *   px     pixels (a dot on the computer screen) 
     *
     * If no unit is specified then pixels is assumed.
     *
     * We recommend only using pixel widths.
     *
     * @access  public
     * @param   mixed       $value      width (including units)
     * @return  boolean                 true if successful
     * @see     GetWidth();
     */ 
    function SetWidth($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_width = null;
            $ret = true;
        } else if (is_integer($value)) {
            $this->_width = "{$value}px";
            $ret = true;
        } else if (is_string($value) &&
                   preg_match('/^([0-9]+)(%|in|cm|mm|em|ex|pt|pc|px)?$/', $value, $matches)) {
            $this->_width = $value;
            $ret = true;
        } else {
            $this->_SetError('value must be an integer or valid CSS measurement');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the windows width.
     *
     * If there is no width defined then null is returned.
     *
     * @access  public
     * @return  string                  window width (including units)
     * @see     SetClass();
     */ 
    function GetWidth() {
        return $this->_width;
    }

    /**
     * Sets the windows height.
     *
     * This value is the minimum window height, the actual height is governed
     * by the windows contents and any overflow constraint set using the
     * {@link SetOverflow()} method. If the overflow constraint don't allow
     * the canvas to expand to its contents size then the window will use
     * this height to constrain the content to.
     *
     * The value can be specified using any of the CSS units of measurements.
     * When a fixed value is given it specifies the height of the windows
     * canvas area, when a percentage value is given it specifies height of
     * the overall window including its frame.
     *
     * Values can be given in the following units;
     *   %      a percentage of parent element
     *   in     inch 
     *   cm     centimeter 
     *   mm     millimeter 
     *   em     the height of the elements font 
     *   ex     the height of the letter "x" in the elements font
     *   pt     point (1 pt is the same as 1/72 inch) 
     *   pc     pica (1 pc is the same as 12 points) 
     *   px     pixels (a dot on the computer screen) 
     *
     * If no unit is specified then pixels is assumed.
     *
     * We recommend only using pixel widths.
     *
     * @access  public
     * @param   mixed       $value      height (including units)
     * @return  boolean                 true if successful
     * @see     GetHeight();
     */ 
    function SetHeight($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_width = null;
            $ret = true;
        } else if (is_integer($value)) {
            $this->_height = "{$value}px";
            $ret = true;
        } else if (is_string($value) &&
                   preg_match('/^([0-9]+)(%|in|cm|mm|em|ex|pt|pc|px)?$/', $value, $matches)) {
            $this->_height = $value;
            $ret = true;
        } else {
            $this->_SetError('value must be an integer or valid CSS measurement');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the windows height.
     *
     * If there is no height defined then null is returned.
     *
     * @access  public
     * @return  string                  window height (including units)
     * @see     SetClass();
     */ 
    function GetHeight() {
        return $this->_height;
    }

    /**
     * Sets the window content overflow constraint.
     *
     * The overflow constraint determins how the canvas should behave if its
     * content exceeds the size given for the window. It can take one of the
     * following values.
     *
     *   'visible'      The canvas is expanded to fit all the content.
     *
     *   'hidden'       The canvas size is fixed and the content will be clipped
     *                  to fit. Any content falling outside the canvas area will
     *                  not be shown.
     *   'scroll'       This will display scrollbars if the content is clipped
     *                  to allow the content falling outside the canvas area to
     *                  be scrolled in to view. The scrollbars will take up part
     *                  of the canvas dimensions thus making a smaller scrolling
     *                  window.
     *   'marquee-X'    The content will automatically scroll across the canvas
     *                  at a fixed rate. No scroll bars will be displayed. The
     *                  scrolling will pause when the mouse if over the box and
     *                  resume again when the mouse leaves the box.
     *                  The direction of marquee is specified by the value of X
     *                  and can be one of 'up', 'down', 'left' or 'right'.
     *
     * The default when no constraint is given is to expand the canvas width to
     * fit the content. In in this senario any box dimension can be thought of
     * as a minimum dimension.
     *
     * This value can also be set using the inline CSS style 'overflow' via the
     * method {@link SetStyle()}. The window treats the CSS overflow styles
     * 'scroll' and 'auto' as the same and behaves as the CSS 'auto' does.
     *
     * @access  public
     * @param   mixed       $value      overflow constraint
     * @return  boolean                 true if successful
     * @see     GetOverflow();
     */ 
    function SetOverflow($value) {

        static $values = array('visible', 'hidden', 'scroll', 'auto', 'marquee-up', 'marquee-down', 'marquee-left', 'marquee-right');

        $ret = false;
        if (is_null($value)) {
            $this->_overflow = null;
            $ret = true;
        } else if (is_string($value) && in_array($value = strtolower($value), $values)) {
            $this->_overflow = $value;
            $ret = true;
        } else {
            $this->_SetError('value must be one of; '.join(', ', $values));
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the window content overflow constraint.
     *
     * If there is no overflow constraint defined then null is returned.
     *
     * @access  public
     * @return  string                  overflow constraint (or null)
     * @see     SetOverflow();
     */ 
    function GetOverflow() {
        return $this->_overflow;
    }

    /**
     * Sets the window alignment within its parent element.
     *
     * The equated to the HTML align attribute as used on 'TABLE' tags etc.
     * It is a more convienant way of specify window alignment than the CSS
     * method with is browser specific.
     *
     * It can take one of the following values.
     *   + 'left' - the window is positioned to the left edge of parent
     *   + 'right' - then window is positioned to the right edge of parent
     *   + 'center' - the window is positioned in the center of parent.
     *
     * This value should not be used along with the CSS 'float' as conflicts
     * will occur.
     *
     * @access  public
     * @param   mixed       $value      window alignment
     * @return  boolean                 true if successful
     * @see     GetAlign();
     */ 
    function SetAlign($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_overflow = null;
            $ret = true;
        } else if (is_string($value) && in_array($value = strtolower($value), array('left', 'right', 'center'))) {
            $this->_align = $value;
            $ret = true;
        } else {
            $this->_SetError('value must be one of \'left\', \'right\' or \'center\'');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the window alignment within its parent element.
     *
     * If there is no alignment defined then null is returned.
     *
     * @access  public
     * @return  string                  window alignment (or null)
     * @see     SetAlign();
     */ 
    function GetAlign() {
        return $this->_align;
    }

    /**
     * Sets the windows title.
     *
     * The window title is an HTML string displayed in the title bar of the
     * window. 
     *
     * If a window frame has a title bar and no title is specified then it
     * is theme dependant what the title bar will show.
     *
     * @access  public
     * @param   mixed       $value      HTML to display as window title
     * @return  boolean                 true if successful
     * @see     GetTitle();
     */ 
    function SetTitle($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_title = null;
            $ret = true;
        } else if (is_scalar($value)) {
            $this->_title = "$value";
            $ret = true;
        } else {
            $this->_SetError('value must be a string');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the windows title.
     *
     * If there is no title defined then null is returned.
     *
     * @access  public
     * @return  array                   array of style names
     * @see     SetClass();
     */ 
    function GetTitle() {
        return $this->_title;
    }

    /**
     * Sets the windows icon.
     *
     * The window icon is a small image displayed in the title bar of the
     * window. usually on the lefthand side of the title bar. 
     *
     * If a window frame has a title bar and no icon is specified then it
     * is theme dependant what the title bar will show.
     *
     * @access  public
     * @param   mixed       $value      icon to display as window icon
     * @return  boolean                 true if successful
     * @see     GetIcon();
     */ 
    function SetIcon($value) {

        $ret = false;
        if (is_null($value)) {
            $this->_icon = null;
            $ret = true;
        } else if (is_a($value, 'tnticon')) {
            $this->_icon =& $value;
            $this->_icon->SetSize(16);
            $ret = true;
        } else if (is_string($value)) {
            TNTLoader::RequireClass('TNTIcon');
            $this->_icon =& new TNTIcon();
            $this->_icon->SetImage($value);
            $this->_icon->SetSize(16);
            $ret = true;
        } else {
            $this->_SetError('value must be a TNTIcon or a string');
            $ret = false;
        }

        return $ret;
    }

    /**
     * Gets the windows icon.
     *
     * If there is no icon defined then null is returned.
     *
     * @access  public
     * @return  TNTIcon&                reference to icon widget
     * @see     SetIcon();
     */ 
    function &GetIcon() {
        return $this->_icon;
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
     * NOTE: This method is overridden from {@link TNTQuiWidget::SetStyle()}
     * to trap the setting of the width, height and overflow styles.
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

        switch (func_num_args()) {
            case 0:
                $ret = parent::SetStyle();
                break;
            case 1:
                $ret = parent::SetStyle(func_get_arg(0));
                break;
            default:
                $ret = parent::SetStyle(func_get_arg(0), func_get_arg(1));
                break;
        }

        if ($ret && $this->_style) {
            $ps = array('height', 'width', 'overflow');
            foreach ($ps as $p) {
                $v = $this->_style->GetItem($p);
                if (!is_null($v)) {
                    if (!($ret = $this->Set($p, $v))) {
                        break;
                    }
                }
            }
        }

        return $ret;
    }

    /**
     * Render the window.
     *
     * The creates the inner canvas and outer frame divs and then passes them
     * to the template for it to render the framework as required by the
     * template.
     *
     * This will return the HTML to render the window frame and contents. The
     * underlying widget will be rendered into the canvas area of the box.
     *
     * @access  public
     * @return  string                  HTML required to render the window
     */
    function Render() {

        $css_window = $css_border = $css_canvas = $css_padding = '';

        $frame = $this->GetFrame();
        $overflow = $this->GetOverflow();
        $class = $this->GetClass();
        $css = $this->GetStyle(null);

        $marquee = (strncmp($overflow, 'marquee-', 8) == 0);

        $html = $this->_content;
        
        // Prepare the width & height of the frame, border and canvas areas
        foreach (array('width', 'height') as $dim) {
            $prop = $this->Get($dim);
            $$dim = $prop;
            if ($prop && preg_match('/^([0-9]+)(%|in|cm|mm|em|ex|pt|pc|px)?$/', $prop, $matches)) {
                $n = (int)$matches[1];
                $u = (isset($matches[2]) ? $matches[2] : 'px');
                $d = $n.$u;

                $percentage = ($u == '%');
                $outer = false;

                $dimensions[$dim] = array('value' => $n, 'units' => $u, 'css' => $d, '$html' => $n.($percentage ? '%' : ''));

    
                if ($marquee || $overflow == 'hidden') {
                    if ($percentage || $outer) {
                        $css_window .= " $dim: $d;";
                        $css_border .= " $dim: 100%; table-layout: fixed;";
                        $css_canvas .= " $dim: 100%; overflow: hidden;";
                    } else {
                        $css_window .= " $dim: $d;";
    		            $css_border .=  " $dim: 100%;";
                        $css_canvas .= " $dim: $d; overflow: hidden;";
                    }
                } else if ($overflow == 'scroll' || $overflow == 'auto') {
                    if ($percentage || $outer) {
                        $css_window .= " $dim: $d;";
                        $css_border .= " $dim: 100%; table-layout: fixed;";
                        $css_canvas .= " $dim: 100%; overflow: auto;";
                    } else {
                        $css_window .= " $dim: $d;";
    		            $css_border .= " $dim: 100%;";
    		            $css_canvas .= " $dim: $d; overflow: auto;";
                    }
                } else {
                    if ($percentage || $outer) {
                        $css_window .= " $dim: $d;";
                        $css_border .= " $dim: 100%;";
                        $css_canvas .= " $dim: 100%;";
                    } else {
                        $css_window .= " $dim: $d;";
    		            $css_border .= " $dim: 100%;";
    		            $css_canvas .= " $dim: $d;";
                        /*
                        $browser =& TNTBrowser::Instance();
                        if ($browser->supportsW3CDOM) {
                            $css_window .= " $dim: auto; min-$dim: $d;";
    		                $css_canvas .= " $dim: auto; min-$dim: $d;";
                        }
                        */
                    }
                }
            }
        }

        // Split the user specified CSS style as required
        if ($css) {
            foreach ($css as $name => $value) {
                if ($name == 'height' || $name == 'width' || $name == 'overflow') {
                    continue;
                } else if (strncmp($name, 'padding', 7) == 0) {
                    $css_padding .= " $name: $value;";
                } else if (strncmp($name, 'margin', 6) == 0 ||
                           strncmp($name, 'border', 6) == 0) {
                    $css_window .= " $name: $value;";
                } else {
                    $css_canvas .= " $name: $value;";
                }
            }
        }

        // Set to marquee if required
        if ($marquee) {
            $scrollamount = (isset($css['marquee-speed']) ? $css['marquee-speed'] : 1);
            $direction = substr($overflow, 8);
            $html = "<marquee id=\"{$this->id}_marquee\" class=\"_marquee\" behavior=\"scroll\" direction=\"$direction\" height=\"100%\" width=\"100%\" scrollamount=\"$scrollamount\" onmouseover=\"if (!this.static) { this.stop(); }\" onmouseout=\"if (!this.static) { this.start(); }\" style=\"background-color: transparent;\">{$html}</marquee>";
        } 

        // Prepare the content margins and alignment.
        //
        // Margins have to be added on an inner div other wise they effect the
        // width of the canvas div.
        if ($css_padding) {
            $html = "
            <div id=\"{$this->id}_canvas\" class=\"_canvas\" style=\"$css_canvas\">
              <div style=\"$css_padding\">
                <!-- Canvas Start -->
                $html
                <!-- Canvas End -->
              </div>
            </div>";
        } else {
            $html = "
            <div id=\"{$this->id}_canvas\" class=\"_canvas\" style=\"$css_canvas\">
              <!-- Canvas (Start) -->
              $html
              <!-- Canvas (End) -->
            </div>";
        }

        if ($frame != 'none') {

            if ($frame == '') {
                $frame = 'default';
            }

            if (!$this->_template) {
                $tpl = dirname(__FILE__).'/../templates/tntwuiwindow';
                $this->SetTemplate("$tpl.$frame.tpl,$tpl.tpl");
            }

            $template =& $this->GetTemplate();

            if (!$template) {
                $this->_SetError("Can't find frame template for frame style '$frame'", $this->_GetError());
                return null;
            }

            if (!$template->IsCached()) {
                if ($this->_icon) {
                    $this->_icon->SetSize(16);
                    if ($this->_collapsible) {
                        $this->icon->alt = translate('Click to maximize/minimize this box');
                        $this->icon->img_attrs['onclick'] = "javascript: TNT_ToggleDisplay('{$this->id}_window_body'); return false;";
                    }
                }

                $template->Assign((array)$this->Get());
                $template->Assign('title', $this->_title);
                $template->Assign('content', $html);
                $template->Assign('collapsible', $this->_collapsible);
                $template->Assign('dimensions', @$dimensions);
                $template->Assign('height', @$dimensions['height']['html']);
                $template->Assign('width', @$dimensions['width']['html']);
                $template->Assign('overflow', $overflow);
                $template->Assign('css_canvas', $css_canvas);
                $template->Assign('css_border', $css_border);
                $template->Assign('css_window', $css_window);
                $template->AssignByRef('id', $this->id);
                $template->AssignByRef('widget', $this);
                $template->AssignByRef('self', $this);
                $template->AssignByRef('css', $css);
                $template->AssignByRef('icon', $this->_icon);
            }

            $html = $template->Parse();

            if (is_null($html)) {
                $this->_SetError('Can\'t render window frame', $template->_GetError());
                return null;
            }

        } else {
            $html = "
<table id=\"{$this->id}_box_border\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" class=\"{$css['border']}\" style=\"$css_border\">
  <tr>
    <td>$html</td>
  </tr>
</table>";
        }

        // Finally align and float the box correctly
        $css_align = '';
        $attr = '';
        if (($align = $this->GetAlign())) {
            switch ($align) {
                case 'left':
                    $css_window .= ' margin-left: 0px; margin-right: auto;';
                    $css_align = 'style="text-align: left;"';
                    break;
                case 'right':
                    $css_window .= ' margin-left: auto; margin-right: 0px;';
                    $css_align = 'style="text-align: right;"';
                    break;
                case 'center':
                    $css_window .= ' margin-left: auto; margin-right: auto;';
                    $css_align = 'style="text-align: center;"';
                    break;
            }
        }

        $html = "
<!-- Window (Start) [{$width} x {$height}] -->
<div $css_align><!-- IE alignment DIV -->
  <table class=\"{$class} tntwuiwindow\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" style=\"{$css_window}\">
    <tr>
      <td>
        <!-- Window Frame (Start) -->
        $html
        <!-- Window Frame (End) -->
      </td>
    </tr>
  </table>
</div>
<!-- Window (End) -->
";

        return $html;
    }

    /**
     * Construct the object
     *
     * @access  public
     */
    function TNTWuiWindow() {

        TNTWuiWidget::TNTWuiWidget();

    }

    /**
     * Destruct the box object.
     *
     * This will destruct the box object.
     *
     * @access  private
     */
    function _TNTWuiWindow() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
