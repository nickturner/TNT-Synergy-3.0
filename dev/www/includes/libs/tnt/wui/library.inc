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
 * Widget Library
 * ~~~~~~~~~~~~~~
 *
 * This library holds the core classes that comprise the basis for the
 * Widgets that are used to build a web page.
 *
 * A Widget is an object used to hold data and present an interface to the
 * user. A widget is a combination of state and procedure. Each widget is a
 * member of a class, which holds the procedures and data structures common
 * to all widgets of that class. A widget instance holds the procedures and
 * data structures particular to that single widget. Each widget class
 * typically provides the general behavior associated with a particular kind
 * of interaction with the user.
 *
 * @package     TNT.Widget
 * @author      Nick Turner <nick@tnt-synergy.com>
 * @version     2.0
 */
$library = array(
    'name'      => 'TNT.Widget',
    'version'   => 0x2000,
    'author'    => 'Nick Turner <nick@tnt-synergy.com>',
    'functions' => array(
        /* Function name    =>      source file */
        ),
    'classes'   => array(
        /* Class name       =>      source file */
        'TNTWidget'         =>      'tntwidget.class.inc',
        'TNTWidgets'        =>      'tntwidgets.class.inc',
        'TNTWindow'         =>      'tntwindow.class.inc',
        ),
    );

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
