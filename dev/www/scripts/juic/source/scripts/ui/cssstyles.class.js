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
 * TNT.Juic.UI.CssStyles Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Class for a hash of HTML CSS styles. Dynamic styles are supported by
 * storing a callback function as the style value.
 *
 * No validation is made on the style names or values, they must be escaped
 * for HTML reserved characters etc.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Styles = TNT.declareClass({
  $class : 'TNT.Juic.Core.Styles',
  $extends : TNT.Juic.Core.Hash,

  /**
   * Constructor
   *
   * @access  public
   * @param   object    context       context for resolving dynamic values
   * @return  object                  reference to constructed object
   */
  constructor : function(context) {
    TNT.Juic.Core.Hash.prototype.constructor.call(this, context);

  },

  /**
   * Set a style.
   *
   * This method is overridden to allow a single CSS formatted string argument
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
      var styles = TNT.splitCSS(key);
      for (var s in styles) {
        TNT.Juic.Core.Hash.prototype.set.call(this, s, styles[s]);
      }
      return old; 
    }
    return TNT.Juic.Core.Hash.prototype.set.call(this, key, value);
  },

  /**
   * Return the value of the hash as a string.
   *
   * This returns a valid CSS markup string for use as the value for an HTML
   * 'style' attribute.
   *
   * The result of this method is cached for speed and will return the cached
   * value until the contents of the hash alters invalidating the cache.
   *
   * @access  public
   * @return  string                  string representation of item
   */
  toString : function() {
    if (this._text == null) {
      this._text = this.join(';', ':');
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

