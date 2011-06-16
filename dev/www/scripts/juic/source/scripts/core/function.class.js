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
 * Function Javascript
 * ~~~~~~~~~~~~~~~~~~~
 *
 * Extensions to the standard Javascript Function Object.
 *
 * @package TNT.Juic
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

Function.prototype._extends = function(superClass, className) {
  if (typeof superClass !== 'function') {
    throw 'extends() - Function/Constructor to extend from is not a function: ' + superClass;
  }

  if (typeof className !== 'string') {
    throw 'extends() - Missing or malformed className: ' + className;
  }

  // build helper function
  // this omits the initial constructor call while inherit properties
  var f = new Function;
  f.prototype = superClass.prototype;
  proto = this.prototype = new f;

  this.$super = superClass;

  proto.$class = this.$class = className;
  proto.constructor = this;

  // remember us
  //Function._classes[className] = this;

  return proto;
};

Function.prototype.property = function(d) {
  if (typeof d != 'object') {
    throw 'property() - Missing or malformed property hash';
  }

  if (typeof d.name != 'string') {
    throw 'property() - Missing or malformed property name';
  }

  if (typeof d.value == 'undefined') {
    value = null;
  }

  if (typeof d.setMethod != 'undefined' && typeof d.setMethod != 'function') {
    throw 'property() - Missing or malformed set method';
  }

  if (typeof d.getMethod != 'undefined' && typeof d.getMethod != 'function') {
    throw 'property() - Missing or malformed set method';
  }

  if (typeof d.chkMethod != 'undefined' && typeof d.chkMethod != 'function') {
    throw 'property() - Missing or malformed set method';
  }

  if (typeof d.modMethod != 'undefined' && typeof d.modMethod != 'function') {
    throw 'property() - Missing or malformed set method';
  }

  var property = '_' + name;
  var method = name.ucfirst();
  var getMethod = 'get' + method;
  var setMethod = 'set' + method;
  var chkMethod = 'chk' + method;
  var modMethod = '_mod' + method;

  var setFunc = d.setMethod;
  var getFunc = d.getMethod;
  var chkFunc = d.chkMethod;
  var modFunc = d.modMethod;

  if (!setFunc) {
    // Use the default set method
    setFunc = function(v) {
      if (v === this[property]) {
        return 0;
      }
      if (this[chkMethod] && !this[chkMethod](v)) {
        return -1;
      }
      this[property] = v;
      return this._propertyChanged(name, this[property]);
    };
  }

  if (!getFunc) {
    // Use the default get method
    get = function() {
      return this._dynamicValue(this[property]);
    };
  }

  // Add the new property and methods to the prototype
  var p = this.prototype;

  p[property] = value;
  p[setMethod] = set;
  p[getMethod] = get;
  p[chkMethod] = chk;
  p[modMethod] = mod;

};

/* vim: set expandtab tabstop=2 shiftwidth=2: */
