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
 * TNT.Juic.Core.HTML Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Generic base class for HTML markup.
 *
 * This object provides a set of methods to manipulate the HTML DOM, including
 * elements, stylesheets, and events.
 *
 * The toString() method will return the properly formatted HTML markup string,
 * which can be passed to the document.write() call or assigned to the an
 * elements innerHTML property.
 *
 * The link between the javascript object and it's DOM element is maintained
 * through the use of the unique object id.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.HTML = TNT.declareClass({
  $class : 'TNT.Juic.Core.HTML',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function() {
    TNT.Juic.Core.Object.prototype.constructor.call(this);

    this._tagName = 'span';
    this._isContainer = true;

    this._attrs = new TNT.Juic.Core.Attrs(this);

    this._styles = new TNT.Juic.Core.Styles(this);

    this._contents = new TNT.Juic.Core.Contents(this);
  },

  /**
   * HTML element tag name
   * @access  private
   * @var     string                  HTML elements tag name
   */
  _tagName : 'span',
  
  /**
   * Hash map of HTML element attrs
   * @access  private
   * @var     object                  Hash object
   */
  _attrs : null,

  /**
   * Hash map of CSS styles
   * @access  private
   * @var     object                  Hash object
   */
  _styles : null,

  /**
   * Hash map of content items
   * @access  private
   * @var     object                  Hash object
   */
  _contents : null,
  
  /**
   * HTML element CSS class names
   * @access  private
   * @var     array                   HTML elements CSS class names
   */
  _classes : null,

  /**
   * HTML element CSS states
   * @access  private
   * @var     array                   HTML elements CSS states
   */
  _states : null,
  
  /**
   * Cache of the elements inline CSS states markup
   * @access  private
   * @var     string                  CSS class names
   */
  _statesMarkup : null,

  /**
   * Cache of the elements innerHTML
   * @access  private
   * @var     string                  HTML markup
   */
  _innerHTML : null,

  /**
   * Cache of the elements outerHTML
   * @access  private
   * @var     string                  HTML markup
   */
  _outerHTML : null,
  
  /**
   * Events captured on this element
   * @access  private
   * @var     map                     array of events captured
   */
  _captures : null,

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
/*
    var clone = TNT.Juic.Core.Object.prototype.constructor.clone(deep);
    clone.$owner = null;
    clone.$parent = null;
*/
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
   * Set the HTML element tag name
   *
   * This will also set the containment type of the element as returned
   * by {@link isContainer()}. IMG & BR tags can not contain content.
   *
   * Setting the tag name will also reset the element clearing all attrs, 
   * styles and content etc. previously set.
   *
   * @access  public
   * @param   string      tagName     valid HTML tag name
   * @return  boolean                 true if changed, false if unchanged
   */
  setTagName : function(tagName) {
/*
    tagName = tagName.toLowerCase();

    if (this.tagName == tagName) {
      return false;
    }

    this._tagName = tagName;

    this._isContainer = true;
    if (this._tagName == 'img' || this._tagName == 'br') {
      this._isContainer = false;
    }

    // Reset the element
    this._attrs.empty();
    this._styles.empty();
    this._contents.empty();

    this._states = [];
    this._classes = [];
    this._statesMarkup = null;

    this._innerHTML = null;
    this._outerHTML = null;

    //this._captures = {};

    //this._changedProperty('tagName');
*/

    return true;
  },
  
  /**
   * Get the HTML element tag name
   *
   * @access  public
   * @return  string                  the HTML tag name
   */
  getTagName : function() {
    return this._tagName;
  },
  
  /**
   * Set the HTML element ID
   *
   * @access  public
   * @param   string      id          valid HTML element ID
   * @return  boolean                 true if changed, false if unchanged
   */
  setId : function(id) {
/*
    if (!TNT.Juic.Core.Object.prototype.setId.call(id)) {
      return false;
    }

    this._outerHTML = null;
*/
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
   * Set the HTML element attribute value
   *
   * If the method is called with no arguments then all attrs are cleared.
   *
   * If the method is called with a single array or object argument then it
   * is assumed to be a map of {name : value} pairs and the method is called
   * recursively to set each pair.
   *
   * If the method is called with a single string argument then it is assumed
   * to be a HTML attribute markup string to pass and the method is called
   * recursively to set each attribute in the string.
   *
   * The name can contain a path to a content with the hierachy created by this
   * object and its content. For example the name 'label/title' will set the
   * attribute 'title' on the content object named 'label'.
   * 
   * @access  public
   * @param   string      name        valid HTML attribute name
   * @param   mixed       value       static/dynamic value for the attribute
   * @return  boolean                 true if changed, false if unchanged
   */
  setAttribute : function(name, value) {

/*
    if (arguments.length == 0) {
      var ret = false;
      for (var name in this._attrs.keys()) {
        //this._changedProperty('attribute.' + name);
        ret = true;
      }
      this._attrs.empty();
      return ret;
    }

    if (arguments.length == 1) {
      // assigning an array or single function
      if (typeof name == 'object') {
        var ret = false;
        for (var i in name) {
          if (this.setAttribute(i, name[i])) {
            ret = true;
          }
        }
        return ret;
      }

      if (typeof name == 'string') {
        throw 'setAttribute() - setting from attribute string not yet supported';
        return -1;
      }

      throw 'setAttribute() - invalid arguments';
      return -1;
    }

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.setAttribute(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    name = name.toLowerCase();
    if (name == 'id') {
      return this.setId(value);
    }

    if (name == 'style') {
      return this.setStyle(value);
    }

    if (name == 'class') {
      this._classes = [];
      if (value && value != '') {
        return this.addClass(value);
      } 
      return true;
    }

    var old = this._attrs.set(name, value);

    if (old == value) {
      // No change
      return false;
    }

    //this._changedProperty('attribute.' + name);

    this._outerHTML = null;
*/

    return true;
  },
  
  /**
   * Get the HTML element attribute value
   *
   * If the method is called with no arguments then the full HTML attribute
   * string is returned.
   *
   * The name can contain a path to a content with the hierachy created by this
   * element and its content. For example the name 'label/title' will get the
   * attribute 'title' on the content object named 'label'.
   * 
   * @access  public
   * @param   string      name        valid HTML attribute name
   * @return  string                  HTML attribute value (or undefined)
   */
  getAttribute : function(name) {

/*
    if (arguments.length == 0) {
      return this._attrs.toString();
    }

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.getAttribute(split[1]);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    name = name.toLowerCase();
    if (name == 'id') {
      return this.getId();
    }

    if (name == 'style') {
      return this.getStyle();
    }

    if (name == 'class') {
      var className = '';
      for (var i in this._classes) {
        className += this.dynamicValue(this._classes[i]) + ' ';
      }
      return className;
    }

    return this._attrs.get(name);
*/
  },
  
  /**
   * Set the HTML elements inline CSS style.
   *
   * Easy method to set inline CSS styles on the element. A call to try and
   * set the 'style' attribute will be broken up and redirected to this
   * method.
   *
   * If the method is called with no arguments then all styles are cleared.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to set each pair.
   *
   * If the method is called with a single string argument then it is assumed
   * to be a CSS markup string to pass and the method is called recursively
   * to set each style in the string.
   *
   * The name can contain a path to a content with the hierachy created by this
   * element and its content. For example the name 'label/width' will set the
   * style 'width' on the content object named 'label'.
   * 
   * @access  public
   * @param   string      name        valid CSS style name
   * @param   mixed       value       static/dynamic value for the style
   * @return  boolean                 true if successful
   */
  setStyle : function(name, value) {

/*
    if (arguments.length == 0) {
      var ret = false;
      for (var i in this._styles) {
        delete this._styles[i];
        //this._changedProperty('style.' + i.substr(1));
        ret = true;
      }
      return ret;
    }

    if (arguments.length == 1) {
      // assigning an array or single function
      if (typeof name == 'object') {
        var ret = false;
        for (var i in name) {
          if (this.setStyle(i, name[i])) {
            ret = true;
          }
        }
        return ret;
      }

      if (typeof name == 'string') {
        var map = {};
        var style;
        var styles = name.split(';');
        for (var i in styles) {
          if (styles[i].match(/^\s*$/)) {
            continue;
          }
          style = styles[i].match(/^\s*([\w-]+)\s*:\s*(.*)$/);
          if (style) {
            map[style[1]] = style[2];
          } else {
            throw 'setStyle() - "' + styles[i] + '" is not a valid CSS markup';
          }
        }
        
        return this.setStyle(map);
      }

      throw 'setStyle() - invalid arguments';
      return;
    }

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.setStyle(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }
*/

/*
    var old = this._styles.set(name, value);

    if (old == value) {
      // No change
      return false;
    }

    this._outerHTML = null;
*/

/*
    // If a dimensional style changes then a layout recalc event is triggered
    // on all children
    if (name.substr(0, 6) == 'border' ||
        name.substr(0, 6) == 'margin' ||
        name.substr(0, 7) == 'padding' ||
        name == 'height' || name == 'minHeight' || name == 'maxHeight' ||
        name == 'width' || name == 'minWidth' || name == 'maxWidth') {
      this._onLayoutChange('style.' + name);
    }

    this._changedProperty('style.' + name);
*/

    return true;
  },
  
  /**
   * Get the HTML elements inline CSS style.
   *
   * Easy method to get inline CSS styles on the element.
   *
   * If the method is called no arguments then the full CSS style string
   * is returned.
   *
   * The name can contain a path to a content with the hierachy created by this
   * element and its content. For example the name 'label/width' will get the
   * style 'width' on the content object named 'label'.
   * 
   * @access  public
   * @param   string      name        valid CSS style name
   * @return  string                  CSS style value (or undefined)
   */
  getStyle : function(name) {

/*
    if (arguments.length == 0) {
      return this._styles.toString();
    }

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.getStyle(split[1]);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    return this._styles.get(name);
*/
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

/*
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

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        return content.setContent(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

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
*/

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

/*
    if (!this._isContainer) {
      throw 'Not a container';
      return;
    }

    if (arguments.length == 0) {
      return this._contents.toString();
    }

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        return content.getContent(split[1]);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    return this._contents.get(name);
*/
  },
  
  /**
   * Add a CSS Class name to the list of class names.
   *
   * All HTML elements can have multiple class names, this method will add
   * a class name to that list.
   *
   * If the class name is already added then this method does nothing and
   * returns successful.
   *
   * Multiple arguments can be specified to add multiple class names at once.
   *
   * The name can be a path to a class with the hierachy created by this
   * element and its content. Separate the individual style names using
   * any none word character.
   *
   * The name can contain a path to a content with the hierachy created by this
   * element and its content. For example the name 'label/red' will set the
   * class name 'red' on the content object named 'label'.
   * 
   * @access  public
   * @param   string      ...         valid CSS class names
   * @return  boolean                 true if successful
   */
  addClass : function(name) {

/*
    var changed = false;

    var i, max_i = arguments.length;
    for (i = 0; i < max_i; i++) {
      var name = arguments[i];
      if (!name) {
        continue;
      }

      if (typeof name == 'string') {
        var split = this.basePath(name);
        if (split) {
          var content = this.contentByPath(split[0]);
          if (content && content instanceof TNT.Juic.Core.HTML) { 
            if (!content.addClass(split[1])) {
              return false;
            }
          } else {
            throw 'Unknown content: ' + split[0];
            return false;
          }
        }
      }

      var i, max_i = this._classes.length;
      for (i = 0; i < max_i; i++) {
        if (this._classes[i] == name) {
          break;
        }
      }
      if (i == max_i) {
        this._classes[i] = name;
        changed = true;
      }
    }

    if (changed) {
      this._statesMarkup = null;
      this._changedProperty('attribute.class');
    }
*/

    return true;
  },
  
  /**
   * Delete a CSS Class name from the list of class names.
   *
   * All HTML elements can have multiple class names, this method will delete
   * a class name from that list.
   *
   * If the class name is not in the list then this method does nothing and
   * returns successful.
   *
   * Multiple arguments can be specified to delete multiple class names at once.
   *
   * The name can contain a path to a content with the hierachy created by this
   * element and its content. For example the name 'label/red' will get the
   * class name 'red' on the content object named 'label'.
   * 
   * @access  public
   * @param   string      ...         valid CSS class names
   * @return  boolean                 true if successful
   */
  delClass : function(name) {
    var changed = false;

/*
    var i, max_i = arguments.length;
    for (i = 0; i < max_i; i++) {
      var name = arguments[i];

      var split = this.basePath(name);
      if (split) {
        var content = this.contentByPath(split[0]);
        if (content && content instanceof TNT.Juic.Core.HTML) { 
          if (!content.delClass(split[1])) {
            return false;
          }
        } else {
          throw 'Unknown content: ' + split[0];
          return false;
        }
        continue;
      }
      var max_i = this._classes.length;
      for (var i = 0; i < max_i; i++) {
        if (this._classes[i] == name) {
          delete this._classes[i];
          changed = true;
        }
      }
    }

    if (changed) {
      this._statesMarkup = null;
      //this._changedProperty('attribute.class');
    }
*/

    return true;
  },
  
  /**
   * Set a CSS Class name.
   *
   * This is an alias for {@link addClass()}.
   * 
   * @access  public
   * @param   string      ...         valid CSS class names
   * @return  boolean                 true if successful
   */
  setClass : function(name) {
    return this.addClass(name);
  },

  /**
   * Get a list of CSS Class names.
   *
   * This returns a space separated list of all the class names.
   * 
   * @access  public
   * @return  string                  class names
   */
  getClass : function(name) {
    return this._classes.join(' ');
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

/*
    if (arguments.length == 0) {
      var ret = false;
      for (var i in this._states) {
        var name = this._states[i];
        delete this._states[i];
        this._onStateChange(name);
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

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.setState(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

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
    this._outerHTML = null;

    this._onStateChange(name);
*/

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
/*
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

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.HTML) { 
        return content.getState(split[1]);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    var max_i = this._states.length;
    for (var i = 0; i < max_i; i++) {
      if (this._states[i] == name) {
        return true;
      }
    }
*/

    return false;
  },

  /**
   * Set the position of the control on the page.
   *
   * This will absolutely or relatively position the control on the page.
   *
   * To return to default position (inline) call the method with no arguments
   * or set both the top and left positions to either '' or 'auto'.
   *
   * Pass any value as undefined to leave it unchanged.
   *
   * @access	public
   * @param   mixed       top         static/dynamic value for the top edge
   * @param   mixed       left        static/dynamic value for the left edge
   * @param   boolean     relative    static/dynamic value for the positioning
   * @return  boolean                 true if changed, false if unchanged
   */
  setPos : function(top, left, relative) {
/*
    TNT.debug('setPos(', top, ',', left, ',', relative, ')');

    if (arguments.length == 0) {
      if (this.setStyle('position', undefined) ||
          this.setStyle('top', undefined) ||
          this.setStyle('left', undefined)) {
        this._changedProperty('position');
        return true;
      }
      return false;
    }

    var changed = true;
 
    if (typeof(top) != 'undefined' && this.setStyle('top', top)) {
      changed = true;
    }

    if (typeof(left) != 'undefined' && this.setStyle('left', left)) {
      changed = true;
    }

    if (changed) {
      var top = this.getStyle('top');
      var left = this.getStyle('left');
      if (top != undefined && top != '' && top != 'auto' &&
          left != undefined && left != '' && left != 'auto') {
        this.setStyle('position', relative ? 'relative' : 'absolute');
      } else {
        this.setStyle('position', 'static');
      }

      this._changedProperty('position');

      return true;
    }

*/
    return false;
  },
  
  /**
   * Get the position of the control on the page.
   *
   * This is the controls absolute position on the page.
   *
   * The method returns an object with the following properties;
   *   'top' : the top coordinate
   *   'left' : the left coordinate
   *   'relative' : true if the positioning is relative to parents position
   *
   * @access	public
   * @return  object                  object with properties holding position
   */
  getPos : function() {
    return { 'top' : this.getStyle('top'),
             'left' : this.getStyle('left'),
             'relative' : (this.getStyle('position') == 'relative') };
  },
  
  /**
   * Set the size of the control on the page.
   *
   * This is the size of the control using box-sizing, ie the size of the
   * area that the control takes including any borders and padding.
   *
   * To return to default size call the method with no arguments.
   *
   * Pass any value as undefined to leave it unchanged.
   *
   * For an HTML Span element to have size in IE it must also be given layout
   * this is done by setting the zoom style to 1.00 if it not already set or
   * is set to 'normal'. Manually changing the zoom style can override this
   * automatic value and prevent the widget from display at the given size.
   *
   * @access	public
   * @param   string    width         valid CSS length for width
   * @param   string    height        valid CSS length for height
   * @return  boolean                 true if changed, false if unchanged
   */
  setDim : function(width, height) {

/*
    if (arguments.length == 0) {
      var zoom = this.getStyle('zoom');
      if (this.setStyle('width', undefined) ||
          this.setStyle('height', undefined) ||
          (zoom == '1.00' ? this.setStyle('zoom', undefined) : true)) {
        this._changedProperty('dimensions');
        return true;
      }
      return false;
    }

    var changed = true;
 
    if (typeof(width) != 'undefined' && this.setStyle('width', width)) {
      changed = true;
    }

    if (typeof(height) != 'undefined' && this.setStyle('height', height)) {
      changed = true;
    }

    if (changed) {
      var width = this.getStyle('width');
      var height = this.getStyle('height');
      var zoom = this.getStyle('zoom');
      if (width != undefined && width != '' && width != 'auto' &&
          height != undefined && height != '' && height != 'auto') {
        if (zoom == undefined || zoom == '' || zoom == 'normal') {
          this.setStyle('zoom', '1.00');
        }
      } else {
        if (zoom == '1.00') {
          this.setStyle('zoom', undefined);
        }
      }
      this._changedProperty('dimension');
      return true;
    }
*/

    return false;
  },
  
  /**
   * Get the size of the control on the page.
   *
   * This is the size of the control using box-sizing, ie the size of the
   * area that the control takes including any borders and padding.
   *
   * @access	public
   * @return  object                  { 'top' : <top> , 'left' : <left> }
   */
  getDim : function(width, height) {
    return { 'width' : this.getStyle('width'), 'height' : this.getStyle('height') };
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
/*
    TNT.Juic.Core.Object.prototype.taint.call(this, event);

    if (!event) {
      // Prob called directory or due to a refresh, so clear all cache
      this._statesMarkup = null;
      this._outerHTML = null;
      this._innerHTML = null;
    } else if (event.target == this) {
      // We changed, so clear our outer cache
      this._statesMarkup = null;
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
*/
  },
  
  /**
   * Return a reference to the actual HTML document containing this Object
   *
   * @access  public
   * @return  object                  reference to HTML DOM document
   */
  document : function() {
/*
    if (!this._document) {
      // Search all documents
      try {
        this._document = window.document;
      }
      catch(e) {
      }
    }
*/
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
/*
    var element = null;
    if (window.document) {
      var id = this._id ? this._id : this.uniqueId();
      element = window.document.getElementById(id);
    }
*/

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
/*
    if (!this._isContainer) {
        return null;
    }
    if (!this._innerHTML || refresh) {
      //TNT.debug(this.className()+'::innerHTML() - generating cached HTML');
      this._innerHTML = this.getContent();
    }
*/
    return this._innerHTML;
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
/*
    if (!this._outerHTML || refresh) {
      //TNT.debug(this.className()+'::outerHTML() - generating cached HTML');
      if (!this._tagName) {
        // No tag means return the innerHTML only not wrapped in an outer tag
*/
        this.outerHTML = this.innerHTML();
/*
      } else {
        var attrs = '';
 
        for (var name in this._attrs) {
          if (name != '!class' && name.substr(0, 3) != '!on') {
            name = name.substr(1);
            var value = this.getAttribute(name);
            if (name == 'class') {
              var state = this.getState();
              if (state) {
                if (TNT.isValid(value)) {
                  value += ' ' + state;
                } else {
                  value = state;
                }
              }
            }
            if (TNT.isValid(value)) {
              attrs += ' ' + name + '="' + value + '"';
            }
          }
        }

        for (var i in this._captures) {
          attrs += ' ' + i + '="TNT.Juic.Core.HTML._event(this, event);"'; 
        }

        attrs += ' class="' + (this.getAttribute('class') || '') + ' ' + (this.getState() || '') + '"';
 
        var id = this._id ? this._id : this.uniqueId();

        attrs += ' id="' + id + '" juic="' + this.className() + '"';

        var style = this.getStyle();
        if (style) {
           attrs += ' style="' + style + '"';
        }

        if (this._isContainer) {
            this._outerHTML = '<' + this._tagName + ' ' + attrs + '>' +
                           this.innerHTML() +
                           '</' + this._tagName + '>';
        } else {
            this._outerHTML = '<' + this._tagName + ' ' + attrs + ' />';
        }
      }
    }
*/
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
/*
    var st = TNT.microsecs();
    TNT.debug(this.className()+'::refresh()');

    if (taint) {
      this.taint();
    }

    var element = this.element();
    if (element) {
      element.innerHTML = this.toString();
    }

    TNT.debug(this.className()+'::refreshed in '+(TNT.microsecs() - st)+' seconds');
*/
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
/*
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
          element[i] = 'TNT.Juic.Core.HTML._event(this, event)'; 
        }
      }
    }

    this._captures = captures;
*/
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
   * Get internal value of an attribute.
   *
   * This protected method allows derived classes to query the internal value
   * of an attribute without knowing how it is stored in this object. Unlike the
   * getAttribute() method the raw value is returned not any dynamic value.
   *
   * This method does not allow the 'name' to be a path.
   */
  _attribute : function(name) {
    return this._attrs[name];
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
/*
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
TNT.Juic.Core.HTML._registerWindow = function(window) {

  // Bubble the event up the DOM raising events on all JUIC objects
  function bubbleEvent(event) {
    var e = event.srcElement;
    while (e) {
      if (e.attrs && e.attrs['juic']) {
        TNT.Juic.Core.HTML._event(e, event); 
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
        TNT.Juic.Core.HTML._event(targets[i], event, 'mouseout');
      }
    }
  
    // Send mouse in to all element we not already sent one to
    for (i in t) {
      if (!targets[i]) {
        TNT.Juic.Core.HTML._event(t[i], event, 'mouseover');
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
TNT.Juic.Core.HTML._registerWindow(window);

/* vim: set expandtab tabstop=2 shiftwidth=2: */

