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
 * Drag & Drop
 * ~~~~~~~~~~~
 *
 * These function allow DOM elements to be dragged around the screen.
 *
 * @package TNT.JScript
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

// Global object to hold drag information.
var _tnt_dragObj = null;

function TNTDragObj() {
  this.elNode       = null; 
  this.cursorStartX = null;
  this.cursorStartY = null;
  this.elStartLeft  = null;
  this.elStartTop   = null;
  this.zIndex       = null;
}

/**
 * Called to start dragging an element.
 *
 * The element to be dragged is assume to be the current event target unless
 * the 'id' parameter is given.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @param  mixed    id              either element or element id to drag
 * @return void
 */
function TNT_DragStart(event, id) {

  var element = null;

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

  if (element.getAttribute('resize')) {
    // We only drag if the cursor is not within 8 pixels of the elements
    // edge as that is where the resize code will kick in
    var xOff, yOff;

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
    if (yOff <= 8 || xOff <= 8 || yOff >= h - 8 || xOff >= w -8)
      return;
  }

  // If an element id was given, find it. Otherwise use the element being
  // clicked on.
  _tnt_dragObj = new TNTDragObj();

  _tnt_dragObj.elNode = element;

  var x, y;

  // Get cursor position with respect to the page.
  if (tnt_browser.isIE) {
    x = window.event.clientX + document.documentElement.scrollLeft
      + document.body.scrollLeft;
    y = window.event.clientY + document.documentElement.scrollTop
      + document.body.scrollTop;
  }
  if (tnt_browser.isNS) {
    x = event.clientX + window.scrollX;
    y = event.clientY + window.scrollY;
  }

  // Ensure element is positionable
  if (_tnt_dragObj.elNode.style.position != "absolute" &&
      _tnt_dragObj.elNode.style.position != "relative")
	_tnt_dragObj.elNode.style.position = "relative";

  // Save starting positions of cursor and element.
  _tnt_dragObj.cursorStartX = x;
  _tnt_dragObj.cursorStartY = y;
  _tnt_dragObj.elStartLeft  = parseInt(_tnt_dragObj.elNode.style.left, 10);
  _tnt_dragObj.elStartTop   = parseInt(_tnt_dragObj.elNode.style.top,  10);

  if (isNaN(_tnt_dragObj.elStartLeft)) _tnt_dragObj.elStartLeft = 0;
  if (isNaN(_tnt_dragObj.elStartTop))  _tnt_dragObj.elStartTop  = 0;

  // Update element's z-index.
  _tnt_dragObj.elNode.style.zIndex = ++_tnt_dragObj.zIndex;

  // Capture mousemove and mouseup events on the page.
  if (tnt_browser.isIE) {
    document.attachEvent("onmousemove", TNT_DragGo);
    document.attachEvent("onmouseup",   TNT_DragStop);
    window.event.cancelBubble = true;
    window.event.returnValue = false;
  }
  if (tnt_browser.isNS) {
    document.addEventListener("mousemove", TNT_DragGo,   true);
    document.addEventListener("mouseup",   TNT_DragStop, true);
    event.preventDefault();
  }
}

/**
 * Called when the mousemoves while dragging an element.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @return void
 */
function TNT_DragGo(event) {

  if (! _tnt_dragObj) {
    TNT_DragStop();
    return;
  }

  // Get cursor position with respect to the page.
  var x, y;
  if (tnt_browser.isIE) {
    x = window.event.clientX + document.documentElement.scrollLeft
      + document.body.scrollLeft;
    y = window.event.clientY + document.documentElement.scrollTop
      + document.body.scrollTop;
  }
  if (tnt_browser.isNS) {
    x = event.clientX + window.scrollX;
    y = event.clientY + window.scrollY;
  }

  // Move drag element by the same amount the cursor has moved.

  _tnt_dragObj.elNode.style.left = (_tnt_dragObj.elStartLeft + x - _tnt_dragObj.cursorStartX) + "px";
  _tnt_dragObj.elNode.style.top  = (_tnt_dragObj.elStartTop  + y - _tnt_dragObj.cursorStartY) + "px";

  if (tnt_browser.isIE) {
    window.event.cancelBubble = true;
    window.event.returnValue = false;
  }
  if (tnt_browser.isNS)
    event.preventDefault();
}

/**
 * Called to stop dragging.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @return void
 */
function TNT_DragStop(event) {

  // Stop capturing mousemove and mouseup events.
  if (tnt_browser.isIE) {
    document.detachEvent("onmousemove", TNT_DragGo);
    document.detachEvent("onmouseup",   TNT_DragStop);
  }
  if (tnt_browser.isNS) {
    document.removeEventListener("mousemove", TNT_DragGo,   true);
    document.removeEventListener("mouseup",   TNT_DragStop, true);
  }

  _tnt_dragObj = null;
}

/**
 * Allow auto dragging by defining an attribute on the element
 *
 * This is called when the mouse is pressed over the document, the target
 * element (and its parents) are checked to see if they have a 'drag'
 * HTML attribute defined. If so dragging starts on the element.
 *
 * The 'drag' attribute can be set to 'yes', 'no', 'true', 'false' or the
 * id of an element on the page. If the attribute is set to 'no' or 'false'
 * then no dragging occurs. If the attribute is set to an element id then
 * the element with that id is dragged.
 *
 * On IE the 'event' parameter is ignored and the global 'windows.event'
 * object is used instead.
 *
 * @access public
 * @param  event    event           event object (not IE)
 * @return void
 */
function _TNT_DragAuto(event) {
  var element = null;
  var tmp, drag;

  if (tnt_browser.isIE)
    tmp = window.event.srcElement;
  if (tnt_browser.isNS)
    tmp = event.target;

  // See if element or ancestor element is draggable
  while ((tmp != null) && (tmp.tagName != 'BODY')) {
    if ((drag = tmp.getAttribute('drag'))) {
        element = tmp;
        break;
    }
    tmp = tmp.parentElement;
  }

  if (!element) {
    return;
  }

  drag = drag.toLowerCase();
  if (drag == '1' || drag == 'yes' || drag == 'true') {
    // Start dragging this element
    TNT_DragStart(event, element);
  } else if (drag != 'no' && drag != 'false' && drag != '0') {
    // Start dragging the named element
    TNT_DragStart(event, drag);
  }
}

TNT_AddEventListener(document, 'mousedown', _TNT_DragAuto);

/* vim: set expandtab: tabstop=2 shiftwidth=2 */
