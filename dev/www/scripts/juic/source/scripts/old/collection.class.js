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
 * TNT.Juic.Core.Collection Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Generic base class for a collection of items.
 *
 * This object provides a set of methods to allow traversal of a collection
 * of data.
 *
 * A collection uses keyed indices, but can be accessed using either the
 * key or index in the collection. For example item('fred') would return
 * the item in the collection with the key 'fred', conversely item(5)
 * would return the 5th item in the collection. If the key is numeric then
 * use the string value of the number to access it, ie. item('5') would
 * return the item with key 5 and not the 5th item.
 *
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Collection = TNT.declareClass({
  $class : 'TNT.Juic.Core.Collection',
  $extends : TNT.Juic.Core.Object,

  /**
   * Constructor
   *
   * @access  public
   * @param   mixed     items         initial items for the collection
   * @return  object                  reference to constructed object
   */
  constructor : function(items) {
    TNT.Juic.Core.Object.prototype.constructor.call(this);

    this._items = {};
    this._count = 0;

    if (items) {
      this.init(items);
    }
  },

  /**
   * The number of items in the collection
   * @access  private
   * @var     integer                 number of items
   */
  _count : 0,

  /**
   * The items in the collection
   * @access  private
   * @var     object                  collection of items
   */
  _items : null,

  /**
   * Initialise the collection to the given items
   *
   * This clears the collection and adds all the items specified.
   *
   * The arguments can either be other Collection objects or a native
   * javascript objects which will be enumerated to give the items.
   *
   * An scalar argument is added with its index as the key.
   *
   * Calling the method with no arguments will clear the collection.
   *
   * @access  public
   * @param   object    ...           collections/objects to add
   * @return  void
   */
  init : function(/*col1, ...*/) {
    this._items = {};
    this._count = 0;
    var argc = arguments.length;
    for (var a = 0; a < argc; a++) {
      var arg = arguments[a];
      if (this.isCollection(arg)) {
        for (var key in arg._items) {
          this._items[key] = arg._items[key];
          this._count++;
        }
      } else if (typeof arg == 'object') {
        for (var p in arg) {
          this._items['!'+p] = arg[p];
          this._count++;
        }
      } else {
        this._items['!'+this._count] = arg;
        this._count++;
      }
    }
  },

  /**
   * Test to see if given object is a collection
   *
   * Check the specified object and returns true if it a collection object
   * derived from this collection class definition.
   *
   * @access  public
   * @param   mixed     object        object to check
   * @return  boolean                 true if object is a collection
   */
  isCollection : function(obj) {
    return (typeof obj == 'object' && obj.constructor == this.constructor);
  },

  /**
   * Empty the collection
   *
   * Removes all items from the collection
   *
   * @access  public
   * @return  void
   */
  empty : function() {
    this._items = {};
    this._count = 0;
  },

  /**
   * Tests if the collection is empty
   *
   * Same as "this.count() == 0" but more efficient as it won't try to count
   * the items if the count is unknown.
   *
   * @access  public
   * @return  void
   */
  isEmpty : function() {
    for (var i in this._items) {
      return false;
    }
    return true;
  },

  /**
   * Return the number of items in the collection
   *
   * @access  public
   * @return  integer                 number of items
   */
  count : function() {
    if (this._count == -1) {
      this._count = 0;
      for (var i in this._items) {
        this._count++;
      }
    }
    return this._count;
  },

  /**
   * Returns an array of keys.
   *
   * The keys method just returns a Array object containing all the
   * keys in the Collection in the order they are stored.
   *
   * @access  public
   * @return  array                   array of keys
   */
  keys : function() {
    var ret = [];
    for (var key in this._items) {
      ret.push(key.substr(1));
    }
    return ret;
  },

  /**
   * Returns an array of items.
   *
   * The items method just returns a Array object containing all the
   * items in the Collection in the order they are stored.
   *
   * @access  public
   * @return  array                   array of items
   */
  items : function() {
    var ret = [];
    for (var key in this._items) {
      ret.push(this._items[key]);
    }
    return ret;
  },

  /**
   * Enumerate the collection.
   *
   * The enum method calls the provided function for each item in the
   * Collection in the order they are stored.
   *
   * The enumeration function is called as follows;
   *   "func(key, item)"
   * where 'key' is the items key and 'item' is the items.
   *
   * If the 'func' returns false then the enumeration stops and the method
   * return false.
   *
   * @access  public
   * @param   function  func          function to call
   * @return  void
   */
  enumerate : function(func) {
    for (var key in this._items) {
      if (!func(key.substr(1), this._items[key])) {
        return false;
      }
    }
    return true;
  },

  /**
   * Return the idx of the given item.
   *
   * If an item is stored in more than one place in the collection then
   * this will return the first occurrance in the order they were stored.
   *
   * @access  public
   * @param   mixed     item          item
   * @return  integer                 idx of item (or undefined)
   */
  idxOf : function(item) {
    var idx = 0;
    for (var key in this._items) {
      if (this._item[key] == item) {
        return idx;
      }
      idx++;
    }
    return undefined;
  },

  /**
   * Return the key the given item.
   *
   * If an item is stored in more than one place in the collection then
   * this will return the first occurrance in the order they were stored.
   *
   * @access  public
   * @param   mixed     item          item
   * @return  integer                 idx of item (or undefined)
   */
  keyOf : function(item) {
    for (var key in this._items) {
      if (this._item[i] == item) {
        return key.substr(1);
      }
    }
    return undefined;
  },

  /**
   * Return the idx of the item at position.
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * @access  public
   * @param   scalar    pos           key|idx
   * @return  integer                 idx of item at pos (or undefined)
   */
  idxAt : function(pos) {
    switch (typeof pos) {
      case 'string':
        // Return idx of item with key 'pos
        var idx = 0;
        var key = '!'+pos;
        for (var i in this._items) {
          if (i == key) {
            return idx;
          }
          idx++;
        }
        return undefined;

      case 'number':
        // Return idx of item with idx 'pos'
        if (pos >= 0 && pos < this.count()) {
            return pos;
        }
        return undefined;
    }

    return undefined;
  },
    
  /**
   * Return the key of the item at position.
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * @access  public
   * @param   scalar    pos           key|idx
   * @return  string                  key of item at pos (or undefined)
   */
  keyAt : function(pos) {
    switch (typeof pos) {
      case 'string':
        // Return key of item with key 'pos
        var key = '!'+pos;
        for (var i in this._items) {
          if (i == key) {
            return pos;
          }
        }
        return undefined;

      case 'number':
        // Return key of item with idx 'pos'
        var idx = 0;
        for (var i in this._items) {
          if (idx == pos) {
            return i.substr(1);
          }
          idx++;
        }
        return undefined;
    }

    return null;
  },

  /**
   * Return the item at position.
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * @access  public
   * @param   scalar    pos           key|idx
   * @return  mixed                   item at given pos (or undefined)
   */
  itemAt : function(pos) {
    switch (typeof pos) {
      case 'string':
        // Return item with key 'pos'
        var key = '!'+pos;
        return (this._items[key] ? this._items[key] : undefined);

      case 'number':
        // Return item with idx 'pos'
        var idx = 0;
        for (var i in this._items) {
          if (pos == idx) {
            return this._items[i];
          }
          idx++;
        }
        return undefined;
    }

    return undefined;
  },

  /**
   * Return the item at position.
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * This is an alias for {@link itemAt()}
   *
   * @access  public
   * @param   scalar    pos           key|idx
   * @return  mixed                   item at given pos (or undefined)
   */
  item : function(pos) {
    return this.itemAt(pos);
  },

  /**
   * Get the item with the given key.
   *
   * If the item does not already exist then undefined is returned.
   *
   * @access  public
   * @param   string    key           items key
   * @return  mixed                   item with given key (or undefined)
   */
  get : function(key) {
    var k = '!' + key;
    return (this._items[k] ? this._items[k] : undefined);
  },

  /**
   * Set the item with the given key.
   *
   * If the item does not already exist then it is appended.
   *
   * @access  public
   * @param   string    key           items key
   * @param   mixed     item          item
   * @return  mixed                   item with given key (or undefined)
   */
  set : function(key, item) {
    var k = '!' + key;
    return (this._items[k] = item);
  },

  /**
   * Return if the position exists.
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * @access  public
   * @param   scalar    pos           key|idx
   * @return  mixed                   item at given pos (or null)
   */
  exists : function(pos) {
    switch (typeof pos) {
      case 'string':
        // Return if an item with key 'pos' exists
        return (this._items['!' + pos] ? true : false);

      case 'number':
        // Return if an item with idx 'pos' exists
        return ((pos >= 0 && pos < this.count()) ? true : false);
    }

    return false;
  },

  /**
   * Return boolean if item is first in the collection
   *
   * @access  public
   * @param   mixed     item          reference to item to check
   * @return  boolean                 true if item is the first item
   */
  isFirst : function(item) {
    var cnt = this.count();
    return (cnt && (this.idxOf(item) == 0));
  },

  /**
   * Return boolean if item is last in the collection
   *
   * @access  public
   * @param   item      item          reference to item to check
   * @return  boolean                 true if item is the last item
   */
  isLast : function(item) {
    var cnt = this.count();
    return (cnt && (this.idxOf(item) == (cnt - 1)));
  },

  /**
   * Return the first item in the collection
   *
   * @access  public
   * @return  mixed                   reference to first item (or null)
   */
  first : function() {
    var cnt = this.count();
    return (cnt ? this.itemAt(0) : null);
  },

  /**
   * Return the last item in the collection
   *
   * @access  public
   * @return  mixed                   reference to last item (or null)
   */
  last : function() {
    var cnt = this.count();
    return (cnt ? this.itemAt(cnt - 1) : null);
  },

  /**
   * Return the next item to the specified item in the collection
   *
   * @access  public
   * @param   mixed     item          reference to item to get next one
   * @return  mixed                   reference to next item (or null)
   */
  next : function(item) {
    var cnt = this.count();
    var idx = (cnt ? this.idxOf(item) : -1);
    return (idx >= 0 && idx <= (cnt - 2) ? this.itemAt(idx + 1) : null);
  },

  /**
   * Return the previous item to the specified item in the collection
   *
   * @access  public
   * @param   mixed     item          reference to item to get previous one
   * @return  mixed                   reference to next item (or null)
   */
  prev : function(item) {
    var cnt = this.count();
    var idx = (cnt ? this.idxOf(item) : -1);
    return (idx >= 1 ? this.itemAt(idx - 1) : null);
  },

  /**
   * Insert an item before the specified item
   *
   * Insert a new item before an existing item or at the beginning of the
   * collection if no existing item is specified.
   *
   * @access  public
   * @param   string    key           key to insert
   * @param   mixed     item          item to insert against this key
   * @param   mixed     item2         insert before this item
   * @return  integer                 index where item was inserted (or -1)
   */
  insert : function(key, item, item2) {
    if (this.exists(key)) {
      return -1;
    }

    if (!item2) {
      var idx = this.count();
      var items = {};
      items['!'+key] = item;
      for (var key in this._items) {
        items[key] = this._items[key];
      }
      this._items = items;
      this._count++;
      return 0;
    }

    var ret = -1;
    var idx = 0;;
    var items = {}
    for (var k in this._items) {
      if ((ret == -1) && (this._items[k] == item2)) {
        items['!'+key] = item;
        ret = idx;
      }
      items[k] = this._items[k];
      idx++;
    }

    if (ret != -1) {
      this._items = items;
      this._count = -1;
    }

    return ret;
  },

  /**
   * Append an item after the specified item
   *
   * Append a new item after an existing item or at the end of the
   * collection if no existing item is specified.
   *
   * @access  public
   * @param   string    key           key to append
   * @param   mixed     item          item to append against this key
   * @param   mixed     item2         append after this item
   * @return  integer                 index where item was appended (or -1)
   */
  append : function(key, item, item2) {
    if (this.exists(key)) {
      return -1;
    }

    if (!item2) {
      var idx = this.count();
      this._items['!'+key] = item;
      this._count++;
      return idx;
    }

    var ret = -1;
    var idx = 0;
    var items = {}
    for (var k in this._items) {
      items[k] = this._items[k];
      if ((ret == -1) && (this._items[k] == item2)) {
        items['!'+key] = item;
        ret = idx;
      }
      idx++;
    }
    
    if (ret != -1) {
      this._items = items;
      this._count = -1;
    }

    return ret;
  },

  /**
   * Remove an item
   *
   * Either the first matching item or all matching items are removed.
   *
   * If more than one item was removed then the return value is the index
   * of the last item to be removed.
   *
   * @access  public
   * @param   mixed     item          item to remove
   * @param   boolean   all           remove all occurrances and not just first
   * @return  integer                 index where item was removed (or -1)
   */
  remove : function(item, all) {
    var ret = -1;
    var idx = 0;;
    for (var k in this._items) {
      if ((ret != -1 || all) && this._items[k] == item) {
          delete this._items[k];
          this._count--;
          ret = idx;
      }
      idx++;
    }
    return ret;
  },

  /**
   * Insert an item before the given position.
   *
   * Insert a new item before an existing position or at the beginning of the
   * collection if no existing position is specified.
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * @access  public
   * @param   string    key           key to insert
   * @param   mixed     item          item to insert against this key
   * @param   scalar    pos           insert before this position
   * @return  integer                 index where item was inserted (or -1)
   */
  insertAt : function(key, item, pos) {
    if (typeof pos == 'undefined' || pos == null) {
      return this.insert(key, item);
    }

    var ret = -1;
    var idx = 0;;
    var key2 = null, idx2 = -1;

    if (typeof pos == 'string') {
      key2 = '!'+pos;
    } else if (typeof pos == 'number') {
      idx2 = pos;
    } else {
      return -1;
    }

    var items = {}
    for (var k in this._items) {
      if ((ret == -1) && (k == key2 || idx == idx2)) {
        items['!'+key] = item;
        ret = idx;
      }
      items[k] = this._items[k];
      idx++;
    }

    if (ret != -1) {
      this._items = items;
      this._count = -1;
    }

    return ret;
  },

  /**
   * Append an item after the specified item
   *
   * Append a new item after an existing item or at the end of the
   * collection if no existing item is specified.
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * @access  public
   * @param   string    key           key to append
   * @param   mixed     item          item to append against this key
   * @param   scalar    pos           append after this position
   * @return  integer                 index where item was appended (or -1)
   */
  appendAt : function(key, item, pos) {
    if (typeof pos == 'undefined' || pos == null) {
      return this.append(key, item);
    }

    var ret = -1;
    var idx = 0;;
    var key2 = null, idx2 = -1;

    if (typeof pos == 'string') {
      key2 = '!'+pos;
    } else if (typeof pos == 'number') {
      idx2 = pos;
    } else {
      return -1;
    }

    var items = {}
    for (var k in this._items) {
      items[k] = this._items[k];
      if ((ret == -1) && (k == key2 || idx == idx2)) {
        items['!'+key] = item;
        ret = idx;
      }
      idx++;
    }

    if (ret != -1) {
      this._items = items;
      this._count = -1;
    }

    return ret;
  },

  /**
   * Remove an item
   *
   * If 'pos' is a number then it is assumed to be the idx of an item.
   * If 'pos' is a string then it is assumed to be the key of an item.
   *
   * @access  public
   * @param   scalar    pos           remove at this position
   * @return  integer                 index where item was removed (or -1)
   */
  removeAt : function(pos) {
    var ret = -1;
    var idx = 0;;
    var key2 = null, idx2 = -1;

    if (typeof pos == 'string') {
      key2 = '!'+pos;
    } else if (typeof pos == 'number') {
      idx2 = pos;
    } else {
      return -1;
    }

    var items = {}
    for (var k in this._items) {
      if ((k == key2 || idx == idx2)) {
          delete this._items[k];
          this._count--;
          ret = idx;
          break;
      }
      idx++;
    }

    return ret;
  },

  /**
   * Join the items in the collection
   *
   * Returns a string value consisting of all the elements of the collection
   * concatenated together and separated by the specified separator characters.
   *
   * The format is "<key><equ><item><sep>...."
   *
   * If the 'sep' separator string is ommitted then a comma is used.
   * If the 'equ' separator string is ommitted then a equals is used.
   * If the 'equ' separator string is null then no key is prefixed.
   *
   * @access  public
   * @param   string    sep           separator string (null for no separator)
   * @param   string    equ           equals character (null for no key)
   * @return  string                  string representation of item
   */
  join : function(sep,equ) {
    if (typeof sep == 'undefined') {
      sep = ',';
    }
    if (typeof equ == 'undefined') {
      equ = '=';
    }
    var idx = 0
    var str = '';
    for (var key in this._items) {
      if (idx) {
        str += sep;
      }
      if (equ) {
        str += (key.substr(1) + equ);
      }
      str += this._items[key];
      idx++;
    }
    return str;
  },

  /**
   * Concatenate two or more collections.
   *
   * Returns a new collection consisting of a combination of two or more
   * collections.
   *
   * @access  public
   * @param   object    ...           collections to concatenate
   * @return  object                  new collection item
   */
  concat : function(/*col1, ...*/) {
    var ret = new Collection();
    for (var key in this._items) {
      ret._items[key] = this._items[key];
    }
    var argc = arguments.length;
    for (var a = 0; a < argc; a++) {
      var arg = arguments[a];
      if (this.isCollection(arg)) {
        for (var key in arg._items) {
          ret._items[key] = arg._items[key];
        }
      } else if (typeof arg == 'object') {
        for (var p in arg) {
          ret._items['!'+p] = arg[p];
        }
      } else {
        return null;
      }
    }
    ret._count = -1;
    return ret;
  },

  /**
   * Reverses the collection
   *
   * The reverse method reverses the items of a Collection item in place.
   * It does not create a new Collection item during execution. 
   *
   * @access  public
   * @return  void
   */
  reverse : function() {
    var keys = [];
    for (var key in this._items) {
      keys.unshift(key);
    }

    var items = {}
    for (var k in keys) {
      items[keys[k]] = this._items[keys[k]];
    }

    this._items = items;
  },

  /**
   * Return a slice of the collection
   *
   * The slice method copies up to, but not including, the item indicated by
   * end. If start is negative, it is treated as count + start where count is
   * the number of items in the collection. If end is negative, it is treated
   * as count + end. If end is omitted, extraction continues to the end of
   * the collection. If end occurs before start, no items are copied to the
   * new collection.
   *
   * @access  public
   * @param   integer   start         start index of slice
   * @param   integer   end           end index of slice
   * @return  object                  collection containing slice
   */
  slice : function(start, end) {
    var ret = new Collection();
    var cnt = this.count();
    if (!cnt) {
      return ret;
    }

    if (typeof start == 'undefined') {
      start = 0;
    }

    if (typeof end == 'undefined') {
      end = cnt;
    }

    if (start < 0) {
      start = cnt + start;
    }

    if (end < 0) {
      end = cnt + end;
    }
    if (end > cnt) {
      end = cnt;
    }

    if (start < 0 || start >= cnt || end < 0 || end < start) {
      return ret;
    }
    
    ret._count = 0;

    var idx = 0;
    for (var key in this._items) {
      if (idx >= start && idx < end) {
        ret._items[key] = this._items[key];
        ret._count++;
      }
      idx++;
    }

    return ret;
  },

  /**
   * Removes/Replaces items from a collection.
   *
   * The splice method modifies the Collection by removing the specified number
   * of items from position start and inserting new items. The deleted
   * items are returned as a new Collection item.
   *
   * @access  public
   * @param   integer   start         start index of slice
   * @param   integer   deleteCount   number of items to delete
   * @param   object    replace       collection to replace deleted slice
   * @return  object                  collection containing deleted slice
   */
  splice : function(start, deleteCount, replace) {
    var ret = new Collection();
    var cnt = this.count();
    if (!cnt) {
      return ret;
    }

    if (typeof start == 'undefined') {
      start = 0;
    }

    if (typeof deleteCount == 'undefined') {
      deleteCount = 0;
    }

    if (start < 0 || start >= cnt) {
      return ret;
    }

    var end = (start + deleteCount);

    var idx = 0;
    for (var key in this._items) {
      if (idx >= start && idx < end) {
        ret._items[key] = this._items[key];
        ret._count++;

        delete this._items[key];
        this._count--;
      }
      idx++;
    }

    if (this.isCollection(replace)) {
      var idx = 0;
      var items = {};
      for (var key in this._items) {
        if (idx == start) {
          for (var _key in replace._items) {
            items[_key] = replace._items[_key];
            idx++;
          }
        }
        items[key] = this._items[key];
        idx++;
      }
      this._items = items;
      this._count = idx;
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
      return a1.sortFunc(a1.collection, a1.key, a1.item, a2.key, a2.item, a1.keySort);
    } else if (a1.keySort) {
      return (a1.key == a2.key ? 0 : (a1.key < a2.key ? -1 : 1)); 
    } else {
      return (a1.item == a2.item ? 0 : (a1.item < a2.item ? -1 : 1)); 
    }
  },

  /**
   * Sort a collection.
   *
   * The sort method returns a new collection with the items sorted.
   *
   * If 'sortFunc' function is supplied it will be called as follows;
   *
   *   "sortFunc(collection, key1, item1, key2, item2, keySort)"
   *
   * It must return one the following values: 
   *
   *   + A negative value if first item is less than second item.
   *   + Zero if firstr item is equivalent to second item. 
   *   + A positive value if first item is greater than second item. 
   *
   * If the 'keySort' is true the the sort function should sort by the
   * key values 'key1', 'key2' else it should sort by the item values
   * 'item1', 'item2';
   *
   * If no 'sortFunc' is supplied then the items are sorted in acending,
   * ASCII charactor order.
   *
   * @access  public
   * @param   boolean   keySort       true if sort by keys and not items
   * @param   callback  sortFunc      optional sort function
   * @return  item                  collection containing sorted items
   */
  sort : function(keySort, sortFunc) {
    var ret = new Collection();

    var t = [];
    for (var key in this._items) {
      t.push({'key' : key, 'item' : this._items[key], 'collection' : this, 'keySort' : keySort, 'sortFunc' : sortFunc});
    }

    ret._items = {};
    for (var i in t.sort(this._sortFunc)) {
      ret._items[t[i]['key']] = t[i]['item'];
      ret._count++;
    }

    return ret;
  },

  /**
   * Return the value of the item as a string.
   *
   * The method returns the {@link join()} with default arguments.
   *
   * @access  public
   * @return  string                  string representation of item
   */
  toString : function() {
    return this.join();
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

