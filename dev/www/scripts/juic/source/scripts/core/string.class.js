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
 * String Javascript
 * ~~~~~~~~~~~~~~~~~
 *
 * Extensions to the standard Javascript String Object.
 *
 * @package TNT.Juic
 * @subpackage Func
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

/**
 * Compare two strings.
 *
 * Returns: -1 if this is less than str;
 *           1 if this is greater than str, and
 *           0 if they are equal. 
 * 
 * The strnicmp() compares not more than n characters. 
 *
 * @access  public
 * @param   string      str         string to compare with
 * @return  integer                 -1, 0, or 1
 */ 
String.prototype.strcmp = function(str) {
  return (this < str ? -1 : (this > str ? 1 : 0));
};

/**
 * Compare two strings ignoring their case.
 *
 * Returns: -1 if this is less than str;
 *           1 if this is greater than str, and
 *           0 if they are equal. 
 *
 * @access  public
 * @param   string      str         string to compare with
 * @return  integer                 -1, 0, or 1
 */ 
String.prototype.stricmp = function(str) {
  var s1 = this.toLowerCase();
  var s2 = str.toLowerCase();
  return (s1 < s2 ? -1 : (s1 > s2 ? 1 : 0));
};

/**
 * Compare the suffix of two strings
 *
 * Returns: -1 if this is less than str;
 *           1 if this is greater than str, and
 *           0 if they are equal. 
 * 
 * The strncmp() compares not more than num characters. 
 *
 * @access  public
 * @param   string      str         string to compare with
 * @param   integer     num         number of characters to compare
 * @return  integer                 -1, 0, or 1
 */ 
String.prototype.strncmp = function(str, num) {
  var s1 = this.substr(0, num);
  var s2 = str.substr(0, num);
  return (s1 < s2 ? -1 : (s1 > s2 ? 1 : 0));
};

/**
 * Compare the suffix of two strings ignoring their case.
 *
 * Returns: -1 if this is less than str;
 *           1 if this is greater than str, and
 *           0 if they are equal. 
 * 
 * The strnicmp() compares not more than num characters. 
 *
 * @access  public
 * @param   string      str         string to compare with
 * @param   integer     num         number of characters to compare
 * @return  integer                 -1, 0, or 1
 */ 
String.prototype.strnicmp = function(str, num) {
  var s1 = this.substr(0, num).toLowerCase();
  var s2 = str.substr(0, num).toLowerCase();
  return (s1 < s2 ? -1 : (s1 > s2 ? 1 : 0));
};

/**
 * Strip whitespace from left of the string.
 *
 * @access  public
 * @param   string      str         string to left strip
 * @return  string                  the stripped space
 */ 
String.prototype.ltrim = function(str) {
  return str.replace(/^\\s+/, '');
};

/**
 * Strip whitespace from right of the string.
 *
 * @access  public
 * @param   string      str         string to right strip
 * @return  string                  the stripped space
 */ 
String.prototype.rtrim = function(str) {
  return str.replace(/\\s+$/, '');
};

/**
 * Strip whitespace from right of the string.
 *
 * @access  public
 * @param   string      str         string to right strip
 * @return  string                  the stripped space
 */ 
String.prototype.trim = function() {
  return this.replace(/^\\s+|\\s+$/g, '');
};

/**
 * Add a string to a separated string list
 *
 * @access  public
 * @param   string      str         string to add
 * @param   string      sep         string separator (defaults to comma)
 * @return  string                  string with value added
 */ 
String.prototype.addCsv = function(str, sep) {
  if (this == str) {
    return this;
  } else if (this == QxConst.CORE_EMPTY) {
    return str;
  } else {
    if (!TNT.isValid(sep)) {
      sep = ',';
    }

    var a = this.split(sep);

    if (a.indexOf(str) == -1) {
      a.push(str);
      return a.join(sep);
    } else {
      return this;
    }
  }
};

/**
 * Delete a string from a separated string list
 *
 * @access  public
 * @param   string      str         string to delete
 * @param   string      sep         string separator (defaults to comma)
 * @return  string                  string with value deleted
 */ 
String.prototype.delCsv = function(str, sep) {
  if (this == str || this == '') {
    return '';
  } else {
    if (!TNT.isValid(sep)) {
      sep = ',';
    }

    var a = this.split(sep);
    var p = a.indexOf(str);

    if (p == -1) {
      return this;
    }

    do {
      a.splice(p, 1);
    } while ((p = a.indexOf(str)) != -1);

    return a.join(sep);
  }
};

/**
 * String XML tags from the string.
 *
 * @access  public
 * @return  string                  string with tags deleted
 */ 
String.prototype.stripTags = function() {
  return this.replace(/<\/?[^>]+>/gi, '');
};

/**
 * Return true if string contains the given string
 *
 * @access  public
 * @param   string      str         containing string
 * @return  boolean                 true if string contains string
 */ 
String.prototype.contains = function(str) {
  return (this.indexOf(str) != -1);
};

/**
 * Return true if string starts with given prefix.
 *
 * @access  public
 * @param   string      str         prefix string
 * @return  boolean                 true if string has given prefix
 */ 
String.prototype.startsWith = function(str) {
  return (this.indexOf(str) === 0);
};

/**
 * Return true if string ends with given suffix.
 *
 * @access  public
 * @param   string      str         suffix string
 * @return  boolean                 true if string has given suffix
 */ 
String.prototype.endsWith = function(str) {
  return this.lastIndexOf(str) === this.length-str.length;
};

/**
 * Right pad string to given length using pad character.
 *
 * @access  public
 * @param   integer     len         length to pad string to
 * @param   string      pad         character to pad with (defaults to space)
 * @return  string                  padded string
 */ 
String.prototype.lpad = function(len, pad) {
  if (TNT.isValid(pad) || pad == '') {
    pad = ' ';
  } else {
    pad = pad.charAt(0);
  }

  var temp = '';
  for (var i = len, l = this.len; l < i; l++) {
    temp += pad;
  }

  return pad + this;
};

/**
 * Right pad string to given length using pad character.
 *
 * @access  public
 * @param   integer     len         length to pad string to
 * @param   string      pad         character to pad with (defaults to space)
 * @return  string                  padded string
 */ 
String.prototype.rpad = function(len, pad) {
  if (TNT.isValid(pad) || pad == '') {
    pad = ' ';
  } else {
    pad = pad.charAt(0);
  }

  var temp = '';
  for (var i = len, l = this.len; l < i; l++) {
    temp += pad;
  }

  return this + temp;
};

/**
 * Convert the first character to uppercase.
 *
 * @access  public
 * @return  string                  string with first character capitalized
 */ 
String.prototype.ucfirst = function() {
  return (this.length > 0 ? this.charAt(0).toUpperCase() + this.substr(1) : this);
};

String.prototype.camelCase = function()
{
  var vArr = this.split(/[-_]+/);
  var vLength = vArr.length;
  var vBit, vNew = '';

  if ((vBit = vArr[0]) != '') {
    vNew += vBit.charAt(0).toLowerCase() + vBit.substr(1);
  }

  for (var i = 1; i < vLength; i++) {
    if ((vBit = vArr[i]) != '') {
      vNew += vBit.charAt(0).toUpperCase() + vBit.substr(1);
    }
  }

  return vNew;
};

/**
 * Truncate the string.
 *
 * If the text contains HTML tags then nothing is done.
 *
 * @access  public
 * @param   number      len1        maximum length of a single word
 * @param   number      len2        maximum length of string
 * @return  string                  truncated string
 */ 
String.prototype.truncate = function(len1, len2) {

  // We dont do any of this if the text is HTML :-).
  // We assume in that case the text has been preformatted
  if (this.length < len1 ||
      (this.indexOf('<') != -1 && this.indexOf('>') != -1)) {
    return this;
  }

  // We have to alter the text so no one word is over len characters.
  // If we find a word over len1 characters we truncate it to len1 - 3
  // characters and append '...'. The whole text is truncated at len2
  // character regardless.
  if (!len2) {
    len2 = (len1 * 2);
  }

  var words = this.split(/\s+/g);

  // Ensure no word is longer that len1
  var text = '';
  var w;
  for (w in words) {
    var word = words[w];
    if (word.length > len1) {
      text += word.substr(0, len1 - 3)+'...';
      break;
    } else {
      text += word;
    }
    if (text.length > len2) {
      text = text.substr(0, len2 - 3)+'...';
      break;
    }
    text += ' ';
  }

  return text;
};


/**
 * Escape HTML characters in the string
 *
 *
 * @access  public
 * @return  string                  escaped string
 */ 
String.prototype.escapeHTML = function() {

  var reps = {
    '&amp;' : /\&/g,
    '&lt;' : /</g,
    '&gt;' : />/g,
    '&quot;' : /"/g,
    '&#39;' : /'/g
  };

  var str = this;
  for (c in reps) {
    str = str.replace(reps[c], c);
  }
  
  return str;
};
  
/* vim: set expandtab tabstop=2 shiftwidth=2: */
