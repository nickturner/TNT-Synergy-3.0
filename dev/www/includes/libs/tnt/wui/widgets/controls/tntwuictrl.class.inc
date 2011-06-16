<?PHP

///////////////////////////////////////////////////////////////////////////////
// TNT-SYNERGY: PHP Application Framework                            v2.0.00 //
// ======================================                                    //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-synergy.com                                                 //
// http://www.tnt-synergy.com/                                               //
//                                                                           //
// TNT-SYNERGY is a PHP Application Framework. It encourages application     //
// structured application architectures based on the "Model2" version of the //
// Model View Controller (MVC) design paradigm. The framework supports the   //
// following key features:                                                   //
//     - Model                                                               //
//         - session persistance (PHP Sessions)                              //
//         - permanent persistance (Database library abstraction)            //
//         - authentication (Multi-user)                                     //
//         - authorization (Access control)                                  //
//         - application model (Object based view of web page)               //
//     - Control                                                             //
//        - state processing (Page states)                                   //
//        - event processing (User supplied events)                          //
//     - View                                                                //
//        - themes (application and/or user themes)                          //
//        - template based presentation (Template library abstraction)       //
//        - internationalisation (Multi-language support)                    //
//                                                                           //
// This program is free software. You can redistribute it and/or modify it   //
// under the terms of the GNU General Public License as published by the     //
// Free Software Foundation; either version 2 of the License, or (at your    //
// option) any later version.                                                //
//                                                                           //
//   #####################################################################   //
//   # This program is distributed in the hope that it will be useful,   #   //
//   # but WITHOUT ANY WARRANTY; without even the implied warranty of    #   //
//   # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU #   //
//   # General Public License for more details.                          #   //
//   #####################################################################   //
//                                                                           //
// TNT-Synergy is a wholly owned subsidiary of TurnerTronics. The project is //
// open development and is the culmination of many peoples time and effort.  //
// For a full list of developers and copyrights please refer to CREDITS.TXT  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * WUI Ctrl Class 
 * ~~~~~~~~~~~~~~~~~~~
 *
 * The WUI Ctrl class is the base for all HTML form based controls.
 *
 * Some of the concepts are taken from the W3C XForms standard.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTWuiHtml');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * The 'Ctrl' class serves as a base class that defines the methods,
 * properties and events common to all Form controls.
 *
 * You can control the appearance and behavior of a Form control by setting
 * properties defined in this class. Normally the style is defined using a
 * standed CSS markup notation, however the 'style' property lets you get and
 * set individual CSS styles.
 *
 * The place of the control in the tab order is controlled by setting the
 * 'TabIndex' property.
 *
 * Note: Not all controls support every property defined in this class. For
 * specific information about whether a property is supported, see the
 * documentation for the specific control.
 *
 * You can set individual properties on the control by passing a valid XML
 * document to the {@link SetContent()} method.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiCtrl extends TNTWuiHtml {

    /**
     * Name of the control - name used to post back data
     * @access private
     * @var string                      controls name
     */
    var $_name = null;

    /**
     * Value of the control
     * @access private
     * @var string                      controls value/content
     */
    var $_value = null;

    /**
     * Label next to the control (null for no label)
     * @access private
     * @var string                      controls label (html)
     */
    var $_label = null;

    /**
     * Label alignment (left | right)
     * @access private
     * @var string                      controles label position
     */
    var $_labelAlign = null;

    /**
     * Help information for the control (null for no help)
     * @access private
     * @var string                      controls help
     */
    var $_help = null;

    /**
     * Hint information for the control
     * @access private
     * @var string                      controls hint
     */
    var $_hint = null;

    /**
     * Alert information for the control
     * @access private
     * @var string                      controls alert
     */
    var $_alert = null;

    /**
     * Tab Index
     * @access private
     * @var integer                     tab index
     */
    var $_tabIndex = null;

    /**
     * Access Key
     * @access private
     * @var string                      a single character string
     */
    var $_accessKey = null;

    /**
     * A value indicating whether the control is read only.
     * @access private
     * @var boolean                     true if the control is read only
     */
    var $_readOnly = null;

    /**
     * A value indicating whether the controls value is mandatory
     * @access private
     * @var boolean                     true if the control is mandatory
     */
    var $_mandatory = null;

    /**
     * Array of server side validation call backs
     * @access private
     * @var array                       server side validation methods
     */
    var $_serverValidators = null;

    /**
     * Array of client side validation call backs
     * @access private
     * @var array                       client side validation methods
     */
    var $_clientValidators = null;

    /**
     * Initial value of the control
     * @access private
     * @var string                      controls initial value
     */
    var $_initialValue = null;

    /**
     * Base part of the $_name property (eg name[5] => name)
     * @access private
     * @var string                      name base
     */
    var $_nameBase = null;

    /**
     * Index part of the $_name property (eg name[5] => [5])
     * @access private
     * @var string                      name index
     */
    var $_nameIndex = null;

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
     * @param   string      $name       string to set the name to
     * @return  boolean                 true if successful
     * @see     GetName();
     */ 
    function SetName($name) {

        if (is_null($name)) {
            $this->_name = null;
            $ret = true;
        } else if (!is_string($name)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else if (!preg_match('/^[_a-z][a-z0-9_=&\'{}[\]]*$/i', $name)) {
            $this->_SetError('control name is invalid');
            $ret = false;
        } else {
            // The name could contain an array extension so we strip it into
            // base and extension parts
            if (preg_match('/([^[]*)(.*)/', $name, $matches)) {
                $this->_nameBase = $matches[1];
                $this->_nameIndex = $matches[2];
            } else {
                $this->_nameBase = $name;
                $this->_nameIndex = '';
            }
            $this->_name = $name;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the controls name
     *
     * If there is no name then no value is posted.
     *
     * @access  public
     * @return  string                  current value of the name
     * @see     SetName();
     */ 
    function GetName() {

        $ret = $this->_name;

        return $ret;
    }

    /**
     * Set the value
     *
     * This member specifies the initial value of the control. It is optional
     * except when the control is a radio button or checkbox.
     *
     * Compound control can override this member to set complex values.
     *
     * @access  public
     * @param   string      $value      string to set the value to
     * @return  boolean                 true if successful
     * @see     GetValue();
     */ 
    function SetValue($value) {

        if (is_null($value)) {
            $this->_value = null;
            $ret = true;
        } else if (!is_string($value)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else {
            if (!$this->_initialValue) {
                $this->_initialValue = $value;
            }
            $this->_value = $value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the value
     *
     * Compound control can override this member to get complex values.
     *
     * If there is no value set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current value
     * @see     SetValue();
     */ 
    function GetValue() {
        return $this->_value;
    }

    /**
     * Set the label
     *
     * This member specifies the controls descriptive label.
     *
     * @access  public
     * @param   string      $label      string to set the label to
     * @return  boolean                 true if successful
     * @see     GetLabel();
     */ 
    function SetLabel($label) {

        if (is_null($label)) {
            $this->_label = null;
            $ret = true;
        } else if (!is_string($label)) {
            $this->_SetError('label must be either null or a string');
            $ret = false;
        } else {
            $this->_label = $label;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the label
     *
     * If there is no label set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current label
     * @see     SetLabel();
     */ 
    function GetLabel() {
        return $this->_label;
    }

        /**
     * Sets the alignment of the text label associated with the control.
     *
     * Use this property to specify the alignment of the label associated with
     * the control. You can specify whether the label appears to the right or
     * left of the check box. Use the Label property to specify the label.
     *
     * @access  public
     * @return  string                  alignment (left | right)
     * @return  boolean                 true if successful
     * @see     GetLabelAlign();
     */ 
    function SetLabelAlign($labelAlign) {

        static $aligns = array('left', 'right');

        if (is_null($labelAlign)) {
            $this->_labelAlign = null;
            $ret = true;
        } else if (!is_string($labelAlign) || !in_array($labelAlign, $aligns)) {
            $this->_SetError('value must be either null, "right" or "left": '.$labelAlign);
            $ret = false;
        } else {
            $this->_labelAlign = $labelAlign;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Gets the alignment of the text label associated with the control.
     *
     * @access  public
     * @return  string                  alignment (left | right)
     * @see     SetLabelAlign();
     */ 
    function GetLabelAlign() {
        return $this->_labelAlign;
    }

    /**
     * Set the hint
     *
     * This optional member provides a convenient way to attach hint
     * information to a form control.
     *
     * @access  public
     * @param   string      $hint       string to set the hint to
     * @return  boolean                 true if successful
     * @see     GetHint();
     */ 
    function SetHint($hint) {

        if (is_null($hint)) {
            $this->_hint = null;
            $ret = true;
        } else if (!is_string($hint)) {
            $this->_SetError('hint must be either null or a string');
            $ret = false;
        } else {
            $this->_hint = $hint;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the hint
     *
     * If there is no hint set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current hint
     * @see     SetHint();
     */ 
    function GetHint() {
        return $this->_hint;
    }

    /**
     * Set the help
     *
     * This optional member provides a convenient way to attach help
     * information to a form control.
     *
     * @access  public
     * @param   string      $help       string to set the help to
     * @return  boolean                 true if successful
     * @see     GetHelp();
     */ 
    function SetHelp($help) {

        if (is_null($help)) {
            $this->_help = null;
            $ret = true;
        } else if (!is_string($help)) {
            $this->_SetError('help must be either null or a string');
            $ret = false;
        } else {
            $this->_help = $help;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the help
     *
     * If there is no help set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current help
     * @see     SetHelp();
     */ 
    function GetHelp() {
        return $this->_help;
    }

    /**
     * Set the alert
     *
     * This optional member provides a convenient way to attach alert or error
     * information to a form control.
     *
     * @access  public
     * @param   string      $alert      string to set the alert to
     * @return  boolean                 true if successful
     * @see     GetAlert();
     */ 
    function SetAlert($alert) {

        if (is_null($alert)) {
            $this->_alert = null;
            $ret = true;
        } else if (!is_string($alert)) {
            $this->_SetError('alert must be either null or a string');
            $ret = false;
        } else {
            $this->_alert = $alert;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Get the alert
     *
     * If there is no alert set then null is returned.
     *
     * @access  public
     * @return  string                  the controls current alert
     * @see     SetAlert();
     */ 
    function GetAlert() {
        return $this->_alert;
    }

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
     * @param   integer     $tabIndex   tab index
     * @return  boolean                 true if successful
     * @see     GetTabIndex();
     */ 
    function SetTabIndex($tabIndex) {

        if (is_null($tabIndex)) {
            $this->_tabIndex = null;
            $ret = true;
        } else if (!is_numeric($tabIndex)) {
            $this->_SetError('value must be numeric');
            $ret = false;
        } else {
            $this->_tabIndex = (int)$tabIndex;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Gets the tab index of the control.
     *
     * If there is no tab index set then null is returned.
     *
     * @access  public
     * @return  integer                 the tab index
     * @see     SetTabIndex();
     */ 
    function GetTabIndex() {
        return $this->_tabIndex;
    }

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
     * @param   string      $accessKey  string to set the access key to
     * @return  boolean                 true if successful
     * @see     GetAccessKey();
     */ 
    function SetAccessKey($accessKey) {
        if (is_null($accessKey)) {
            $this->_accessKey = null;
            $ret = true;
        } else if (!is_string($accessKey)) {
            $this->_SetError('value must be either null or a single character string');
            $ret = false;
        } else {
            $this->_accessKey = substr($accessKey, 0, 1);
            $ret = true;
        }
        return $ret;
    }

    /**
     * Get the access key
     *
     * Gets the access key (underlined letter) that allows you to quickly
     * navigate to the control.
     *
     * If there is no access key set then null is returned.
     *
     * @access  public
     * @return  string                  current value of the access key
     * @see     SetAccessKey();
     */ 
    function GetAccessKey() {
        return $this->_accessKey;
    }

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
     * @param   mixed       $readOnly   true if control is read only
     * @return  boolean                 true if successful
     * @see     GetReadOnly();
     */ 
    function SetReadOnly($readOnly) {
        if (is_null($readOnly)) {
            $this->_readOnly = false;
            $ret = true;
        } else {
            if (is_string($readOnly)) {
                if (strcasecmp($value, 'readOnly') == 0) {
                    $readOnly = true;
                } else if (strcasecmp($value, '') == 0) {
                    $readOnly = false;
                } else {
                    $readOnly = strtoboolean($readOnly);
                }
            }
            if (!is_boolean($readOnly)) {
                $this->_SetError('value must be a boolean');
                $ret = false;
            } else {
                $this->_readOnly = $readOnly;
                $ret = true;
            }
        }

        return $ret;
    }

    /**
     * Gets a value indicating whether the control is read only.
     *
     * If there is no read only state defined then null is returned.
     *
     * @access  public
     * @return  boolean                 true if control is readOnly
     * @see     SetReadOnly();
     */ 
    function GetReadOnly() {
        return $this->_readOnly;
    }

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
     * @param   mixed       $mandatory   true if control is mandatory
     * @return  boolean                 true if successful
     * @see     GetMandatory();
     */ 
    function SetMandatory($mandatory) {

        if (is_null($mandatory)) {
            $this->_mandatory = false;
            $ret = true;
        } else {
            if (is_string($mandatory)) {
                if (strcasecmp($value, 'mandatory') == 0) {
                    $mandatory = true;
                } else if (strcasecmp($value, '') == 0) {
                    $mandatory = false;
                } else {
                    $mandatory = strtoboolean($readOnly);
                }
            }
            if (!is_boolean($mandatory)) {
                $this->_SetError('value must be a boolean');
                $ret = false;
            } else {
                $this->_mandatory = $mandatory;
                $ret = true;
            }
        }

        return $ret;
    }

    /**
     * Gets a value indicating whether the control is mandatory.
     *
     * If there is no mandatory state defined then null is returned.
     *
     * @access  public
     * @return  boolean                 true if control is mandatory
     * @see     SetMandatory();
     */ 
    function GetMandatory() {
        return $this->_mandatory;
    }

    /**
     * Add a server validation function.
     *
     * @access  public
     * @param   mixed   $validator  validator callable object
     * @param   string  $file       optional file to include before validation
     * @return  boolean             true if successfull
     */
    function AddServerValidator($validator, $file = null) {

        if (is_string($validator)) {
            if (preg_match('(\w)::(\w)', $validator, $matches)) {
                if ($matches[1] == 'this') {
                    $validator = array(&$this, $matches[2]);
                } else if ($matches[1] == 'parent') {
                    $validator = array(&$this->parent, $matches[2]);
                } else {
                    $validator = array($matches[1], $matches[2]);
                }
            }
        }

        if (!is_callable($validator, false, $name)) {
            $this->_SetError("Can not add server validator as not callable: $name", $this->_GetError);
            $ret = false;
        } else {
            $validators =& $this->_serverValidators;

            // See if this callable has allready been added
            $discard = false;
            if (is_array($validators)) {
                foreach ($validators as $v) {
                    if ($validator === $v) { 
                        $discard = true;
                        break;
                    }
                }
            }

            if (!$discard) {
                if (!$validators) {
                    $validators = array();
                }
                array_push($validators, $validator);
            }
            $ret = true;
        }

        return $ret;
    }

    /**
     * Remove a server side validator
     *
     * This method allows the removal of server validators from the control.
     * 
     * @access  public
     * @param   callable    $validator  valid callable object
     * @return  boolean                 true if successful
     */
    function RemoveServerValidator($validator) {

        $ret = true;

        if (is_string($validator)) {
            if (strstr($validator, '::') &&
                preg_match('(\w+)::(\w+)', $validator, $matches)) {
                if ($matches[1] == 'this') {
                    $validator = array(&$this, $matches[2]);
                } else if ($matches[1] == 'parent') {
                    $validator = array(&$this->parent, $matches[2]);
                } else {
                    $validator = array($matches[1], $matches[2]);
                }
            }
        }

        $validators =& $this->_serverValidators;

        // See if this callable has been added
        if (is_array($validators)) {
            foreach ($validators as $k => $v) {
                if ($validator === $v) { 
                    unset($listeners[$k]);
                    break;
                }
            }
        }

        return $ret;
    }

    /**
     * Add a client validation function.
     *
     * @access  public
     * @param   mixed   $validator  validator callable object
     * @param   string  $file       optional file to include before validation
     * @return  boolean             true if successfull
     */
    function AddClientValidator($validator, $file = null) {

        if (is_string($validator)) {
            if (preg_match('(\w)::(\w)', $validator, $matches)) {
                if ($matches[1] == 'this') {
                    $validator = array(&$this, $matches[2]);
                } else if ($matches[1] == 'parent') {
                    $validator = array(&$this->parent, $matches[2]);
                } else {
                    $validator = array($matches[1], $matches[2]);
                }
            }
        }

        if (!is_callable($validator, false, $name)) {
            $this->_SetError("Can not add client validator as not callable: $name", $this->_GetError);
            $ret = false;
        } else {
            $validators =& $this->_clientValidators;

            // See if this callable has allready been added
            $discard = false;
            if (is_array($validators)) {
                foreach ($validators as $v) {
                    if ($validator === $v) { 
                        $discard = true;
                        break;
                    }
                }
            }

            if (!$discard) {
                if (!$validators) {
                    $validators = array();
                }
                array_push($validators, $validator);
            }
            $ret = true;
        }

        return $ret;
    }

    /**
     * Remove a client side validator
     *
     * This method allows the removal of client validators from the control.
     * 
     * @access  public
     * @param   callable    $validator  valid callable object
     * @return  boolean                 true if successful
     */
    function RemoveClientValidator($validator) {

        $ret = true;

        if (is_string($validator)) {
            if (strstr($validator, '::') &&
                preg_match('(\w+)::(\w+)', $validator, $matches)) {
                if ($matches[1] == 'this') {
                    $validator = array(&$this, $matches[2]);
                } else if ($matches[1] == 'parent') {
                    $validator = array(&$this->parent, $matches[2]);
                } else {
                    $validator = array($matches[1], $matches[2]);
                }
            }
        }

        $validators =& $this->_clientValidators;

        // See if this callable has been added
        if (is_array($validators)) {
            foreach ($validators as $k => $v) {
                if ($validator === $v) { 
                    unset($listeners[$k]);
                    break;
                }
            }
        }

        return $ret;
    }

    /**
     * Render the given value as hidden controls.
     *
     * @access  public
     * @param   string                  name to render value as
     * @param   string                  value to render
     * @return  string                  HTML required to render hidden value
     * @see     RenderInitialValue(), RenderCurrentValue();
     */ 
    function RenderValue($name, $value) {
        return TNTHtml::HiddenVal($name, $value);
    }
    
    /**
     * Render the forms current value as hidden controls.
     *
     * @access  public
     * @return  string                  HTML required to render hidden value
     * @see     RenderInitialValue();
     */ 
    function RenderCurrentValue() {

        $ret = '';

        $name = $this->GetName('name');

        // Special controls do not have an initial value
        if ($name && strncmp($name, '_form_', 6) != 0) {
            $ret = $this->RenderValue($name, $this->_value);
        }

        return $ret;
    }

    /**
     * Render the forms initial value as hidden controls.
     *
     * @access  public
     * @return  string                  HTML required to render hidden value
     * @see     RenderCurrentValue();
     */ 
    function RenderInitialValue() {

        $ret = '';

        $name = $this->GetName();

        // Special controls do not have an initial value
        if ($name && strncmp($name, '_form_', 6) != 0) {
            // The name could contain an array extension so we strip it into
            // base and extension parts. We also ignore any optional '[]'
            // extension.
            if (preg_match('/([^[]*)(.*)(\[\])?/', $name, $matches)) {
                $nameBase = $matches[1];
                $nameIndex = $matches[2];
            } else {
                $nameBase = $name;
                $nameIndex = '';
            }

            $ret = $this->RenderValue("initial_values[{$nameBase}]{$nameIndex}", $this->_initialValue);
        }

        return $ret;
    }

    /**
     * Render the client side script required to validate this control
     *
     * This must render the script as valid HTML including any
     * '<script></script>' tags as required.
     *
     * @access  public
     * @return  string                  HTML required to render client script
     */ 
    function _RenderClientScript() {

        $ret = '';

        // If there is no form or no client validators then we don't
        // render any script.
        if ($this->_clientValidators && $form =& $this->Form()) {
            $form_id = $form->TagAttribute('id');
            $ctrl_id = $this->TagAttribute('id');

            $obj = array();
            foreach ($this->tagAttrNames as $n) {
                $obj[$n] = $this->TagAttribute($n);
            }
            $obj['clientValidators'] = array();
            foreach ($this->_clientValidators as $n => $v) {
                $obj['clientValidators'][$n] = $v;
            }

            $obj = $this->ValueToJavaScript($obj);

            $ret = <<<EOS
<script language="javascript">
<!--
/* JScript Control Validation */
document.forms['$form_id'].smart.ctrls['$ctrl_id'] = $obj;
//-->
</script>
EOS;
        }

        return $ret;
    }

    /**
     * Render the controls label
     *
     * @access  public
     * @return  string                  HTML required to render label
     * @see     Render();
     */ 
    function _RenderLabel() {

        $label = $this->GetLabel();
        if (!$label) {
            return '';
        }

        if ($this->GetMandatory()) {
            $label .= '<span class="tntwuictrl-mandatory">*</span>';
        }

        $attrs = array('for' => $this->TagAttribute('id'),
                       'accesskey' => $this->TagAttribute('accesskey'));
        return TNTHtml::BlockTag('label', $attrs, $label);
    }

    /**
     * Render the controls help
     *
     * @access  public
     * @return  string                  HTML required to render help
     * @see     Render();
     */ 
    function _RenderHelp() {

        $help = $this->GetHelp();
        if (!$help) {
            return '';
        }

        return '';
    }

    /**
     * Render the control
     *
     * By default a control is rendered as follows;
     * [Initial Value]
     * <span id="$id-ctrl" class="tntwuictrl $class" title="$hint">
     *   <label class="tntwuictrl-label" for="$id">$label</label>
     *   <input class="tntwuictrl-ctrl" id="$id" type="$type" name="$name"
     *     value="$value" tabIndex="$tabIndex" accessKey="$accessKey"
     *     disabled="$disabled?" readonly="$readonly?" />
     *   <span class="tntwuictrl-help">?</span>
     * </span>
     * <script>[Validation Script]</script>
     *
     * @access  public
     * @return  string                  HTML required to render the control
     * @see     Render();
     */ 
    function Render() {

        $ret = $this->RenderInitialValue();

        if ($this->_hidden) {
            $ret .= $this->RenderCurrentValue();
        } else {
            $align = $this->GetLabelAlign();
            if ($align != 'left' && $align != 'right') {
                $align = 'left';
            }

            $label = $this->_RenderLabel();
            $help = $this->_RenderHelp();
            $ctrl = parent::Render();

            if ($align == 'left') {
                $html = $label.$ctrl.$help;
            } else {
                $html = $help.$ctrl.$label;
            }

            $ret = TNTHtml::BlockTag('span', 
                array(
                    'id' => $this->TagAttribute('id').'-ctrl',
                    'class' => 'tntwuictrl '.$this->TagAttribute('class'),
                    'title' => $this->TagAttribute('hint')), $html).
                $this->_RenderClientScript();
        }

        return $ret;
    }

    /**
     * Set widgets content.
     *
     * When the widget is created from a template as a 'block' this is called
     * with the blocks content. If it is a valid XML format string then it is
     * parsed to set the member variables using the {@link Set()} method.
     *
     * @access  public
     * @param   mixed&      $value      value to set property to
     * @return  boolean                 true if successfull
     */
    function SetContent(&$value) {
        TNTDebug::Dump('SetContent(', $value, ')');

        if (is_object($value)) {
            $this->_content =& $value;
        } else {
            $this->_content = $value;
            if (is_string($value) && !is_blank_str($value)) {
                // Parse as XML content
                $data = $this->_ParseContent($value);
                if (is_null($data) || ($data && !$this->Set($data))) {
                    return true;
                }
            }
        }

        return true;
    }
    
    /**
     * Parse any valid XML content into an associative array of values to pass
     * to {@link Set()}.
     *
     * When the widget is created from a template as a 'block' this is called
     * with the blocks content. If it is a valid XML format string then it is
     * parsed to set the member variables using the {@link Set()} method.
     *
     * @access  public
     * @param   string      $value      value to parse as XML content
     * @return  array                   associative array of values
     */
    function _ParseContent($value) {

        // Parse as XML
        TNTLoader::RequireClass('TNTXmlParser');

        $data = "<content>{$value}</content>" ;
        $opts = TNT_TAGPARSER_OPTS_SIMPLE;

        $xmlParser =& new TNTXmlParser();

        if (!$xmlParser->ParseData($data, $opts)) {
            $this->_SetError('Failed to parse the controls contents', $xmlParser);
            return null;
        }

        $root = $xmlParser->Root();

        $ret = $root['content'];

        TNTDebug::Dump('_ParseContent(', $value, ') - data=', $ret);

        return $ret;
    }

    /**
     * Validate any control value
     *
     * Any validation error message can be obtained via the normal
     * {@link TNTWuiNode::Error()} method.
     *
     * @access  public
     * @return  boolean                 true if validation is successful
     */ 
    function Validate() {

        $ret = true;

        // Run all validation callbacks over this control
        if (!empty($this->_value) && !empty($this->_serverValidators)) {
            foreach ((array)$this->_serverValidators as $validator) {
                if (($e = call_user_func_array($validator, array(&$this)))) {
                    $this->_SetError("$name: $e", $this->_GetError());
                    $ret = false;
                }
            }
        }
        
        return $ret;
    }
    
    /**
     * Return the value of the data array element that corresponds to the name
     * of this control.
     *
     * This could be a deep link in to the array. For example if the name of
     * the control is "profile['preferences'][5]" then the data would be at
     * "$data['profile']['preferences'][5]".
     *
     * The method returns true if the correct element was found in the array
     * and was returned.
     *
     * @access  public
     * @param   array      $data        reference to array of data
     * @param   mixed      $value       reference to store value to
     * @return  boolean                 true if successful
     */ 
    function _SelectDataValue(&$data, &$value) {

        $ret = true;

        $code = "
            if (isset(\$data['{$this->_nameBase}']{$this->_nameIndex})) {
                \$value = \$data['{$this->_nameBase}']{$this->_nameIndex};
                \$ret = true;
            } else {
                \$value = null;
                \$ret = false;
            }
            ";

        eval($code);

        if ($ret) {
        } else {
        }

        return $ret;
    }

    /**
     * Update the value of the data array element that corresponds to the name
     * of this control.
     *
     * This could be a deep link in to the array. For example if the name of
     * the control is "profile['preferences'][5]" then the data would be at
     * "$data['profile']['preferences'][5]".
     *
     * The method returns true if the correct element was found in the array
     * and was updated. The method never creates the element.
     *
     * @access  public
     * @param   array      $data        reference to array of data to check
     * @param   mixed      $value       value to update it with
     * @return  boolean                 true if successful
     */ 
    function _UpdateDataValue(&$data, $value) {

        $ret = false;

        $code = "
            if (isset(\$data['{$this->_nameBase}']{$this->_nameIndex})) {
                \$data['{$this->_nameBase}']{$this->_nameIndex} = \$value;
                \$ret = true;
            } else {
                \$ret = false;
            }
            ";

        eval($code);

        return $ret;
    }

    /**
     * Load form data
     *
     * The form requests each of its child controls to load its initial value
     * from the given associative array of data.
     *
     * Control names prefixed with '_' are assumed to be private and not part
     * of the data saving process.
     *
     * @access  public
     * @param   mixed[]     $formData   collection of form data
     * @return  boolean                 true if the data was loaded
     */ 
    function LoadFormData(&$formData) {

        $ret = true;

        if ($this->_name && $this->_name{0} != '_') {
            // Set to default value first
            $this->SetValue(null);

            if ($this->_SelectDataValue($formData, $value)) {
                $ret = $this->SetValue($value);
            }
        }

        return $ret;
    }

    /**
     * Save form data
     *
     * The form requests each of its child controls to save its current value
     * into given associative array of data.
     *
     * Control names prefixed with '_' are assumed to be private and not part
     * of the data saving process.
     *
     * @access  public
     * @param   mixed[]     $formData   collection of form data
     * @return  boolean                 true if the data was saved
     */ 
    function SaveFormData(&$formData) {

        $ret = true;

        if ($this->_name && $this->_name{0} != '_') {
            $value = $this->GetValue();

            $ret =  $this->_UpdateDataValue($formData, $value);
        }

        return $ret;
    }

    /**
     * Load post data
     *
     * The Page tracks all the control's that return true to this method call,
     * then invokes the {@link RaisePostDataChangedEvent} on those controls.
     *
     * This does not validate the posted value. The controls value must be set
     * to the given value even if it would result in an invalid value.
     *
     * @access  public
     * @param   string[]    $postData   collection of incoming post data
     * @return  boolean                 true if the controls value changed
     */ 
    function LoadPostData(&$postData) {

        $ret = false;

        if ($this->_name) {
            if ($this->_SelectDataValue($postData, $newValue)) {
                $oldValue = $this->_value;
                if ($oldValue != $newValue) {
                    $this->_value = $newValue;
                    $ret = true;
                }
            }
        }

        $this->_changed = $ret;

        return $ret;
    }

    /**
     * OnChange
     *
     * Called when the value of the control changes between post backs
     * between posts to the server.
     *
     * @access  protected
     * @return  void
     */
    function OnChange() {
    }

    /**
     * Format a postback control name.
     *
     * This string is assigned to the name of the control that should cause
     * a postback. This is normally a form submission button etc.
     *
     *
     * @access  public
     * @param   mixed[]     $args       optional arguments to send with postback
     * @return  string                  name to use for postback control
     */ 
    function FormatPostBackName($args = null) {

        $dn = $this->DN();

        if ($args) {
            $dn .= '?'.serialize($args);
        }

        $ret = TNTTag::EncodeAttrVal("_form_submit_dn[$dn]");

        return $ret;
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiCtrl() {

        TNTWuiHtml::TNTWuiHtml();

        $this->tagName = 'input';
        $this->tagComplete = 'true';

        array_push($this->tagAttrNames, 'name', 'value', 'tabIndex', 'accessKey', 'disabled', 'readonly');

    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiCtrl() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
