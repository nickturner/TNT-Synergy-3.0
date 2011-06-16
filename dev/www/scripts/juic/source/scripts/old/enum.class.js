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
 * @package TNT.Juic
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Array = {

  /**
   * Check if an array index exists
   *
   * Checks that the index exists in the array.
   *
   * Any object can be used for the array argument, and the method will
   * check to see if the object has the specified property.
   * 
   * @access  public
   * @param   scalar      i           array index to check
   * @param   array       a           array to search
   * @return  boolean                 true array has given key
   */ 
  isIndex : function(i, a) {
    return ((a && typeof a == 'object' && typeof a[i] != 'undefined') ? true : false);
  },

  /**
   * Check if an array value exists
   *
   * Checks that the value exists in the array.
   *
   * Any enumerable object can be used for the array argument, and the
   * enumerable properties will be checked for the given value.
   * 
   * @access  public
   * @param   mixed       v           array value to check
   * @param   array       a           array to search
   * @return  boolean                 true array has given key
   */ 
  isValue : function(v, a) {
    if (!a || typeof a != 'object') {
      return false;
    }
    for (i in a) {
      if (a[i] == v) {
        return true;
      }
    }
    return false;
  },

  /**
   * Return all the indices of an array.
   *
   * This returns an Array of all the enumerable indices/properties of the
   * given array/object.
   *
   * Any enumerable object can be used for the array argument, and the
   * enumerable properties names will be returned.
   * 
   * @access  public
   * @param   array       a           array to return the indices of
   * @return  array                   array of indices
   */ 
  indices : function(a) {
    var ret = null;
    if (typeof a == 'object') {
      ret = [];
      for (i in a) {
        ret.push(i);
      }
    }
    return ret;
  },

  /**
   * Return all the values of an array.
   *
   * This returns an Array of the values of all enumerable indices/properties
   * of the given array/object.
   *
   * Any enumerable object can be used for the array argument, and the
   * enumerable properties values will be returned.
   * 
   * @access  public
   * @param   array       a           array to return the indices of
   * @return  array                   array of indices
   */ 
  values : function(a) {
    var ret = null;
    if (typeof a == 'object') {
      ret = [];
      for (i in a) {
        ret.push(a[i]);
      }
    }
    return ret;
  },

  /**
   * Create an array by using one array for its keys and another for its values
   *
   * Returns an array by using the values from the 'indices' array as indices
   * and the values from the 'values' array as the corresponding values. 
   *
   * Returns undefined if the number of elements for each array isn't equal or
   * if the arrays are empty.
   *
   * @access  public
   * @param   array       indices     array of indices
   * @param   array       values      array of values
   * @return  array                   combined array
   */ 
  combine : function(indices, values) {
    var ret = null;
    if (!indices || !values ||
        typeof indices != 'object' || typeof values != 'object' ||
        indices.constructor != Array || values.constructor != Array ||
        indices.length == 0 || values.length == 0 ||
        indices.length != values.length) {
        return null;
    }

    ret = [];

    for (i in indices) {
      if (typeof values[i] == 'undefined') {
         return null;
      }
      ret[indices[i]] = values[i];
    }

    return ret;
  },

  /**
   * Returns the specified item from the array.
   *
   * Returns undefined if the index does not exist in the array.
   *
   * This method is usefull over just doing "a[i]" as it will not throw
   * an error if a is not an array/object, it will just return undefined.
   *
   * Any enumerable object can be used for the array argument, and the
   * property with 'i' as a name will be returned.
   * 
   * @access  public
   * @param   scalar      i           array index to obtain
   * @param   array       a           array to obtain from
   * @return  mixed                   value of a[i]
   */ 
  value : function(i, a) {
    return ((a && typeof a == 'object' && typeof a[i] != 'undefined') ? a[i] : undefined);
  },

  /**
   * Applies the function to the elements of the array
   *
   * Returns an array containing all the elements of 'a' after applying the
   * callback function to each one.
   *
   * Example:
   *   function cube(n) { return(n * n * n); }
   *   a = [1, 2, 3, 4, 5];
   *   b = map(cube, a);
   *
   *   results in b == [1,8,27,64,125]
   *
   * Any enumerable object can be used for the array argument, and the
   * properties of that object are mapped.
   *
   * @access  public
   * @param   function    f           function to call
   * @param   array       a           array to map
   * @return  array                   resultant array
   */ 
  map : function(f, a) {
    if (typeof a != 'object') {
      return a;
    }
    var ret = [];
    for (i in a) {
      ret[i] = f(a[i]);
    }
    return ret;
  },

  /**
   * Calls the function for each element in the array
   *
   * Returns an array containing all the elements of 'a' after applying the
   * callback function to each one.
   *
   * The function receives three arguments: 'index', 'value', 'u'; being the
   * current elements index and value and the user data 'u' passed to the
   * method.
   *
   * The function may not change the array itself. e.g. Add/delete elements,
   * unset elements, etc. If the array that is walked is changed, the behavior
   * of this function is undefined, and unpredictable. 
   *
   * Any enumerable object can be used for the array argument, and the
   * properties of that object are walked.
   * 
   * @access  public
   * @param   function    f           function to call
   * @param   array       a           array/object to walk
   * @param   mixed       u           user data
   * @return  void                   
   */ 
  walk : function(f, a, u) {
    if (typeof f == 'function' && typeof a == 'object') {
      for (i in a) {
        f(i, a[i], u);
      }
    }
  }

};

/* vim: set expandtab tabstop=2 shiftwidth=2: */
