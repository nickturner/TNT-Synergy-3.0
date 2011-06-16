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
 * TNT.Juic.Ctrl.Atom Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Basic atom building block, consisting of an icon and a label.
 *
 * @package TNT.JUIC
 * @subpackage Core
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */

if (!TNT.Juic.Ctrl) { TNT.Juic.Ctrl = {}; }

TNT.Juic.Ctrl.Atom = TNT.declareClass({
  $class : 'TNT.Juic.Ctrl.Atom',
  $extends : TNT.Juic.Ctrl.Ctrl,

  /**
   * Constructor
   *
   * @access  public
   * @param   string      text        text to display with atom (optional)
   * @param   string      iconSrc     icon to display with atom (optional)
   * @param   string      iconWidth   width of icon (optional)
   * @param   string      iconHeight  height of icon (optional)
   * @return  object                  reference to constructed object
   */
  constructor : function(text, iconSrc, iconWidth, iconHeight) {
    TNT.Juic.Ctrl.Ctrl.prototype.constructor.call(this);

    //this.setTagName('span');

    //this.addClass('juic-atom');
    //this.addClass(function() { return 'juic-atom-layout-' + this.getIconPos() });

    this._icon = new TNT.Juic.Ctrl.Icon();
    //this._icon.addClass('juic-atom-icon');
    this._name = new TNT.Juic.HTML.Span();
    //this._name.addClass('juic-atom-name');

/*
    this._iconGap = 0;
    this._iconPos = 'left';

    this.setContent('icon', this._icon);
    this.setContent('text', this._name);

    if (TNT.isValid(text)) {
      this.setText(text);
    }

    if (TNT.isValid(iconSrc)) {
      this.setIconSrc(iconSrc);

      if (TNT.isValid(iconWidth)) {
          this.setIconWidth(iconWidth);
      }

      if (TNT.isValid(iconHeight)) {
          this.setIconHeight(iconHeight);
      }
    }
*/

  },

  /**
   * HTML SPAN object to display the name.
   * @access  private
   * @var     object                  reference to HTML SPAN object
   */
  _name : null,

  /**
   * HTML IMG object to display the image.
   * @access  private
   * @var     object                  reference to HTML IMG object
   */
  _icon : null,
  
  /**
   * Gap between the image and the text (in CSS units).
   * @access  private
   * @var     string                  gap in CSS format
   */
  _iconGap : 0,

  /**
   * Position of icon relative to the text.
   * @access  private
   * @var     string                  position (left|right|top|bottom)
   */
  _iconPos : 'left',

  /**
   * Return a reference to the internal icon widget.
   *
   * @access	public
   * @return  object                  TNT.Juic.Ctrl.Icon widget
   */
  icon : function() {
    return this._icon;
  },
  
  /**
   * Return a reference to the internal name widget.
   *
   * @access	public
   * @return  object                  TNT.Juic.Ctrl.Span widget
   */
  name : function() {
    return this._name;
  },
  
  /**
   * Set the name to display. HTML formatted.
   *
   * @access	public
   * @param   string      name        name to display
   * @return  boolean                 true if changed, false if unchanged
   */
  setName : function(name) {
    if (this._name.setContent('html', name)) {
      this._changedProperty('name');
    }
  },
  
  /**
   * Get the name to display.
   *
   * @access	public
   * @return  string                  name to display
   */
  getName : function() {
    return this._name.getContent('html');
  },

  /**
   * Set the icon to display.
   *
   * Alias for {@link setIconSrc()}
   *
   * @access	public
   * @param   string      icon        icon to display
   * @return  boolean                 true if changed, false if unchanged
   * @see     setIconSrc
   */
  setIcon : function(icon) {
    return this.setIconSrc(icon);
  },
  
  /**
   * Get the icon to display.
   *
   * Alias for {@link setIconSrc()}
   *
   * @access	public
   * @return  string                  icon to display
   * @see     getIconSrc
   */
  getIcon : function() {
    return this.getIconSrc();
  },

  /**
   * Set the source of the icon to display.
   *
   * @access	public
   * @param   string      iconSrc     icon source path
   * @return  boolean                 true if changed, false if unchanged
   */
  setIconSrc : function(iconSrc) {
    if (this._icon.setSrc(iconSrc)) {
      this._changedProperty('iconsrc');
    }
  },
  
  /**
   * Get the source of the icon to display.
   *
   * @access	public
   * @return  string                  icon source path
   */
  getIconSrc : function() {
    return this._icon.getSrc();
  },

  /**
   * Set the dimensions of the icon to display.
   *
   * This is the size of the control using box-sizing, ie the size of the
   * area that the control takes including any borders and padding.
   *
   * Pass any value as undefined to leave it unchanged.
   *
   * @access	public
   * @param   string    width         valid CSS length for width
   * @param   string    height        valid CSS length for height
   * @return  boolean                 true if changed, false if unchanged
   */
  setIconDim : function(width, height) {
    if (this._icon.setDim(width, height)) {
      this._changedProperty('icondim');
    }
  },
  
  /**
   * Get the source of the icon to display.
   *
   * This is the size of the control using box-sizing, ie the size of the
   * area that the control takes including any borders and padding.
   *
   * @access	public
   * @return  object                  { 'top' : <top> , 'left' : <left> }
   */
  getIconDim : function() {
    return this._icon.getDim();
  },

  /**
   * Set the gap between the icon and the text.
   *
   * The icon gap is the margin around the icon, it understands the normal
   * CSS margin values 'top left bottom right' etc.
   *
   * @access	public
   * @param   string      margin      icon gap (CSS units)
   * @return  boolean                 true if changed, false if unchanged
   */
  setIconGap : function(margin) {
    if (this._iconGap != margin) {
      this._iconGap = margin;
      this._changedProperty('icongap');
      return true;
    }
    return false;
  },
  
  /**
   * Get the gap between the icon and the text.
   *
   * The icon gap is the margin around the icon, it understands the normal
   * CSS margin values 'top left bottom right' etc.
   *
   * @access	public
   * @return  string                  icon gap (CSS units)
   */
  getIconGap : function() {
    var value = this._iconGap;
    return (typeof value == 'function' ? value.call(this) : value);
  },

  /**
   * Set the icon position.
   *
   * The icon can be positioned as follows;
   *   + 'left'   - icon is positioned on the left side of the text
   *   + 'right   - icon is positioned on the right side of the text
   *   + 'top'    - icon is positioned above the text
   *   + 'bottom' - icon is positioned below the text
   *
   * @access	public
   * @param   string      pos         icon position (left|right|top|bottom)
   * @return  boolean                 true if changed, false if unchanged
   */
  setIconPos : function(pos) {
    if (this._iconPos != pos) {
      if (pos != 'left' && pos != 'right' &&
          pos != 'top' && pos != 'bottom') {
        throw 'Invalid position: ' + position;
        return;
      }
      this._iconPos = pos;
      this._changedProperty('iconpos');
      return true;
    }
    return false;
  },
  
  /**
   * Get the icon position.
   *
   * The icon can be positioned as follows;
   *   + 'left'   - icon is positioned on the left side of the text
   *   + 'right   - icon is positioned on the right side of the text
   *   + 'top'    - icon is positioned above the text
   *   + 'bottom' - icon is positioned below the text
   *
   * @access	public
   * @return  string                  icon position (left|right|top|bottom)
   */
  getIconPos : function() {
    var value = this._iconPos;
    return (typeof value == 'function' ? value.call(this) : value);
  },

  /**
   * Return the inner HTML for the object.
   *
   * This displays the icon and text in the correct positions.
   *
   * @access	public
   * @return  string                  inner HTML
   */
  innerHTML : function() {
    if (!this._innerHTML) {
      // Most of the styles are set in the stylesheet but we have to
      // override those to set the icon gap
      switch (this._iconPos) {
        case 'left':    // Icon centered to the left of the name.
          this._icon.setStyle('margin', this.getIconGap());
          this._innerHTML = this._icon + this._name;
          break;

        case 'right':   // Icon centered to the right of the name.
          this._icon.setStyle('margin', this.getIconGap());
          this._innerHTML = this._name + this._icon;
          break;

        case 'top':     // Icon centered above the name.
          this._icon.setStyle('margin', this.getIconGap());
          this._icon.setStyle('margin-left', 'auto');
          this._icon.setStyle('margin-right', 'auto');
          this._innerHTML = this._icon + this._name;
          break;

        case 'bottom':  // Icon centered below the name.
          this._icon.setStyle('margin', this.getIconGap());
          this._icon.setStyle('margin-left', 'auto');
          this._icon.setStyle('margin-right', 'auto');
          this._innerHTML = this._name + this._icon;
          break;
      }
    }
    return this._innerHTML;
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
