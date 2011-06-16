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
 * TNT.Juic.HTML.Img Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Basic IMG HTML element.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

if (!TNT.Juic.HTML) { TNT.Juic.HTML = {}; }

TNT.Juic.HTML.Img = TNT.declareClass({
  $class : 'TNT.Juic.HTML.Img',
  $extends : TNT.Juic.Core.HTML,

  /**
   * The image src.
   *
   * Set to null to display a blank image.
   *
   * @public
   * @param   string      src         path to image source file
   * @return  void
   */
  setSrc : function(src) {
    if (!src) {
      src = JUIC_DIR + 'images/blank.gif';
    }
    this.setAttribute('src', src);
  },
  
  /**
   * Get the image src.
   *
   * @public
   * @return  number                  controls tab index
   */
  getSrc : function(src) {
    var src = this.getAttribute('src');
    if (src == JUIC_DIR + 'images/blank.gif') {
      src = null;
    }
    return src;
  },

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function() {
    TNT.Juic.Core.HTML.prototype.constructor.call(this);
    this.setTagName('img');

    // The alt attribute is mandatory but not used as we use the 'title'
    // attribute to set a tooltip.
    this.setAttribute('alt', '');
  }

});

/* vim: set expandtab tabstop=2 shiftwidth=2: */
