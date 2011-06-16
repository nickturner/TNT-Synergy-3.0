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
 * TNT.Juic.UI.Widget Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Generic base class for all UI objects.
 *
 * It is assumed that there is a one to one mapping between a UI object and
 * the browser HTML DOM object that displays the UI object in the users browser.
 *
 * This object provides a set of methods to manipulate the HTML DOM, including
 * elements, stylesheets, and events. It provides methods to define an HTML
 * elements attributes, inline styles, css classes and event handlers, as
 * either static properties or dynamic behaviours.
 *
 * The toString() method will return the properly formatted HTML markup string,
 * which can be passed to the document.write() call or assigned to the an
 * elements innerHTML property.
 *
 * The link between the javascript object and it's DOM element is maintained
 * through the use of the unique object id.
 *
 * As the base building block of all elements this object is highly optimised
 * and efficient.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Widget = TNT.declareClass({
  $class : 'TNT.Juic.UI.Widget',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * @access  public
   * @param   object    data          optional data map
   * @param   object    owner         optional owning Object
   * @return  object                  reference to constructed object
   */
  constructor : function(data, owner) {
    TNT.Juic.Core.Object.prototype.constructor.call(this, data, owner);
  },

  /**
   * HTML element tag name
   * @access  private
   * @var     string                  HTML elements tag name
   */
  _tagName : 'span',
  
  /**
   * HTML element attributes
   * @access  private
   * @var     object                  hash of HTML elements attributes
   */
  _attributes : null,

  /**
   * HTML element inline CSS styles
   * @access  private
   * @var     object                  hash of HTML elements inline CSS styles
   */
  _styles : null,
  
  /**
   * HTML element CSS class names
   * @access  private
   * @var     object                  hash of HTML elements CSS class names
   */
  _classes : null,

  /**
   * HTML element CSS states
   * @access  private
   * @var     array                   array of HTML elements CSS states
   */
  _states : null,
  
  /**
   * Cache of the elements innerHTML
   * @access  private
   * @var     string                  HTML markup string
   */
  _innerHTML : null,

  /**
   * Cache of the elements outerHTML
   * @access  private
   * @var     string                  HTML markup string
   */
  _outerHTML : null,
  
  /**
   * Reference to document contain the realized HTML element
   * @access  private
   * @var     object                  HTML document element
   */
  _document : null,

  /**
   * Reference to actual HTML element
   * @access  private
   * @var     object                  HTML element
   */
  _element : null,

  /**
   * Set the tag name of the rendered outer HTML element.
   *
   * Any tagName is automatically converted to lowercase.
   *
   * Setting the tag name will also reset the element clearing all attributes, 
   * styles and content etc. previously set.
   *
   * @access  public
   * @param   string      tagName     valid HTML tag name
   * @return  boolean                 true if changed, false if unchanged
   */
  setTagName : function(tagName) {
    tagName = tagName.toLowerCase();

    if (this.tagName == tagName) {
      return false;
    }

    this._tagName = tagName;

    // Reset the element
    this._attributes = null;
    this._styles = null;
    this._classes = null;
    this._states = null;
    this._innerHTML = null;
    this._outerHTML = null;

    //this._changedProperty('tagName');

    return true;
  },
  
  /**
   * Get the tag name of the rendered outer HTML element.
   *
   * @access  public
   * @return  string                  the HTML tag name
   */
  getTagName : function() {
    return this._tagName;
  },
  
  /**
   * Set the widget ID
   *
   * Every object has two id's, a unique id and an assigned id. This method
   * will set the objects assigned id.
   *
   * The assigned id's may or may not be unique. If the assigned id is not
   * unique then the {@link instance()} method will return an array of all
   * the instances with that id.
   *
   * The ID must begin with a letter ([A-Za-z]) and may be followed by any
   * number of letters, digits ([0-9]), hyphens ('-'), underscores ('_'),
   * colons (':'), and periods ('.').
   *
   * To remove an assigned id then pass a value of null, undefined or an
   * empty string.
   *
   * The assigned ID is used as the ID of the outer HTML element used to render
   * this widget.
   *
   * @access	public
   * @param   string      id          valid HTML element ID
   * @return  boolean                 true if changed, false if unchanged
   */
  setId : function(id) {

    var element = this.element();

    if (!TNT.Juic.Core.Object.prototype.setId.call(this, id)) {
      return false;
    }

    this._outerHTML = null;

    if (element) {
      element.id = this.getId();
    }

    return true;
  },
  
  /**
   * Get the object ID
   *
   * Every object has two id's, a unique id and an assigned id. This method
   * will set the objects assigned id.
   *
   * The assigned id's may or may not be unique. If the assigned id is not
   * unique then the {@link instance()} method will return an array of all
   * the instances with that id.
   * @access  public
   * @return  string                  HTML element ID
   */
  getId : function() {
    return TNT.Juic.Core.Object.prototype.getId.call(this);
  },

  /**
   * Get an HTML attribute that does not correspond to a widget's property.
   *
   * If the method is called with no arguments then all attributes are cleared.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to set each pair.
   *
   * If the method is called with a single string argument then it is assumed
   * to be a HTML attribute markup string to pass and the method is called
   * recursively to set each attribute in the string.
   *
   * To clear an attribute then pass a value of null or undefined.
   *
   * All attributes are rendered in the opening tag of the control, regardless
   * of whether they are supported or not. Attributes that directly correspond
   * to properties on a widget or its derivatives should be altered by setting
   * that property. Setting the value here may have undesired effects.
   *
   * Dynamic values are allowed.
   *
   * @access  public
   * @param   string      name        valid HTML attribute name
   * @param   mixed       value       static/dynamic value for the attribute
   * @return  boolean                 true if changed, false if unchanged
   * @see     getAttribute()
   */
  setAttribute : function(name, value) {
    switch (arguments.length) {
      case 0:
        var ret = false;
        for (var i in this._attributes) {
          this._changedProperty('attribute.' + i.substr(1));
          ret = true;
        }
        this._attributes = null;
        return ret;

      case 1:
        switch (typeof name) {
          case 'string':
            name = TNT.splitAttrs(name);
            // drop through

          case 'object': 
            // assigning from an object hash
            var ret = false;
            for (var i in name) {
              if (this.setAttribute(i, name[i])) {
                ret = true;
              }
            }
            return ret;

          default:
            throw 'setAttribute() - invalid arguments';
            return -1;
        }

      default:
        name = name.toLowerCase();

        if (this._attributes && this._attributes[name] === value) {
          // No change in attributes value
          return false;
        }
    
        if (TNT.isValid(value)) {
          // Modify the named attribute
          if (!this._attributes) {
            this._attributes = {}
          }
          this._attributes[name] = value;
        } else {
          // Delete the named attribute
          if (this._attributes && this._attributes[name]) {
            destroy (this._attributes[name]);
          }
        }

        this._attributesMarkup = null;
        this._outerHTML = null;
    
        this._changedProperty('attribute.' + name);

        return true;
    }

  },
  
  /**
   * Set an HTML attribute that does not correspond to a widget's property.
   *
   * If the method is called with no arguments then the full HTML attribute
   * string is returned.
   *
   * All attributes are rendered in the opening tag of the control, regardless
   * of whether they are supported or not. Attributes that directly correspond
   * to properties on a widget or its derivatives should be obtained by getting
   * that property. Getting the value here may have undesired effects.
   *
   * @access  public
   * @param   string      name        valid HTML attribute name
   * @return  string                  HTML attribute value (or undefined)
   * @see     setAttribute()
   */
  getAttribute : function(name) {
    if (arguments.length == 0) {
      if (!this._attributesMarkup) {
        this._attributesMarkup = '';
        if (this._attributes) {
          for (var name in this._attributes)  {
            var value = this.getAttribute(name);
            if (TNT.isValid(value)) {
              this._attributesMarkup += ' ' + name + '="' + value + '"';
            }
          }
        }
      }
      return this._attributesMarkup;
    }

    name = name.toLowerCase();

    return this.dynamicValue(this._attributes[name]);
  },
  
  /**
   * Set an CSS style that does not correspond to a widget's property.
   *
   * If the method is called with no arguments then all styles are cleared.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to set each pair.
   *
   * If the method is called with a single string argument then it is assumed
   * to be a CSS style markup string to pass and the method is called
   * recursively to set each style in the string.
   *
   * To clear an style then pass a value of null or undefined.
   *
   * Dynamic values are allowed.
   *
   * @access  public
   * @param   string      name        valid CSS style name
   * @param   mixed       value       static/dynamic value for the style
   * @return  boolean                 true if changed, false if unchanged
   * @see     getStyle()
   */
  setStyle : function(name, value) {
    switch (arguments.length) {
      case 0:
        var ret = false;
        for (var i in this._styles) {
          this._changedProperty('style.' + i.substr(1));
          ret = true;
        }
        this._styles = null;
        return ret;

      case 1:
        switch (typeof name) {
          case 'string':
            name = TNT.splitCSS(name);
            // drop through

          case 'object': 
            // assigning from an object hash
            var ret = false;
            for (var i in name) {
              if (this.setStyle(i, name[i])) {
                ret = true;
              }
            }
            return ret;

          default:
            throw 'setStyle() - invalid arguments';
            return -1;
        }

      default:
        name = name.toLowerCase();

        if (this._styles && this._styles[name] === value) {
          // No change in styles value
          return false;
        }

        if (TNT.isValid(value)) {
          // Modify the named style
          if (!this._styles) {
            this._styles = {}
          }
          this._styles[name] = value;
        } else {
          // Delete the named style
          if (this._styles && this._styles[name]) {
            destroy (this._styles[name]);
          }
        }

        this._stylesMarkup = null;
        this._outerCSS = null;
    
        this._changedProperty('style.' + name);

        return true;
    }

  },
  
  /**
   * Set an CSS style that does not correspond to a widget's property.
   *
   * If the method is called with no arguments then the full CSS style
   * string is returned.
   *
   * @access  public
   * @param   string      name        valid CSS style name
   * @return  string                  CSS style value (or undefined)
   * @see     setStyle()
   */
  getStyle : function(name) {
    if (arguments.length == 0) {
      if (!this._stylesMarkup) {
        this._stylesMarkup = '';
        if (this._styles) {
          for (var name in this._styles)  {
            var value = this.getStyle(name);
            if (TNT.isValid(value)) {
              this._stylesMarkup += ' ' + name + ':' + value + ';';
            }
          }
        }
      }
      return this._stylesMarkup;
    }

    name = name.toLowerCase();

    return this.dynamicValue(this._styles[name]);
  },
  
  /**
   * Set the element CSS class value
   *
   * CSS class names are usually just names, however this gives the concept of
   * a name value pair to the name. This allows the names to represent different
   * values or presentation options.
   *
   * For example, setClass('layout', 'flow') will result is the real class name
   * 'layout-flow', but this can be quickly altered to 'layout-fixed' by a call
   * to setClass('layout-fixed'). Removal of the 'layout-???' class can be done
   * by calling setClass('layout', null). To add a simple class use a value of
   * '' and no suffix will be appended.
   *
   * If the method is called with no arguments then all classes are cleared.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to set each pair.
   *
   * If the method is called with a single string argument then it is assumed
   * to be a CSS class markup string to pass and the method is called
   * recursively to set each class in the string.
   *
   * To clear an class then pass a value of null or undefined.
   *
   * Dynamic values are allowed.
   *
   * @access  public
   * @param   string      name        valid CSS class name
   * @param   mixed       value       static/dynamic value for the class
   * @return  boolean                 true if changed, false if unchanged
   * @see     getClass(), setState(), getState()
   */
  setClass : function(name, value) {
    switch (arguments.length) {
      case 0:
        var ret = false;
        for (var i in this._classes) {
          this._changedProperty('class.' + i.substr(1));
          ret = true;
        }
        this._classes = null;
        return ret;

      case 1:
        switch (typeof name) {
          case 'string':
            var tmp = TNT.splitClassName(name);
            var classes = tmp[classes];
            var ret = false;
            for (var i = 0, max_i = classes.length; i < max_i; i++) {
              if (this.setClass(classes[i], true)) {
                ret = true;
              }
            }
            return ret;

          case 'object': 
            // assigning from an object hash
            var ret = false;
            for (var i in name) {
              if (this.setClass(i, name[i])) {
                ret = true;
              }
            }
            return ret;

          default:
            throw 'setClass() - invalid arguments';
            return -1;
        }

      default:
        name = name.toLowerCase();

        if (this._classes && this._classes[name] === value) {
          // No change in classes value
          return false;
        }

        if (TNT.isValid(value)) {
          // Modify the named class
          if (!this._classes) {
            this._classes = {}
          }
          this._classes[name] = value;
        } else {
          // Delete the named class
          if (this._classes && this._classes[name]) {
            destroy (this._classes[name]);
          }
        }

        this._classesMarkup = null;
        this._outerCSS = null;
    
        this._changedProperty('class.' + name);

        return true;
    }

  },
  
  /**
   * Get the elements CSS class value
   *
   * If the method is called with no arguments then the full CSS class string
   * is returned, this will include any psuedo states set with the method
   * {@link setState()}.
   *
   * @access  public
   * @param   string      name        valid CSS class name
   * @return  string                  CSS class value (or undefined)
   * @see     setClass(), setState(), getState()
   */
  getClass : function(name) {
    if (arguments.length == 0) {
      if (!this._classesMarkup) {
        var classes = null;
        if (this._classes) {
          classes = [];
          for (var name in this._classes)  {
            var value = this.getClass(name);
            if (TNT.isValid(value)) {
              if (value != '') {
                classes.push(name + '-' + value);
              } else {
                classes.push(name);
              }
            }
          }
        }
        this._classesMarkup = TNT.makeClassName(classes, this._states);
      }
      return this._classesMarkup;
    }

    name = name.toLowerCase();

    return this.dynamicValue(this._classes[name]);
  },
  
  /**
   * Set the CSS psuedo state of the widget.
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
   * mydiv-clicked-opened mydiv-opened-clicked'.
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
   * If the method is called with a single string argument then it is assumed
   * to be a CSS class markup string to pass and the method is called
   * recursively to set each class in the string.
   *
   * Dynamic values are NOT allowed.
   *
   * @access  public
   * @param   string      name        CSS state name
   * @param   boolean     value       boolean value for the state
   * @return  boolean                 true if changed, false if unchanged
   * @see     getState(), setClass(), getClass()
   */
  setState : function(name, value) {
    switch (arguments.length) {
      case 0:
        var ret = false;
        for (var i in this._states) {
          this._changedProperty('state.' + this._states[i]);
          ret = true;
        }
        this._states = null;
        return ret;

      case 1:
        switch (typeof name) {
          case 'string':
            var tmp = TNT.splitClassName(name);
            var states = tmp[states];
            var ret = false;
            for (var i = 0, max_i = states.length; i < max_i; i++) {
              if (this.setState(states[i], true)) {
                ret = true;
              }
            }
            return ret;

          case 'object': 
            // assigning from an object hash
            var ret = false;
            for (var i in name) {
              if (this.setState(i, name[i])) {
                ret = true;
              }
            }
            return ret;

          default:
            throw 'setState() - invalid arguments';
            return -1;
        }

      default:
        name = name.toLowerCase();

        if (!value) {
          // clear the state
          if (!this._states || !this._states.contains(name)) {
            // no change
            return false;
          }
          this._states.remove(name);
        } else {
          // set the state
          if (this._states && this._states.contains(name)) {
            // no change
            return false;
          }
          if (!this._states) {
            this._states = [ name ];
          } else {
            this._states._push(name);
          }
        }

        this._statesMarkup = null;
        this._outerCSS = null;
    
        this._changedProperty('state.' + name);

        return true;
    }
  },
  
  /**
   * Get the CSS psuedo state of the widget.
   *
   * If the method is called with no arguments then the full CSS class string
   * is returned, this will include any classes set with the {@link setClass()}
   * method.
   *
   * @access  public
   * @param   string      name        valid HTML attribute name
   * @return  boolean                 boolean value of the state
   * @see     setState(), setClass(), getClass()
   */
  getState : function(name) {
    if (arguments.length == 0) {
      return this.getClass();
    }

    return (this._states && this._states.contains(name));
  },

  /**
   * Return the id to use as the HTML DOM element id.
   *
   * This returns the value to assign to the ID property of the HTML DOM
   * element used to render this widget.
   *
   * The returned value will be that returned by the {@link getId()} method
   * unless it is null, in which case it will use that returned by the
   * {@link uniqueId()} method.
   *
   * @access  public
   * @return  string                  HTML DOM Id
   */
  domId : function() {
    var id = this.getId();
    return (id ? id : this.uniqueId());
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
  attachEventHandler : function(name, handler) {
    TNT.Juic.Core.Object.prototype.attachEventHandler.call(this, name, handler);
    this._refreshEventCaptures();
  },
  
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
  removeEventHandler : function(name, handler) {
    TNT.Juic.Core.Object.prototype.detachEventHandler.call(this, name, handler);
    this._refreshEventCaptures();
  },

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
  raiseEvent : function(name, event) {
    TNT.Juic.Core.Object.prototype.raiseEvent.call(this, name, event);
    return event.returnValue;
  },

  /**
   * Taint the object.
   *
   * This base method does nothing, but it provides a hook for objects
   * to be tainted either automatically due to an 'propertychange'
   * event being raised or manually by calling this method.
   *
   * Tainted objects should regard any cached information they have about
   * themselves to be invalid and to be regenerated when required from the
   * current objects properties.
   *
   * @access  public
   * @param   object      event       event that caused the taint (optional)
   * @return  void
   */
  taint : function(event) {
    TNT.Juic.Core.Object.prototype.taint.call(this, event);

    if (!event) {
      // Prob called directory or due to a refresh, so clear all cache
      this._stylesMarkup = null;
      this._statesMarkup = null;
      this._attributesMarkup = null;
      this._outerHTML = null;
      this._innerHTML = null;
    } else if (event.target == this) {
      // We changed, so clear our outer cache
      this._stylesMarkup = null;
      this._statesMarkup = null;
      this._attributesMarkup = null;
      this._outerHTML = null;
      if (event.type == 'layoutchange' ||
          (event.type == 'propertychange' &&
           event.propertyName.substr(0, 8) == 'content.')) {
        // Refresh the contents
        this._innerHTML = null;
      }
    } else {
      // Must have been a child that changed, so clear html cache
      this._outerHTML = null;
      this._innerHTML = null;
    }
  },
  
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
      element = window.document.getElementById(this.domId());
    }

/*
    if (!element) {
      TNT.debug('object has no DOM element associated with it');
    }
*/

    return element;
  },

  /**
   * Return the inner HTML for the object.
   *
   * By defaul the innerHTML is the concatenated markup of the contents as
   * returned by {@link getContent()} when called with no arguments.
   *
   * @access  public
   * @param   boolean     refresh     force refresh of any cached values
   * @return  string                  inner HTML
   */
  innerHTML : function(refresh) {
    return 'hi';
  },

  /**
   * Return the outer HTML for the object.
   *
   * The outerHTML is the HTML tag and elements.
   *
   * @access  public
   * @param   boolean     refresh     force refresh of any cached values
   * @return  string                  outer HTML
   */
  outerHTML : function(refresh) {
    if (!this._outerHTML || refresh) {
      //TNT.debug(this.className()+'::outerHTML() - generating cached HTML');

      var tagName = this.getTagName();
      if (!tagName) {
        // No tag means return the innerHTML only not wrapped in an outer tag
        this.outerHTML = this.innerHTML();

      } else {

        var attrs = this.getAttribute();
        attrs += ' style="' + this.getStyle() + '"';
        attrs += ' class="' + this.getClass() + '"';
        attrs += ' id="' + this.domId() + '"';
        attrs += ' juic="' + this.uniqueId() + '"';

        if (tagName == 'br' || tagName == 'img') {
          // Tag does not contain inner content
          this._outerHTML = '<' + tagName + ' ' + attrs + ' />';
        } else {
          // Tag does not contain inner content
          this._outerHTML = '<' + tagName + ' ' + attrs + '>' +
                             this.innerHTML() + 
                             '</' + tagName + '>';
        }
      }
    }

    return this._outerHTML;
  },

  /**
   * Refresh the HTML element in the browser.
   *
   * This just rewrites the element to the page.
   *
   * @access  public
   * @param   boolean     taint       taint the object before refreshing
   * @return  string                  objects HTML markup
   */
  refresh : function(taint) {
    var st = TNT.microsecs();
    TNT.debug(this.className()+'::refresh()');

    if (taint) {
      this.taint();
    }

    var element = this.element();
    if (element) {
      TNT.Juic.browser.setOuterHTML(element, this.toString());
    }

    TNT.debug(this.className()+'::refreshed in '+(TNT.microsecs() - st)+' ms');
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
    return this.outerHTML();
  },

  /**
   * Refresh the events captured on the element.
   *
   * This handles capturing any events requested by event handlers that can
   * not be captured at the document level because they do not bubble. This
   * is principally the following events;
   *    + onfocus
   *    + onblur
   *
   * The 'onfocus' and 'onblur' events will also be captured if the HTML
   * 'tabIndex' attribute is set to any value other than -1, and this will
   * cause the CSS state 'focused' to be handled (via {@link setState()}.
   *
   * @access  public
   * @return  void
   */
  _refreshEventCaptures : function() {
    var element = this.element();
    var captures = {};

    var max_i = this._eventHandlers.length;
    for (var i = 0; i < max_i; i++) {
      switch (this._eventHandlers[i].name) {
        case 'onfocus':
        case 'onblur':
          captures[this._eventHandlers[i].name] = true;
          break;
      }
    }

    var tabindex = this.getAttribute('tabindex');
    if (tabindex != undefined && tabindex != -1) {
      captures['onfocus'] = true;
      captures['onblur'] = true;
    }

    if (element) {
      // Remove any captures that were set and are now not set
      for (var i in this._captures) {
        if (!captures[i]) {
          alert('Releasing '+i);
          element[i] = null;
        }
      }

      // Add any captures that were not set and are now set
      for (var i in captures) {
        if (!this._captures[i]) {
          alert('Capturing '+i);
          element[i] = 'TNT.Juic.UI.Widget._event(this, event)'; 
        }
      }
    }

    this._captures = captures;
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

  },

  /**
   * Get internal value of an attribute.
   *
   * This protected method allows derived classes to query the internal value
   * of an attribute without knowing how it is stored in this object. Unlike the
   * getAttribute() method the raw value is returned not any dynamic value.
   *
   * This method does not allow the 'name' to be a path.
   */
  _attribute : function(name) {
    return this._attributes[name];
  },

  /**
   * Get internal value of a style.
   *
   * This protected method allows derived classes to query the internal value
   * of a style without knowing how it is stored in this object. Unlike the
   * getStyle() method the raw value is returned not any dynamic value.
   *
   * This method does not allow the 'name' to be a path.
   */
  _style : function(name) {
    return this._styles[name];
  },

  /**
   * Get internal value of a state.
   *
   * This protected method allows derived classes to query the internal value
   * of a state without knowing how it is stored in this object. Unlike the
   * getState() method the raw value is returned not any dynamic value.
   *
   * NB: Dynamic states are currently not supported !!!
   *
   * This method does not allow the 'name' to be a path.
   */
  _state : function(name) {
    var max_i = this._states.length;
    for (var i = 0; i < max_i; i++) {
      if (this._states[i] == name) {
        return true;
      }
    }
    return false;
  },

  /**
   * Raise a layout recalc event.
   *
   * This should be called whenever the dimensions of the object change.
   *
   * This is automatically called by {@link setStyle()} when a style effecting
   * the objects dimensions is altered.
   *
   * @access  public
   * @param   string       name       name of the state that changed
   * @return  void
   */
  _onLayoutChange : function(name) {
    var event = {
                  'type' : 'layoutchange',
                  'target' : this,
                  'propertyName' : name,
                  'returnValue' : true
                };

    this.descendEvent('onlayoutchange', event);
    
    if (typeof this._doStateChange == 'function') {
      this._doLayoutChange(event);
    }
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
  
    if (!type) {
      type = event.type;
    }
  
    // Fire the event handlers on the element which cause the event
    this.raiseEvent('on' + type, event);
  
    // Perform the default action for the event (if not cancelled)
    if (event.returnValue) {
      //this._refreshState(type, event);

      var fcn = ('_do' + type).camelCase();
      if (typeof this[fcn] == 'function') {
        this[fcn](event, element, type);
      }
    }
  
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


/* vim: set expandtab tabstop=2 shiftwidth=2: */

