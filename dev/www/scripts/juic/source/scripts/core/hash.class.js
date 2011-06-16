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
 * TNT.Juic.Core.Map Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Generic hash class, this maintains an array of 'name' => 'value' mappings,
 * and preserves the order they were added in.
 *
 * Dynamic attributes are supported by using a callback function as the value
 * and providing a context argument to the object constructor. The methods
 * that return a value or use a value will call the callback function to return
 * the correct value.
 *
 * If the context is given as null then no dynamic support is provided and any
 * stored functions are assumed to be the values to store, this allows a hash
 * of functions to be supported.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Hash = TNT.declareClass({
  $class : 'TNT.Juic.Core.Hash',

  /**
   * Constructor
   *
   * @access  public
   * @param   object    context       context for resolving dynamic values
   * @return  object                  reference to constructed object
   */
  constructor : function(context) {

    this._context = context;

    this.empty();

  },

  /**
   * The context to evaluate dynamic values in
   * @access  private
   * @var     object                  context to call function in
   */
  _context : null,

  /**
   * The hash
   * @access  private
   * @var     object                  hash
   */
  _hash : null,

  /**
   * An array of they hashed keys (kept to preserve ordering)
   * @access  private
   * @var     array                   string array
   */
  _keys : null,

  /**
   * An cache of the {@link toString()} representation of the hash
   * @access  private
   * @var     array                   string array
   */
  _text : null,

  /**
   * Empty the hash
   *
   * Removes all items from the hash
   *
   * @access  public
   * @return  void
   */
  empty : function() {
    this._hash = {};
    this._keys = [];
    this._text = null;
  },

  /**
   * Tests if the hash is empty
   *
   * Same as "this.count() == 0".
   *
   * @access  public
   * @return  void
   */
  isEmpty : function() {
    return (this._keys.length == 0);
  },

  /**
   * Return the number of items in the hash
   *
   * @access  public
   * @return  integer                 number of items
   */
  count : function() {
    return this._keys.length;
  },

  /**
   * Get an array of keys used in the hash
   *
   * You can enumerate over this array to get the hash values in the correct
   * order.
   *
   * @access  public
   * @return  array                   array of hash keys
   */
  keys : function() {
    return this._keys;
  },

  /**
   * Get an array of values used in the hash
   *
   * The values are returned in correct order.
   *
   * @access  public
   * @return  array                   array of hash keys
   */
  values : function() {
    var ret = [];
    var max = this._keys.length;
    for (var i = 0; i < max; i++) {
      ret.push(this._hash['!' + this._keys[i]]);
    }
    return ret;
  },

  /**
   * Set an item in the hash
   *
   * If the key does not exist then it is added.
   *
   * If you pass a value of 'undefined' then any existing item in the
   * hash is deleted.
   *
   * The old value is the stored value, no attempt is made to resolve any
   * dynamic values.
   *
   * If the method is called with a single object argument then that object
   * is enumerated and the method called recursively with each property/value
   * pair.
   *
   * @access  public
   * @param   string    name          items name
   * @param   mixed     value         items value
   * @return  mixed                   items old value (or undefined)
   */
  set : function(key, value) {
    if (arguments.length == 1 && typeof key == 'object') {
      var ret = [];
      for (var i in key) {
        ret[i] = this.set(i, key[i]);
      }
      return ret;
    }

    var k = '!' + key;
    var old = this._hash[k];

    if (old == value) {
      // No change
      return old;
    }
 
    if (value == undefined) {
      // Delete value with key
      var max = this._keys.length;
      for (var i = 0; i < max; i++) {
        if (this._keys[i]) {
          this._keys.splice(i, 1);
          break;
        }
      }
      delete this._hash[k];
      this._text = null;
    } else if (old == undefined) {
      // Create new value
      this._keys.push(key);
      this._hash[k] = value;
      this._text = null;
    } else {
      // Update existing value
      this._hash[k] = value;
      this._text = null;
    }

    return old;
  },

  /**
   * Get an item in the hash
   *
   * If the key does not exist then undefined is returned.
   *
   * @access  public
   * @param   string    name          items name
   * @return  mixed                   items value (or undefined)
   */
  get : function(key) {
    var val = this._hash['!'+key];
    if (this._context != null && typeof val == 'function') {
      val = val.call(this._context);
    }
    return val;
  },
    
  /**
   * Join the items in the hash
   *
   * Returns a string value consisting of all the items in the hash
   * concatenated together and separated by the specified separator
   * characters.
   *
   * The format is "<key><equ><item><sep>...."
   *
   * If the 'sep' separator string is ommitted then a comma is used.
   * If the 'equ' separator string is ommitted then a equals is used.
   * If the 'equ' separator string is null then no key is prefixed.
   *
   * Dynamic values are resolved before joining where supported.
   *
   * @access  public
   * @param   string    sep           separator string (null for no separator)
   * @param   string    equ           equals character (null for no key)
   * @return  string                  string representation of item
   */
  join : function(sep, equ) {
    if (typeof sep == 'undefined') {
      sep = ',';
    }
    if (typeof equ == 'undefined') {
      equ = '=';
    }
    var idx = 0
    var str = '';
    var key, max = this._keys.length;
    for (var i = 0; i < max; i++) {
      key = this._keys[i];
      if (idx) {
        str += sep;
      }
      if (equ) {
        str += (key + equ);
      }
      str += this.get(key);
      idx++;
    }

    return str;
  },

  /**
   * Returns a new hash consisting of a combination of two or more hashes
   *
   * The concat method returns a Hash object containing the concatenation of
   * the supplied arguments. 
   *
   * The items to be added to the hash are added, in order, from left to right.
   *
   * The item to be added can either be other Hash objects or enumerable
   * objects.
   * 
   * Elements of source items are copied to the resulting array as follows: 
   *
   *   + For an object reference copied from any of the arguments being
   *     concatenated to the new hash, the object reference continues to
   *     point to the same object. A change in either the new hash or the
   *     original argument will result in a change to the other. 
   *
   *   + For a numeric or string value being concatenated to the new hash,
   *     only the value is copied. Changes in a value in one object does not
   *     affect the value in the other. 
   *
   * @access  public
   * @param   object    ...           Hash objects to concatenate
   * @return  object                  new Hash object
   */
  concat : function(/*[item1[, item2[, ..., itemN]]]*/) {
    var ret = new this.constructor();
    ret._keys = this._keys.concat();
    ret._hash = this._hash_concat();

    var argc = arguments.length;
    for (var a = 0; a < argc; a++) {
      var arg = arguments[a];
      if (this.isHash(arg)) {
        for (key in arg._keys) {
          ret.set(key, arg._hash['!' + key]);
        }
      } else if (typeof arg == 'object') {
        for (var p in arg) {
          ret.set(p, arg[p]);
        }
      } else {
        return null;
      }
    }

    return ret;
  },

  /**
   * Returns a Hash object with the elements reversed
   *
   * @access  public
   * @return  object                  new Hash object
   */
  reverse : function() {
    var ret = new this.constructor();
    ret._keys = this._keys.reverse();
    ret._hash = this._hash_concat();
    return ret;
  },

  /**
   * Return a slice of the hash
   *
   * The slice method copies up to, but not including, the item indicated by
   * end. If start is negative, it is treated as count + start where count is
   * the number of items in the collection. If end is negative, it is treated
   * as count + end. If end is omitted, extraction continues to the end of the
   * hash. If end occurs before start, no items are copied to the new hash.
   *
   * @access  public
   * @param   integer   start         start index of slice
   * @param   integer   end           end index of slice
   * @return  object                  new Hash object containing slice
   */
  slice : function(start, end) {
    var ret = new this.constructor();

    ret._keys = this._keys.slice(start, end);

    var k, max = this._keys.length;
    for (var i = 0; i < max; i++) {
      k = '!' + ret._keys[i];
      ret._hash[k] = this._hash[k];
    }

    return ret;
  },

  /**
   * Private sort function.
   *
   * @access  private
   * @param   object    a1            details of first item
   * @param   object    a2            details of second item
   * @return  integer                 sort order (-1, 0, 1)
   */
  _sortFunc : function(a1, a2) {
    if (a1.sortFunc) {
      return a1.sortFunc(a1.obj, a1.key, a1.val, a2.key, a2.val, a1.keySort);
    } else if (a1.keySort) {
      return (a1.key == a2.key ? 0 : (a1.key < a2.key ? -1 : 1)); 
    } else {
      return (a1.val == a2.val ? 0 : (a1.val < a2.val ? -1 : 1)); 
    }
  },

  /**
   * Returns a Hash object with the elements sorted. 
   *
   * The sort method returns a new hash with the items sorted.
   *
   * If 'sortFunc' function is supplied it will be called as follows;
   *
   *   "sortFunc(collection, key1, val1, key2, val2, keySort)"
   *
   * It must return one the following values: 
   *
   *   + A negative value if first item is less than second item.
   *   + Zero if firstr item is equivalent to second item. 
   *   + A positive value if first item is greater than second item. 
   *
   * If the 'keySort' is true the the sort function should sort by the
   * key values 'key1', 'key2' else it should sort by the item values
   * 'val1', 'val2';
   *
   * Dynamic values are resolved before passing to the user sort function.
   *
   * If no 'sortFunc' is supplied then the items are sorted in acending,
   * ASCII charactor order.
   *
   * @access  public
   * @param   boolean   keySort     true if sort by keys and not values
   * @param   callback  sortFunc    optional sort function
   * @return  item                  a new Hash containing sorted items
   */
  sort : function(keySort, sortFunc) {
    var ret = new Collection();

    var t = [];
    var key, max = this._keys.length;
    for (var i = 0; i < max; i++) {
      key = this._keys[i];
      t.push({'key' : key, 'val' : this.get(key), '_hash' : this._hash['!' + key], 'obj' : this, 'keySort' : keySort, 'sortFunc' : sortFunc});
    }

    for (var i in t.sort(this._sortFunc)) {
      var key = t[i]['key'];
      ret._keys.push(key);
      ret._hash['!' + key] = t[i]['_hash'];
    }

    return ret;
  },

  /**
   * Return the value of the hash as a string.
   *
   * The method returns the {@link join()} with default arguments.
   *
   * The result of this method is cached for speed and will return the cached
   * value until the contents of the hash alters invalidating the cache.
   *
   * @access  public
   * @return  string                  string representation of item
   */
  toString : function() {
    if (this._text == null) {
      this._text = this.join();
    }
    return this._text;
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

