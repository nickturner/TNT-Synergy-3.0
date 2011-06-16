<!--*
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
 * Window Widget Template 
 * ~~~~~~~~~~~~~~~~~~~~~~
 *
 * Window widgets draw a window like framework around their content, including
 * a title bar if required.
 *
 * The content can be constrained to a specific size and if the contents do
 * not fit the constraints the window can display scroll bars or the content
 * can be clipped or marqueed.
 *
 * The frame style lets you choose from a number of styles supported by
 * the current template. You can pass a comma separated list of styles
 * and the window will use the first one that exists in the current theme.
 *
 * The default style is used if no alternative style is specified. The
 * default style consists of a fully collapsable frame with title bar.
 *
 * The following common alternative frame styles are implemented as
 * standard.
 *   + Simple - a simple frame with no title bar (not collapsable)
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
 * The content constraint set via {@link SetOverflow()} specified what to do
 * if the content does not fit with in the allowed canvas width. The permissable
 * values are as follows;
 *
 *   Hidden:   The canvas size is fixed and the content will be clipped
 *             to fit. Any content falling outside the canvas area will
 *             not be shown.
 *   Scroll:   This will display scrollbars if the content is clipped
 *             to allow the content falling outside the canvas area to
 *             be scrolled in to view. The scrollbars will take up part
 *             of the canvas dimensions thus making a smaller scrolling
 *             window.
 *   Marquee:  The content will automatically scroll across the canvas
 *             at a fixed rate. No scroll bars will be displayed. The
 *             scrolling will pause when the mouse if over the box and
 *             resume again when the mouse leaves the box.
 *             If a height is specified but no width then the canvas
 *             expand to fit the content width and the content will
 *             scroll vertically bottom to top.
 *             If a width is specified but no height then the canvas
 *             expand to fit the content height and the content will
 *             scroll horizontally left to right.
 *             If a width and height are specified then the content will
 *             be truncated at the height and will scroll horizontally
 *             left to right.
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
 * Any height, width, overflow set via {@link TNTWuiWidget::SetStyle()} will
 * take presidence over any set via {@link SetHeight()}, {@link SetWidth()},
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
*-->
<table border="0" cellpadding="0" cellspacing="0" class="border" style="{$css_border}">
  <col width="100%" />
  <thead class="title">
  <tr id="{$id}_window_head" class="title">
    <td width="100%" align="left" valign="middle" class="title" {if $collapsible}onclick="javascript: TNT_ToggleDisplay('{$id}_window_body'); return false;"{/if}>
      {$title}
    </td>
  </tr>
  </thead>
  <tbody class="content">
  <tr id="{$id}_window_body" class="content">
    <td width="100%" align="left" valign="top" class="content">
      {$content}
    </td>
  </tr>
  </tbody>
</table>
