///////////////////////////////////////////////////////////////////////////////
// TNT.Juic. Javascript User Interface Components                    v2.0.01 //
// ==============================================                            //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-juic.com                                                    //
// http://www.tnt-juic.com/                                                  //
//                                                                           //
// TNT.Juic.is an Javascript library of advanced user interface components   //
// for use on web based forms.                                               //
//                                                                           //
// WARNING: This package is protected by copyright law and international     //
// treaties. Unauthorized reproduction or distribution of this package,      //
// or any portion of it, may result in severe civil and criminal penalties,  //
// and will be prosecuted to the maximum extent possible under the law.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * TNT.Juic.Ctrl.Icon Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Basic icon widget.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

if (!TNT.Juic.Ctrl) { TNT.Juic.Ctrl = {}; }

TNT.Juic.Ctrl.Icon = TNT.declareClass({
  $class : 'TNT.Juic.Ctrl.Icon',
  $extends : TNT.Juic.Ctrl.Ctrl,

  /**
   * Constructor
   *
   * @access  public
   * @param   string      src         path to icon to display
   * @param   string      width       width of icon (optional)
   * @param   string      height      height of icon (optional)
   * @return  object                  reference to constructed object
   */
  constructor : function(src, width, height) {
    TNT.Juic.Ctrl.Ctrl.prototype.constructor.call(this);

    this.setTagName('span');

    this._img = new TNT.Juic.HTML.Img();

    // Replicate some of our properties to the image
    this.attachEventHandler('onpropertychange', function(event) {
      switch (event.propertyName) {
        case 'style.height':
          this._img.setStyle('height', this.getStyle('height'));
          break;
        case 'style.width':
          this._img.setStyle('width', this.getStyle('width'));
          break;
        case 'style.vertical-align':
          this._img.setStyle('vertical-align', this.getStyle('vertical-align'));
          break;
        case 'attribute.title':
          this._img.setAttribute('alt', this.getToolTip());
          break;
        case 'attribute.align':
          this._img.setAttribute('align', this.getAttribute('align'));
          break;
      }
    });

    this.setContent('img', this._img);

    this.setSrc(null);

    if (TNT.isValid(src)) {
      this.setSrc(src);
    }

    if (TNT.isValid(width) || TNT.isValid(height)) { 
      this.setDimension(width, height);
    }
  },

  /**
   * HTML IMG object to display the image.
   * @access  private
   * @var     object                  reference to HTML IMG object
   */
  _img : null,
  
  /**
   * Set the source of the image to display.
   *
   * @access  public
   * @param   string      src         image source path
   * @return  void
   */
  setSrc : function(src) {
    this.setState('blank', !src);
    if (this._img.setSrc(src)) {
      this.raiseEvent('onsrcchange');
      return true;
    }
    return false;
  },
  
  /**
   * Get the source of the image to display.
   *
   * @access	public
   * @param   string      src         image source path
   * @return  void
   */
  getSrc : function(src) {
    return this._img.getSrc();
  },
  
  /**
   * Destructor
   *
   * @access  public
   * @return  void
   */
  _destructor : function() {

  }

});

/* vim: set expandtab tabstop=2 shiftwidth=2: */
