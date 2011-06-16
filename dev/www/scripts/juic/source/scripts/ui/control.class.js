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
 * TNT.Juic.UI.Control Javascript
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Serves as the base class that defines the methods, properties and events
 * common to all controls in the TNT.Juic.UI.Controls namespace.
 *
 * The Control class provides the properties, methods, and events that are
 * common to all controls. You can control the appearance and behavior of a
 * control by setting properties defined in this class.
 *
 * The behavior of the control can be specified by setting certain properties.
 * You can enable and disable a control by setting the Enabled property. The
 * place of the control in the tab order is controlled by setting the TabIndex
 * property. You can specify a ToolTip for the control by setting the ToolTip
 * property.
 *
 * Note: Not all controls support every property defined in this class. For
 * specific information about whether a property is supported, see the
 * documentation for the specific control.
 *
 * Note: Some properties in this class render differently, depending on the
 * browser. Some properties do not render at all, while others render, but have
 * no effect.
 *
 * @package TNT.JUIC
 * @subpackage UI.Controls
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0
 */
TNT.Juic.UI.Control = TNT.declareClass({
  $class : 'TNT.Juic.UI.Control',
  $extends : TNT.Juic.UI.Widget,

  /**
   * Constructor
   *
   * @access  public
   * @param   object    data          optional data map
   * @param   object    owner         optional owning Object
   * @return  object                  reference to constructed object
   */
  constructor : function(data, owner) {
    TNT.Juic.UI.Widget.prototype.constructor.call(this, data, owner);

/*
    this.setTabIndex(0);
    this.addClass('_inline');

    // Derived controls can remove these if not required
    this.setAttribute('name', function() { return this.getName(); });
    this.setAttribute('tabindex', function() { return this.getTabIndex(); });
    this.setAttribute('accesskey', function() { return this.getAccessKey(); });
    this.setAttribute('value', function() { return this.getValue(); });
    this.setAttribute('readonly', function() { return (this.getReadOnly() ? 'readOnly' : null); });
*/

  },

  /**
   * Name of the control - name used to post back data
   * @access private
   * @var string            controls name
   */
  _name : null,

  /**
   * Value of the control
   * @access private
   * @var string            controls value/content
   */
  _value : null,

  /**
   * Tab Index
   * @access private
   * @var integer           tab index
   */
  _tabIndex : null,

  /**
   * Access Key
   * @access private
   * @var mixed             static/dynamic value
   */
  _accessKey : null,

  /**
   * A value indicating whether the control is read only.
   * @access private
   * @var mixed             static/dynamic value
   */
  _readOnly : null,

  /**
   * A value indicating whether the controls value is mandatory
   * @access private
   * @var boolean           true if the control is mandatory
   */
  _mandatory : null,

  /**
   * Array of client side validation call backs
   * @access private
   * @var array             client side validation methods
   */
  _clientValidators : null,

  /**
   * Initial value of the control
   * @access private
   * @var string            controls initial value
   */
  _initialValue : null,

  /**
   * The context menu associated with the control
   * @access private
   * @var object            context menu object
   */
  _contextMenu : null,

  /**
   * Set the controls name
   *
   * The name is user to identify the value in the posted data.
   *
   * If the parent form is bound to a data source then the name should be the
   * same as column in the row of data.
   *
   * If there is no name then no value is posted.
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getName()
   */ 
  setName : function(value) {
    ret = false;

    if (this._name != value) {
      if (value == null || TNT.isFunction(value) || TNT.isString(value)) {
        this._name = value;
        ret = this._changedProperty('name');
      } else {
        ret = this._invalidProperty('name', value, 'Invalid value');
      }
    }

    return ret;
  },

  /**
   * Get the controls name
   *
   * If there is no name then no value is posted.
   *
   * If the name is a dynamic value then this method returns the static
   * value obtained from the dynamic source.
   *
   * @access  public
   * @return  string                  current value of the name
   * @see     setName()
   */ 
  getName : function() {
    return this.dynamicValue(this._name);
  },

  /**
   * Set the value
   *
   * This member specifies the initial value of the control. It is optional
   * except when the control is a radio button or checkbox.
   *
   * Compound control can override this member to set complex values.
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getValue()
   */ 
  setValue : function(value) {
    ret = false;

    if (this._value != value) {
      this._value = value;
      ret = this._changedProperty('value');
    }

    return ret;
  },

  /**
   * Get the value
   *
   * Compound control can override this member to get complex values.
   *
   * If there is no value set then null is returned.
   *
   * @access  public
   * @return  string          the controls current value
   * @see     setValue()
   */ 
  getValue : function() {
    return this.dynamicValue(this._value);
  },

  /**
   * Sets the tab index of the control.
   *
   * Use the TabIndex property to specify or determine the tab index of a
   * control on the form. When you press the Tab key, the order in which the
   * controls receive focus is determined by the TabIndex property of each
   * control. The controls on a form are tabbed to in ascending order, based
   * on the value of the TabIndex property of each control, starting with the
   * smallest positive, nonzero value. If multiple controls share the same
   * tab index, the controls will receive focus in the order they are
   * declared on the page. Finally, controls that have a tab index of zero
   * are tabbed to in the order they are declared.
   *
   * Note: Only controls with a nonzero tab index will render the tabindex
   * attribute.
   *
   * You can remove a control from the tab order by setting the TabIndex
   * property to a negative value.
   *
   * Note: This property is supported only in Internet Explorer 4 and later.
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getTabIndex()
   */ 
  setTabIndex : function(value) {
    ret = false;

    if (this._tabIndex != value) {
      if (value == null || TNT.isFunction(value) || TNT.isNumeric(value)) {
        this._tabIndex = value;
        ret = this._changedProperty('tabIndex');
      } else {
        ret = this._invalidProperty('tabIndex', value, 'Invalid value');
      }
    }

    return ret;
  },

  /**
   * Gets the tab index of the control.
   *
   * If there is no tab index set then null is returned.
   *
   * @access  public
   * @return  integer                 the tab index
   * @see     setTabIndex()
   */ 
  getTabIndex : function() {
    return this.dynamicValue(this._tabIndex);
  },

  /**
   * Set the access key
   *
   * Sets the access key (underlined letter) that allows you to quickly
   * navigate to the control.
   *
   * Use the AccessKey property to specify the keyboard shortcut for the
   * control. This allows you to navigate quickly to the control by pressing
   * the ALT key and the key for the specified character on the keyboard. For
   * example, setting the access key of a control to the string "D" indicates
   * that the user can navigate to the control by pressing ALT+D.
   *
   * Only a single character string is allowed for the AccessKey property. If
   * you attempt to set this property to a value that is not either null, or
   * an string, the method will fail. If you try to set it to a string longer
   * than a single character only the first character will be used.
   *
   * This property is supported only in Internet Explorer 4.0 and later
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getAccessKey()
   */ 
  setAccessKey : function(value) {
    ret = false;

    if (this._accessKey != value) {
      if (value == null || TNT.isFunction(value) || TNT.isString(value)) {
        this._accessKey = value;
        ret = this._changedProperty('accessKey');
      } else {
        ret = this._invalidProperty('accessKey', value, 'Invalid value');
      }
    }

    return ret;
  },

  /**
   * Get the access key
   *
   * Gets the access key (underlined letter) that allows you to quickly
   * navigate to the control.
   *
   * If there is no access key set then null is returned.
   *
   * @access  public
   * @return  string          current value of the access key
   * @see     setAccessKey()
   */ 
  getAccessKey : function() {
    return this.dynamicValue(this._accessKey);
  },

  /**
   * Sets a value indicating whether the control is read only.
   *
   * Use the readOnly property to specify or determine whether a control can
   * have its value altered. When set to true, any changes to the control are
   * ignored.
   *
   * This property propagates down the control hierarchy. Therefore,
   * making a container control read only will make all child controls
   * within that container read only.
   *
   * Note: Not all controls support this property. See the indivual controls
   * for details.
   *
   * The value can be specified as either a boolean or one of the following
   * strings;
   *   'true', 'on', 'yes', '1', 'readOnly' => true
   *   'false', 'off', 'no', '0', '' => false
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getReadOnly()
   */ 
  setReadOnly : function(value) {
    ret = false;

    if (this._readOnly != value) {
      // Validate it (dynamic values are validated when retrieved)
      var _values = ['true', 'false', 'on', 'off', 'yes', 'no', '1', '0', 'readonly'] 
      if (value == null || TNT.isBoolean(value) || (TNT.isString(value) && TNT.Array.isValue(value.toLowerCase(), _values))) {
        this._readOnly = value;
        ret = _changedProperty('readonly');
      } else {
        ret = _invalidProperty('readOnly', value, 'Value must be either null, boolean or one of ' + _values);
      }
    }

    return ret;
  },

  /**
   * Gets a value indicating whether the control is read only.
   *
   * If there is no read only state defined then null is returned.
   *
   * @access  public
   * @return  boolean                 true if control is readOnly
   * @see     setReadOnly()
   */ 
  getReadOnly : function() {
    var value = this.dynamicValue(this._readOnly);
    if (value == null) {
      ret = null;
    } else if (TNT.isBoolean(value)) {
      ret = value;
    } else if (TNT.isString(value)) {
      switch (value.toLowerCase()) {
        case 'true':
        case 'on':
        case '1':
        case 'readOnly':
          ret = true;
          break;
        case 'false':
        case 'off':
        case '0':
          ret = false;
          break;
        default:
          this._invalidProperty('readOnly', value);
          ret = null;
      }
    } else {
      this._invalidProperty('readOnly', value);
      ret = null;
    }

    return ret;
  },

  /**
   * Sets a value indicating whether the control is mandatory.
   *
   * Use the Mandatory property to specify that a value for the control must
   * be given and a blank/null value is not allowed.
   *
   * This differs from, for example, a min/max length validation in that you
   * can set that the mimumum length of a entered string must be 5 characters
   * but an empty string will pass the validation unless the mandatory flag is
   * set in which case it will only pass if it is 5 characters long or more.
   *
   * The value can be specified as either a boolean or one of the following
   * strings;
   *   'true', 'on', 'yes', '1', 'mandatory' => true
   *   'false', 'off', 'no', '0', '' => false
   *
   * @access  public
   * @param   mixed       value       new value
   * @return  boolean                 true if changed, false if unchanged
   * @see     getMandatory()
   */ 
  setMandatory : function(value) {
    ret = false;

    if (this._readOnly != value) {
      // Validate it (dynamic values are validated when retrieved)
      var _values = ['true', 'false', 'on', 'off', 'yes', 'no', '1', '0', 'mandatory'] 
      if (value == null || TNT.isBoolean(value) || (TNT.isString(value) && TNT.Array.isValue(value.toLowerCase(), _values))) {
        this._mandatory = value;
        ret = _changedProperty('mandatory');
      } else {
        ret = _invalidProperty('mandatory', value, 'Value must be either null, boolean or one of ' + _values);
      }
    }

    return ret;
  },

  /**
   * Gets a value indicating whether the control is mandatory.
   *
   * If there is no mandatory state defined then null is returned.
   *
   * @access  public
   * @return  boolean                 true if control is mandatory
   * @see     setMandatory()
   */ 
  getMandatory : function() {
    var value = this.dynamicValue(this._mandatory);
    if (value == null) {
      ret = null;
    } else if (TNT.isBoolean(value)) {
      ret = value;
    } else if (TNT.isString(value)) {
      switch (value.toLowerCase()) {
        case 'true':
        case 'on':
        case '1':
        case 'mandatory':
          ret = true;
          break;
        case 'false':
        case 'off':
        case '0':
          ret = false;
          break;
        default:
          this._invalidProperty('mandatory', value);
          ret = null;
      }
    } else {
      this._invalidProperty('mandatory', value);
      ret = null;
    }

    return ret;
  },

  /**
   * Retrieves the form that the control is on.
   *
   * This method only works once the control is rendered to the document.
   *
   * If there is no ancestor form control then null is returned.
   *
   * @access  public
   * @return  object                  HTML DOM form element
   */ 
  form : function() {
    return null;
  },

  /**
   * Sets input focus to the control.
   *
   * This method returns true if the control successfully received input focus.
   * The control can have the input focus while not displaying any visual cues
   * of having the focus.
   *
   * This method only works once the control is rendered to the document.
   *
   * @access  public
   * @return  boolean                 true if successful; otherwise, false
   */ 
  focus : function() {
    return false;
  },

  /**
   * Retrieves the next JUIC control in the tab order of child controls.
   *
   * This returns the next JUIC control in the current form.
   *
   * This method only works once the control is rendered to the document.
   *
   * @access  public
   * @return  object                  Next JUIC control in the tab order
   */ 
  nextControl : function() {
    return null;
  },

  /**
   * Retrieves the previous JUIC control in the tab order of child controls.
   *
   * This returns the previous JUIC control in the current form.
   *
   * This method only works once the control is rendered to the document.
   *
   * @access  public
   * @return  object                  Previous JUIC control in the tab order
   */ 
  prevControl : function() {
    return null;
  },

  /**
   * Activate the control.
   *
   * The Select method activates the control if the control's
   * ControlStyles.Selectable style bit is set to true, it is contained in
   * another control, and all its parent controls are both visible and enabled.
   *
   * @access  public
   * @return  void
   */ 
  select : function() {
  },

  /**
   * Add a validation function.
   *
   * @access  public
   * @param   mixed   validator  validator callable object
   * @param   string  file     optional file to include before validation
   * @return  boolean       true if successfull
   * @see     detachValidator()
   */
  /*
 attachValidator : function(validator, file = null) {

    if (TNT.isString(validator)) {
      if (preg_match('(\w)::(\w)', validator, matches)) {
        if (matches[1] == 'this') {
          validator = array(&this, matches[2]);
        } else if (matches[1] == 'parent') {
          validator = array(&this.parent, matches[2]);
        } else {
          validator = array(matches[1], matches[2]);
        }
      }
    }

    if (!is_callable(validator, false, name)) {
      this._SetError("Can not add client validator as not callable: name", this._GetError);
      ret = false;
    } else {
      validators =& this._clientValidators;

      // See if this callable has allready been added
      discard = false;
      if (is_array(validators)) {
        foreach (validators as v) {
          if (validator === v) { 
            discard = true;
            break;
          }
        }
      }

      if (!discard) {
        if (!validators) {
          validators = array();
        }
        array_push(validators, validator);
      }
      ret = true;
    }

    return ret;
  },
*/

  /**
   * Remove a validation function
   *
   * 
   * @access  public
   * @param   callable  validator  valid callable object
   * @return  boolean         true if successful
   * @see     attachValidator()
   */
/*
  removeValidator : function(validator) {
    ret = true;

    if (TNT.isString(validator)) {
      if (strstr(validator, '::') &&
        preg_match('(\w+)::(\w+)', validator, matches)) {
        if (matches[1] == 'this') {
          validator = array(&this, matches[2]);
        } else if (matches[1] == 'parent') {
          validator = array(&this.parent, matches[2]);
        } else {
          validator = array(matches[1], matches[2]);
        }
      }
    }

    validators =& this._clientValidators;

    // See if this callable has been added
    if (is_array(validators)) {
      foreach (validators as k => v) {
        if (validator === v) { 
          unset(listeners[k]);
          break;
        }
      }
    }

    return ret;
  },
*/

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

