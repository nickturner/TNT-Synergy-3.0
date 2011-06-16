<?php

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
 * Wui Button Class 
 * ~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic button control on a Wui form.
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
TNTLoader::RequireClass('TNTWuiCtrl');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * The Button class allows you to add a push button control to a form.
 *
 * <pre>
 * {tntwui_ctrl ctrl="button"
 *   id="Button1" 
 *   type="submit|reset|cancel|command"
 *   label="string"
 *   cmdname="string"
 *   cmdargs="string"
 *   validate="true|false"
 * }
 * </pre>
 *
 * The Button control allows you to create push buttons on a Form
 * which either perform client or server side commands on the Form.
 *
 * The 'type' attribute gives the basic type of buttons and there default
 * actions;
 *   + 'submit'  - calls parent Form's {@link TNTWuiForm::RaiseSubmitEvent()}
 *   + 'cancel'  - calls parent Form's {@link TNTWuiForm::RaiseCancelEvent()}
 *   + 'reset'   - calls parent Form's {@link TNTWuiForm::RaiseResetEvent()}
 *   + 'command' - raises a 'command' event.
 *
 * There is no support for a client side 'reset' button as this would only
 * reset back to the values when the page was loaded or postedback and not
 * to any initial values.
 *
 * You can use the {@link OnCommand()} method to make the Button control behave
 * like a command button. A command name can be associated with the control
 * using the 'CmdName' property. This allows multiple command Button controls
 * to be placed on the same Form. The value of the 'CmdName' property can then
 * be programmatically identified in the {@link OnCommand()} method.
 * The 'CmdArgs' property can also be used to pass additional information about
 * the command, such as specifying ascending order.
 *
 * The label on the Button can either be specified using the 'label' attribute
 * or any content set via the {@link SetContent()} method.
 * When a label is given a "<input type="submit" />" HTML element is used.
 * When inner content is given then a "<button></button>" HTML element is used
 * and any label is ignored.
 *
 * By default, form validation is performed when an Button control with a
 * type of 'submit' is clicked. Form validation determines whether the controls
 * pass the validation rules specified for the control. If you want to disable
 * this behaviour then set the 'validate' property to false.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiButton extends TNTWuiCtrl {

    /**
     * Type of button required
     * @access private
     * @var string                      HTML control type 
     */
    var $_type = null;

    /**
     * Label on the button
     * @access private
     * @var string                      button label (text only)
     */
    var $_label = null;

    /**
     * Command name that is passed to the 'command' event 
     * @access private
     * @var string                      command name
     */
    var $_cmdName = null;

    /**
     * Command arguments that are passed to the 'command' event 
     * @access private
     * @var mixed                       command argument
     */
    var $_cmdArgs = null;

    /**
     * A value indicating whether Form validation is performed when clicked.
     * @access private
     * @var boolean                     true if validation is performed
     */
    var $_validate = null;

    /**
     * Set the label that is displayed on the Button control.
     *
     * The label can be used instead on child tags to set the 'content' of the
     * button. When used this way the label can only contain text and no HTML
     * tags.
     *
     * @access  public
     * @param   string      $label      string to set the label to
     * @return  boolean                 true if successful
     * @see     GetLabel();
     */ 
    function SetLabel($label) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('label' =>& $label));

        $ret = false;
        if (is_null($label)) {
            $this->_label = null;
            $ret = true;
        } else if (!is_string($label)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else {
            $this->_label = $label;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the label that is displayed on the Button control.
     *
     * @access  public
     * @return  string                  current value of the label
     * @see     SetLabel();
     */ 
    function GetLabel() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_label;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Set the command name associated with the Button that is passed to the
     * Command event
     *
     * When you have multiple Button controls on a page, use the CmdName
     * property to specify or determine the command name associated with each
     * Button control. You can set the CmdName property with any string
     * that identifies the command to perform. You can then programmatically
     * determine the command name of the Button control and perform the
     * appropriate actions.
     *
     * Additional information about the command, such as 'Ascending', may be
     * specified by using the CmdArgs property.
     *
     * @access  public
     * @param   string      $cmdName    string to set the cmdName to
     * @return  boolean                 true if successful
     * @see     GetCmdName();
     */ 
    function SetCmdName($cmdName) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('label' =>& $label));

        $ret = false;
        if (is_null($cmdName)) {
            $this->_cmdName = null;
            $ret = true;
        } else if (!is_string($cmdName)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else {
            $this->_cmdName = $cmdName;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the command name associated with the Button that is passed to the
     * Command event
     *
     * @access  public
     * @return  string                  current value of the cmdName
     * @see     SetCmdName();
     */ 
    function GetCmdName() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->EvalBindings($this->_cmdName);

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Set the command args associated with the Button that is passed to the
     * Command event
     *
     * The cmdArgs are passed with the 'Command' event to allow additional
     * information to be given for the command.
     *
     * You can pass multiple arguments as an associative array or using url
     * style encoding in the value.
     * For example, 'commandargs='?fred=1&harry=2' will pass the argument to the
     * event as the associative array('fred' => 1, 'harry' => 2). To invoke this
     * decoding the first character must be '?'.
     *
     * @access  public
     * @param   mixed       $cmdArgs    value to set the cmdArgs to
     * @return  boolean                 true if successful
     * @see     GetCmdArgs();
     */ 
    function SetCmdArgs($cmdArgs) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('cmdArgs' =>& $cmdArgs));

        $ret = false;
        if (is_null($cmdArgs)) {
            $this->_cmdArgs = null;
            $ret = true;
        } else if (is_string($cmdArgs) && $cmdArgs{0} == '?') {
            parse_str(substr($cmdArgs, 1), $this->_cmdArgs);
            $ret = true;
        } else {
            $this->_cmdArgs = $cmdArgs;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the command args associated with the Button that is passed to the
     * Command event.
     *
     * @access  public
     * @param   boolean     $asString   return the value as URL encoded string
     * @return  string                  current value of the command
     * @see     SetCmdArgs();
     */ 
    function GetCmdArgs($asString = false) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('asString' =>& $asString));

        if ($asString && is_array($this->_cmdArgs)) {
            $ret = '?'.TNT_LinkQueryBuild($this->_cmdArgs);
        } else {
            $ret = $this->_cmdArgs;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Set the buttons type
     *
     * The 'type' attribute gives the basic type of the button as follows;
     *   + 'submit' - submits the form (validating) (default)
     *   + 'cancel' - cancels the submission of the form (non validating)
     *   + 'reset'  - resets the form to initial values (client side)
     *
     * @access  public
     * @param   string      $type       string to set the type to
     * @return  boolean                 true if successful
     * @see     GetType();
     */ 
    function SetType($type) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('type' =>& $type));

        static $types = array('submit', 'cancel', 'reset', 'command');

        if (is_null($type)) {
            $this->_type = null;
            $ret = true;
        } else if (!is_string($type)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else if (!empty($type) && !in_array(strtolower($type), $types)) {
            $this->_SetError('value must be one of the following types: '.join(',', $types));
            $ret = false;
        } else {
            $this->_type = $type;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Get the buttons type
     *
     * @access  public
     * @return  string                  current value of the type
     * @see     SetType();
     */ 
    function GetType() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_type;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Sets a value indicating whether Form validation is performed when the
     * Button control is clicked.
     *
     * By default, form validation is performed when a Button control of type
     * 'submit' is clicked and not when any other type of Button control is
     * clicked. Form validation determines whether the controls on the form
     * all pass their validation rules.
     *
     * You can specify or determine whether validation is performed on both the
     * client and the server when a Button control is clicked by using the
     * 'validate' property. To prevent validation from being performed, set the
     * 'validate' property to false.
     *
     * @access  public
     * @param   string      $validate   string to set the validate to
     * @return  boolean                 true if successful
     * @see     GetValidate();
     */ 
    function SetValidate($validate) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('validate' =>& $validate));

        $ret = false;
        if (is_null($validate)) {
            $this->_validate = null;
            $ret = true;
        } else if (!is_string($validate)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else {
            $this->_validate = $validate;
            $ret = true;
        }

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Gets a value indicating whether Form validation is performed when the
     * Button control is clicked.
     *
     * @access  public
     * @return  string                  current value of the validate
     * @see     SetValidate();
     */ 
    function GetValidate() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        $ret = $this->_validate;

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Return the value of a html tag attribute.
     *
     * This method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'type' - returns 'submit'
     *   + 'value' - returns value of 'label' property
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__, array('name' => $name));

        $ret = null;

        switch (strtolower($name)) {
            case 'type':
                $ret = 'submit';
                break;
            case 'value':
                $ret = $this->Get('label');
                break;
            default:
                $ret = parent::TagAttribute($name);
                break;
        }
  
        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * Render the control
     *
     * This renders the control and any associated validation script and any
     * child controls.
     *
     * @access  public
     * @return  string                  HTML required to render the control
     * @see     Render();
     */ 
    function Render() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        if ($this->_content) {
            $this->tagName = 'button';
            $this->tagComplete = false;
        } else {
            $this->tagName = 'input';
            $this->tagComplete = true;
        }

        // This passes the id of the submitting button for access by page
        // framework.
        $this->_name = '_form_submit['.$this->id.']';

        $ret = parent::Render();

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__, $ret);
        return $ret;
    }

    /**
     * OnClick
     *
     * This method is called when the button is clicked.
     * 
     * @access  protected
     * @return  void
     */
    function OnClick() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiButton() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);

        TNTWuiCtrl::TNTWuiCtrl();

        $this->tagName = 'input';
        $this->tagComplete = true;

        array_push($this->tagAttrNames, 'type');

        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiButton() {
        //TRACE_EE TNTClass::TraceEntry(__CLASS__, __FUNCTION__);


        //TRACE_EE TNTClass::TraceExit(__CLASS__, __FUNCTION__);
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
