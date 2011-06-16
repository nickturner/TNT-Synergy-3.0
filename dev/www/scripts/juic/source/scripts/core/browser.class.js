///////////////////////////////////////////////////////////////////////////////
// TNT-JUIC: Javascript User Interface Components                    v2.0.01 //
// ==============================================                            //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-juic.com                                                    //
// http://www.tnt-juic.com/                                                  //
//                                                                           //
// TNT-JUIC is an Javascript library of advanced user interface components   //
// for use on web based forms.                                               //
//                                                                           //
// WARNING: This package is protected by copyright law and international     //
// treaties. Unauthorized reproduction or distribution of this package,      //
// or any portion of it, may result in severe civil and criminal penalties,  //
// and will be prosecuted to the maximum extent possible under the law.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * TNT.Juic.Core.Browser Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Class to determine client browser capabilities and manipulate client browers.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Browser = TNT.declareClass({
  $class : 'TNT.Juic.Core.Browser',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function() {
	  if (window.external) {
      this._name = "ie";
    } else if (window.__defineGetter__) {
      this._name = "gecko"
    } else if (window.opera) {
      this._name = "opera";
    } else if (navigator.userAgent.match("Safari")) { 
      this._name = "safari";
    }

	  if (navigator.userAgent.match("Windows")) {
      this._os = 'windows';
    } else if (navigator.userAgent.match("Mac OS")) {
      this._os = "mac";
    } else if (navigator.userAgent.match("Linux")) {
      this._os = "linux";
    } else if (navigator.userAgent.match("Unix")) {
      this._os = "unix";
    }

    this._isStrict =
      (document.compatMode && document.compatMode.match("CSS")) ||
      (this._name == "safari");


    // Add a class to the root HTML element to allow selection on the browser
    // name etc.

    var className = '';
    if (this._isStrict) {
      className += " juic-css-strict";
    }
    if (this._name) {
      className += " juic-client-" + this._name;
    }
    if (this._os) {
      className += " juic-os-" + this._os;
    }

    if (className != '') {
	    document.getElementsByTagName("html")[0].className += className;
    }

    if (window.__defineSetter__) {
      var p = HTMLElement.prototype;

      p.__defineSetter__("outerHTML", function(s) {
		    var range = this.ownerDocument.createRange();
		    range.setStartBefore(this);
		    var fragment = range.createContextualFragment(s);
		    this.parentNode.replaceChild(fragment, this);
        });
    }

    // Register the window that included this script implicitly
    this.registerWindow(window);

  },

  /**
   * The name of the browser.
   * @access  private
   * @var     string                  browser name
   */
  _name : undefined,

  /**
   * The operating system the browser is running on.
   * @access  private
   * @var     string                  operating system name
   */
  _os : undefined,

  /**
   * Is strict CSS parsing enabled on the browser.
   * @access  private
   * @var     boolean                 true if strict parsing is enabled
   */
  _isStrict : undefined,

  /**
   * Retrieve the name of the client browser.
   *
   * @access	public
   * @return  string                  browser name
   */
  name : function() {
    return this._name;
  },

  /**
   * Retrieve the name of the client operating system.
   *
   * @access	public
   * @return  string                  operating system name
   */
  os : function() {
    return this._os;
  },

  /**
   * Retrieve whether the client browser is in strict mode or quirks mode.
   *
   * @access	public
   * @return  boolean                 parsing mode
   */
  isStrict : function() {
    return this._isStrict;
  },

  /**
   * Attach an event to a DOM element
   *
   * @access	public
   * @param   object    element       DOM element to attach to
   * @param   string    name          name of event
   * @param   function  handler       function to call when event triggered
   * @return  boolean                 true if event attached successfully
   */
  attachEvent : function(element, name, handler) {
    if (element.attachEvent) {
      element.attachEvent(name, handler);
    } else if (element.addEventListener) {
      element.addEventListener(name.replace(/^on/, ''), handler, false);
    } else {
      throw 'attachEvent not implemented';
    }
  },

  /**
   * Detach an event from a DOM element
   *
   * @access	public
   * @param   object    element       DOM element to attach to
   * @param   string    name          name of event
   * @param   function  handler       function to call when event triggered
   * @return  boolean                 true if event attached successfully
   */
  detachEvent : function(element, name, handler) {
    if (element.attachEvent) {
      element.detachEvent(name, handler);
    } else if (element.removeEventListener) {
      element.removeEventListener(name.replace(/^on/, ''), handler, false);
    } else {
      throw 'detachEvent not implemented';
    }
  },

  /**
   * Get a cross browser event.
   *
   * The cross browser event contains the following members and methods;
   *   srcElement :       element that caused the event
   *   toElement :        element related to the event
   *   preventDefault() : prevents default event action
   *   cancelBubble() :   cancels event bubbling
   *   setCapture() :     sets mouse capturing
   *   releaseCapture() : releases mouse capturing
   *
   * @access	public
   * @param   object    event         the existing browser event
   * @return  string                  name of class
   */
  event : function(e) {
    if (!e) {
      e = window.event;
    }
    if (!e.srcElement) {
      e.srcElement = (e.target && e.target.nodeType == 3) ? e.target.parentNode : e.target;
    }

    if (!e.toElement) {
      e.toElement = (e.relatedTarget && e.relatedTarget.nodeType == 3) ? e.relatedTarget.parentNode : e.relatedTarget;
    }

    if (!e.preventDefault) {
      e.preventDefault = function() { this.returnValue = false; }
    }

    if (!e.setCapture) {
      e.setCapture = function() {};
    }

    if (!e.releaseCapture) {
      e.releaseCapture = function() {};
    }

    return e;
  },

  /**
   * Add a stylesheet rule.
   *
   * @access	public
   * @param   object    stylesheet    the browser stylesheet
   * @param   string    selector      the selector to select the rule
   * @param   string    rule          the stylesheet rule to add
   * @return  boolean                 true if successfull
   */
  addStyleSheetRule : function(stylesheet, selector, rule) {
    if (stylesheet.addRule) {
      stylesheet.addRule(selector, rule);
    } else {
			var i = stylesheet.cssRules.length;
			stylesheet.insertRule(selector + "{" + rule + "}", i);
			stylesheet.cssRules[i].style.cssText = rule; 
    }
    return true;
  },

  /**
   * Retrieves a collection of rules defined in a style sheet.
   * 
   * This collection is always accessible, even if the style sheet is not
   * enabled. Rules are added to the rules collection with the {@link
   * addStyleSheetRule()} method on the individual style sheet. A rule that is
   * added to a disabled style sheet does not apply to the document unless the
   * style sheet is enabled. Rules are deleted with the removeRule method.
   *
   * The rules in this collection are in the source order of the document. As
   * rules are added or deleted through the Cascading Style Sheets (CSS) Object
   * Model, a rule's absolute position in the rules collection might change,
   * but its position relative to other rules remains the same.
   *
   * @access	public
   * @param   object
   * @return  string                  the objects unique id
   */
  stylesheetRules : function(stylesheet) {
    return stylesheet.cssRules ? stylesheet.cssRules : stylesheet.rules;
  },

  /**
   * Returns a class name list given given an orginal class name and a list
   * of states to apply.
   * 
   * This collection is always accessible, even if the style sheet is not
   * enabled. Rules are added to the rules collection with the {@link
   * addStyleSheetRule()} method on the individual style sheet. A rule that is
   * added to a disabled style sheet does not apply to the document unless the
   * style sheet is enabled. Rules are deleted with the removeRule method.
   *
   * The rules in this collection are in the source order of the document. As
   * rules are added or deleted through the Cascading Style Sheets (CSS) Object
   * Model, a rule's absolute position in the rules collection might change,
   * but its position relative to other rules remains the same.
   *
   * @access	public
   * @param   object
   * @return  string                  the objects unique id
   */
  stylesheetRules : function(stylesheet) {
    return stylesheet.cssRules ? stylesheet.cssRules : stylesheet.rules;
  },

  /**
   * Set the outerHTML of an element.
   * 
   * This basically replaces the element with the HTML specified.
   *
   * @access	public
   * @param   object    element       the element who's outerHTML should be set
   * @param   string    html          the markup to set the outerHTML to
   * @return  void
   */
  setOuterHTML : function(element, html) {
    if (element.outerHTML) {
      element.outerHTML = html;
    } else {
		  var range = element.ownerDocument.createRange();
		  range.setStartBefore(element);
		  var fragment = range.createContextualFragment(html);
		  element.parentNode.replaceChild(fragment, element);
    }
  },

  /**
   * Modify a pseudo state on an HTML element.
   *
   * The state of an object can be reflected in its CSS class selectors thus
   * allowing different object states to be styled differently. This is the
   * same concept as CSS psuedo-classes however those can't be set from within
   * a script.
   *
   * Setting a CSS state adds a state class to the elements class names,
   * however as Internet Explorer does not correct allow CSS selectors
   * based on multiple class names we have to create unique class names
   * for each style combination.
   *
   * For Example, setting the 'clicked' state on an element with the CSS
   * class name 'mydiv' would ideally just add the class name 'clicked' to
   * the elements CSS class names, but as IE doesn't supported the CSS
   * selector '.mydiv.clicked' correctly, we actually append the suffix
   * '--clicked' to the elements current class name resulting in the element
   * having the class name 'mydiv mydiv--clicked' and then you can use the CSS
   * selector '.mydiv--clicked' to uniquely select all clicked elements with
   * the class 'mydiv'.
   *
   * Multiple states can be set resulting in longer and more complex class
   * names, ie. Setting the 'clicked' and 'opened' states would result in the
   * element being given the class names 'mydiv mydiv--clicked mydiv--opened
   * mydiv--clicked-opened mydiv--opened-clicked'.
   *
   * The states are separated from the class name using '--' and from each other
   * using '-'. Because a CSS class name can not start with a '-' if the element
   * has no CSS class then a dummy one of 'juic' is used.
   *
   * The drawback with this method is that the order of states must remain
   * consistant to keep the CSS selectors simple. Therefore the states are
   * added in alphabetical order.
   *
   * @access  public
   * @param   object      element     HTML DOM element
   * @param   mixed       set         state or array of states to set
   * @param   mixed       clr         state or array of states to clear
   * @return  void
   */ 
  modifyPsuedoState : function(element, set, clr) {
    if (!element || (!set && !clr)) {
      return;
    }

    var cn = element.className;

    var c = TNT.splitClassName(cn);

    if (set) {
      if (set instanceof Array) {
        for (var i = 0, max_i = set.length; i < max_i; i++) {
          if (!c.states.contains(set[i])) {
            c.states.push(set[i]);
          }
        }
      } else {
        if (!c.states.contains(set)) {
          c.states.push(set);
        }
      }
    }

    if (clr) {
      if (clr instanceof Array) {
        for (var i = 0, max_i = clr.length; i < max_i; i++) {
          c.states.remove(clr[i]);
        }
      } else {
        c.states.remove(clr);
      }
    }

    cn = TNT.makeClassName(c.classes, c.states);

    element.className = cn;

  },

  /**
   * All DOM events are routed to this global event handler which then
   * calls {@link raiseEvent()} on the correct javascript object.
   *
   * The handler also handles the generic psuedostates which don't require
   * a javascript object to be tied to the HTML DOM element.
   *
   * Only HTML DOM elements with the attribute 'juic' will trigger this
   * event call, at the moment the value of the attribute is ignored.
   *
   * @access  private
   * @param   object       element    HTML DOM element
   * @param   object       event      HTML DOM event
   * @param   string       type       event name
   * @return  integer                 event.returnValue value
   */
  _event : function(element, event, type) {

    if (!event && window.event) {
      event = window.event;
    }

    if (!type) {
      type = event.type;
    }
    
    if (typeof event.returnValue == 'undefined') {
      event.returnValue = true; // perform default action
    }

    var object = TNT.Juic.Core.Object.prototype.instance(element.id);
    if (object) {
      var ret = object._event(element, event, type);
      if (!ret || !event.returnValue) {
        TNT.debug('Object cancelled the event');
        return false;
      }
    }

    // Handle the generic psuedostates
    switch (type) {
      case 'mousemove':
        this.modifyPsuedoState(element, 'hover', null);
        break;
      case 'mouseover':
        this.modifyPsuedoState(element, 'hover', null);
        break;
      case 'mouseout':
        this.modifyPsuedoState(element, null, 'hover');
        break;
      case 'mousedown':
        this.modifyPsuedoState(element, 'active');
        break;
      case 'mouseup':
        this.modifyPsuedoState(element, null, 'active');
        break;
      case 'focus':
        this.modifyPsuedoState(element, 'focus', null);
        break;
      case 'blur':
        this.modifyPsuedoState(element, null, 'focus');
        break;
    }

    return true;
  },

  /**
   * Sets up a window who's document will be monitored.
   *
   * The document is is set up to route events through to the {@link _event()}
   * function. This internally handles mouse events etc.
   *
   * @access  private
   * @return  void
   */
  registerWindow : function(window) {
  
    // Bubble the event up the DOM raising events on all JUIC objects
    function bubbleEvent(event) {
      var e = event.srcElement;
      while (e) {
        if (e.getAttribute && e.getAttribute('juic')) {
          TNT.Juic.browser._event(e, event); 
        }
        e = e.parentNode;
      }
    }
    
    var targets = {};
    
    // Same as bubbleEvent but we ensure that mouseout is called for any element
    // that the mouse was in previously
    function mouseEvent(event) {
      var t = {};
      var e = (event.type == 'mouseout' ? event.toElement : event.srcElement);
    
      while (e) {
        if (e.getAttribute && e.getAttribute('juic')) {
          t[e.id] = e;
        }
        e = e.parentNode;
      }
    
      // Ensure a mouse out occurrs on every element we sent a mouse in to 
      for (i in targets) {
        if (!t[i]) {
          TNT.Juic.browser._event(targets[i], event, 'mouseout');
        }
      }
    
      // Send mouse in to all element we not already sent one to
      for (i in t) {
        if (!targets[i]) {
          TNT.Juic.browser._event(t[i], event, 'mouseover');
        }
      }
    
      targets = t;
    }
    
    var handlers = {
      onmouseover: function(event) {mouseEvent(event)},
      onmouseout: function(event) {mouseEvent(event)},
      onmousemove: function(event) {bubbleEvent(event)},
      onmousedown: function(event) {bubbleEvent(event)},
      onmouseup: function(event) {bubbleEvent(event)},
      onclick: function(event) {bubbleEvent(event)},
      ondblclick: function(event) {bubbleEvent(event)}
    };
    
    this.attachEvent(window, 'onload', function(event) {
    		for (name in handlers) {
          TNT.Juic.browser.attachEvent(document, name, handlers[name]);
        }
      });
    
    this.attachEvent(window, 'onunload', function(event) {
    		for (name in handlers) {
          TNT.Juic.browser.detachEvent(document, name, handlers[name]);
        }
        TNT.Juic.Core.Object.destroy_all();
      });
    
  },

  /**
   * Destructor
   *
   * @access  private
   */
  _destructor : function() {
  }

});

/**
 * Global browser object.
 * @access  private
 * @var     object                  global browser object
 */
TNT.Juic.browser = new TNT.Juic.Core.Browser();

  
/* vim: set expandtab tabstop=2 shiftwidth=2: */

