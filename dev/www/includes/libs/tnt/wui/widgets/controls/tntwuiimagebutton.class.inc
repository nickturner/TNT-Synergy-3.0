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
 * Smart Image Button Class 
 * ~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents a generic image button control on a Smart form.
 *
 * @package TNT.Smart
 * @subpackage Controls
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTSmartButton');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * The ImageButton class allows you to add a button on to a form which
 * is rendered as an image and has functionality similar to an image map.
 *
 * <pre>
 * <smart:imagebutton
 *        id="ImageButton1" 
 *        type="submit|reset|cancel"
 *        src="string"
 *        cmdname="string"
 *        cmdargs="string"
 *        validate="true|false"
 *        onclick="php: code;" />
 * </pre>
 *
 * Use the ImageButton control to display an image that responds to mouse
 * clicks. Specify the image to display in the control by setting the 'src'
 * property.
 *
 * The 'type' attribute gives the basic type of buttons and there default
 * actions;
 *   + 'submit'  - calls parent Form's {@link TNTSmartForm::RaiseSubmitEvent()}
 *   + 'cancel'  - calls parent Form's {@link TNTSmartForm::RaiseCancelEvent()}
 *   + 'reset'   - calls parent Form's {@link TNTSmartForm::RaiseResetEvent()}
 *   + 'command' - raises a 'command' event.
 *
 * There is no support for a client side 'reset' button as this would only
 * reset back to the values when the page was loaded or postedback and not
 * to any initial values.
 *
 * All Buttons raise the 'click' event before performing their default action.
 *
 * The 'click' and 'command' events bubble through the hierachy allowing parent
 * nodes to handle the events raise by its children.
 *
 * By using the {@link OnClick} event handler, you can programmatically
 * determine the coordinates where the image is clicked. You can then code a
 * response based on the values of the coordinates. Note the origin (0, 0) is
 * located at the upper-left corner of the image.
 *
 * You can use the {@link OnCommand()} event handler to make the ImageButton
 * control behave like a command button. A command name can be associated with
 * the control using the 'CmdName' property. This allows multiple ImageButton
 * controls to be placed on the same Form. The value of the 'CmdName' property
 * can then be programmatically identified in the {@link OnCommand()} event
 * handler to determine the appropriate action to perform when each ImageButton
 * control is clicked. The 'CmdArgs' property can also be used to pass
 * additional information about the command, such as specifying ascending order.
 *
 * Note: Because the <smart:imagebutton> element has no content, you can close
 * the tag with /> instead of using a separate closing tag.
 *
 * By default, form validation is performed when an ImageButton control with a
 * type of 'submit' is clicked. Form validation determines whether the controls
 * pass the validation rules specified for the control. If you want to disable
 * this behaviour then set the 'validate' property to false.
 *
 * @package TNT.Smart
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTSmartImageButton extends TNTSmartButton {

    /**
     * Source URL of the image to display
     * @access private
     * @var string                      URL of image to display
     */
    var $_src = null;

    /**
     * Set the location of an image to display for the button. 
     *
     * Use the <b>src</b> property to specify the URL of an image to display
     * on the <b>ImageButton</b> control.
     * 
     * You can use a relative, absolute or virtual path. A relative URL relates
     * the location of the image to the location of the page without specifying
     * a complete path on the server. An absolute URL provides the complete 
     * path. A virtual URL is given as an absolute path starting at the virtual
     * root director '/synery/' - this makes it easier to use preconfigured
     * paths.
     *
     * Relative and Virtual paths make it easier to move the entire site to
     * another directory.
     *
     * @access  public
     * @param   string      $src        string to set the src to
     * @return  boolean                 true if successful
     * @see     GetSrc();
     */ 
    function SetSrc($src) {

        $ret = false;
        if (is_null($src)) {
            $this->_src = null;
            $ret = true;
        } else if (!is_string($src)) {
            $this->_SetError('value must be either null or a string');
            $ret = false;
        } else {
            $this->_src = $src;
            $ret = true;
        }
  
        return $ret;
    }

    /**
     * Get the location of an image to display for the button. 
     *
     * @access  public
     * @return  string                  current value of the src
     * @see     SetSrc();
     */ 
    function GetSrc() {

        $ret = $this->_src;
  
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

        $ret = null;

        switch (strtolower($name)) {
            case 'type':
                $ret = 'image';
                break;
            default:
                $ret = parent::TagAttribute($name);
                break;
        }
  
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

        // If the image is local and no height or width have been given then
        // try and add them ourselves.

        // This passes the id of the submitting button for access by page
        // framework.
        $this->_name = '_form_submit_dn['.$this->DN().']';

        $ret = parent::Render();

        return $ret;
    }
    
    /**
     * Raise a click event.
     *
     * Overriden from TNTSmartButton in order to add mouse coordinates to
     * the click event.
     *
     * @access  public
     * @return  boolean                 true if default action should occur
     */ 
    function RaiseClickEvent() {

        // Get the X and Y coordinates of the mouse click
        if (isset($_POST[$this->_name])) {
            TNT_Error("\$_POST[$this->_name] = ", $_POST[$this->_name]);
            $x = 0;
            $y = 0;
        } else {
            $x = 0;
            $y = 0;
        }

        $evt =& new TNTSmartEvent();
        $evt->type = 'click';
        $evt->data = array('x' => $x, 'y' => $y);
        $this->DispatchEvent($evt);
        $ret = !$evt->_preventDefault;

        return $ret;
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTSmartImageButton() {

        TNTSmartButton::TNTSmartButton();

        array_remove($this->tagAttrNames, array('value'), false);

        array_push($this->tagAttrNames, 'type', 'src');

    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTSmartImageButton() {


    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
