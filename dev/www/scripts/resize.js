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
 * Resize
 * ~~~~~~
 *
 * These function allow DOM elements to be resized dynamically.
 *
 * @package TNT.JScript
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

// Global object to hold resize information.
var _tnt_resizeObj = null;

function TNTResizeObj() {
  this.elNode = null; 
  this.dir    = "";
  this.grabx  = null;
  this.graby  = null;
  this.width  = null;
  this.height = null;
  this.left   = null;
  this.top    = null;
}

/**
 * Called to start resizing an element.
 *
 * The element to be resizeed is assume to be the current event target unless
 * the 'id' parameter is given.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @param  mixed    id              either element or element id to resize
 * @return void
 */
function TNT_ResizeStart(event, id) {
  var element = null;

  // If an element id was given, find it. Otherwise use the element being
  // clicked on.

  if (id) {
     if (typeof id == 'object') {
        element = id;
     } else {
        element = document.getElementById(id);
     }
  } else {
    if (tnt_browser.isIE)
      element = window.event.srcElement;
    if (tnt_browser.isNS)
      element = event.target;

    // If this is a text node, use its parent element.
    if (element.nodeType == 3)
      element = element.parentNode;
  }

  dir = _TNT_ResizeDirection(event, element);
  if (dir == '')
    return;

  //alert('TNT_ResizeStart()');

  _tnt_resizeObj = new TNTResizeObj();

  // Save cursor position.
  if (tnt_browser.isIE) {
    _tnt_resizeObj.grabx = window.event.x;
    _tnt_resizeObj.graby = window.event.y;
  }
  if (tnt_browser.isNS) {
    _tnt_resizeObj.grabx = event.pageX;
    _tnt_resizeObj.graby = event.pageY;
  }

  _tnt_resizeObj.elNode = element;
  _tnt_resizeObj.dir = dir;

  // Ensure element is positionable
  if (_tnt_resizeObj.elNode.style.position != "absolute" &&
      _tnt_resizeObj.elNode.style.position != "relative")
	_tnt_resizeObj.elNode.style.position = "relative";

  _tnt_resizeObj.width = element.offsetWidth;
  _tnt_resizeObj.height = element.offsetHeight;
  _tnt_resizeObj.left = element.offsetLeft;
  _tnt_resizeObj.top = element.offsetTop;

  // Capture mousemove and mouseup events on the page.
  if (tnt_browser.isIE) {
    document.attachEvent("onmousemove", TNT_ResizeGo);
    document.attachEvent("onmouseup",   TNT_ResizeStop);
    window.event.cancelBubble = true;
    window.event.returnValue = false;
  }
  if (tnt_browser.isNS) {
    document.addEventListener("mousemove", TNT_ResizeGo,   true);
    document.addEventListener("mouseup",   TNT_ResizeStop, true);
    event.preventDefault();
  }
}

/**
 * Called when the mousemoves while resizing an element.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @return void
 */
function TNT_ResizeGo(event) {
  if (!_tnt_resizeObj) {
    TNT_ResizeStop(event);
    return;
  }

  // Minimum size
  var minWidth = 8;
  var minHeight = 8;

  var dir = _tnt_resizeObj.dir;

  var north = (dir.indexOf('n') != -1);
  var south = (dir.indexOf('s') != -1);
  var east = (dir.indexOf('e') != -1);
  var west = (dir.indexOf('w') != -1);

  // Find change in cursor position.
  var dx, dy;
  if (tnt_browser.isIE) {
    dx = window.event.x - _tnt_resizeObj.grabx;
    dy = window.event.y - _tnt_resizeObj.graby;
  }
  if (tnt_browser.isNS) {
    dx = event.pageX - _tnt_resizeObj.grabx;
    dy = event.pageY - _tnt_resizeObj.graby;
  }

  window.status = '('+dx+','+dy+')';

  // If resizing north or west, reverse corresponding amount.
  if (west)
    dx = -dx;
  if (north)
    dy = -dy;

  // Check new size.
  w = _tnt_resizeObj.width  + dx;
  h = _tnt_resizeObj.height + dy;
  if (w <= minWidth) {
    w = minWidth;
    dx = w - _tnt_resizeObj.width;
  }
  if (h <= minHeight) {
    h = minHeight;
    dy = h - _tnt_resizeObj.height;
  }

  // Resize the window.
  if (east || west)
    _tnt_resizeObj.elNode.style.width = w + "px";
  if (north || south)
    _tnt_resizeObj.elNode.style.height = h + "px";

  // For a north or west resize, move the window.
  if (west)
    _tnt_resizeObj.elNode.style.left = (_tnt_resizeObj.left - dx) + "px";
  if (north)
    _tnt_resizeObj.elNode.style.top  = (_tnt_resizeObj.top  - dy) + "px";

  if (tnt_browser.isIE) {
    window.event.cancelBubble = true;
    window.event.returnValue = false;
  }
  if (tnt_browser.isNS)
    event.preventDefault();
}

/**
 * Called to stop resizing.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @return void
 */
function TNT_ResizeStop(event) {

  // Stop capturing mousemove and mouseup events.
  if (tnt_browser.isIE) {
    document.detachEvent("onmousemove", TNT_ResizeGo);
    document.detachEvent("onmouseup",   TNT_ResizeStop);
  }
  if (tnt_browser.isNS) {
    document.removeEventListener("mousemove", TNT_ResizeGo,   true);
    document.removeEventListener("mouseup",   TNT_ResizeStop, true);
  }

  // Restore cursor.
  if (_tnt_resizeObj) {
    _tnt_resizeObj.elNode.style.cursor = _tnt_resizeObj.elNode._tnt_cursor;
  }

  if (tnt_browser.isIE)
    document.body.style.cursor = 'auto';

  _tnt_resizeObj = null;
}

/**
 * Given an event see if the target or ancestor of the target is resizable.
 *
 * The event target element (and its ancestors) are checked to see if they
* have a 'resize' HTML attribute defined. If so the element is returned.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @return element                  the resizable element
 */
function _TNT_ResizeElement(event) {
  var element = null;
  var tmp, resize;

  if (tnt_browser.isIE)
    tmp = window.event.srcElement;
  if (tnt_browser.isNS)
    tmp = event.target;

  // See if element or ancestor element is resizeable
  while ((tmp != null) && (tmp.tagName != 'BODY')) {
    if ((resize = tmp.getAttribute('resize'))) {
        element = tmp;
        break;
    }
    tmp = tmp.parentElement;
  }

  if (!element)
      return null;
  
  resize = resize.toLowerCase();
  if (resize == '1' || resize == 'yes' || resize == 'true')
    return element;
  
  return null;
}

/**
 * Called to check if the mouse is over the edge of the given element.
 *
 * Returns with edge the mouse is over given as a compass point,
 * (ie. 'n', 's', 'e', 'w', 'ne', 'se', 'sw', 'nw').
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access private
 * @param  event    event           event object (not IE)
 * @return string                   edge the mouse if over (or null)
 */
function _TNT_ResizeDirection(event, element) {
  var xOff, yOff, edge;

  // Find resize direction.
  if (tnt_browser.isIE) {
    xOff = window.event.offsetX;
    yOff = window.event.offsetY;
  }
  if (tnt_browser.isNS) {
    xOff = event.layerX;
    yOff = event.layerY;
  }

  var w = element.offsetWidth;
  var h = element.offsetHeight;

  edge = '';
  if (yOff <= 8)
    edge += "n";
  else if (yOff >= h - 8)
    edge += "s";
  if (xOff <= 8)
    edge += "w";
  else if (xOff >= w - 8)
    edge += "e";

  return edge;
}

/**
 * Allow auto resizing by defining an attribute on the element
 *
 * This is called when the mouse is pressed over the document, the target
 * element (and its parents) are checked to see if they have a 'resize'
 * HTML attribute defined. If so resizing starts on the element.
 *
 * The 'resize' attribute can be set to 'yes', 'no', 'true', 'false'.
 * If the attribute is set to 'no' or 'false' then no resizing occurs.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access private
 * @param  event    event           event object (not IE)
 * @return void
 */
function _TNT_ResizeAuto(event) {
  var element = _TNT_ResizeElement(event);
  if (element == null) {
    _tnt_resizeObj = null;
    return;
  }

  // Start resizing this element
  TNT_ResizeStart(event, element);
}

TNT_AddEventListener(document, 'mousedown', _TNT_ResizeAuto);

/**
 * Called to set the resize cursor as the mouse moves over the element.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access private
 * @param  event    event           event object (not IE)
 * @return void
 */
function _TNT_ResizeCursor(event) {
  if (_tnt_resizeObj)
    return;

  var element = _TNT_ResizeElement(event);
  if (element == null) {
    if (document.body)
      document.body.style.cursor = 'auto';
    return;
  }

  // Save original cursor style
  if (! element._tnt_cursor)
    element._tnt_cursor = (element.style.cursor ? element.style.cursor : 'auto');

  var dir = _TNT_ResizeDirection(event, element);

  // If not on window edge, restore cursor and exit.
  if (dir == '') {
    // Restore cursor.
    element.style.cursor = element._tnt_cursor;
    if (tnt_browser.isIE)
      document.body.style.cursor = 'auto';
    return;
  }

  // Change cursor.
  element.style.cursor = dir + '-resize';

  // In IE we change the body cursor as this prevents the cursor from flickering
  // if we drag the mouse outside of the element before the element resizes.
  if (tnt_browser.isIE)
    document.body.style.cursor = element.style.cursor;
}

TNT_AddEventListener(document, 'mousemove', _TNT_ResizeCursor);

/* vim: set expandtab: tabstop=2 shiftwidth=2 */
