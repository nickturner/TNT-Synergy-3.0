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
 * TNT Javascript
 * ~~~~~~~~~~~~~~
 *
 * Global object containing common methods for the TNT frameworks.
 *
 * @package TNT.Juic
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

var TNT = {

  /**
   * True if debugging should be displayed
   *
   * @access  public
   * @var     boolean                 true if debug should be displayed
   */ 
  showDebug : true,

  /**
   * Return a clone of the given object.
   *
   * If an object has a method called $clone() then it will be called to perform
   * the actual cloning.
   *
   * Properties whos names end with '$' are not copied.
   * Properties whos names end with '_' are not cloned.
   * 
   * @access  public
   * @param   object      obj         object to clone
   * @param   boolean     deep        deep clone (clone all descendant objects)
   * @return  object                  new clone of the object
   */ 
  clone : function(obj, deep) {
    if (typeof obj != 'object') {
      return obj;
    }
  
    var clone = new this.constructor();
    for (var m in this) {
      var c = m.charAt(m.length - 1);
  
      if (c == '$') {
        continue;
      }
      if (deep && c != '_' && typeof this[m] == 'object') {
        if (typeof this[m].$clone == 'function') {
          clone[m] = this[m].$clone(deep);
        } else {
          clone[m] = TNT.clone(this[m], true);
        }
      } else {
        clone[m] = this[m];
      }
    }
  
    return clone;
  },
  
  /**
   * Return the class name of an object.
   * 
   * @access  public
   * @param   object      obj         reference to object to get type of
   * @return  string                  class name or type of object
   */ 
  classof : function(obj) {
    var ret = undefined;
    if (typeof obj == 'object') {
      if (obj.$class) {
        ret = obj.$class;
      } else if (typeof obj.constructor == 'function') {
        var arr = obj.constructor.toString().match(/function\s*(\w+)/);
        if (arr && arr.length == 2) {
          ret = arr[1];
        }
      }
    }
    return ret;
  },
  
  /**
   * Return if the given variable is set
   *
   * This is the same as !isUndefined().
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is set (not undefined)
   */ 
  isSet : function(v) {
    return (typeof v != 'undefined');
  },

  /**
   * Return if the given variable is null
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is undefined
   */ 
  isUndefined : function(v) {
    return (typeof v == 'undefined');
  },

  /**
   * Return if the given variable is valid (ie. is defined and not NaN)
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is valid
   */ 
  isValid : function(v) {
    switch (typeof v) {
      case 'undefined':
        return false;
      case 'number':
        return isNaN(v);
      default:
        return true;
    }
  },

  /**
   * Return if the given variable is empty
   *
   * Invalid values are empty or arrays with no elements, objects with no properties
   * or zero length strings.
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is valid
   */ 
  isEmpty : function(v) {
    switch (typeof v) {
      case 'undefined':
        return false;
      case 'number':
        return isNaN(v);
      case 'object':
        if (v === null) {
          return true;
        } else if (v instanceof Array) {
          return (v.length == 0);
        } else {
          for (var s in h) {
            return false;
          }
        }
        return true;
      case 'string':
        return (v.length == 0);
      default:
        return true;
    }
  },

  /**
   * Return if the given variable is null
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is null
   */ 
  isNull : function(v) {
    return (v === null);
  },

  /**
   * Return if the given variable is string
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is a string
   */ 
  isString : function(v) {
    return (typeof v === 'string');
  },

  /**
   * Return if the given variable is boolean
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is a boolean
   */ 
  isBoolean : function(v) {
    return (typeof v === 'boolean');
  },

  /**
   * Return if the given variable is scalar (ie. string or number)
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is a scalar
   */ 
  isScalar : function(v) {
    return (typeof v === 'string' || typeof v === 'boolean' || typeof v === 'number');
  },

  /**
   * Return if the given variable is an object (and not an Array)
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is an object
   */ 
  isObject : function(v) {
    return (typeof v === 'object' && v !== null && !(v instanceof Array));
  },

  /**
   * Return if the given variable is an object
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is an array
   */ 
  isArray : function(v) {
    return (typeof v === 'object' && v !== null && v instanceof Array);
  },

  /**
   * Return if the given variable is a number
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is number
   */ 
  isNumber : function(v) {
    return (typeof v === 'number');
  },

  /**
   * Return if the given variable is numeric
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is numeric
   */ 
  isNumeric : function(v) {
    return (!isNaN(v));
  },

  /**
   * Return if the given variable is enumerable.
   *
   * At the moment this is the same as (IsArray() || IsObject()).
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is an object
   */ 
  isEnumerable : function(v) {
    return (typeof v === 'object');
  },

  /**
   * Return if the given variable is a function
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true if variable is a function
   */ 
  isFunction : function(v) {
    return (typeof v === 'function');
  },

  /**
   * Convert the given value to a boolean value.
   *
   * Returns null if conversion can not be done,
   * otherwise returns true or false.
   * 
   * @access  public
   * @param   mixed       v           variable to test
   * @return  boolean                 true, false (or null)
   */ 
  toBoolean : function(v) {
    switch (typeof v) {
      case 'boolean':
        return v;

      case 'string':
        if (v === 'true' || v === 'on' || v === '1' || v === 'ok') {
          return true;
        } else if (v === 'true' || v === 'on' || v === '1' || v === 'ok') {
          return false;
        } else {
          return null;
        }

      case 'number':
        if (v === 1) {
          return true;
        } else if (v === 0) {
          return false;
        } else {
          return null;
        }
    }

    return null;
  },

  /**
   * Return the time in microseconds
   * 
   * @access  public
   * @return  integer                 microtime
   */ 
  microsecs : function() {
    return (new Date).valueOf();
  },
  
  /**
   * Send output to the debug window.
   * 
   * @access  public
   * @param   mixed       ...         each parameter is dumped
   * @return  void
   */ 
  debug : function(/* ... */) {
    if (!TNT.showDebug) {
      return;
    }
    if (!window.tnt) {
      window.tnt = {};
    }
    if (!window.tnt.debug_win || window.tnt.debug_win.closed) {
      window.tnt.debug_win = window.open('', 'tnt_debug_win');
      if (!window.tnt.debug_win) {
        alert('Popup blocker probably prevented debug window from opening');
        TNT.showDebug = false;
        return;
      }
  
      window.tnt.debug_win.opener = window;
      window.tnt.debug_lineno = 0;
  
      var doc = window.tnt.debug_win.document.open("text/html", "replace");
      doc.writeln('\
        <html> \n\
        <head> \n\
        <title>Debug: ' + window.location + '</title> \n\
        <style>\n\
        BODY { \n\
          font-size: 8pt; \n\
          color: blue; \n\
        } \n\
        DIV.line { \n\
          white-space: nowrap; \n\
        } \n\
        SPAN { \n\
          display: inline-block; \n\
          word-wrap: break-word; \n\
          padding-left: 1ex; \n\
          text-overflow: ellipsis; \n\
          overflow: hidden; \n\
        } \n\
        SPAN.pop { \n\
          text-decoration: underline; \n\
          text-transform: capitalize; \n\
          color: green; \n\
          cursor: hand; \n\
        } \n\
        DIV.pop { \n\
          display: none; \n\
          position: absolute; \n\
          border: groove; \n\
          margin: 0px; \n\
          text-decoration: none; \n\
          padding: 5px; \n\
          background-color: #CCCCCC; \n\
          color: black; \n\
          overflow: visible; \n\
        } \n\
        </style> \n\
        <script> \n\
        document.onclick = function click(event) { \n\
          var elm; \n\
          var x, y; \n\
          if (window.event) { \n\
            event = window.event; \n\
            elm = event.srcElement; \n\
            x = event.clientX + document.body.scrollLeft; \n\
            y = event.clientY + document.body.scrollTop; \n\
          } else { \n\
            elm = event.target; \n\
            x = e.pageX; \n\
            y = e.pageY; \n\
          } \n\
          if (window._popdiv) { \n\
            window._popdiv.style.display = "none"; \n\
            window._popdiv = null; \n\
            return false; \n\
          } \n\
          if (elm.tagName == "SPAN" && elm.className == "pop") { \n\
            var id = elm.id.replace("s-", "d-"); \n\
            var pop = document.getElementById(id); \n\
            if (pop) { \n\
              pop.style.left = x + "px"; \n\
              pop.style.top = y + "px"; \n\
              pop.style.display = "block"; \n\
              window._popdiv = pop; \n\
            } \n\
            return false; \n\
          } \n\
          return true; \n\
        }; \n\
        window._lineno = 0; \n\
        </script> \n\
        </head> \n\
        <body> \n\
        ');
  
      window.tnt.debug_win.focus();
    }
    
    var t = String((new Date).valueOf()-TNT.startTime);
    while (t.length < 6) {
      t = '0' + t;
    }

    var pop = '';
    var html = '<div class="line">'
    html += '<span class="time">'+t+'</span>';
    var argc = arguments.length;
    for (var i = 0; i < argc; i++) {
      var arg = arguments[i];
      var t = typeof arg;
      var id = (window.tnt.debug_lineno + '-' + i);
      switch (t) {
        case 'number':
          html += '<span class="' + t + '">' + arg + '</span>';
          break;
        case 'string':
          html += '<span class="' + t + '">' + TNT.escapeHTML(arg) + '</span>';
          break;
        case 'boolean':
          html += '<span class="boolean">' + (arg ? 'true' : 'false') + '</span>';
          break;
        default:
          pop += '<div id="d-' + id + '" class="pop"><pre>' + TNT.escapeHTML(TNT.dump(arg)) + '</pre></div>';
          html += '<span id="s-' + id + '" class="pop">' + t + '</span>';
          break;
      }
    }
    html += '</div>' + pop;
  
    window.tnt.debug_win.document.writeln(html);
  
    window.tnt.debug_lineno++;
  
    window.tnt.debug_win.scrollBy(0, 1000);
  },
  
  /**
   * Copy properties of obj2 to obj1.
   *
   * Either a named list of properties or all of them if none named.
   *
   * Non enumerable properties are not copied unless named.
   * 
   * @access  public
   * @param   object      obj1        object to copy properties to
   * @param   object      obj2        object to copy properties from
   * @param   string      ...         property names
   * @return  object                  reference to obj1
   */ 
  extend : function(obj1, obj2) {
    var argc = arguments.length;
    if (argc < 2) {
      throw 'Invalid Arguments';
    }
    if (argc == 2) {
      for (var p in obj2) {
        obj1[p] = obj2[p];
      }
    } else {
      for (var i = 2; i < argc; i++) {
        var p = arguments[i];
        obj1[p] = obj2[p];
      }
    }
  },
  
  /**
   * dump a variable to a text string. Used for debugging etc.
   * 
   * Recursive references are supported.
   *
   * @access  public
   * @param   mixed       v           variable to dump
   * @param   integer     max_depth   maximum depth to dump hierachies to
   * @return  string                  textual description of object
   */ 
  dump : function(v, max_depth) {
    function _dump(v, pad, depth, max_depth) {
      var x = '';
      if (v == null) {
          x += 'null';
      } else if (typeof v == 'function') {
        x += 'Function(contents not displayed);';
      } else if (typeof v == 'object') {
        if (v instanceof Array) {
          x += 'Array[' + v.length + '] ';
        } else {
          x += 'Object[' + TNT.classof(v) + '] ';
        }
        if (max_depth >= 0 && depth >= max_depth) {
          /* do nothing */
        } else if (depth > 50) {
          x += '{ !max depth exceeded! }';
        } else if (typeof v.$__obj != 'undefined') {
          x += '{ Reference to <' + v.$__obj + '> }';
        } else {
          v.$__obj = $objs.length;
          $objs[$objs.length] = v;
          x += '<' + v.$__obj + '> {\n';
          for (var i in v) {
            if (v instanceof Array && isNaN(i)) {
              continue;
            }
            if (i == '$__obj') {
              continue;
            }
            if (i.charAt(0) == '$') {
              x += pad + '  ' + i + ' : ' + _dump(v[i], pad + '  ', depth + 1, 0);
            } else {
              x += pad + '  ' + i + ' : ' + _dump(v[i], pad + '  ', depth + 1, max_depth);
            }
          }
          x += pad + '}';
        }
      } else if (typeof v == 'boolean') {
        x += (v ? 'true' : 'false');
      } else if (typeof v == 'undefined') {
        x += '!undefined!';
      } else if (typeof v == 'string') {
        x += '"' + v.replace(/([\n\"])/g, '\\$1') + '"'
      } else {
        x += v.toString().replace(/\n/g, '\n' + pad);
      }
  
      x += '\n';
  
      return x;
    }
  
    try {
      var $objs = [];
  
      if (!TNT.isValid(max_depth)) {
        max_depth = -1;
      }
  
      var ret = _dump(v, '', 0, max_depth);
  
      var max_i = $objs.length;
      for (var i = 0; i < max_i; i++) {
        var v = $objs[i];
        delete v.$__obj;
      }
    }
    catch(e) {
    }
  
    return ret;
  },
  
  /**
   * Get a cookie from the browsers cookies.
   * object prototype to set up object classes. Single and Multiple heritance
   * are supported along with direct access to the inherited prototypes and
   * super class constructor(s).
   *
   * The method takes a single object as its argument who's properties are added
   * to the class prototype after added the properties of any super classes.
   *
   * The following properties have special meaning;
   *   $class        : name of the class (optional)
   *   $extends      : object to derive from (single heritance)
   *                   or array of objects to derive from (multiple heritance)
   *   constructor   : function to use as the class constructor
   *
   *
   * When deriving the following properties are available on the object;
   *   $super       = an object containing the merged prototypes of the super
   *                  classes this is effectively this objects prototype before
   *                  overriding with methods and properties of this class.
   *  
   * For example if 'ClassC' extends 'ClassA' and 'ClassB' then the class
   * would contructed as follows;
   *   var classC = TNT.class({
   *                           $class : 'ClassC',
   *                           $extends : [ClassA, ClassB],
   *                           constructor : function() { alert('ClassC'); },
   *                           method1 : function() {},
   *                           property1 : 'hello'
   *                          });
   *
   * By default the super class constructors are not called, however the
   * derived class can easily call these as follows;
   *    constructor : function() {
   *      this.$classA(); // Call super ClassA constructor
   *      this.$classB(); // Call super ClassA constructor
   *      };
   *
   * Note the $ prefix on a property means its part of the TNT object framework,
   * no user properties should start with a $.
   * 
   * @access  public
   * @param   object      obj         properties to assign to the prototype
   * @return  function                reference to the constructor function
   */ 
  declareClass : function(c) {
    if (typeof c != 'object') {
      alert('Class definition missing');
      return null;
    }
  
    if (c.constructor) {
      if (typeof c.constructor != 'function') {
        alert('Class definition "constructor" must be a function');
        return null;
      }
    } else {
      // Dummy constructor
      c.constructor = function() {};
    }
  
    if (c._destructor) {
      if (typeof c._destructor != 'function') {
        alert('Class definition "destructor" must be a function');
        return null;
      }
    } else {
      // Dummy destructor
      c._destructor = function() {};
    }
  
    if (c.$extends) {
      if (typeof c.$extends == 'object' && typeof c.$extends.length == 'number') {
        // Assume an array of classes to extend from - multiple inheritance
        for (var o in c.$extends) {
          if (typeof c.$extends[o].prototype != 'object') {
            alert('Class definitions "$extends['+o+']" must be a prototyped object');
          }
          throw 'Multiple inheritance not supported yet';
        }
      } else {
        // Assume an object to inherit from - single inheritance
        if (typeof c.$extends.prototype != 'object') {
          alert('Class definitions "$extends" must be a prototyped object');
        }

        // build helper function
        // this omits the initial constructor call while inherit properties
        var f = new Function;
        f.prototype = c.$extends.prototype;

        c.constructor.prototype = new f;
        c.constructor.prototype.$super = c.$extends.prototype;
        c.constructor.prototype.constructor = c.constructor;
        c.constructor.prototype.$class = c.$class;
        c.constructor.prototype.$extends = c.$extends;
      }
    }
  
    TNT.extend(c.constructor.prototype, c);
  
    // toString and toValue are not enumerable
    if (c.hasOwnProperty('toString')) {
      c.constructor.prototype.toString = c.toString;
    }
    if (c.hasOwnProperty('toValue')) {
      c.constructor.prototype.toValue = c.toValue;
    }
  
  
    return c.constructor;
  },

  /**
   * Set a browser cookie.
   * 
   * @access  public
   * @param   string      key         cookie key
   * @param   string      value       cookie value
   * @return  void
   */ 
  setCookie : function(key, value) {
    document.cookie = key + '=' + escape(value);
  },

  /**
   * Get a browser cookie.
   * 
   * @access  public
   * @param   string      key         cookie key
   * @return  string                  cookie value
   */ 
  getCookie : function(key) {
    if (!document.cookie) {
      return null;
    }

    var cookies = ' ' + document.cookie;

    var start = cookies.indexOf(' ' + key + '=');
    if (start == -1) {
      return null;
    }

    var end = cookies.indexOf(';', start);
    if (end == -1) {
      end = cookies.length;
    }

    var cookie = cookies.substr(start, end - start);
    var equpos = cookie.indexOf('=');

    return unescape(cookie.substr(equpos + 1, cookie.length - equpos + 1));
  },

  /**
   * Format a string according to a format specification.
   *
   * Based on the standard method found in all 'C' libraries.
   * 
   * @access  public
   * @param   string      fmt         format string
   * @param   mixed       ...         optional parameters to the format string
   * @return  string                  formatted string
   */ 
  sprintf : function(/*fmt, ...*/) {
  	if (!arguments || arguments.length < 1 || !RegExp) {
  		return;
  	}
  	var str = arguments[0];
  	var re = /([^%]*)%('.|0|\x20)?(-)?(\d+)?(\.\d+)?(%|b|c|d|u|f|o|s|x|X)(.*)/;
  	var a = b = [], numSubstitutions = 0, numMatches = 0;
  	while (a = re.exec(str)) {
  		var leftpart = a[1], pPad = a[2], pJustify = a[3], pMinLength = a[4];
  		var pPrecision = a[5], pType = a[6], rightPart = a[7];
  		
  		numMatches++;
  		if (pType == '%') {
  			subst = '%';
  		} else {
  			numSubstitutions++;
  			if (numSubstitutions >= arguments.length) {
  				alert('Error! Not enough function arguments (' + (arguments.length - 1) + ', excluding the string)\nfor the number of substitution parameters in string (' + numSubstitutions + ' so far).');
  			}
  			var param = arguments[numSubstitutions];
  			var pad = '';
        if (pPad && pPad.substr(0,1) == "'")
          pad = leftpart.substr(1,1);
  			else if (pPad)
          pad = pPad;
  			var justifyRight = true;
        if (pJustify && pJustify === "-")
          justifyRight = false;
  			var minLength = -1;
        if (pMinLength)
          minLength = parseInt(pMinLength);
  			var precision = -1;
  			if (pPrecision)
          precision = parseInt(pPrecision.substring(1));
  			var subst = param;
  			if (pType == 'b')
          subst = parseInt(param).toString(2);
  			else if (pType == 'c')
          subst = String.fromCharCode(parseInt(param));
  			else if (pType == 'd')
          subst = parseInt(param) ? parseInt(param) : 0;
  			else if (pType == 'u')
          subst = Math.abs(param);
  			else if (pType == 'f')
          subst = (precision > -1) ? parseFloat(param).toFixed(precision) : parseFloat(param);
  			else if (pType == 'o')
          subst = parseInt(param).toString(8);
  			else if (pType == 's')
          subst = param;
  			else if (pType == 'x')
          subst = ('' + parseInt(param).toString(16)).toLowerCase();
  			else if (pType == 'X')
          subst = ('' + parseInt(param).toString(16)).toUpperCase();

         var padding = "";
         if (minLength > 0) {
           var length = (pType == 'f' ? subst.toString().split('.')[0].length : subst.toString().length);
           var padLeft = minLength - length;
           if (padLeft > 0) {
              var arrTmp = new Array(padLeft+1);
              padding = arrTmp.join(pad?pad:" ");
           }
         }
  		}
  		str = leftpart + padding + subst + rightPart;
  	}
    return str;
  },

  /**
   * Covert a decimal number to a hexadecimal string.
   *
   * 
   * @access  public
   * @param   number      decimal     decimal number
   * @param   number      size        left pad to this length
   * @return  string                  hexadecimal string
   */ 
  decToHex : function(decimal, size) {
    var digits = '0123456789ABCDEF';

    var temp = parseInt(decimal,10);
    if (isNaN(temp)) { 
      TNT.debug("decToHex() - Not a decimal number: "+decimal);
      return 0;
    }

    var hex = "";
    while (temp > 15) {
       remainder = temp % 16;
       hex = digits.charAt(remainder) + "" + hex;
       temp = parseInt(temp / 16);
    }
    if (temp >= 0) {
       hex = digits.charAt(temp) + "" + hex;
    } else {
       return 0;
    }

    while (hex.length < size) {
      hex = '0'+hex;
    }

    return hex;
  },

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
  isArrayIndex : function(i, a) {
    return ((a && TNT.isEnumerable(a) && TNT.isSet(a[i])) ? true : false);
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
  isArrayValue : function(v, a) {
    if (!a || !TNT.isEnumerable(a)) {
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
   * Return an array of the indices of an array.
   *
   * Any object can be used for the array argument, and the method will
   * return an array of its enumerable properties.
   * 
   * @access  public
   * @param   array       a           array to search
   * @return  array                   array of the indices
   */ 
  arrayIndices : function(i, a) {
    if (!a || !TNT.isEnumerable(a)) {
      return null;
    }
    var ret = [];
    for (i in a) {
      ret.push(i);
    }
    return ret;
  },

  /**
   * Return an array of the values of an array.
   *
   * Any object can be used for the array argument, and the method will
   * return an array of its enumerable properties.
   * 
   * @access  public
   * @param   array       a           array to search
   * @return  array                   array of the indices
   */ 
  arrayValues : function(i, a) {
    if (!a || !TNT.isEnumerable(a)) {
      return null;
    }
    var ret = [];
    for (i in a) {
      ret.push(a[i]);
    }
    return ret;
  },

  /**
   * Returns array member or object property if it exists,
   * otherwise returns default value.
   *
   * @access  public
   * @param   oject       obj         array/object to check
   * @param   scalar      key         element/property to get
   * @param   mixed       def         default to return if not set
   * @return  boolean                 true array has given key
   */ 
  item : function(obj, key, def) {
    return ((typeof obj != 'object' || typeof obj[key] == 'undefined') ? def : obj[key]);
  },

  /**
   * Convert an associative array to CSS string
   *
   * Each property of the given object is a CSS style name and its value
   * a CSS value suitable for that style name.
   *
   * @access  public
   * @param   object      style       hash of CSS styles
   * @return  string                  CSS style string
   */ 
  makeCSS : function(style) {
  
    var a, text = '';
    for (a in style) {
      if (style[a] == null || style[a] == '') {
        continue;
      }
      if (a == 'css') {
        text += (style[a]+' ');
      } else {
        text += (a+': '+style[a]+'; ');
      }
    }
  
    return text;
  },
  
  /**
   * Convert a CSS string to an hash.
   *
   * Each property of the returned object is a CSS style name and its value
   * a CSS value suitable for that style name.
   *
   * @access  public
   * @param   string      style       CSS styles string
   * @return  array                   hash of CSS styles
   */ 
  splitCSS : function(style) {
    var ret = {};
    var style;
    var styles = style.split(';');
    for (var i = 0, max_i = styles.length; i < max_i; i++) {
      if (styles[i].match(/^\s*$/)) {
        continue;
      }
      style = styles[i].match(/^\s*([\w-]+)\s*:\s*(.*)$/);
      if (style) {
        ret[style[1]] = style[2];
      } else {
        throw 'splitCSS() - "' + styles[i] + '" is not a valid CSS markup';
      }
    }

    return ret;
  },
  
  /**
   * Convert an hash to HTML attribute string
   *
   * Each property of the given object is a HTML attribute name and its value
   * a attribute value suitable for that attribute name.
   *
   * @access  public
   * @param   array       style       hash of HTML attributes
   * @return  string                  HTML attribute string
   */ 
  makeAttrs : function(attrs) {
  
    var a, text = '';
    for (a in attrs) {
      if (attrs[a] == null) {
        continue;
      }
      if (a == 'attrs') {
        text += (attrs[a]+' ');
      } else if (a == 'style') {
        text += (a+'="'+TNT.escapeHTML(TNT.makeCSS(attrs[a]))+'" ');
      } else if (attrs[a] != '') {
        text += (a+'="'+TNT.escapeHTML(attrs[a])+'" ');
      }
    }
  
    return text;
  },

  /**
   * Convert a HTML attribute string to an associative array.
   *
   * Each property of the returned object is a HTML attribute name and its value
   * a attribute value suitable for that attribute name.
   *
   * @access  public
   * @param   string      style       HTML attributes string
   * @return  array                   hash of HTML attributes
   */ 
  splitAttrs : function(attrs) {
    throw 'splitAttrs() - method is currently not supported';
    return null;
  },

  /**
   * Make a HTML class name list given an array of CSS class names and
   * CSS psuedo states.
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
   * selector '.mydiv.clicked' correctly, we actually append the suffix
   * '--clicked' to the elements current class name resulting in the element
   * having the class name 'mydiv mydiv--clicked' and then you can use the CSS
   * selector '.mydiv--clicked' to uniquely select all clicked elements with
   * the class 'mydiv'.
   *
   * Multiple states can be set resulting in longer and more complex class
   * names, ie. Setting the 'clicked' and 'opened' states would result in the
   * element being given the class names 'mydiv mydiv--clicked mydiv--opened
   * mydiv--clicked-opened mydiv--opened-clicked'.
   *
   * The states are separated from the class name using '--' and from each other
   * using '-'.
   *
   * The drawback with this method is that the order of states must remain
   * consistant to keep the CSS selectors simple. Therefore the states are
   * added in alphabetical order.
   *
   * @access  public
   * @param   array       classes     array of CSS class names
   * @param   array       states      array of CSS Psuedo states
   * @return  string                  className string
   */ 
  makeClassName : function(classes, states) {
    var i, max_i, j, max_j, s;
 
    if (!classes && !states) {
      return '';
    }

    if (!TNT.isArray(classes)) {
      s = classes.toString().trim();
      classes = [];
      if (cn != '') {
        classes = s.split(/\\s+/);
      }
    }

    if (!states) {
      return classes.join(' ');
    }

    if (!TNT.isArray(states)) {
      s = states.toString().trim();
      states = [];
      if (s != '') {
        states = s.split(/\\s+/);
      }
    }

    // sort the states;
    states = states.sort();
  
    var sufixes = [];
  
    max_i = states.length;
    max_j = (1 << max_i);
    for (j = 1; j < max_j; j++) {
      s = '';
      for (i = 0; i < max_i; i++) {
        if (j & (1 << i)) {
          s += '-' + states[i];
        }
      }
      sufixes.push(s.substr(1));
    }
   
    if (classes.length) {
      s = classes.join(' ') + ' ';
    } else {
      s = '';
      classes = ['juic'];
    }
  
    max_i = classes.length;
    max_j = sufixes.length;
    for (i = 0; i < max_i; i++) {
      var cn = classes[i];
      if (cn.substr(0, 1) != '_' && !cn.match(/layout/)) {
        for (j = 0; j < max_j; j++) {
          s += cn + '--' + sufixes[j] + ' ';
        }
      }
    }
  
    return s;
  },

  /**
   * Get the classes and states applied to a class name.
   *
   * This method returns an array of classes and states that were passed to
   * the {@link makeClassName()} method to generate the given string.
   *
   * @access  public
   * @param   string      className   className to split
   * @return  object                  hash with 'classes' and 'states' entries
   */ 
  splitClassName : function(className) {
    if (!className || className.trim() == '') {
      return {'classes' : [], 'states' : []};
    }

    var cns = className.split(/\s+/);
  
    var classes = [];
    var states = '', max_l = 0, l, cn, ss, p;
    for (var i = 0, max_i = cns.length; i < max_i; i++) {
      cn = cns[i];
      if ((p = cn.indexOf('--')) !== -1) {
        if ((l = (ss = cn.substr(p + 2)).length) > max_l) {
          states = ss;
  	      max_l = l;
        }
      } else {
        classes.push(cn);
      }
    }

    return {'classes' : classes, 'states' : (states != '' ? states.split('-') : [])};
  },

  /**
   * Escape the HTML tags in the string
   *
   * If the argument is not a string then nothing is done.
   *
   * @access  public
   * @param   string      str         escape the string
   * @return  string                  escaped string
   */ 
  escapeHTML : function(str) {
    return ((typeof str === 'string') ? str.escapeHTML() : str);
  },

  /**
   * Create a new item of the given class using an array as the
   * arguments to the constructor.
   *
   * This is similar to Function.apply() but can be used with the
   * constructor.
   *
   * (cf. "return new <type>.apply(<args>)")
   *
   * @access  public
   * @param   mixed       type        class or name of class to construct
   * @param   array       args        array of arguments to pass to constructor
   * @return  object                  constructed object
   */ 
  vnew : function(type, args) {

    var a = [];
    for (i = 0, l = args.length; i < l; i++) {
      a.push = 'args['+i+'],';
    }

    if (typeof type != 'string') {
      type = TNT.classof(type);
    }

    eval('var o = new '+type+'(' + a + ');'); 

    return o;
  },

  /**
   * Get a possibly dynamic value
   *
   * If the value is a function then it returns the result of calling that
   * function, otherwise the it returns the value unaltered.
   *
   * @access  public
   * @param   mixed       value       dynamic value
   * @return  mixed                   resulting value
   */ 
  dynamicValue : function(value) {
    return (typeof value == 'function' ? value() : value);
  }

}

TNT.startTime = (new Date()).valueOf();

/* vim: set expandtab tabstop=2 shiftwidth=2: */
