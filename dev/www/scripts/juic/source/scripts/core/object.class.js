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
 * TNT.Juic.Core.Object Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Base class for all JUIC objects. Object can talk to each other via the
 * basic event handling built into this core object.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core = {};

test_all = { '_all' : 0 };

TNT.Juic.Core.Object = TNT.declareClass({
  $class : 'TNT.Juic.Core.Object',

  /**
   * Constructor
   *
   * @access  public
   * @param   object    data          optional data map
   * @param   object    owner         optional owning Object
   * @return  object                  reference to constructed object
   */
  constructor : function(data, owner) {
    if (data) {
      this.set(data);
    }
    if (owner) {
      this._owner$ = owner;
    }
  },

  /**
   * The unique identifier for this object.
   * The '$' suffix means this property is never copied during cloning.
   * @access  private
   * @var     string                  unique identification string
   */
  _uniqueId$ : undefined,

  /**
   * The owner of this object.
   * The '$' suffix means this property is never copied during cloning.
   * @access  private
   * @var     object                  owning object
   */
  _owner$ : undefined,

  /**
   * The user assignable identifier for this object
   * @access  private
   * @var     string                  user identification string
   */
  _id : undefined,

  /**
   * Event handlers
   * @access	private
   * @var     array                   array of event handlers
   */
  _eventHandlers : null,

  /**
   * Cached property setter names
   * @access	private
   * @var     array                   static array of method names
   */
  _setters : [],

  /**
   * Cached property getter names
   * @access	private
   * @var     array                   static array of method names
   */
  _getters : [],

  /**
   * Return a clone of this object
   *
   * @access	public
   * @param   boolean   deep          deep clone (clone all descendants)
   * @return  object                  clone of object
   */
  clone : function(deep) {
    return TNT.clone(this, deep);
  },

  /**
   * Return the name of the object's class.
   *
   * @access	public
   * @return  string                  name of class
   */
  className : function(id) {
    return this.$class;
  },

  /**
   * Return the name of the object's super class.
   *
   * @access	public
   * @return  string                  name of super class
   */
  superClassName : function(id) {
    return this.$super ? this.$super.$class : undefined;
  },

  /**
   * Get the objects unique ID
   *
   * Every object has two id's, a unique id and an assigned id. This method
   * will return the object unique id. The unique id can not be altered once
   * request via this method and no two objects will be assigned the the same
   * unique id. This unique id can be passed to the {@link instance()} method
   * at any time to return the object assigned that id.
   *
   * Once assigned an ID the object will persist until explicitely destroyed or
   * the script terminates. It will not be garbage collected once done with as
   * calling {@link instance()} may need to return it at any time.
   *
   * @access	public
   * @return  string                  the objects unique id
   */
  uniqueId : function() {
    if (!this._uniqueId$) {
      // Assign a unqiue ID - done on request as no need to slow down the
      // creation of simple objects that do not need id persistance.
      var all = TNT.Juic.Core.Object._all;
      all['!' + (this._uniqueId$ = ('juic#' + all._all++))] = this;
    }
    return this._uniqueId$;
  },

  /**
   * Get the objects owner
   *
   * The owner property can only be changed by the owner.
   *
   * @access	public
   * @return  object                  the owning object (if owned)
   */
  owner : function() {
    return this._owner$;
  },

  /**
   * Return a reference to the instance of an object with the given id
   *
   * The id can either be the objects uniqueId as returned by
   * {@link uniqueId()} or the objects assigned id as returned by
   * {@link getId()}.
   *
   * When using the assigned id if more than one object are assigned the
   * same id then this method will return an array of objects with that
   * id.
   *
   * @access	public
   * @param   string      id          object id (user or unique)
   * @return  object                  reference to object with given id
   */
  instance : function(id) {
    return TNT.Juic.Core.Object._all['!' + id];
  },

  /**
   * Return the global reference to the object as a string
   *
   * This will cause the object to persist until explicitly destroyed or
   * the script terminates.
   *
   * @return  string                  string notation of the object
   */
  self : function() {
    return 'TNT.Juic.Core.Object._all[\'!' + this.uniqueId() + '\']';
  },

  /**
   * Set the object ID
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
   * @access	public
   * @param   string      id          valid HTML element ID
   * @return  boolean                 true if changed, false if unchanged
   */
  setId : function(id) {
    if (id == null || id == '') {
      id = undefined;
    }

    if (id != undefined && !id.match(/^[A-Za-z]([A-Za-z0-9-_:.]*)$/)) {
      return _invalidProperty('id', id, 'Invalid value');
    }

    if (id == this._id) {
      return false;
    }

    var n = '!' + this._id;

    // Remove this object from its original position
    if (TNT.classof(TNT.Juic.Core.Object._all[n]) == 'Array') {
      var newArray = new Array();
      var max_i = TNT.Juic.Core.Object._all[n].length;
      for (var i = 0; i < max_i; i++) {
        if (TNT.Juic.Core.Object._all[n][i] != this) {
          newArray[newArray.length] = TNT.Juic.Core.Object._all[n][i];
        }
      }
      if (newArray.length == 1) {
        TNT.Juic.Core.Object._all[n] = newArray[0];
      } else {
        TNT.Juic.Core.Object._all[n] = newArray;
      }
    } else if (TNT.Juic.Core.Object._all[n] == this) { 
      delete TNT.Juic.Core.Object._all[n];
    }

    // Change the id
    this._id = id;

    // Add to new position
    if (id != undefined) {
      n = '!' + this._id;
      if (TNT.classof(TNT.Juic.Core.Object._all[n]) == 'Array') {
        TNT.Juic.Core.Object._all[n][TNT.Juic.Core.Object._all[n].length] = this;
      } else if (TNT.Juic.Core.Object._all[n]) { 
        TNT.Juic.Core.Object._all[n] = [ TNT.Juic.Core.Object._all[n], this ];
      } else {
        TNT.Juic.Core.Object._all[n] = this;
      }
    }

    this._changedProperty('id');

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
   *
   * @access	public
   * @return  string                  HTML element ID
   */
  getId : function() {
    return this._id;
  },

  /**
   * Set a property on the object.
   *
   * If a 'set<property>' method exists it will be used, otherwise normal
   * assignment is used.
   *
   * If the method is called with a single object argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to set each pair in the map. The return value will be false if any any
   * property could not be set, however all properties are tried, there is no
   * way to determine which property failed from the return value.
   *
   * Due to the fact that certain names are not enumerable on an object due to
   * them being built in properties of the object this method will allow the
   * property name to be prefixed with a '!' sign which will be removed before
   * the name is used. This name clash prefix is hardly ever required.
   *
   * Protected properties (ie. those starting with an '_' or '$') can not be
   * set via this method.
   *
   * Direct assignment will call the {@link _changedProperty()} method if the
   * value changes. Assignment via a 'set<property>' method will expect that
   * method to call the {@link _onProperyChange()} method is it changed the
   * property.
   *
   * Property names are case sensitive !! However the 'set<property>' method
   * will uppercase the first character of the property inorder to create the
   * name of the method to set it. For example, a property called 'fooBar'
   * will look for a method called 'setFooBar()'.
   *
   * All set methods should return true if the properties value was altered
   * or false if the value was unaltered. If for some reason the property
   * can not be set to the given value (ie. the value is invalid for this
   * property then an 'Invalid Value' exception object should be thrown). 
   *
   * @access	public
   * @param   string       name       name of the property
   * @param   mixed        value      value of the new property
   * @return  boolean                 true if changed, false if unchanged
   */
  set : function(name, value) {
    if (arguments.length == 1) {
      if (typeof name == 'object') {
        var ret = false;
        for (var name in data) {
          if (this.set(name, data[name])) {
            ret = true;
          }
        }
        return ret;
      }

      throw 'set() - invalid arguments';
      return;
    }

    var c = name.charAt(0);
    if (c == '_' || c == '$') {
      return this._invalidProperty(name, value, 'Can not alter private property');
    }

    if (c == '!') {
      name = name.substr(1);
      c = name.charAt(0);
    }

    // If property contains a '.' then its a sub property change
    var split = name.match(/^([^\.]+)\.+(.+)$/);
    if (split) {

      var name1 = split[1];
      var name2 = split[2];

      // Cache the camel casing
      setter = this._setters[name1];
      if (!setter) {
        setter = this._setters[name1] = ('set_' + name1).camelCase();
      }

      if (typeof this[setter] == 'function') {
        return this[setter](name2, value);
      }

      if (typeof this[name1] != 'object') {
        return this._invalidProperty(name, value, 'Not a compound property');
      }

      if (this[name1][name2] != value) {
        this[name1][name2] = value;
        this._changedProperty(name);
        return true;
      }

    } else {

      // Cache the camel casing
      setter = this._setters[name];
      if (!setter) {
        setter = this._setters[name] = ('set_' + name).camelCase();
      }

      if (typeof this[setter] == 'function') {
        return this[setter](value);
      }

      if (this[name] != value) {
        this[name] = value;
        this._changedProperty(name);
        return true;
      }

    }

    return false;
  },

  /**
   * Get a property on the object.
   *
   * If a 'get<property>' method exists it will be used, otherwise normal
   * assignment is used.
   *
   * If the method is called with a single object argument then it is assumed
   * to be a map of {name : value} pairs and the method is called recursively
   * to get the value of each pair in the map.
   *
   * Due to the fact that certain names are not enumerable on an object due to
   * them being built in properties of the object this method will allow the
   * property name to be prefixed with a '!' sign which will be removed before
   * the name is used. This name clash prefix is hardly ever required.
   *
   * Protected properties (ie. those starting with an '_' or '$') can not be
   * got via this method.
   *
   * Property names are case sensitive !! However the 'get<property>' method
   * will uppercase the first character of the property inorder to create the
   * name of the method to get it. For example, a property called 'fooBar'
   * will look for a method called 'getFooBar()'.
   *
   * @access	public
   * @param   string       name       name of the property
   * @return  mixed                   value of the property (or undefined)
   */
  get : function(name) {
    if (typeof name == 'object') {
      for (var n in name) {
        name[n] = this.get(n);
      }
      return name;
    }

    var c = name.charAt(0);
    if (c == '_' || c == '$') {
      return undefined;
    }

    if (c == '!') {
      name = name.substr(1);
      c = name.charAt(0);
    }

    // Cache the camel casing
    getter = this._getters[name];
    if (!getter) {
      getter = this._getters[name] = ('get_' + name).camelCase();
    }

    if (typeof this[getter] == 'function') {
      return this[getter]();
    }

    return this[name];
  },

  /**
   * Throw a property error.
   *
   * This method should be used by 'set<property>' methods to throw an error
   * when it fails to set the property due to an invalid value or condition.
   *
   * The default method just formats up an error message, writes it to the
   * debug log and the throws it as an exception.
   *
   * The return value from this method should be used as the return value from
   * the 'set<property>()' method. It is normally never used as an error is
   * thrown, however derived class can choose not to throw and error and return
   * a value instead. It is assumed that the value will be < 0 as 0 means ok
   * and 1 means property changed.
   *
   * @access	public
   * @param   string       name       name of the property
   * @param   mixed        value      value of the new property
   * @param   string       error      reason property could not be set
   * @return  integer                 -1
   */
  _invalidProperty : function(name, value, reason) {
    TNT.debug('Failed to set property, this=', this, ', name=', name, ', value=', value, ', reason=', reason);
    var msg = this.className() + ' - failed to set "' + name + '": ' + reason; 
    throw msg;
    return -1
  },

  /**
   * Raise a property changed event.
   *
   * This method should be used by 'set<property>' methods to raise a property
   * change event when it successfully sets the property.
   *
   * The default method just formats up an event and calls raiseEvent().
   *
   * The return value from this method should be used as the return value from
   * the 'set<property>()' method. It is normally allways 1 indicating the
   * property was changed.
   *
   * This is automatically called by {@link set()} and should be called by any
   * 'set<property>()' method when the properties value changes.
   *
   * @access	public
   * @param   string       name       name of the property that changed
   * @return  boolean                 true
   */
  _changedProperty : function(name) {
    var event = {
                  'type' : 'propertychange',
                  'target' : this,
                  'propertyName' : name
                };

    this.raiseEvent('onpropertychange', event);

    return true;
  },

  /**
   * Calls a method after a specified time interval has elapsed.
   *
   * This method has the same effect as window.setTimeout except that the
   * function will be evaluated not as a global function but as a method
   * of the current object.
   *
   * @param   function    handler     method to call
   * @param   integer     delay       time interval in milliseconds.
   * @return  handle                  handle to pass to window.clearTimeout
   */
  timeout : function(handler, delay) {
    var self = this;
    var wrapper = function() { handler.call(self) };
    return window.setTimeout(wrapper, delay ? delay : 0);
  },

  /**
   * Attach an event handler to the object or sub object.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : handler} pairs and the method is called recursively
   * to attach each pair.
   *
   * @access	public
   * @param   string      name        name of event
   * @param   mixed       handler     the code/function to handle the event
   * @return  void
   */
  attachEventHandler : function(name, handler) {
    if (arguments.length == 1) {
      // assigning an array or single function
      if (typeof name == 'object') {
        for (var i in name) {
          if (name.substr(0, 1) = '!') {
            name = name.substr(1);
          }
          this.attachEventHandler(i, name[i]);
        }
      } 
      return;
    }

    if (this._eventHandlers) {
      if (this._eventHandles[name]) {
        if (this._eventHandlers[name].contains(handler)) {
          return;
        }
      } else {
        this._eventHandlers[name] = [handler];
      }
    } else {
      this._eventHandlers = { name : [handler] }
    }
  },
  
  /**
   * Remove an event handler from the object.
   *
   * If the method is called with no arguments then all event handlers are
   * removed.
   *
   * If the method is called with a single array argument then it is assumed
   * to be a map of {name : handler} pairs and the method is called recursively
   * to remove each pair.
   *
   * If the method is called with a single string argument then all handlers
   * for the named event are removed.
   *
   * @access	public
   * @param   string      name        name of event
   * @param   mixed       handler     the code/function to handle the event
   * @return  void
   */
  removeEventHandler : function(name, handler) {
    if (arguments.length == 0) {
      this._eventHandlers = null;
      return;
    }

    if (this._eventHandlers[name]) {
      this._eventHandlers[name].remove(handler);
    }
  },

  /**
   * Raise an event on the object.
   *
   * Events call any event handlers for the given event.
   *
   * @access	public
   * @param   string      name        event name
   * @param   object      event       event object
   * @return  void
   */
  raiseEvent : function(name, event) {
    // If a property change event is raised then taint the object as it or
    // one of its descendants has now been changed. This prevents each object
    // from having to actually trap property change events.
    if (name == 'onpropertychange') {
      this.taint(event);
    }

    if (this._eventHandlers) {
      var max_i = this._eventHandlers.length;
      for (var i = 0; i < max_i; i++) {
        if (this._eventHandlers[i].name == name) {
          var handler = this._eventHandlers[i].handler;
          if (typeof handler == 'function') {
            handler.call(this, event);
          } else if (typeof handler == 'string') {
            if (handler.substr(0, 1) == '@' && handler.substr(1) != name) {
              // Pass event to another handler on another? object
              this.raiseEvent(handler.substr(1), event);
            } else {
              // evaluate the string as javascript
              eval(handler);
            }
          }
        }
      }
    }
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
   * @access	public
   * @param   object      event       event that caused the taint (optional)
   * @return  void
   */
  taint : function(event) {
  },

  /**
   * Debug a method entry/exit.
   *
   * This should be at the entry and exit of a method to be debugged.
   *
   * Pairs must be maintained to preserve the indentation.
   *
   * Eg;
   *   exampleMethod : function(arg1, arg2) {
   *     this._debug(1, 'exampleMethod, arguments);
   *     var ret = ... method guts ...
   *     this._debug(0, 'exampleMethod, 'debug message');
   *
   *     this._debug(-1, 'exampleMethod, ret);
   *     return ret;
   *   },
   *
   * @access	public
   * @param   integer      type       debug type (1 enter, 0 inline, -1 exit)
   * @param   string       method     method name ('<class>::<method>)
   * @param   mixed        ...        arguments/return or data to dump
   * @return  void
   */
  _debug : function(type, method /*, ...*/) {
    if (type > 0) {
        // Method entry
        TNT.debug(this.className()+'::'+method+'(', (arguments.length > 2 ? arguments[2] : 'void'), ')');
    } else if (type < 0) {
        // Method exit
        TNT.debug(this.className()+'::'+method+'() : ret=', (arguments.length > 2 ? arguments[2] : 'void'));
    } else {
        TNT.debug(this.className()+'::'+method+'() : ', arguments);
    }
  },

  /**
   * Get a possibly dynamic value
   *
   * If the value is a function then it returns the result of calling that
   * function, otherwise the it returns the value unaltered.
   *
   * Unlike the {@link TNT::dynamicValue()} static method this one calls any
   * function as if it was a method of this object.
   *
   * @access  public
   * @param   mixed       value       dynamic value
   * @return  mixed                   resulting value
   */ 
  dynamicValue : function(value) {
    return (typeof value == 'function' ? value.call(this) : value);
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
 * Global object containing reference to all objects created
 * @access  private
 * @var     object                  global collection of objects
 */
TNT.Juic.Core.Object._all = { '_all' : 0 };

TNT.Juic.Core.Object.destroy_all = function() {
  var _all = TNT.Juic.Core.Object._all;
  for (var o in _all) {
    if (o != '_all') {
    }
  }
  TNT.Juic.Core.Object._all = { '_all' : 0 };
}

/* vim: set expandtab tabstop=2 shiftwidth=2: */

