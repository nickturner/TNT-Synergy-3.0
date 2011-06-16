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
 * TNT.Juic.Core.Contents Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Class for a hash of HTML contents. Dynamic attributes are supported by
 * storing a callback function as the attributes value.
 *
 * The content can be any value that can be converted to a string.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

TNT.Juic.Core.Contents = TNT.declareClass({
  $class : 'TNT.Juic.Core.Contents',
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
   * Return the value of the hash as a string.
   *
   * This returns the concatenated string value of each item in the hash using
   * no separators.
   *
   * The result of this method is cached for speed and will return the cached
   * value until the contents of the hash alters invalidating the cache.
   *
   * @access  public
   * @return  string                  string representation
   */
  toString : function() {
    if (this._text == null) {
      this._text = '';
      var max = this._keys.length;
      for (var i = 0; i < max; i++) {
        this._text += this.get(this._keys[i]);
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

