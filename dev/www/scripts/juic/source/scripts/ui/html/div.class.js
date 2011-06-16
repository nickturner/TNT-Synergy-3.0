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
 * TNT.Juic.HTML.Div Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Basic DIV HTML element.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

if (!TNT.Juic.HTML) { TNT.Juic.HTML = {}; }

TNT.Juic.HTML.Div = TNT.declareClass({
  $class : 'TNT.Juic.HTML.Div',
  $extends : TNT.Juic.Core.HTML,

  /**
   * Constructor
   *
   * @access  public
   * @return  object                  reference to constructed object
   */
  constructor : function() {
    TNT.Juic.Core.HTML.prototype.constructor.call(this);
    this.setTagName('div');
  }

});

/* vim: set expandtab tabstop=2 shiftwidth=2: */
