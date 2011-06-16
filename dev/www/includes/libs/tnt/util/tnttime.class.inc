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
 * Time Class 
 * ~~~~~~~~~~
 *
 * This is a small class that handle time conversions etc.
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
TNTLoader::RequireClass('TNTFmtStr');

/*
 * Globals
 */
global $tnt;
$tnt['_static_class_members']['tnttime']['_elementNames'] = array(
    'year', 'month', 'day', 'period', 'hour', 'minute', 'second', 'tz');

/*
 * Defines
 */
define('TNT_TIME_STANDARD',     1);     // Standard structure (structure)
define('TNT_TIME_STRUCTTM',     2);     // UNIX time structure (structure)
define('TNT_TIME_TEXT',         3);     // Standard text format (string)
define('TNT_TIME_MYSQLTEXT',    4);     // MYSQL Databse text format (string)
define('TNT_TIME_TIMESTAMP',    5);     // Timestamp text format (string)
define('TNT_TIME_DESC',         6);     // English descriptive text (string)
define('TNT_TIME_UTC',          9);     // UTC numeric text (integer)

/**
 * Class to handle date/time stamps
 *
 * It uses the standard TNTFmtStr interface.
 *
 * The class can accept times in any of the given formats and reformat it
 * back into one of the given formats.
 *
 * The class stores the date/time in named component form (format 1);
 *    array('year' => '',       // Date: Full year number
 *          'month' => '',      // Date: Month (1 - 12)
 *          'day' => '',        // Date: Day (1 - 31)
 *          'period' => '',     // Date: Period (AD | BC) (optional)
 *          'hour' => '',       // Time: Hour (0 - 23)
 *          'minute' => '',     // Time: Minute (0 - 59)
 *          'second' => '',     // Time: Second (0 - 59)
 *          'tz' => '',         // Time: Timezone (optional)
 *          );
 *
 * Date and/or Time should be provided, either being optional. Complete values
 * must be provided except when using the English description in which case the
 * date & time can be in relation to the current date & time.
 *
 * If either Date or Time is ommitted then the corresponding array items will
 * be ommitted (or set to null).
 *
 * Array input formats can not mix keys, such mixtures will produce incorrect
 * results. Array keys other than those shown below will be ignored.
 *
 * An all zero Date and Time will result in null being returned
 *
 * The following formats are accepted:
 *    1) array ('day'=>'dd', 'month'=>'mm', 'year'=>'yyyy', 'period'='pp',
 *              'hour'=>'hh', 'minute'=>'ii', 'second'=>'ss', 'tz'=>'zz')
 *    2) array ('tm_mday'=>'dd', 'tm_mon'=>'mm', 'tm_year'=>'yy',
 *              'tm_hour'=>'hh', 'tm_min'=>'ii', tm_sec=>'ss')
 *    3) string ("dd/mm/yyyy [AD|BC] hh:ii[:ss] [TZ]")
 *    4) string ("yyyy-dd-mm hh:ii:ss" - as used by database)
 *    5) string ("yyyyddmmhhiiss" - as used by database)
 *    6) string (English description as understood by strtotime())
 *    9) number (UNIX time stamp)
 *
 * NB: Format (1) is the same as the normalised format returned by the function
 * If only the Time is specified then format (3) and (4) are indistiguishable
 * and the code will assume format (3)
 *
 * Period is always optional in all formats but must be provided in array
 * formats where an absent value will contain a null value.
 *
 * Seconds are normally optional in all string formats but must be provided in
 * array formats. Where a string format ommits the seconds then the array format
 * will contain a null value for the seconds and not a zero value.
 *
 * @package TNT.Util
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 */
class TNTTime extends TNTFmtStr {

    /**
     * Format the time was originally decode from
     * @var     integer                 one of the TNT_TIME_ enumerations
     * @access  public
     */
    var $_format = null;

    /**
     * Check the object is valid for use.
     *
     * @access  public
     * @return  boolean                 true if object is valid
     */
    function IsValid() {
        return (!is_null($this->_value) && $this->_format);
    }

    /**
     * Set the object using the given data.
     *
     * To clear the object pass the data as null or empty string.
     *
     * @access  public
     * @param   mixed       $data      value to set object to
     * @return  boolean                 true if successfull
     * @see Get(), SetElement(), GetElement()
     */
    function Set($data) {

        $this->_elements = null;
        $this->_format = null;

        $ret = true;

        if (empty($data)) {
            $this->_elements = null;
            $this->_format = null;
        } else if (is_object($data) && is_a($data, 'tnttime')) {
            $this->_elements = $data->_elements;
            $this->_format = $data->_format;
        } else if (is_array($data)) {
            $data = array_change_key_case($data, CASE_LOWER); 
            if ((isset($data['year']) && isset($data['month']) && isset($data['day'])) ||
                (isset($data['hour']) && isset($data['minute']) && isset($data['second']))) {
                $format = TNT_TIME_STANDARD;
                if (isset($data['year'])) {
                    $v['year'] = $data['year'];
                    $v['month'] = $data['month'];
                    $v['day'] = $data['day'];
                    $v['period'] = (isset($data['period']) ? $data['period'] : null);
                }
                if (isset($data['hour'])) {
                    $v['hour'] = $data['hour'];
                    $v['minute'] = $data['minute'];
                    $v['second'] = $data['second'];
                    $v['tz'] = (isset($data['tz']) ? $data['tz'] : '');
                }
            } else if ((isset($data['tm_year']) && isset($data['tm_mon']) && isset($data['tm_mday'])) ||
                       (isset($data['tm_hour']) && isset($data['tm_min']) && isset($data['tm_sec']))) {
                $format = TNT_TIME_STRUCTTM;
                if (isset($data['tm_year'])) {
                    $v['year'] = $data['tm_year'] + 1900;
                    $v['month'] = $data['tm_mon'] + 1;
                    $v['day'] = $data['tm_mday'];
                    $v['period'] = (!empty($data['period']) ? $data['period'] : null);
                }
                if (isset($data['hour'])) {
                    $v['hour'] = $data['tm_hour'];
                    $v['minute'] = $data['tm_min'];
                    $v['second'] = $data['tm_sec'];
                    $v['tz'] = (!empty($data['tz']) ? $data['tz'] : '');
                }
            } else {
                $this->_SetError('unrecognised array input format - value=', $data);
                $ret = false;
            }
        } else if (is_string($data)) {
            $num_matches = 0;
    
            // Try "yyyy-mm-dd[ AD|BC][ hh:mm[:ss] [tz]]" format 
            if (preg_match('/^\s*([0-9]{4})-([0-9]{2})-([0-9]{2})(?:\s*(AD|BC))?(?:\s+([0-9]{2}):([0-9]{2})(?::([0-9]{2}))?(?:\s+(.*))?)?\s*$/', $data, $matches) && is_array($matches) && ($num_matches = count($matches)) && $num_matches >= 4) {
                $format = TNT_TIME_MYSQLTEXT;
                $v['year'] = $matches[1];
                $v['month'] = $matches[2];
                $v['day'] = $matches[3];
                $v['period'] = (!empty($matches[4]) ? $matches[4] : null);
                $v['hour'] = (!empty($matches[5]) ? $matches[5] : null);
                $v['minute'] = (!empty($matches[6]) ? $matches[6] : null);
                $v['second'] = (!empty($matches[7]) ? $matches[7] : null);
                $v['tz'] = (!empty($matches[8]) ? $matches[8] : null);
    
            // Try "yyyymmdd[hhii[ss]]" format
            } else if (preg_match('/^\s*([0-9]{4})([0-9]{2})([0-9]{2})(?:([0-9]{2})([0-9]{2})([0-9]{2})?)?\s*$/', $data, $matches) && is_array($matches) && ($num_matches = count($matches)) && $num_matches >= 4) {
                $format = TNT_TIME_TIMESTAMP;
                $v['year'] = $matches[1];
                $v['month'] = $matches[2];
                $v['day'] = $matches[3];
                $v['period'] = null;
                $v['hour'] = (!empty($matches[4]) ? $matches[4] : null);
                $v['minute'] = (!empty($matches[5]) ? $matches[5] : null);
                $v['second'] = (!empty($matches[6]) ? $matches[6] : null);
                $v['tz'] = null;
    
            // Try "dd/mm/yyyy[ AB|BC][ hh:mm[:ss] [tz]]" format
            } else if (preg_match('/^\s*([0-9]{1,2})[\/-]([0-9]{1,2})[\/-]([0-9]{1,4})(?:\s*(AD|BC))?(?:\s+([0-9]{1,2})[:-]([0-9]{1,2})(?:[:-]([0-9]{1,2}))?(?:\s+(.*))?)?\s*$/i', $data, $matches) && is_array($matches) && ($num_matches = count($matches)) && $num_matches >= 4) {
                $format = TNT_TIME_TEXT;
                $v['day'] = $matches[1];
                $v['month'] = $matches[2];
                $v['year'] = $matches[3];
                $v['period'] = (!empty($matches[4]) ? $matches[4] : null);
                $v['hour'] = (!empty($matches[5]) ? $matches[5] : null);
                $v['minute'] = (!empty($matches[6]) ? $matches[6] : null);
                $v['second'] = (!empty($matches[7]) ? $matches[7] : null);
                $v['tz'] = (!empty($matches[8]) ? $matches[8] : null);
    
            // Try "hhii[ss]" format
            } else if (preg_match('/^\s*([0-9]{2})([0-9]{2})([0-9]{2})?\s*$/', $data, $matches) && is_array($matches) && ($num_matches = count($matches)) && ($num_matches == 3 || $num_matches == 4)) {
                $format = TNT_TIME_TIMESTAMP;
                $v['hour'] = $matches[1];
                $v['minute'] = $matches[2];
                $v['second'] = (!empty($matches[3]) ? $matches[3] : null);
                $v['tz'] = (!empty($matches[4]) ? $matches[4] : null);
    
            // Try "hh:mm[:ss][ tz]" format
            } else if (preg_match('/^\s*([0-9]{1,2})[:-]([0-9]{1,2})(?:[:-]([0-9]{1,2}))?(?:\s+(.*))?\s*$/i', $data, $matches) && is_array($matches) && ($num_matches = count($matches)) && ($num_matches >= 3 && $num_matches <= 5)) {
                $format = TNT_TIME_TEXT;
                $v['hour'] = $matches[1];
                $v['minute'] = $matches[2];
                $v['second'] = (!empty($matches[3]) ? $matches[3] : null);
                $v['tz'] = (!empty($matches[4]) ? $matches[4] : null);
    
            // Try English language description
            // (possibly prefixed with (time: or date:) to mask the value
            } else if (($t = strtotime(preg_replace('/^(time:|date:)/U', '', $data))) != -1) {
                $format = TNT_TIME_DESC;
                if ($t == 0) {
                    $v = null;
                } else {
                    $tm = localtime($t, true);
                    if (strncasecmp($data, 'time:', 5) != 0) {
                        // We want the date bit of this structure
                        $v['year'] = $tm['tm_year'] + 1900;
                        $v['month'] = $tm['tm_mon'] + 1;
                        $v['day'] = $tm['tm_mday'];
                        $v['period'] = null;
                    }
                    if (strncasecmp($data, 'date:', 5) != 0) {
                        // We only want the time bit of this structure
                        $v['hour'] = $tm['tm_hour'];
                        $v['minute'] = $tm['tm_min'];
                        $v['second'] = $tm['tm_sec'];
                        $v['tz'] = '';
                    }
                }
            } else {
                $this->_SetError('string value must be of the format "yyyymmdd[hhiiss]", "yyyy-mm-dd[ hh:mm[:ss][ tz]]", "dd/mm/yyyy[ AB|BC][ hh:mm[:ss][ tz]]", "hhiiss[ tz]", "hh:mm[:ss][ tz]" or English description: '.$data);
                $ret = false;
            }
        } else if (is_numeric($data)) {
            $format = TNT_TIME_UTC;
            $tm = localtime($data, true);
            $v['year'] = $tm['tm_year'] + 1900;
            $v['month'] = $tm['tm_mon'] + 1;
            $v['day'] = $tm['tm_mday'];
            $v['period'] = null;
            $v['hour'] = $tm['tm_hour'];
            $v['minute'] = $tm['tm_min'];
            $v['second'] = $tm['tm_sec'];
            $v['tz'] = '';
        } else {
            $this->_SetError('unknown input type');
            $ret = false;
        }
    
        if ($ret && isset($v['year'])) {
            // Force the values to be numeric
            $v['year'] += 0;
            $v['month'] += 0;
            $v['day'] += 0;
    
            // Check range of the values (this function only supports AD dates)
            if (!checkdate($v['month'], $v['day'], $v['year'])) {
                $this->_SetError('invalid date: '."{$v['year']}-{$v['month']}-{$v['day']}");
                $ret = false;
            }
        }
    
        if ($ret && isset($v['hour'])) {
            // Force the values to be numeric
            $v['hour'] += 0;
            $v['minute'] += 0;
            if (!is_null($v['second'])) {
                $v['second'] += 0;
            }
    
            // Check range of the values
            if ($v['hour'] < 0 || $v['hour'] > 23 ||
                $v['minute'] < 0 || $v['minute'] > 59 ||
                $v['second'] < 0 || $v['second'] > 59) {
                $this->_SetError('invalid time: '."{$v['hour']}:{$v['minute']}:{$v['second']}");
                $ret = false;
            }
        }
    
        if ($ret) {
            $this->_format = $format;
            $this->_elements = $v;
        }
    
        return $ret;
    }

    /**
     * Cast this object to another type
     *
     * <pre>
     * The following type to formats are used,
     *   'string'  - TNT_TIME_TEXT
     *   'array'   - TNT_TIME_STANDARD
     *   'integer' - TNT_TIME_UTC
     * </pre>
     *
     * @access  public
     * @param   string      $type       PHP type or class name to cast to
     * @return  mixed                   reference to object in given type
     * @see Get
     */
    function &Cast($type) {

        switch ($type) {
            case 'string':
                $ret = $this->Format(TNT_TIME_TEXT);
                break;
            case 'array':
                $ret = $this->Format(TNT_TIME_STANDARD);
                break;
            case 'integer':
                $ret = $this->Format(TNT_TIME_UTC);
                break;
            default:
                $ret = parent::Cast('type');
                break;
        }

        return $ret;
    }

    /**
     * Get the current stored date/time in the given format
     *
     * @access  public
     * @param   integer     $format     the format to return the data in
     * @return  mixed                   time in one of the predefined formats
     */
    function Format($format = 0) {
    
        if (!$format) {
            $format = $this->_format;
        }
    
        switch ($format) {
            case TNT_TIME_STANDARD:
                /* array (normalized) */
                $ret = $this->_elements;
                break;
            case TNT_TIME_STRUCTTM:
                /* array (struct tm) */
                $ret = null;
                if (isset($this->_elements['year'])) {
                    $ret['tm_year'] = $this->_elements['year'] - 1900;
                    $ret['tm_mon'] = $this->_elements['month'] - 1;
                    $ret['tm_mday'] = $this->_elements['day'];
                }
                if (isset($this->_elements['hour'])) {
                    $ret['tm_hour'] = $this->_elements['hour'];
                    $ret['tm_min'] = $this->_elements['minute'];
                    $ret['tm_sec'] = $this->_elements['second'];
                }
                break;
            case TNT_TIME_TEXT:
                /* string ("dd/mm/yyyy [AD|BC] hh:ii[:ss] [TZ]") */
                $ret = '';
                if (isset($this->_elements['year'])) {
                    $ret .= sprintf('%02u/%02u/%04u',
                                      $this->_elements['day'],
                                      $this->_elements['month'],
                                      $this->_elements['year']);
                    if (!is_null($this->_elements['period'])) {
                        $ret .= (' '.$this->_elements['period']);
                    }
                }
                if (isset($this->_elements['hour'])) {
                    $ret .= sprintf(' %02u:%02u',
                                      $this->_elements['hour'],
                                      $this->_elements['minute']);
                    if (!is_null($this->_elements['second'])) {
                        $ret .= sprintf(':%02u', $this->_elements['second']);
                    }
                    if (!is_null($this->_elements['tz'])) {
                        $ret .= (' '.$this->_elements['tz']);
                    }
                }
                break;
            case TNT_TIME_MYSQLTEXT:
                /* string ("yyyy-dd-mm hh:ii[:ss]" - as used by database) */
                $ret = '';
                if (isset($this->_elements['year'])) {
                    $ret .= sprintf('%04u-%02u-%02u',
                                      $this->_elements['year'],
                                      $this->_elements['month'],
                                      $this->_elements['day']);
                }
                if (isset($this->_elements['hour'])) {
                    $ret .= sprintf(' %02u:%02u',
                                      $this->_elements['hour'],
                                      $this->_elements['minute']);
                    if (!is_null($this->_elements['second'])) {
                        $ret .= sprintf(':%02u', $this->_elements['second']);
                    }
                }
                break;
            case TNT_TIME_TIMESTAMP:
                /* string ("yyyyddmmhhiiss" - as used by database) */
                $ret = '';
                if (isset($this->_elements['year'])) {
                    $ret .= sprintf('%04u%02u%02u',
                                      $this->_elements['year'],
                                      $this->_elements['month'],
                                      $this->_elements['day']);
                }
                if (isset($this->_elements['hour'])) {
                    $ret .= sprintf('%02u%02u%02u',
                                      $this->_elements['hour'],
                                      $this->_elements['minute']);
                    if (!is_null($this->_elements['second'])) {
                        $ret .= sprintf(':%02u', $this->_elements['second']);
                    }
                }
                break;
            case TNT_TIME_DESC:
                /* English description as understood by strtotime() */
                /* can't reproduce the exact format given to Set() */
                $ret = '';
                if (isset($this->_elements['year'])) {
                    $ret .= sprintf('%04u-%02u-%02u',
                                      $this->_elements['year'],
                                      $this->_elements['month'],
                                      $this->_elements['day']);
                }
                if (isset($this->_elements['hour'])) {
                    $ret .= sprintf('%02u:%02u:%02u',
                                      $this->_elements['hour'],
                                      $this->_elements['minute'],
                                      $this->_elements['second']);
                }
                break;
            case TNT_TIME_UTC:
                /* number (UNIX time stamp) */
                $ret = 0;
                if (isset($this->_elements['year']) && isset($this->_elements['hour'])) {
                    $ret = mktime($this->_elements['hour'], $this->_elements['minute'], $this->_elements['second'], $this->_elements['month'], $this->_elements['day'], $this->_elements['year']);
                } else if (isset($this->_elements['year'])) {
                    $ret = mktime(0, 0, 0, $this->_elements['month'], $this->_elements['day'], $this->_elements['year']);
                } else if (isset($this->_elements['hour'])) {
                    $ret = mktime($this->_elements['hour'], $this->_elements['minute'], $this->_elements['second']);
                }
                break;
            default:
                $this->_SetError('unsupported format specified: '.$format);
                $ret = null;
                break;
        }
    
        return $ret;
    }

    /**
     * Compare another date/time to this one
     *
     * Dates are compared using absolute values and not UTC values so that
     * dates outside of the UTC range can be compared.
     *
     * Returns: < 0 if we are less
     *          > 0 if we are greater
     *          0 if equal
     *          false if error in comparison
     *
     * @access  public
     * @param   mixed   $that       value representing date/time to compare
     * @return  integer             result of comparison
     */
    function Compare($that) {

        $ret = false;

        // Start error section
        do {
            if (!$this->IsValid()) {
                $this->_SetError('invalid self time');
                break;
            }

            if (!is_a($that, 'tnttime')) {
                $that = &new TNTTime($that);
            }

            if (!$that->IsValid()) {
                $this->_SetError('invalid comparison time');
                break;
            }

            $v1 = &$this->_value;
            $v2 = &$that->_value;
    
            if ((isset($v1['year']) != isset($v2['year'])) ||
                (isset($v1['hour']) !=  isset($v2['hour']))) {
                    $this->_SetError('times are not of the same precision');
                break;
            }
    
            $ret = 0;

            $v1_period = (($v1['period'] == 'BC') ? 0 : 1);
            $v2_period = (($v2['period'] == 'BC') ? 0 : 1);
    
            if (isset($v1['year'])) {
                // Compare the date
                if ($v1_period < $v2_period) {
                    $ret = -1;
                } else if ($v1_period > $v2_period) {
                    $ret = 1;
                } else if ($v1['year'] < $v2['year']) {
                    $ret = -1;
                } else if ($v1['year'] > $v2['year']) {
                    $ret = 1;
                } else if ($v1['month'] < $v2['month']) {
                    $ret = -1;
                } else if ($v1['month'] > $v2['month']) {
                    $ret = 1;
                } else if ($v1['day'] < $v2['day']) {
                    $ret = -1;
                } else if ($v1['day'] > $v2['day']) {
                    $ret = 1;
                }
            }
    
            if ($ret == 0 && isset($v1['hour'])) {
                // Compare the time
                if ($v1['hour'] < $v2['hour']) {
                    $ret = -1;
                } else if ($v1['hour'] > $v2['hour']) {
                    $ret = 1;
                } else if ($v1['minute'] < $v2['minute']) {
                    $ret = -1;
                } else if ($v1['minute'] > $v2['minute']) {
                    $ret = 1;
                } else if ($v1['second'] < $v2['second']) {
                    $ret = -1;
                } else if ($v1['second'] > $v2['second']) {
                    $ret = 1;
                }
            }
        } while (false);
        // End error section

        return $ret;
    }

    /**
     * Set the value
     *
     * DEPRECIATED: Use standard {@link Set()}
     *
     * @status  depeciated use Set()
     * @access  public
     * @param   mixed       $data       value to set object to
     * @return  boolean                 true if successfull
     * @see Get(), SetElement(), GetElement()
     */
    function SetValue($data) {
        return $this->Set($data);
    }

    /**
     * Get the value
     *
     * DEPRECIATED: Use standard {@link Format()}
     *
     * @status  depeciated use Format()
     * @access  public
     * @param   integer     $format     the format to return the data in
     * @return  mixed                   time in one of the predefined formats
     */
    function GetValue($format = 0) {
        return $this->Format($format);
    }

    /**
     * Construct the object
     *
     * @access  public
     * @param   mixed       $data       initial value for object
     * @return  void                    constructor
     */
    function TNTTime($data = null) {
        TNTFmtStr::TNTFmtStr($data, $GLOBALS['tnt']['_static_class_members']['tnttime']['_elementNames']);
    }

    /**
     * Destruct the object.
     *
     * @access  private
     */
    function _TNTTime() {
    }
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
