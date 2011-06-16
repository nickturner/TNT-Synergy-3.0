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
 * TNT.Juic.Core.ObjectCollection Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents the collection of objects.
 * 
 * A Collection is an array of objects which will relate each object with an
 * parent object.
 *
 * To add objects, you can use either the {@link add()} method or the
 * {@addAt()} if you want to add objects at a specific location in the
 * collection. To add multiple objects use the {@link addMany()} method.
 *
 * To remove objects, you can use either the {@link remove()} method or the
 * {@link removeAt()} if you want to remove an object at a specific location
 * in the collection. To remove multiple objects use the {@link removeMany()}
 * method. The {@link clear()} method will remove all objects.
 *
 * In addition to methods and properties for adding and removing object, the
 * Collection also provides methods to find an object. The {@link contains()}
 * method enables you to determine whether an object is in the collection. The
 * {@link indexOf()} method will tell you where the object is located in the
 * collection.
 *
 * @package TNT.Juic
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.Core.ObjectCollection = TNT.declareClass({
  $class : 'TNT.Juic.Core.ObjectCollection',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   *
   * @access  public
   * @param   object      owner       Object that owns the collection
   * @param   class       type        Class of objects to be stored (optional)
   * @return  object                  reference to constructed object
   */
  constructor : function(owner, type) {
    if (!(owner instanceof TNT.Juic.Core.Object)) {
      throw 'TNT.Juic.Core.Collection() - Owner must be an instance of TNT.JUIC.Core.Object';
      return;
    }

    if (!(type instanceof Function)) {
      throw 'TNT.Juic.Core.Collection() - type must be valid class';
      return;
    }

    TNT.Juic.Core.Object.prototype.constructor.call(this, null, owner);

    this._owner$ = owner;
    this._type = type;
    this._objects = [];

  },

  /**
   * The class type of objects in the collection. Objects must be an instance
   * of this type to be stored.
   * @access  private
   * @var     function                class constructor function
   */
  _type : null,

  /**
   * The actual array of objects in the collection
   * @access  private
   * @var     array                   array of objects
   */
  _objects : null,

  /**
   * Monitor collection changes.
   *
   * This method will be called every time an object is added or removed to or
   * from the collection.
   *
   * @access  public
   * @param   object      obj         object added or removed
   * @param   boolean     add         true if added, false if removed
   * @return  void
   */
  _changed : function(add, obj) {
    if (add) {
      // Object was added
      obj._collection$ = this;
      obj._owner$ = this._owner$;

      //this._owner$.raiseEvent('addChild', {'child' : obj});
      //obj.raiseEvent('adopted', {'owner' : $this._owner$});

    } else {
      // Object was removed
      obj._collection$ = null;
      obj._owner$ = null;

      //this._owner$.raiseEvent('delChild', {'child' : obj});
      //obj.raiseEvent('orphaned', {'owner' : $this._owner$});
    }
  },

  /**
   * Adds an object to the collection.
   *
   * If the method is called with a single argument which is an object of the
   * class specified by the Type property then that object is added.
   *
   * Otherwise a new object of the class specified by the Type property is
   * created and the arguments are passed to the objects constructor to
   * initialise the object. The new object is then added.
   *
   * To add multiple objects use the {@link addMany()} method.
   * To add an object at a specific index use the {@link addAt()} method.
   *
   * The method returns the object added.
   *
   * @access  public
   * @param   mixed       o           object to add
   * @return  object                  object added
   * @see     addMany(), addAt()
   */
  add : function(o) {
    if (arguments.length != 1 || !(o instanceof this._type)) {
      o = TNT.vnew(TNT.classOf(this._type), arguments);
    }

    this._objects.push(o);

    this._changed(true, o);

    return o;
  },

  /**
   * Adds multiple objects to the collection.
   *
   * This method just calls the {@link add()} method for each value in the
   * specified array.
   *
   * To add a single objects use the {@link add()} method.
   * To add an object at a specific index use the {@link addAt()} method.
   *
   * The method returns an array of the objects added.
   *
   * @access  public
   * @param   array       a           array of objects to add
   * @return  array                   array of objects added
   * @see     add(), addAt()
   */
  addMany : function(a) {
    var ret = [];
    for (var i = 0, max_i = a.length; i < max_i; i++) {
      ret[i] = this.add(a[i]);
    }
    return ret;
  },

  /**
   * Add an object in the collection at the given index.
   *
   * If the method is called with a single argument which is an object of the
   * class specified by the Type property then that object is inserted at the
   * index specified.
   *
   * Otherwise a new object of the class specified by the Type property is
   * created and the arguments are passed to the objects constructor to
   * initialise the object. The new object is then inserted at the index
   * specified.
   *
   * To add a single objects use the {@link add()} method.
   * To add multiple objects use the {@link addMany()} method.
   *
   * The method returns the object inserted.
   *
   * If the index is out of range and exception is thrown.
   *
   * @access  public
   * @param   integer     i           index at which to insert the object
   * @param   mixed       o           object to insert
   * @return  object                  object added
   * @see     add(), addMany()
   */
  addAt : function(i, o) {
    if (i < 0 || i > this._objects.length) {
      throw 'TNT.Juic.Core.Collection.addAt() - Index out of range';
      return null;
    }

    if (arguments.length != 2 || !(o instanceof this._type)) {
      o = TNT.vnew(TNT.classOf(this._type), arguments);
    }

    this._objects.insertAt(i, o);

    this._changed(true, o);

    return o;
  },

  /**
   * Removes an object from the collection.
   *
   * Removes the first occurrance of the object as found in the collection.
   *
   * When you remove an object from the collection, the indexes change for
   * subsequent objects in the collection. You can use this method to remove
   * a specific object from the collection by specifying the actual object to
   * remove.
   *
   * To remove multiple objects use the {@link removeMany()} method.
   * To remove an object at a specific index use the {@link removeAt()} method.
   * To remove all objects from the collection use the {@link clear()} method.
   *
   * If the object is not in the collection then null is returned.
   *
   * @access  public
   * @param   object      o           object to remove from the collection
   * @return  object                  removed object (or null)
   * @see     removeMany(), removeAt(), clear()
   */ 
  remove : function(o) {
    var ret = this._objects.remove(o);
    if (ret) {
      this._changed(false, ret);
    }
    return ret;
  },
  
  /**
   * Removes multiple objects to the collection.
   *
   * This method just calls the {@link remove()} method for each value in the
   * specified array.
   *
   * The method returns an array of the objects removed.
   *
   * When you remove an object from the collection, the indexes change for
   * subsequent objects in the collection. You can use this method to remove
   * a specific object from the collection by specifying the actual object to
   * remove.
   *
   * To remove a single object use the {@link remove()} method.
   * To remove an object at a specific index use the {@link removeAt()} method.
   * To remove all objects from the collection use the {@link clear()} method.
   *
   * If the object is not in the collection then null is entered into the
   * array returned.
   *
   *
   * @access  public
   * @param   array       a           array of objects to remove
   * @return  array                   array of objects removed
   * @see     remove(), removeAt(), clear()
   */ 
  removeMany : function(o) {
    var ret = [];
    for (var i = 0, max_i = a.length; i < max_i; i++) {
      ret[i] = this.remove(a[i]);
    }
    return ret;
  },
  
  /**
   * Removes the object at the specified index within the collection.
   *
   * When you remove an object from the collection, the indexes change for
   * subsequent objects in the collection. You can use this method to remove
   * a specific object from the collection by specifying the actual object to
   * remove.
   *
   * To remove a single object use the {@link remove()} method.
   * To remove multiple objects use the {@link removeMany()} method.
   * To remove all objects from the collection use the {@link clear()} method.
   *
   * If the index is out of range and exception is thrown.
   *
   * @access  public
   * @param   integer     i           index at which to remove the object
   * @return  object                  removed object
   */ 
  removeAt : function(i) {
    if (i < 0 || i >= this._objects.length) {
      throw 'TNT.Juic.Core.Collection.removeAt() - Index out of range';
      return null;
    }
    var ret = this._objects.removeAt(i);
    if (ret) {
      this._changed(false, ret);
    }
    return ret;
  },
  
  /**
   * Removes all objects from the collection.
   *
   * This method will remove all the objects from the collection.
   *
   * To remove a single object use the {@link remove()} method.
   * To remove multiple objects use the {@link removeMany()} method.
   * To remove an object at a specific index use the {@link removeAt()} method.
   *
   * @access  public
   * @param   array       a           array of objects to add
   * @return  array                   array of objects added
   */
  clear : function(a) {
    var o = this._objects;
    var l = this._objects.length;
    for (var i = 0; i < max_i; i++) {
      this._changed(false, o[i]);
    }
    this._objects.length = 0;
  },

  /**
   * Determines whether the specified object is located in the collection.
   * 
   * The method enables you to determine whether an object is a member of the
   * collection. To determine the position of the object in the collection, use
   * the {@link indexOf()} method.
   *
   * @access  public
   * @param   object      o           object to see if is in the collection
   * @return  boolean                 true if the object is contained
   */ 
  contains : function(o) {
    return this._objects.contains(o);
  },

  /**
   * Returns the index within the collection of the specified object.
   * 
   * The method enables you to determine the position of an object in the
   * collection. To simply determine whether an object is contained in the
   * collection, use the {@link contains()} method.
   *
   * If the object is not located in the collection, the return value -1.
   *
   * @access  public
   * @param   object      o           object to locate in the collection
   * @return  integer                 zero based index of object, or -1
   */ 
  indexOf : function(o) {
    return this._objects.indexOf(o);
  },

  /**
   * Returns the number of objects in the collection.
   *
   *
   * @access  public
   * @return  integer                 number of objects in the collection
   */
  count : function() {
    return this._objects.length;
  },
  
  /**
   * Set a given object in the collection.
   *
   * This method will throw an exception if the index is out of range.
   *
   * This can be used to alter the object stored at a given location. If the
   * object is altered then the {@link _changed()} method is called for both
   * the old and new objects.
   *
   * @access  public
   * @param   integer     i           index of object to set
   * @param   object      o           object to set
   * @return  object                  the object at the given index
   */
  setItem : function(i, o) {
    if (i < 0 || i >= this._objects.length) {
      throw 'TNT.Juic.Core.Collection.setItem() - Index out of range';
    }
    if (!(o instanceof this._type)) {
      throw 'TNT.Juic.Core.Collection.setItem() - object must be of type '.TNT.classof(this._type);
    }
    var old = this._objects[i];
    if (o != old) {
      this._changed(false, old);
      this._changed(true, o);
    }
    return this._objects[i] = o;
  },
  
  /**
   * Get a given object in the collection.
   *
   * This method will throw an exception if the index is out of range.
   *
   * @access  public
   * @param   integer     i           index of object to return
   * @return  object                  the object at the given index
   */
  getItem : function(i) {
    if (i < 0 || i >= this.length) {
      throw 'TNT.Juic.Core.Collection.getItem() - Index out of range';
    }
    return this._objects[i];
  },
  
  /**
   * Returns a given object in the collection.
   *
   * This is an alias to the {@link getItem()} method.
   *
   * This method will throw an exception if the index is out of range.
   *
   * @access  public
   * @param   integer     i           index of object to return
   * @return  object                  the object at the given index
   */
  item : function(i) {
    return this.getItem(i);
  },

  /**
   * Return boolean if object is first in the collection
   *
   * This is an efficient way of doing;
   *   'indexOf(o) === 0'
   *
   * @access  public
   * @param   object    o             object to check
   * @return  boolean                 true if object is first in the collection
   */
  isFirst : function(o) {
    return (this._objects.length > 0 && this._objects[0] === o);
  },

  /**
   * Return boolean if object is last in the collection
   *
   * This is an efficient way of doing;
   *   'indexOf(o) === (count() - 1)'
   *
   * @access  public
   * @param   object    o             object to check
   * @return  boolean                 true if object is last in the collection
   */
  isLast : function(o) {
    return (this._objects.length > 0 && this._objects[this._objects.length - 1] === o);
  },

  /**
   * Return the first object in the collection
   *
   * This is the same as doing;
   *   'item(0)'
   *
   * @access  public
   * @return  object                  first object (or null)
   */
  first : function() {
    return (this._nodes.length > 0 ? this._nodes[0] : null);
  },

  /**
   * Return the last node in the collection
   *
   * This is the same as doing;
   *   'item(count() - 1)'
   *
   * @access  public
   * @return  object                 last node (or null)
   */
  last : function() {
    return (this._objects.length > 0 ? this._objects[this._objects.length - 1] : null);
  },

  /**
   * Return the next object to the specified object in the collection
   *
   * @access  public
   * @param   object    o             object to get neighbour
   * @return  object                  next object (or null)
   */
  next : function(o) {
    var i = this.indexOf(o);
    return (i >= 0 && i <= (this._objects.length - 2) ? this._objects[i + 1] : null);
  },

  /**
   * Return the prev object to the specified object in the collection
   *
   * @access  public
   * @param   object    o             object to get neighbour
   * @return  object                  previous object (or null)
   */
  prev : function(o) {
    var i = this.indexOf(o);
    return (i >= 1 ? this._objects[i - 1] : null);
  },

  /**
   * Set the internal pointer of an collection to its first element.
   *
   * Every collection has an internal pointer to its 'current' element, which
   * is initialized to the first entry in the collection. 
   *
   * This method resets the internal pointer to the first entry and returns
   * the object at that position, or undefined if the collection is empty. 
   *
   * @access  public
   * @return  object                  first object in the collection
   */ 
  moveFirst : function() {
    return this._objects.reset();
  },
  
  /**
   * Set the internal pointer of an collection to its last element.
   *
   * Every collection has an internal pointer to its 'current' element, which
   * is initialized to the first entry in the collection. 
   *
   * This method resets the internal pointer to the last entry and returns
   * the object at that position, or undefined if the collection is empty. 
   *
   * @access  public
   * @return  object                  last object in the collection
   */ 
  moveLast : function() {
    return this._objects.reset();
  },
  
  /**
   * Return the next element in an collection.
   *
   * Every collection has an internal pointer to its 'current' element, which
   * is initialized to the first entry in the collection. 
   *
   * This method returns the object in the collection in the next place that's
   * pointed to by the internal collection pointer, or returns undefined if
   * there are no more objects.
   *
   * next() behaves like current(), with one difference. It advances the
   * internal collection pointer one place forward before returning the
   * object. That means it returns the next object and advances the internal
   * collection pointer by one. If advancing the internal collection pointer
   * results in going beyond the end of the collection it returns undefined.
   *
   * @access  public
   * @return  mixed                   current object in the collection
   */ 
  moveNext : function() {
    return this._objects.next();
  },
  
  /**
   * Return the prev element in an collection.
   *
   * Every collection has an internal pointer to its 'current' element, which
   * is initialized to the first entry in the collection. 
   *
   * This method returns the object in the collection in the previous place
   * that's pointed to by the internal collection pointer, or returns undefined
   * if there are no previous objects.
   *
   * prev() behaves like current(), with one difference. It rewindes the
   * internal collection pointer one place backwards before returning the
   * object. That means it returns the previous object and rewinds the internal
   * collection pointer by one. If rewinding the internal collection pointer
   * results in going beyond the start of the collection it returns undefined.
   *
   * @access  public
   * @return  mixed                   current object in the collection
   */ 
  movePrev : function() {
    return this._objects.prev();
  },
  
  /**
   * Return the index of current element in an collection.
   *
   * Every collection has an internal pointer to its 'current' element, which
   * is initialized to the first entry in the collection. 
   *
   * This method returns the value of the iterator.
   *
   * @access  public
   * @return  integer                 current iterator object
   */ 
  index : function() {
    return this._objects.index();
  },
  
  /**
   * Return the current element in an collection.
   *
   * Every collection has an internal pointer to its 'current' element, which
   * is initialized to the first entry in the collection. 
   *
   * This method returns the object in the collection that's currently being
   * pointed to by the internal pointer. It does not move the pointer in any
   * way. If the internal pointer points beyond the end of the collection, the
   * method returns undefined.
   *
   * @access  public
   * @return  mixed                   current object in the collection
   */ 
  current : function() {
    return this._objects.current();
  },
  
  /**
   * Return the current index and object pair from an collection and advance
   * the internal pointer.
   *
   * Every collection has an internal pointer to its 'current' element, which
   * is initialized to the first entry in the collection. 
   *
   * This method returns the current index and object pair from the collection
   * and advances the collections internal pointer by one. This pair is returned
   * in an array, with the elements 0 and 1, and the properties 'index' and
   * 'value'. Element 0 and property 'index' contain the index of the object,
   * and element 1 and propery 'value' contain the object in the collection.
   *
   * If the internal pointer for the collection points past the end of the
   * collection contents, each() returns undefined.
   *
   * @access  public
   * @return  object                  current object in the collection
   */ 
  each : function() {
    return this._objects.each();
  },

  /**
   * Return the value of the object as a string.
   *
   * The method returns the concatenated string values of the objects in
   * the collection.
   *
   * @access  public
   * @return  string                  string representation of object
   */
  toString : function() {
    var str = '';
    for (var i = 0, max_i = this._objects.length; i < max_i; i++) {
      str += this._objects[i].toString();
    }
    return str;
  },

  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {
    this.clear();
  }

});

/* vim: set expandtab tabstop=2 shiftwidth=2: */

