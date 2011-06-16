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
 * TNT.Juic.UI.Attributes Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Class for a hash of HTML attributes. Dynamic attributes are supported by
 * storing a callback function as the attributes value.
 *
 * No validation is made on the attribute names or values, they must be escaped
 * for HTML reserved characters etc.
 *
 *
 * @package TNT.Juic
 * @subpackage UI
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Attributes = TNT.declareClass({
  $class : 'TNT.Juic.UI.Attributes',
  $extends : TNT.Juic.Core.Hash,

  /**
   * Constructor
   *
   * @access  public
   * @param   object    owner         owning object
   * @return  object                  reference to constructed object
   */
  constructor : function(owner) {
    TNT.Juic.Core.Hash.prototype.constructor.call(this, owner);

  },

  /**
   * Set an attribute.
   *
   * This method is overridden to allow a single HTML formatted string argument
   * which is parsed and each style recursively set.
   *
   * @access  public
   * @param   string    name          items name
   * @param   mixed     value         items value
   * @return  mixed                   items old value (or undefined)
   */
  set : function(key, value) {
    if (arguments.length == 1 && typeof key == 'string') {
      var old = this.toString();
      var attrs = TNT.splitAttr(key);
      for (var a in attrs) {
        TNT.Juic.Core.Hash.prototype.set.call(this, a, attrs[a]);
      }
      return old; 
    }
    return TNT.Juic.Core.Hash.prototype.set.call(this, key, value);
  },

  /**
   * Return the value of the hash as a string.
   *
   * This returns a valid HTML attribute list for use withing an HTML tag.
   *
   * NB: This method does not validate the attributes value, it assumes
   * valid attributes are stored by the caller, this includes all HTML
   * reserved character escaping.
   *
   * The result of this method is cached for speed and will return the cached
   * value until the contents of the hash alters invalidating the cache.
   *
   * @access  public
   * @return  string                  string representation of item
   */
  toString : function() {
    if (this._text == null) {
      this._text = '';
      var key, max = this._keys.length;
      for (var i = 0; i < max; i++) {
        key = this._keys[i];
        this._text += (key + '="' + this.get(key) + '" ');
      }
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

