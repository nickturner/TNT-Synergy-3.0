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
 * Array Javascript
 * ~~~~~~~~~~~~~~~~~
 *
 * Extensions to the standard Javascript Array Object.
 *
 * Some Mozilla 1.8 has support for indexOf, lastIndexOf, forEach, filter,
 * map, some, every - so we add support for other browsers.
 *
 * @package TNT.Juic
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

/**
 * Returns the number of items in the collection.
 *
 * This just returns the value of the length property.
 *
 * @access  public
 * @return  integer                 number of items in the collection
 */
Array.prototype.count = function() {
  return this.length;
};

/**
 * Set a value at the given index in the array.
 *
 * Unlike the [] operator this method will not extend the array.
 * 
 * This method will throw an exception if the index is out of range.
 *
 * @access  public
 * @param   scalar      i           array index to return
 * @param   mixed       v           the value to set at the index
 * @return  mixed                   the value at the given index
 */
Array.prototype.setItem = function(i, v) {
  if (i < 0 || i >= this.length) {
    throw 'Array.setItem() - Index out of range';
  }
  return this[i] = v;
};

/**
 * Get a value at the given index in the array.
 *
 * This method will throw an exception if the index is out of range.
 *
 * @access  public
 * @param   scalar      i           array index to return
 * @return  mixed                   the value at the given index
 */
Array.prototype.getItem = function(i) {
  if (i < 0 || i >= this.length) {
    throw 'Array.setItem() - Index out of range';
  }
  return this[i];
};

/**
 * Get a value at the given index in the array.
 *
 * This is an alias to the {@link getItem()} method.
 *
 * This method will throw an exception if the index is out of range.
 *
 * @access  public
 * @param   scalar      i           array index to return
 * @return  mixed                   the value at the given index
 */
Array.prototype.item = function(i) {
  this.getItem(i);
};

/**
 * Check if an array index exists
 *
 * Checks that the index exists in the array.
 *
 * @access  public
 * @param   scalar      i           array index to check
 * @return  boolean                 true array has given key
 */ 
Array.prototype.isIndex = function(i) {
  return ((typeof this[i] === 'undefined') ? false : true);
},

/**
 * Check if an array value exists
 *
 * Checks that the value exists in the array.
 *
 * @access  public
 * @param   mixed       v           array value to check
 * @return  boolean                 true array has given key
 */ 
Array.prototype.isValue = function(v) {
  var l = this.length;
  for (var i = 0; i < l; i++) {
    if (this[i] === v) {
      return true;
    }
  }
  return false;
};

/**
 * Applies the function to the elements of the array
 *
 * Returns an array containing all the elements of 'a' after applying the
 * callback function to each one.
 *
 * Example:
 *   function cube(value, index, array) { return(value * value * value); }
 *   a = [1, 2, 3, 4, 5];
 *   b = a.map(cube);
 *
 *   results in b == [1,8,27,64,125]
 *
 * Any enumerable object can be used for the array argument, and the
 * properties of that object are mapped.
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:map
 *
 * @access  public
 * @param   function    f           function to call
 * @return  array                   resultant array
 */ 
if (!Array.prototype.map) {
  Array.prototype.map = function(f, obj) {
    var ret = [];
    var l = this.length;
  
    for (var i = 0; i < l; i++) {
      ret.push(f.call(obj, this[i], i, this));
    }
  
    return ret;
  };
};

/**
 * Returns the indexOf a given value in the array.
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:indexOf
 *
 * @access  public
 * @param   mixed       value       value to search for in the array
 * @param   integer     fromIndex   index to start looking at (defaults to 0)
 * @return  integer                 index of value in the array
 */ 
if (!Array.prototype.indexOf) {
  Array.prototype.indexOf = function(value, fromIndex) {
    if (fromIndex == null) {
      fromIndex = 0;
    } else if (fromIndex < 0) {
      fromIndex = Math.max(0, this.length + fromIndex);
    }

    for (var i = fromIndex; i < this.length; i++) {
      if (this[i] === value) {
        return i;
      }
    }

    return -1;
  }
};

/**
 * Returns the indexOf a given value in the array.
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:lastIndexOf
 *
 * @access  public
 * @param   mixed       value       value to search for in the array
 * @param   integer     fromIndex   index to start looking at (defaults to 0)
 * @return  integer                 index of value in the array
 */ 
if (!Array.prototype.lastIndexOf) {
  Array.prototype.lastIndexOf = function(value, fromIndex) {
    if (fromIndex == null) {
      fromIndex = this.length - 1;
    } else if (fromIndex < 0) {
      fromIndex = Math.max(0, this.length + fromIndex);
    }

    for (var i=fromIndex; i>=0; i--) {
      if (this[i] === value) {
        return i;
      }
    }

    return -1;
  };
};

/**
 * Calls the given function for each item in the array.
 *
 * The function is called within the context of the given 'obj' with the
 * arguments (item, index, array).
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:forEach
 *
 * @access  public
 * @param   function    f           function to call
 * @param   object      obj         object to use as 'this' when calling f
 * @return  void
 */ 
if (!Array.prototype.forEach) {
  Array.prototype.forEach = function(f, obj) {
    for (var i = 0, l = this.length; i < l; i++) {
      f.call(obj, this[i], i, this);
    }
  };
};

/**
 * Calls the given function for each object in the array.
 *
 * The function is called within the context of the object at the current array
 * index with the arguments (array, index).
 *
 * @access  public
 * @param   function    f           function to call
 * @return  void
 */ 
if (!Array.prototype.forEachObject) {
  Array.prototype.forEachObject = function(f) {
    for (var i = 0, l = this.length; i < l; i++) {
      f.call(this[i], this, i)
    }
  };
};

/**
 * Creates a new array with all elements that pass the test implemented by the
 * provided function. 
 *
 * The function is called within the context of the given 'obj' with the
 * arguments (item, index, array).
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:filter
 *
 * @access  public
 * @param   function    f           function to call
 * @param   object      obj         object to use as 'this' when calling f
 * @return  array                   filtered array
 */ 
if (!Array.prototype.filter) {
  Array.prototype.filter = function(f, obj) {
    var ret = [];
    for (var i = 0, l = this.length; i < l; i++) {
      if (f.call(obj, this[i], i, this)) {
        ret.push(this[i]);
      }
    }
    return ret;
  };
};

/**
 * Creates a new array with the results of calling a provided function on
 * every element in this array. 
 *
 * The function is called within the context of the given 'obj' with the
 * arguments (item, index, array).
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:map
 *
 * @access  public
 * @param   function    f           function to call
 * @param   object      obj         object to use as 'this' when calling f
 * @return  array                   mapped array
 */ 
if (!Array.prototype.map) {
  Array.prototype.map = function(f, obj) {
    var ret = [];
    for (var i = 0, l = this.length; i < l; i++) {
      rer.push(f.call(obj, this[i], i, this));
    }
    return ret;
  };
};

/**
 * Tests whether some element in the array passes the test implemented by
 * the provided function. 
 *
 * The function is called within the context of the given 'obj' with the
 * arguments (item, index, array).
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:some
 *
 * @access  public
 * @param   function    f           function to call
 * @param   object      obj         object to use as 'this' when calling f
 * @return  boolean                 true if f returned true for any items
 */ 
if (!Array.prototype.some) {
  Array.prototype.some = function(f, obj) {
    for (var i = 0, l = this.length; i < l; i++) {
      if (f.call(obj, this[i], i, this)) {
        return true;
      }
    }
    return false;
  };
};

/**
 * Tests whether all elements in the array pass the test implemented by
 * the provided function. 
 *
 * The function is called within the context of the given 'obj' with the
 * arguments (item, index, array).
 *
 * Refer to http://developer-test.mozilla.org/docs/Core_JavaScript_1.5_Reference:Objects:Array:every
 *
 * @access  public
 * @param   function    f           function to call
 * @param   object      obj         object to use as 'this' when calling f
 * @return  boolean                 true if f returned true for all items
 */ 
if (!Array.prototype.every) {
  Array.prototype.every = function (f, obj) {
    for (var i = 0, l = this.length; i < l; i++) {
      if (!f.call(obj, this[i], i, this)) {
        return false;
      }
    }
    return true;
  };
};

/**
 * Tests whether the given value is in the array.
 *
 * @access  public
 * @param   mixed       value       value to search array for
 * @return  boolean                 true if value is in the array
 */ 
Array.prototype.contains = function(value) {
  return this.indexOf(value) != -1;
};

/**
 * Return a copy of the array.
 *
 * This is an alias for {@link clone()}.
 *
 * @access  public
 * @return  array                   copy of the array
 */ 
Array.prototype.copy = function(obj) {
  return this.concat();
};

/**
 * Return a clone of the array.
 *
 * @access  public
 * @return  array                   clone of the array
 */ 
Array.prototype.clone = function(obj) {
  return this.concat();
};

/**
 * Append a value to the end of the array.
 *
 * This is an alias for {@link push()}.
 *
 * @access  public
 * @param   mixed       value       value to append to the array
 * @return  void
 */ 
Array.prototype.append = function(value) {
  Array.prototype.push.apply(this, value);
};

/**
 * Insert a value in the array at the given index.
 *
 * This is the same as insertBefore
 *
 * @access  public
 * @param   integer     index       index at which to insert the value
 * @param   mixed       value       value to insert into the array
 * @return  void
 */ 
Array.prototype.insertAt = function(index, value) {
  this.splice(index, 0, value);
};

/**
 * Insert a value in the array before another value.
 *
 * @access  public
 * @param   mixed       value       value to insert into the array
 * @param   mixed       value2      value to insert the value before
 * @return  void
 */ 
Array.prototype.insertBefore = function(value, value2)
{
  var index = this.indexOf(value2);
  if (index == -1) {
    this.push(value);
  } else {
    this.splice(index, 0, value);
  }
};

/**
 * Insert a value in the array after another value.
 *
 * @access  public
 * @param   mixed       value       value to insert into the array
 * @param   mixed       value2      value to insert the value before
 * @return  void
 */ 
Array.prototype.insertAfter = function(value, value2) {
  var index = this.indexOf(value2);
  if (index == -1 || index == (this.length - 1)) {
    this.push(value);
  } else {
    this.splice(index + 1, 0, value);
  }
};

/**
 * Remove value from the array at the given index.
 *
 * @access  public
 * @param   integer     index       index at which to remove the value
 * @return  mixed                   the value removed (or null)
 */ 
Array.prototype.removeAt = function(index) {
  var ret = this.splice(index, 1);
  return (ret ? ret[0] : null);
};

/**
 * Remove a value from the array.
 *
 * Removes the first occurrance of the value as found in the array.
 *
 * @access  public
 * @param   mixed       value       value to remove from the array
 * @return  mixed                   the value removed (or null)
 */ 
Array.prototype.remove = function(value) {
  var index = this.indexOf(value);
  return (index != -1 ? this.removeAt(index) : null);
};

/**
 * Returns a new array consisting of the subtraction of two of more arrays.
 *
 * The subtract method returns an Array object containing the subtraction of
 * array1 and any other supplied items. 
 *
 * The items to be subtracted (item1 ... itemN) to the array are subtracted,
 * in order, from left to right. If one of the items is an array, its contents
 * are subtracted from the new Array. If the item is anything other than an
 * array, it is removed from the Array if it exists.
 *
 * This is the opposite to concat.
 *
 * @access  public
 * @param   mixed       item1       value to remove from the array
 * @param   mixed       ... itemN   value to remove from the array
 * @return  array                   array with the value removed
 */ 
Array.prototype.subtract = function(/*Item1 [, item2[, ... itemN]]*/) {
  var ret = this.concat();
  for (var a = 0, max_a = arguments.length; a < max_a; a++) {
    var item = arguments[a];
    if (item instanceof Array) {
      for (var i = 0; max_i = item.length; i++) {
        ret = ret.remove(item[i]);
      }
    } else {
      ret = ret.remove(item);
    }
  }

  return ret;
};

/**
 * Removes all values from the array.
 *
 * @access  public
 * @return  void
 */ 
Array.prototype.clear = function() {
  return this.length = 0;
};

/**
 * Returns the first value in the array.
 *
 * @access  public
 * @return  mixed                   first value in the array
 */ 
Array.prototype.getFirst = function() {
  return this[0];
};

/**
 * Returns the last value in the array.
 *
 * @access  public
 * @return  mixed                   last value in the array
 */ 
Array.prototype.getLast = function() {
  return this[this.length - 1];
};

/**
 * The array interator.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * @access  private
 * @var     integer                 the 'current' index in the array
 */ 
Array.prototype._iterator = 0;

/**
 * Set the internal pointer of an array to its first element.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * This method resets the internal pointer to the first element and returns
 * the value of the first element, or undefined if the array is empty. 
 *
 * @access  public
 * @return  mixed                   first value in the array
 */ 
Array.prototype.reset = function() {
  return this[this._iterator = 0];
};

/**
 * Set the internal pointer of an array to its last element.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * This method resets the internal pointer to the last element and returns
 * the value of the last element, or undefined if the array is empty. 
 *
 * @access  public
 * @return  mixed                   first value in the array
 */ 
Array.prototype.reset = function() {
  return this[this._iterator = (this.length - 1)];
};

/**
 * Return the index of current element in an array.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * This method returns the value of the iterator or returns undefined if the
 * iterator has been advanced beyond the end of the array.
 *
 * @access  public
 * @return  integer                 current iterator value
 */ 
Array.prototype.index = function() {
  if (this._iterator >= 0 && this._iterator < this.length) {
    return this._iterator;
  } else {
    return undefined;
  }
};

/**
 * Return the current element in an array.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * This method returns the value of the array element that's currently being
 * pointed to by the internal pointer. It does not move the pointer in any way.
 * If the internal pointer points beyond the end of the array, the method
 * throws an exception.
 *
 * @access  public
 * @return  mixed                   current value in the array
 */ 
Array.prototype.current = function() {
  return this[this._iterator];
};

/**
 * Return the next element in an array.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * This method returns the array value in the next place that's pointed to by
 * the internal array pointer, or returns undefined if there are no more
 * elements. 
 *
 * next() behaves like current(), with one difference. It advances the internal
 * array pointer one place forward before returning the element value. That
 * means it returns the next array value and advances the internal array pointer
 * by one. If advancing the internal array pointer results in going beyond the
 * end of the element list, next() returns undefined.
 *
 * @access  public
 * @return  mixed                   current value in the array
 */ 
Array.prototype.next = function() {
  return this[++this._iterator];
};

/**
 * Return the prev element in an array.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * This method returns the array value in the prev place that's pointed to by
 * the internal array pointer, or returns undefined if there are no more
 * elements. 
 *
 * prev() behaves like current(), with one difference. It rewinds the internal
 * array pointer one place backwards before returning the element value. That
 * means it returns the previous array value and rewinds the internal array
 * pointer by one. If rewinding the internal array pointer results in going
 * beyond the start of the array, prev() returns undefined.
 *
 * @access  public
 * @return  mixed                   current value in the array
 */ 
Array.prototype.prev = function() {
  return this[--this._iterator];
};

/**
 * Return the current key and value pair from an array and advance the
 * internal pointer.
 *
 * Every array has an internal pointer to its 'current' element, which is
 * initialized to the first element inserted into the array. 
 *
 * This method returns the current index and value pair from the array and
 * advances the arrays internal pointer by one. This pair is returned in an
 * array, with the elements 0 and 1, and the properties 'index' and 'value'.
 * Element 0 and property 'index' contain the index of the array element, and
 * element 1 and propery 'value' contain the value of the array element.
 *
 * If the internal pointer for the array points past the end of the array
 * contents, each() returns undefined.
 *
 * @access  public
 * @return  mixed                   current value in the array
 */ 
Array.prototype.each = function() {
  if (this._iterator >= 0 && this._iterator < this.length) {
    var ret = [this._iterator, this[this._iterator]];
    ret.index = ret[0];
    ret.value = ret[1];
    return ret;
  }
  return undefined;
};

/* vim: set expandtab tabstop=2 shiftwidth=2: */
