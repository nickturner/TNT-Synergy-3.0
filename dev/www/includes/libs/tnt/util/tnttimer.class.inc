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
 * Timer Class 
 * ~~~~~~~~~~~
 *
 * This is a small class that handle interval timing.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTObject');

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */

/**
 * Class to create timing intervals to time execution of parts of the code.
 *
 * @package TNT.Util
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 */
class TNTTimer extends TNTObject {
    /**
     * Start time in microseconds
     * @var     float                   time in microseconds
     * @access  private
     */
    var $_start_utime = 0;

    /**
     * End time in microseconds
     * @var     float                   time in microseconds
     * @access  private
     */
    var $_end_utime = 0;

    /**
     * Return the current microtime structure
     *
     * @access  public
     * @return  float                   time in microseconds
     */
    function Microtime() {
        list($usec, $sec) = explode(' ', microtime());
        return ((float)$usec + (float)$sec);
    }

    /**
     * Duration between start and end or start and now if not end
     *
     * @access  public
     * @param   integer                 precision to return number to
     * @return  float                   duration in microseconds
     */
    function Duration($precision = 3) {
        if (!$this->_start_utime) {
            return (float)0;
        }
        if (!$this->_end_utime) {
            $end = $this->Microtime();
        } else {
            $end = $this->_end_utime;
        }

        return round((float)($end - $this->_start_utime), $precision);
    }

    /**
     * Start the timer
     *
     * @access  public
     * @return  void
     */
    function Start() {
        $this->_start_utime = (float)$this->MicroTime();
        $this->_end_utime = (float)0;
    }

    /**
     * Stop the timer
     *
     * @access  public
     * @return  void
     */
    function Stop() {
        $this->_end_utime = (float)$this->MicroTime();
    }

    /**
     * Constructor
     *
     * @access  public
     */
    function TNTTimer() {
        $this->Start();
    }

    /**
     * Destructor
     *
     * @access  public
     */
    function _TNTTimer() {
        $this->Stop();
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
