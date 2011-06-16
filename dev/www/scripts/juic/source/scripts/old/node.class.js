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
 * TNT.Juic.Core.Node Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Generic base class for objects that participate in a tree like hierachy.
 *
 * This object provides a set of methods to allow object tree placement, with
 * methods to add children and search the tree hierachy.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Node = TNT.declareClass({
  $class : 'TNT.Juic.Core.Node',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function() {
    TNT.Juic.Core.Object.prototype.constructor.call(this);

    this._contents = {};
  },

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
   * The children of this node, children are stored as named items.
   * @access  private
   * @var     map                     node contents
   */
  _contents : null,

  /**
   * Cache of the concatenated contents as strings.
   * @access  private
   * @var     string                  concatenated contents
   */
  _contentsString : null,

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
   * Get the dirname and basename of a given path.
   *
   * If the path contains one of more '/' separators then an array is
   * returned with the first element being the path minus the last component
   * and the second element is the last component.
   *
   * If the path contains no '/' separators then the method returns false.
   *
   * For example, the path 'fe/fi/fo' would return Array('fe/fi', 'fo');
   *
   * @access  public
   * @param   string      path        path to split
   * @return  array                   array containing dirname and basename
   */
  basePath : function(path) {
    if (path.indexOf('/') == -1) {
      return false;
    }
    var split = path.match(/^(.+)\/+([^\/]+)$/);
    if (split) {
      split = [ split[1], split[2] ];
    }
    return split;
  },

  /**
   * Get the root and remainder of a given path.
   *
   * If the path contains one of more '/' separators then an array is
   * returned with the first element being the first component in the
   * path and the second element being the remainder.
   *
   * If the path contains no '/' separators then the method returns false.
   *
   * For example, the path 'fe/fi/fo' would return Array('fe', 'fi/fo');
   *
   * @access  public
   * @param   string      path        path to split
   * @return  array                   array containing root and remainder
   */
  rootPath : function(path) {
    if (path.indexOf('/') == -1) {
      return false;
    }
    var split = path.match(/^([^\/]+)\/+(.+)$/);
    if (split) {
      split = [ split[1], split[2] ];
    }
    return split;
  },

  /**
   * Get reference to the named content in the hierachy.
   *
   * The name can be a path to any object in the hierachy created by this
   * element and its content. The individual names are separated using the
   * '/' character. In keeping with standard filesystem notation the '.'
   * and '..' content names can be used to refer to the current and parent
   * levels in the hierachy.
   *
   * @access  public
   * @param   string      path        path to object in hierachy
   * @return  object                  reference to named object (or null)
   */
  contentByPath : function(path) {
    var content = null;

    var split = this.rootPath(path);

    if (split) {
      var dir = split[0];
      if (dir == '.') {
        content = this;
      } else if (dir == '..') {
        content = this._parent_;
      } else {
        content = this._contents[dir];
      }
      if (content && content instanceof TNT.Juic.Core.Node) { 
        content = content.contentByPath(split[1]);
      }
    } else {
      if (path == '.') {
        content = this;
      } else if (path == '..') {
        content = this._parent_;
      } else {
        content = this._contents[path];
      }
    }

    return content;
  },
  
  /**
   * Set the nodes content.
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

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Node) { 
        return content.setContent(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    if (this._contents[name] == value) {
      return false;
    }

    // do we already have some content of that name, if so an its an
    // object the orphan it.
    if (typeof this._contents[name] != 'undefined') {
      var oldValue = this._contents[name];

      if (typeof oldValue.$parent == 'object') {
        if (value instanceof TNT.Juic.Core.Node) { 
          value._parent_ = undefined;
        }
        delete this._contents[name];
      }
      value.$owner_ = undefined;
    }

    // If the new value is an object then parent it
    if (typeof value == 'object') {
      // If the new value is a node then parent it, if it already has a parent
      // then remove it from its original parent first.
      if (value instanceof TNT.Juic.Core.Node) { 
        if (value._parent_) {
          //TNT.debug('Removed child ' + value.uniqueId() + ' from parent ' + value._parent_.uniqueId());
          value._parent_.setContent(name, undefined);
        }
        //TNT.debug('Added child ' + value.uniqueId() + ' to parent ' + this.uniqueId());
        value._parent_ = this;
      } else {
        //TNT.debug('Added child of class ' + TNT.classof(value) + ' to parent ' + this.uniqueId());
      }
      value.$owner_ = this;
    } else {
      //TNT.debug('Added child of type ' + typeof value + ' to parent ' + this.uniqueId());
    }

    this._contents[name] = value;

    this._contentsString = null;

    this._changedProperty('content.' + name);

    return true;
  },
  
  /**
   * Get the nodes content
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
      if (!this._contentsString) {
        this._contentsString = '';
        for (var name in this._contents) {
          this._contentsString += this.dynamicValue(this._contents[name]);
        }
      }
      return this._contentsString;
    }

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Node) { 
        return content.getContent(split[1]);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    return this.dynamicValue(this._contents[name]);
  },

  /**
   * Set a property of the object or contained object.
   *
   * This is the same as the base object method only the name can be the
   * full path to the node in the hierachy to raise the event on, in the
   * format '<path>/eventname'.
   *
   * @access  public
   * @param   string       name       name of the property
   * @param   mixed        value      value of the new property
   * @return  boolean                 true if the propery was set
   */
  setProperty : function(name, value) {
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        return content.setProperty(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    return TNT.Juic.Core.Object.prototype.setProperty.call(this, name, value);
  },
  
  /**
   * Get a property of the object or contained object.
   *
   * This is the same as the base object method only the name can be the
   * full path to the node in the hierachy to raise the event on, in the
   * format '<path>/eventname'.
   *
   * @access  public
   * @param   string       name       name of the property
   * @return  boolean                 true if the propery was set
   */
  getProperty : function(name, value) {
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        return content.getProperty(split[1], value);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    return TNT.Juic.Core.Object.prototype.getProperty.call(this, name);
  },
  
  /**
   * Attach an event handler to the object or sub object.
   *
   * This is the same as the base object method only the name can be the
   * full path to the node in the hierachy to raise the event on, in the
   * format '<path>/eventname'.
   *
   * @access  public
   * @param   string      name        name of event
   * @param   mixed       handler     the code/function to handle the event
   * @return  void
   */
  attachEventHandler : function(name, handler) {
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        content.attachEventHandler(split[1], event);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    return TNT.Juic.Core.Object.prototype.attachEventHandler.call(this, name, handler);
  },
  
  /**
   * Remove an event handler from the object or contained object.
   *
   * This is the same as the base object method only the name can be the
   * full path to the node in the hierachy to raise the event on, in the
   * format '<path>/eventname'.
   *
   * @access  public
   * @param   string      name        name of event
   * @param   mixed       handler     the code/function to handle the event
   * @return  void
   */
  removeEventHandler : function(name, handler) {
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        content.removeEventHandler(split[1], event);
      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    return TNT.Juic.Core.Object.prototype.removeEventHandler.call(this, name, handler);
  },

  /**
   * Raise an event on the object or a contained object.
   *
   * This is the same as the base object method only the name can be the
   * full path to the node in the hierachy to raise the event on, in the
   * format '<path>/eventname'.
   *
   * It also allows for event bubbling. If this object has a parent object
   * then the raiseEvent() method will be called on the parent unless the
   * 'cancelBubble' property is set in the event.
   *
   * @access  public
   * @param   string      name        event name or path
   * @param   mixed       event       event object
   * @return  void
   */
  raiseEvent : function(name, event) {
 
    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        content.raiseEvent(split[1], event);

      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    TNT.Juic.Core.Object.prototype.raiseEvent.call(this, name, event);

    if (!event.cancelBubble &&
      this._parent_ && this._parent_ instanceof TNT.Juic.Core.Object) { 
      this._parent_.raiseEvent(name, event);
    }
  },

  /**
   * Descend an event on the object and all descendant objects.
   *
   * This the name can be the full path to the node in the hierachy to descend
   * the the event from, in the format '<path>/eventname'.
   *
   * If the object has no content then this is the same as {@link raiseEvent()},
   * however if the object has content then the event will descend by calling
   * the descendEvent() on all the objects children.
   *
   * @access  public
   * @param   string      name        event name or path
   * @param   mixed       event       event object
   * @return  void
   */
  descendEvent : function(name, event) {

    var split = this.basePath(name);
    if (split) {
      var content = this.contentByPath(split[0]);
      if (content && content instanceof TNT.Juic.Core.Object) { 
        content.descendEvent(split[1], event);

      }
      throw 'Unknown content: ' + split[0];
      return;
    }

    TNT.Juic.Core.Object.prototype.raiseEvent.call(this, name, event);

    if (!event.cancelDescent && this._content) {
      for (var c in this._contents) {
        var content = this.getContent(c.substr(1));
        if (content && content instanceof TNT.Juic.Core.Object) { 
          content.descendEvent(name, event);
        }
      }
    }
  },

  /**
   * Return the value of the object as a string.
   *
   * This is a native javascript method which can be called directly or
   * implicitely when using the object in a string context.
   *
   * The method returns the concatenated string values of the nodes contents.
   *
   * @access  public
   * @return  string                  string representation of object
   */
  toString : function() {
    return this.getContent();
  },

  /**
   * Taint the object.
   *
   * This base method does nothing, but it provides a hook for objects
   * to be tainted either automatically due to an 'onpropertychange'
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
    this._contentsString = null;
  },

  /**
   * Get internal value of a content item.
   *
   * This protected method allows derived classes to query the internal value
   * of a content item without knowing how it is stored in this object. Unlike
   * the getContent() method the raw value is returned not any dynamic value.
   *
   * This method does not allow the 'name' to be a path.
   *
   * @access  private
   * @param   string      name        name of content item
   * @return  mixed                   value of content item
   */
  _content : function(name) {
    return this._contents[name];
  },

  /**
   * Destructor
   *
   * @access  private
   */
  _destructor : function() {
  }

});

/* vim: set expandtab tabstop=2 shiftwidth=2: */

