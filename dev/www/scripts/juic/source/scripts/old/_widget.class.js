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
 * TNT.Juic.Core.Widget Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Generic base class for UI widgets. A widget is the base of any UI element
 * and is responsible with capturing DOM events.
 *
 * This object provides a set of methods to manipulate the HTML DOM.
 *
 * The toString() method will return the properly formatted HTML markup string,
 * which can be passed to the document.write() call.
 *
 * The link between the javascript object and it's DOM element is maintained
 * through the use of the unique object id.
 *
 * By default a widget can act as a container for other widgets.
 *
 * A widget has the concept of states which a represented to the DOM as CSS
 * class names. This mimics the idea of CSS3 psuedo-classes for browsers that
 * do not support them. 
 *
 * The main W3C states of 
 *  :hover 
 *  :active 
 *  :focus 
 *  :enabled 
 *  :disabled 
 *  :checked 
 *  :indeterminate 
 *  :read-only
 *
 * are supported as classes on the object.
 *
 * @package TNT.Juic
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Widget = TNT.declareClass({
  $class : 'TNT.Juic.Core.Widget',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function() {
    TNT.Juic.Core.Object.prototype.constructor.call(this);

    this._isContainer = true;

    this._contents = new TNT.Juic.Core.Contents(this);
    this._classes = [];
    this._states = [];
    this._toolTip = '';

  },

  /**
   * Hash map of content items
   * @access  private
   * @var     object                  Hash object
   */
  _contents : null,
  
  /**
   * HTML element states
   * @access  private
   * @var     array                   HTML elements states
   */
  _states : null,
  
  /**
   * Reference to actual HTML element
   * @access  private
   * @var     object                  HTML element
   */
  _element : null,

  /**
   * Reference to any parent node (not clonable)
   * @access  private
   * @var     object                  reference to parent node
   */
  _parent_ : null,
  
  /**
   * Boolean flag to indicate that this node can have contents.
   * @access  private
   * @var     boolean                 node can have contents
   */
  _isContainer : true,

  /**
   * Return a clone of this object
   *
   * The cloned object will not have a parent.
   *
   * @access  public
   * @param   boolean   deep          deep clone (clone all descendants)
   * @return  object                  clone of object
   */
  clone : function(deep) {
    var clone = TNT.Juic.Core.Object.prototype.constructor.clone(deep);
    clone.$owner = null;
    clone.$parent = null;
    return clone;
  },

  /**
   * Return if the element can contain content.
   *
   * Elements which can not contain content will return undefined for calls
   * to access the content.
   *
   * The containment type for a node is set by the derived class and hence
   * there is no public method to set this value.
   *
   * @access  public
   * @return  boolean                 true if the element is a container
   */
  isContainer : function(value) {
    return this._isContainer;
  },

  /**
   * Return reference to parent node.
   *
   * When this node is added to the content of another node then it
   * becomes a child of that node and this method will return a reference
   * to the parent node (the node containing this node).
   *
   * A node is not designed to have multiple parents, that is it can not
   * be added as the content of more than one node. If a parented node is
   * added to another node then it is removed from the original parent.
   *
   * @access  public
   * @return  object                  reference to parent node
   */
  parent : function() {
    return this._parent_;
  },

  /**
   * Set the widgets ID.
   *
   * This is used as the HTML element ID of the containing element.
   *
   * If the widget has been rendered then the change is made to the HTML DOM
   * accordingly. IDs should be unique.
   *
   * @access  public
   * @param   string      id          valid HTML element ID
   * @return  boolean                 true if changed, false if unchanged
   */
  setId : function(id) {
    if (!TNT.Juic.Core.Object.prototype.setId.call(id)) {
      return false;
    }

    return true;
  },
  
  /**
   * Get the HTML element ID
   *
   * @access  public
   * @return  string                  HTML element ID
   */
  getId : function() {
    return TNT.Juic.Core.Object.prototype.getId.call();
  },

  /**
   * Set the HTML elements CSS state.
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
   * selector '.mydiv.clicked' correctly we actually append the suffix
   * '-clicked' to the elements current class name resulting in the element
   * having the class name 'mydiv mydiv-clicked' and then you can use the CSS
   * selector '.mydiv-clicked' to uniquely select all clicked elements with
   * the class 'mydiv'.
   *
   * Multiple states can be set resulting in longer and more complex class
   * names, ie. Setting the 'clicked' and 'opened' states would result in the
   * element being given the class names 'mydiv mydiv-clicked mydiv-opened
   * mydiv-clicked-opened'.
   *
   * The drawback with this method is that the order of states must remain
   * consistant to keep the CSS selectors simple. Therefore the states are
   * added in alphabetical order.
   *
   * If the method is called with no arguments then all states are cleared.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to set each pair.
   *
   * The name can contain a path to a content with the hierachy created by this
   * element and its content. For example the name 'label/hover' will set the
   * state 'hover' on the content object named 'label'.
   * 
   * @access  public
   * @param   string      name        CSS state name
   * @param   boolean     value       boolean value for the state
   * @return  boolean                 true if changed, false if unchanged
   */
  setState : function(name, value) {

    if (arguments.length == 0) {
      var ret = false;
      for (var i in this._states) {
        var name = this._states[i];
        delete this._states[i];
        //this._onStateChange(name);
        ret = true;
      }
      return ret;
    }

    if (arguments.length == 1) {
      // assigning an array or single function
      this._states = [];
      if (typeof name == 'object') {
        var ret = false;
        for (var i in name) {
          if (this.setState(i, name[i])) {
            ret = true;
          }
        }
        return ret;
      }

      throw 'setState() - invalid arguments';
      return;
    }

/*
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.setState(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }
*/

    var max_i = this._states.length;
    name = name.toLowerCase();
    if (value) {
      for (var i = 0; i < max_i; i++) {
        if (this._states[i] == name) {
          return false;
        }
      }
      this._states[this._states.length] = name;
    } else {
      var states = [];
      for (var i = 0; i < max_i; i++) {
        if (this._states[i] != name) {
          states[states.length] = this._states[i];
        }
      }
      if (this._states.length == states.length) {
        return false;
      }
      this._states = states;
    }


    this._states.sort();
    this._statesMarkup = null;

    //this._onStateChange(name);

    return true;
  },
  
  /**
   * Set the HTML elements CSS state.
   *
   * If the method is called no arguments then a string to use as the elements
   * class name is returned.
   *
   * The name can contain a path to a content with the hierachy created by this
   * element and its content. For example the name 'label/hover' will get the
   * state 'hover' on the content object named 'label'.
   *
   * @access  public
   * @param   string      name        valid HTML attribute name
   * @return  boolean                 boolean value of the state
   */
  getState : function(name) {
    if (arguments.length == 0) {
      if (!this._statesMarkup) {
        this._statesMarkup = '';
        if (this._states.length) {
          var cn = this.getAttribute('class');

          var cs = (cn ? cn.split(/\s+/) : null);
          var ss = this._states.concat();

          var i, max_i = this._states.length;
          var j, max_j = (1 << max_i);
          for (var c in cs) {
            if (cs[c].substr(0, 1) == '_' || cs[c].match(/layout/)) {
              continue;
            }
            for (j = 1; j < max_j; j++) {
              this._statesMarkup += ' ';
              if (cs[c]) {
                this._statesMarkup += cs[c] + '-';
              }
              for (i = 0; i < max_i; i++) {
                if (j & (1 << i)) {
                  this._statesMarkup += this._states[i] + '-';
                }
              }
              this._statesMarkup = this._statesMarkup.substr(0, this._statesMarkup.length - 1);
            }
          }
        }
      }
      return this._statesMarkup;
    }

/*
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.getState(split[1]);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }
*/

    var max_i = this._states.length;
    for (var i = 0; i < max_i; i++) {
      if (this._states[i] == name) {
        return true;
      }
    }

    return false;
  },
  
  /**
   * Set the objects content.
   *
   * The content value can be a static HTML markup string, a JavaScript Object
   * or another JUIC HTML object. It can even be an array of a mixture of those.
   *
   * If the method is called with no arguments then all contents are cleared.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to set each pair.
   *
   * If the method is called with a single string argument then it is assumed
   * to be the name of some content to remove. Alternatively pass in the value
   * of the content as undefined.
   *
   * The name can be a path to a content within the hierachy created by this
   * element and its content. 
   * 
   * For example, the content name 'label/text' will set the grandchild called
   * text whose parent is our child called 'label'.
   *
   * Changing the nodes content will cause the {@link _changedProperty()}
   * method to be called with the property name 'content.<name>' where <name>
   * is the name of the content item that was changed.
   *
   * Content names are case sensitive !!
   * 
   * @access  public
   * @param   string      name        content name or path
   * @param   mixed       value       static/dynamic value for the content
   * @return  boolean                 true if changed, false if unchanged
   */
  setContent : function(name, value) {

    if (!this._isContainer) {
      throw 'Not a container';
      return;
    }

    if (arguments.length == 0) {
      if (typeof name == 'object') {
        var ret = false;
        for (var i in this._contents) {
          delete this._contents[i];
          this._changedProperty('content.' + i.substr(1));
          ret = true;
        }
        return ret;
      }

      throw 'setAttribute() - invalid arguments';
      return;
    }

    // assigning an array of content items 
    if (arguments.length == 1 && typeof name == 'object') {
      var ret = false;
      for (var i in name) {
        if (this.setContent(i, name[i])) {
          ret = true;
        }
      }
      return ret;
    }

/*
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        return content.setContent(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }
*/

    if (this._contents[name] == value) {
      return false;
    }

    // If the new value is an object then parent it
    if (typeof value == 'object') {
      if (value instanceof TNT.Juic.Core.HTML) {
        if (value._parent_) { 
          throw 'setContent() - content is already parented';
        }
        value._parent_ = this;
      }
      value.$owner_ = this;
    }

    var old = this._contents.set(name, value);

    // do we already have some content of that name, if so an its an
    // object the orphan it.
    if (typeof old == 'object') {
      if (old instanceof TNT.Juic.Core.HTML && old._parent_ == this) {
        old._parent_ = undefined;
      }
      if (old.$owner_ == this) {
        old.$owner_ = undefined;
      }
    }

    //this._changedProperty('content.' + name);

    return true;
  },
  
  /**
   * Get the objects content
   *
   * If the method is called no arguments then the full concatenated
   * string values of the contents are returned.
   *
   * Content names are case sensitive !!
   *
   * @access  public
   * @param   string      name        content name or path
   * @return  mixed                   value of named content
   */
  getContent : function(name) {

    if (!this._isContainer) {
      throw 'Not a container';
      return;
    }

    if (arguments.length == 0) {
      return this._contents.toString();
    }

/*
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        return content.getContent(split[1]);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }
*/

    return this._contents.get(name);
  },
  
  /**
   * Set the tooltip that will be displayed when the user hovers over
   * the widget.
   *
   * Only text is allowed in the tooltip, HTML is not supported.
   *
   * @access	public
   * @param   mixed       value       static/dynamic value for the tooltip
   * @return  boolean                 true if changed, false if unchanged
   */
  setToolTip : function(value) {
    this._toolTip = value;
  },

  /**
   * Get the tooltip that will be displayed when the user hovers over
   * the widget.
   *
   * Only text is allowed in the tooltip, HTML is not supported.
   *
   * @access	public
   * @return  string                  value for the tooltip
   */
  getToolTip : function() {
    return this.dynamicValue(this._toolTip);
  },

  /**
   * Attach an event handler to the object or sub object.
   *
   * Same as superclass method but refreshes the events that require capturing
   * directly on the HTML DOM element.
   *
   * @access  public
   * @param   string      name        name of event
   * @param   mixed       handler     the code/function to handle the event
   * @return  void
   */
/*
  attachEventHandler : function(name, handler) {
    TNT.Juic.Core.Object.prototype.attachEventHandler.call(this, name, handler);
    this._refreshEventCaptures();
  },
*/
  
  /**
   * Remove an event handler from the object.
   *
   * Same as superclass method but refreshes the events that require capturing
   * directly on the HTML DOM element.
   *
   * @access  public
   * @param   string      name        name of controller to remove
   * @return  void
   */
/*
  removeEventHandler : function(name, handler) {
    TNT.Juic.Core.Object.prototype.detachEventHandler.call(this, name, handler);
    this._refreshEventCaptures();
  },
*/

  /**
   * Raise an event on the object.
   *
   * Same as superclass method but refreshes the elements state on certain
   * events.
   *
   * @access  public
   * @param   string      name        event name
   * @param   mixed       event       event object
   * @return  boolean                 the value of 'event.returnValue'
   */
/*
  raiseEvent : function(name, event) {
    TNT.Juic.Core.Object.prototype.raiseEvent.call(this, name, event);
    return event.returnValue;
  },
*/

  /**
   * Return a reference to the actual HTML document containing this Object
   *
   * @access  public
   * @return  object                  reference to HTML DOM document
   */
  document : function() {
    if (!this._document) {
      // Search all documents
      try {
        this._document = window.document;
      }
      catch(e) {
      }
    }
    return this._document;
  },
  
  /**
   * Return a reference to the actual HTML element for this JavaScript Object
   *
   * @access  public
   * @return  string      xxxx        HTML element ID
   * @return  object                  reference to HTML DOM element
   */
  element : function() {
    var element = null;
    if (window.document) {
      var id = this._id ? this._id : this.uniqueId();
      element = window.document.getElementById(id);
    }

    if (!element) {
      TNT.debug('object has no DOM element associated with it');
    }

    return element;
  },

  /**
   * Render the widget.
   *
   * This will return the XHTML required to render the widget in the users
   * browser. The widget will normally be combined with other widgets to
   * build up a complex XHTML document.
   *
   * The default method just renders the contents in a span element.
   *
   * @access  public
   * @return  string                  the XHTML to render this widget
   */
  render : function() {
  },

  /**
   * Return the value of the object as a string.
   *
   * This is a native javascript method which can be called directly or
   * implicitely when using the object in a string context.
   *
   * The method returns the outerHTML which can be written to the document
   * or assigned to an DOMs innerHTML property.
   * call or assigned to the page innerHTML 
   *
   * @access  public
   * @return  string                  objects HTML markup
   */
  toString : function() {
    return this.render();
  },

  /**
   * Refresh the CSS state when an event occurs.
   *
   * Called by {@link raiseEvent()} to refresh the CSS state on
   * certain events.
   *
   * At the moment it handles the follow CSS states;
   *   hover, active, focus
   *
   * @access  public
   * @param   string      name        event name
   * @param   mixed       event       event object
   * @return  void
   * @return  void
   */
  _refreshState : function(name, event) {
/*
    switch (name) {
      case 'mousemove':
        this.setState('hover', true);
        break;
      case 'mouseover':
        this.setState('hover', true);
        break;
      case 'mouseout':
        this.setState('hover', false);
        break;
      case 'mousedown':
        this.setState('active', true);
        break;
      case 'mouseup':
        this.setState('active', false);
        break;
      case 'focus':
        this.setState('focus', true);
        break;
      case 'blur':
        this.setState('focus', false);
        break;
      default:
        return;
    }
*/

  },

  /**
   * Raise a state change event.
   *
   * This should be called whenever the state of the object is changed.
   *
   * This is automatically called by {@link setState()} when the state
   * changes.
   *
   * The default action will actually change the state on the HTML DOM
   * element.
   *
   * @access  public
   * @param   string       name       name of the state that changed
   * @return  void
   */
  _onStateChange : function(name) {
/*
    var event = {
                  'type' : 'statechange',
                  'target' : this,
                  'stateName' : name,
                  'returnValue' : true
                };

    this.raiseEvent('onstatechange', event);

    if (event.returnValue) {
      if (typeof this._doStateChange == 'function') {
        this._doStateChange(event);
      }

      var element = this.element();
      if (element) {
        var _class = this.getAttribute('class') || '';
        var _state = this.getState() || '';
        element.className = _class + ' ' + _state;
      }
    }
*/

  },

  /**
   * All DOM events are routed to this event handler which then
   * calls {@link raiseEvent()} on the correct javascript object.
   *
   * @access  private
   * @param   object       element    HTML DOM element
   * @param   object       event      HTML DOM event
   * @param   string       type       event name
   * @return  integer                 event.returnValue value
   */
  _event : function(element, event, type) {
  
/*
    if (!type) {
      type = event.type;
    }
  
    // Fire the event handlers on the element which cause the event
    this.raiseEvent('on' + type, event);
  
    // Perform the default action for the event (if not cancelled)
    if (event.returnValue) {
      this._refreshState(type, event);

      var fcn = TNT.camelCase('_do', type);
      if (typeof this[fcn] == 'function') {
        this[fcn](event, element, type);
      }
    }
*/
  
    return event.returnValue;
  },

  /**
   * Destructor
   *
   * @access  private
   * @return  void
   */
  _destructor : function() {
  }

});

/**
 * All DOM events are routed to this global event handler which then
 * calls {@link raiseEvent()} on the correct javascript object.
 *
 * @access  private
 * @param   object       element    HTML DOM element
 * @param   object       event      HTML DOM event
 * @param   string       type       event name
 * @return  integer                 event.returnValue value
 */
TNT.Juic.Core.HTML._event = function(element, event, type) {

  if (!event && window.event) {
    event = window.event;
  }

  if (typeof event.returnValue == 'undefined') {
    event.returnValue = true; // perform default action
  }

  var object = TNT.Juic.Core.HTML.prototype.instance(element.id);
  if (!object) {
    TNT.debug('No object with id: ' + element.id);
    return;
  }

  return object._event(element, event, type);
}

/**
 * Sets up a window who's document will be monitored.
 *
 * The document is is set up to route events through to the {@link _event()}
 * function. This internally handles mouse events etc.
 *
 * @access  private
 * @return  void
 */
TNT.Juic.Core.Widget._registerWindow = function(window) {

  // Bubble the event up the DOM raising events on all JUIC objects
  function bubbleEvent(event) {
    var e = event.srcElement;
    while (e) {
      if (e.attrs && e.attrs['juic']) {
        TNT.Juic.Core.Widget._event(e, event); 
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
      if (e.attrs && e.attrs['juic']) {
        t[e.id] = e;
      }
      e = e.parentNode;
    }
  
    // Ensure a mouse out occurrs on every element we sent a mouse in to 
    for (i in targets) {
      if (!t[i]) {
        TNT.Juic.Core.Widget._event(targets[i], event, 'mouseout');
      }
    }
  
    // Send mouse in to all element we not already sent one to
    for (i in t) {
      if (!targets[i]) {
        TNT.Juic.Core.Widget._event(t[i], event, 'mouseover');
      }
    }
  
    targets = t;
  }
  
  
  var handlers = {
    onmousemove: function(event) {mouseEvent(event)},
    onmouseover: function(event) {mouseEvent(event)},
    onmouseout: function(event) {mouseEvent(event)},
    onmousedown: function(event) {bubbleEvent(event)},
    onmouseup: function(event) {bubbleEvent(event)},
    onclick: function(event) {bubbleEvent(event)},
    ondblclick: function(event) {bubbleEvent(event)}
  };
  
  TNT.Juic.browser.attachEvent(window, 'onload', function(event) {
  		for (name in handlers) {
        TNT.Juic.browser.attachEvent(document, name, handlers[name]);
      }
    });
  
  TNT.Juic.browser.attachEvent(window, 'onunload', function(event) {
  		for (name in handlers) {
        TNT.Juic.browser.detachEvent(document, name, handlers[name]);
      }
    });
  
}

// Register the window that included this script implicitly
TNT.Juic.Core.Widget._registerWindow(window);

/* vim: set expandtab tabstop=2 shiftwidth=2: */

