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
 * Wui UploadBox Class 
 * ~~~~~~~~~~~~~~~~~~~~~
 *
 * Represents a file upload selection box on a Wui form.
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
 * The UploadBox class allows you to add a file selection control to a Form.
 *
 * <pre>
 * {tnt_ctrl ctrl="uploadbox"
 *   id="string"
 *   name="string"
 *   autopostback="true|false"
 *   maxsize="number"
 *   filetypes="string"
 *   value="string"
 * }
 * </pre>
 *
 * Use the UploadBox control to allow the user to select a file to upload to
 * the server.
 *
 * The maximum size of the uploaded file can be specified using the 'MaxSize'
 * property. A CVS list of the mime types of the files to accept can be
 * specified by the 'FileTypes' property.
 *
 * @package TNT.Wui
 * @subpackage Controls
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
 */
class TNTWuiUploadBox extends TNTWuiCtrl {

    /**
     * Maximum size of the uploaded file in bytes
     * @access private
     * @var integer                     optional max file size in bytes
     */
    var $_maxsize = null;

    /**
     * Comma separated list of mime types of files to accept
     * @access private
     * @var string                      csv list of mime types
     */
    var $_filetypes = null;

    /**
     * Temporarily save any file uploaded on a tab before the form is
     * actually submitted.
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function _SaveUploadedFile($f) {

        global $tnt;

        $p = TNTFileSystem::ExpandPath('~/temp/uploaded_files');
        if (!@file_exists($p)) {
            TNTFileSystem::MkDir($p, 0777);
        }
 
        $t = $p.'/'.basename(str_replace('\\', '/', $f));

        //TRACE $this->Trace("Saving upload file from '$f' to '$t'");

        if (!move_uploaded_file($f, $t)) {
            //TRACE $this->Trace('failed to save file');
            $ret = $f;
        } else {
            $ret = $t;
        }
 
        return $ret;
    }

    /**
     * Return the value of a html tag attribute.
     *
     * This method supports the following none attribute tags or those
     * requiring custom encoding for the following; 
     *   + 'type' - returns 'file'
     *
     * @access  public
     * @param   string      $name       name of html attribute to get
     * @return  string                  value of attribute (or null)
     */
    function TagAttribute($name) {

        $ret = null;

        switch (strtolower($name)) {
            case 'type':
                $ret = 'file';
                break;
            default:
                $ret = parent::TagAttribute($name);
                break;
        }
  
        return $ret;
    }

    /**
     * Construct the widget
     *
     * This will construct widget, initialising all member variables etc.
     *
     * @access  public
     */
    function TNTWuiUploadBox() {

        TNTWuiCtrl::TNTWuiCtrl();

        $this->tagName = 'input';
        $this->tagComplete = 'true';

        array_push($this->tagAttrNames, 'type');

    }

    /**
     * Destruct the widget.
     *
     * This will destruct the widget and any children.
     *
     * @access  private
     */
    function _TNTWuiUploadBox() {

    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
