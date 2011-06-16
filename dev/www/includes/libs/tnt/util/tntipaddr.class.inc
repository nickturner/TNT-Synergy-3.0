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
 * IP Address
 * ~~~~~~~~~~
 *
 * This class manipulates IP address values.
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTFmtStr'); 

/*
 * Globals
 */

/*
 * Defines
 */

/**
 * Class to manipulate an IP address.
 *
 * Understands the following IP address classes;
 * Class A - n.h
 *  + First Octet:  1 to 126. (Starts with binary bit - 0).
 *  + Network Address: 8 bits or first octets.
 *  + Host/Node Address: 24 bits or last 3 octects.
 *
 * Class B - 
 *  + First Octet: 128 to 191. (Starts with binary bits - 10).
 *  + Network Address: 16 bits or first 2 octets.
 *  + Host/Node Address: 16 bits or last 2 octects.
 *
 * Class C -
 *  + First Octet: 192 to 223. (Starts with binary bits - 110).
 *  + Network Address: 24 bits or first 3 octets.
 *  + Host/Node Address: 8 bits or last octet.
 *
 *
 * Class D -
 *  + First Octet: 224 to 239. (Starts with binary bits - 1110).
 *
 * Class E - 
 * + First Octet: 240 to 255. (Starts with binary bits - 1111).
 *
 * Reserved IP addresses are;
 *  + 000.000.000.000 - default address
 *  + 127.000.000.001 - loop back address
 *  + 255.255.255.255 - loop back address
 *
 * @package TNT.Util
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 */
class TNTIpAddr extends TNTFmtStr {

    /**
     * Return the class of the IP address ('A' - 'E')
     *
     * Returns null on failure.
     *
     * @access  public
     * @return  string                  class of address ('A' - 'E')
     */
    function IpClass() {

        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if ($this->_elements) {
            $o = $this->_elements[0];

            if (($o & 1) == 0) {
                $ret = 'A';
            } else if (($o & 3) == 2) {
                $ret = 'B';
            } else if (($o & 7) == 6) {
                $ret = 'C';
            } else if (($o & 15) == 14) {
                $ret = 'D';
            } else if (($o & 15) == 15) {
                $ret = 'E';
            }
        }

        return $ret;
    }

    /**
     * Return true if this is the Default IP address "0.0.0.0"
     *
     * @access  public
     * @return  boolean                 true if is 'default' IP address
     */
    function IsDefault() {

        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        $ret = ($this->_elements &&
                ($this->_elements[0] == 0) &&
                ($this->_elements[1] == 0) &&
                ($this->_elements[2] == 0) &&
                ($this->_elements[3] == 0));

        return $ret;
    }

    /**
     * Return true if this is the Loopback IP address "127.0.0.1"
     *
     * @access  public
     * @return  boolean                 true if is 'loopback' IP address
     */
    function IsLoopback() {

        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        $ret = ($this->_elements &&
                ($this->_elements[0] == 127) &&
                ($this->_elements[1] == 0) &&
                ($this->_elements[2] == 0) &&
                ($this->_elements[3] == 1));

        return $ret;
    }

    /**
     * Return true if this is the Broadcast IP address "255.255.255.255"
     *
     * @access  public
     * @return  boolean                 true if is 'broadcast' IP address
     */
    function IsBroadcast() {

        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        $ret = ($this->_elements &&
                ($this->_elements[0] == 255) &&
                ($this->_elements[1] == 255) &&
                ($this->_elements[2] == 255) &&
                ($this->_elements[3] == 255));

        return $ret;
    }

    /**
     * Return the normalised form of the ip address.
     *
     * This the expanded dotted decimal notation, for example, the address
     * "10.7" will be returned "010.000.000.007" where each element is
     * zero padded to 3 digits.
     *
     * @access  public
     * @param   boolean     $wild       return in wild card format
     * @return  string                  the normalised address (or null)
     */
    function Normalise() {

        $ret = null;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if ($this->_elements) {
            $ret = sprintf('%03d.%03d.%03d.%03d',
                           $this->_elements[0],
                           $this->_elements[1],
                           $this->_elements[2],
                           $this->_elements[3]);
        }

        return $ret;
    }

    /**
     * Return the value of a single octet in the IP address
     *
     * @access  public
     * @param   integer     $index      index of octet to get (0 - 4)
     * @return  string                  value of the octet (0 - 255)
     */
    function GetOctet($index) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if ($index >= 0 && $index <= 3) {
            $ret = $this->_elements[$index];
        }

        return $ret;
    }

    /**
     * Set the value of a single octet in the IP address
     *
     * @access  public
     * @param   integer     $index      index of octet to set (0 - 3)
     * @param   string      $value      value to set item to (0 - 255)
     * @return  boolean                 true if successfull
     */
    function SetOctet($index, $value) {

        $ret = false;

        if (!$this->_elements) {
            $this->_Explode();
        }

        if (($value >= 0 && $value <= 255) &&
            ($index >= 0 && $index <= 3)) {
            $this->_elements[$index] = (string)$value;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Return an array of octets that make up the ip address
     *
     * @access  public
     * @return  integer[4]              array of octets in the IP address
     */
    function GetOctets() {

        if (!$this->_elements) {
            $this->_Explode();
        }

        return $this->_elements;
    }

    /**
     * Set the array of octets that make up the IP address.
     *
     * If less than 4 octets are provided in the array then the array
     * is padded by inserting zero octets after the first octet.
     * (ie. 10.7 => 10.0.0.7).
     *
     * @access  public
     * @param   integer[]   $items      items to convert to CSV string
     * @return  boolean                 true if successfull
     */
    function SetOctets($items) {

        $ret = false;

        $this->_elements = null;

        if (!$items) {
            $this->_elements = array(0,0,0,0);
            $this->_string = null;
            $ret = true;
        } else if (is_array($items) && ($l = count($items)) <= 4) {
            if ($l == 0) {
                $this->_elements = array(0,0,0,0);
            } else {
                $this->_elements = array();
                reset($items);

                // Add in first item
                $elm = each($items);
                $this->_elements[] = $elm['value'];

                // Add in padding
                for ($i = 0; $i < (4 - $l); $i++) {
                    $this->_elements[] = 0;
                }

                // Copy in remaining elements
                while (($elm = each($items))) {
                    $this->_elements[] = $elm['value'];
                }
            }
            $this->_string = null;
            $ret = true;
        }

        return $ret;
    }

    /**
     * Explode a value into a list of octets
     *
     * @access  protected
     * @return  boolean                 true if successfull
      */
    function _Explode() {

        $elms = explode('.', $this->_string);

        if (count($elms) > 4) {
            $this->_SetError("Too many elements: {$this->string}");
        } else {
            $l = 0;
            foreach ($elms as $elm) {
                if (!is_numeric($elm) || (int)$elm < 0 || (int)$elm > 255) {
                    $this->_SetError("Element $l is not a number between 0 and 255: $elm");
                    $ret = false;
                    break;
                }
                $l++;
            }
            $ret = $this->SetOctets($elms);
        }

        return true;
    }

    /**
     * Implode a value from a list of octets
     *
     * This will always produce a condensed IP address of the form
     *   "n.n.n.n".
     *
     * To produce a normalise address then call {@link Normalise()}.
     *
     * @access  protected
     * @return  boolean                 true if successfull
     */
    function _Implode() {
        $this->_string = sprintf('%d.%d.%d.%d',
                                 $this->_elements[0],
                                 $this->_elements[1],
                                 $this->_elements[2],
                                 $this->_elements[3]);

        return true;
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   mixed       $data       initial value for object
     * @param   string      $delim      delimiter string
     * @param   string      $quote      quote character
     * @param   string      $case       strings are case sensitive
     * @return  void
     */
    function TNTIpAddr($data = null) {
        TNTFmtStr::TNTFmtStr($data);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTIpAddr() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
