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
 * Password Class
 * ~~~~~~~~~~~~~~
 *
 * This handles the manipulation and checking of basic passwords.
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 */

/*
 * Dependencies
 */
TNTLoader::RequireClass('TNTClass');

/*
 * Globals
 */

/*
 * Defines
 */
#define('TNT_PWD_ENC_NONE'     'none');
#define('TNT_PWD_ENC_CRYPT'    'crypt');
#define('TNT_PWD_ENC_MD5'      'md5');

/**
 * Static class to handle the manipulation of basic passwords.
 *
 * @package TNT.Security
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   21/10/03
 * @version 2.0
 * @singleton
 */
class TNTPwd extends TNTClass {

    /**
     * Generate a pronouncable password.
     *
     * @static
     * @access  public
     * @return  string              the password
     */
    /*static*/ function Generate() {

        static $syllables = 'er,in,tia,wol,fe,pre,vet,jo,nes,al,len,son,cha,ir,ler,bo,ok,tio,nar,sim,ple,bla,ten,toe,cho,co,lat,spe,ak,er,po,co,lor,pen,cil,li,ght,wh,at,the,he,ck,is,mam,bo,no,fi,ve,any,way,pol,iti,cs,ra,dio,sou,rce,sea,rch,pa,per,com,bo,sp,eak,st,fi,rst,gr,oup,boy,ea,gle,tr,ail,bi,ble,brb,pri,dee,kay,en,be,se';
        static $syllable_array = null;
        if (!$syllable_array) {
            $syllable_array = explode(',', $syllables);
        }
    
        $ret = '';
        srand((double)microtime() * 1000000);
        for ($count = 1; $count <= 4; $count++) {
            if (rand() % 10 == 1) {
                $ret .= sprintf('%0.0f',(rand() % 50) + 1);
            } else {
                $ret .= sprintf('%s', $syllable_array[rand() % 62]);
            }
        }

        return $ret;
    }
    
    /**
     * Encrypt a password.
     *
     * @static
     * @access  public
     * @param   string      $text       text to encrypt
     * @param   string      $salt       any required password salt
     * @param   string      $type       optional encryption type to use
     * @return  string                  the encrypted password
     */
    /*static*/ function Encrypt($text, $salt = null, $type = null) {
    
        // Allow differing encryption levels (default to most secure)
        switch ($type) {
            case 'none':
                $ret = $text;
                break;
            case 'crypt':
                $ret = ($salt ? crypt($text, $salt) : crypt($text));
                break;
            case 'md5':
            default:
                $ret = md5($text);
                break;
        }
    
        return $ret;
    }
    
    /**
     * Authenticate a password.
     *
     * @static
     * @access  public
     * @param   string      $text       text to check against the password
     * @param   string      $pass       the encrypted password
     * @param   string      $type       optional encryption type to use
     * @return  boolean                 true if the text matches the password
     */
    /*static*/ function Authenticate($text, $pass, $type) {

        // Encrypt the text using the existing password as a seed
        $p = TNTPwd::Encrypt($text, $pass, $type);
    
        // If encrypted text matches pass the we are ok
        $ret = (strcmp($p, $pass) == 0);

        return $ret;
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
