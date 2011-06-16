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
 * Debug Class 
 * ~~~~~~~~~~~
 *
 * This is a small static class containing methods used to configure and
 * control the TNT framework.
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */

/**
 */

/*
 * Dependencies
 */

/*
 * Globals
 */
global $tnt;

/*
 * Defines
 */
if (!defined('TNT_DEBUG_DIR')) {
    define('TNT_DEBUG_DIR', TNT_TEMP_DIR.'/debug');
}

if (!defined('TNT_DEBUG_EXT')) {
    define('TNT_DEBUG_EXT', '.dbg');
}

if (!defined('TNT_DEBUG_MAX')) {
    define('TNT_DEBUG_MAX', 64 * 1024);
}

/**
 * This static object is used to read and write debug files
 *
 * @package TNT.Core
 * @author  Nick Turner <nick@tnt-synergy.com>
 * @since   22/10/03
 * @version 2.0
 */
class TNTDebug {
    /**
     * Write an entry to the debug file.
     *
     * Each argument to the method is copied to the debug dump and is available
     * to be viewed via the debug viewer. Arguments can be of any type.
     *
     * @access  public
     * @param   mixed   ...         value to dump to debug file.
     * @return  boolean             true if could dump to debug file
     */
    /*static*/ function Dump(/*...*/) {
        static $fp = null;

        if (is_null($fp)) {
            $path = TNT_DEBUG_DIR;
            if (!file_exists($path)) {
                mkdir($path, 0777);
            }
            if (file_exists($path)) {
                $file = $path.'/'.date('d-m-y', time()).TNT_DEBUG_EXT;
                if (!($fp = fopen($file, 'at+'))) {
                   trigger_error('Can not open debug file: '.$file, E_USER_NOTICE);
                   return false;
                }
            }
        }

        if ($fp) {

            $msecs = microtime();

            $rec = array(
                'time' => microtime(),
                'path' => $_SERVER['SCRIPT_NAME'],
                'args' => func_get_args()
                );

            $data = addcslashes(serialize($rec), "\0..\37!@\@\177..\377")."\n";
            $len = strlen($data);

            if ($len >= TNT_DEBUG_MAX) {
                $rec['args'] = array('! DEBUG LINE TOO LONG !');

                $data = addcslashes(serialize($rec), "\0..\37!@\@\177..\377")."\n";
                $len = strlen($data);
            }

            if (!(flock($fp, LOCK_EX) &&
                  (fwrite($fp, $data, $len) === $len) &&
                  flock($fp, LOCK_UN) &&
                  fflush($fp))) {
                trigger_error('Can not write to  debug file: '.$file, E_USER_NOTICE);
                return false;
            }
        }

        return true;
    }

    /**
     * Returns a list of available debug files.
     *
     * @access  public
     * @return  array               array of absolute paths to debug files
     */
    /*static*/ function Files() {
        $path = TNT_DEBUG_DIR;
        if (!($dh = opendir($path))) {
            trigger_error('Can not scan directory: '.$path, E_USER_ERROR);
            return null;
        }

        $ret = array();

        while (($file = readdir($dh))) {
            if (substr($file, -strlen(TNT_DEBUG_EXT)) == TNT_DEBUG_EXT) {
                $ret[] = "{$path}/{$file}";
            }
        }

        closedir($dh);

        return $ret;
    }

    /**
     * Display the contents of a debug file.
     *
     * The contents are displayed using HTML formating.
     *
     * @access  public
     * @param   string  $path       absolute path to debug file to display
     * @return  boolean             true if could display the file
     */
    /*static*/ function Display($path) {

        if (!($fp = fopen($path, 'r'))) {
            trigger_error('Can not open debug file: '.$path, E_USER_ERROR);
            return false;
        }

?>

<html>
  <head>
    <title>Debug: <?php echo $path; ?></title>
    <style>
    BODY {
      font-size: 8pt;
      font-family: monospace;
    }
    DIV.line {
      white-space: nowrap;
    }
    SPAN {
      display: inline-block;
      word-wrap: break-word;
      padding-left: 1ex;
      text-overflow: ellipsis;
      overflow: hidden;
    }
    SPAN.line {
      color: #7f7f7f;
      padding-left: 0px;
    }
    SPAN.date {
      color: #00007F;
    }
    SPAN.time {
      color: #7F0000;
    }
    SPAN.path {
      color: #007F00;
    }
    SPAN.win {
      text-decoration: underline;
      text-transform: capitalize;
      color: green;
      cursor: hand;
    }
    DIV.pop {
      display: none;
      position: absolute;
      border: groove;
      margin: 0px;
      text-decoration: none;
      padding: 5px;
      background-color: #CCCCCC;
      color: black;
      overflow: visible;
      /*
      height: 800px;
      width: 400px;
      overflow: auto;
      word-wrap: normal;
      white-space: nowrap;
      */
    }
    PRE {
      padding: 0px;
      margin: 0px;
    }
    </style>
    <script>
    document.onclick = function click(event) {
      var elm;

      if (window.event) {
        event = window.event;
        elm = event.srcElement;
      } else {
        elm = event.target;
      }

      if (window._popwin) {
        window._popwin.hide();
        window._popwin = null;
        return false;
      } else if (window._popdiv) {
        window._popdiv.style.display = 'none';
        window._popdiv = null;
        return false;
      }

      if (elm.tagName == 'SPAN' && elm.className == 'win') {
        var id = elm.id.replace('win', 'pop');
        var pop = document.getElementById(id);
        if (pop) {
          var x, y;
          if (window.event) {
            x = event.clientX + document.body.scrollLeft;
            y = event.clientY + document.body.scrollTop;
          } else {
            x = e.pageX;
            y = e.pageY;
          }
          if (false /*window.createPopup && (popup = window.createPopup())*/) {
            var body = popup.document.body;
            body.style.color = '#000000';
            body.style.backgroundColor = '#CCCCCC';
            body.style.padding = '5px';
            body.style.overflow = 'visible';
            body.innerHTML = pop.innerHTML;
            popup.show(x, y, 200, 300, document.body);
            window._popwin = popup;
          } else {
            pop.style.left = x + 'px';
            pop.style.top = y + 'px';
            pop.style.display = 'block';
            window._popdiv = pop;
          }
        }
        return false;
      }
      
      /*
      if (elm.tagName == 'SPAN') {
        var className = elm.className.split(' ', 1)[0];
        var elms = document.getElementsByTagName('SPAN');
        for (var i = 0; i < elms.length; i++) {
          var _className = elms[i].className.split(' ', 1)[0];
          if (_className == className) {
            if (elms[i]._contracted) {
              elms[i].style.width = elms[i].scrollWidth + 'px';
              elms[i]._contracted = false;
            } else {
              elms[i].style.width = '3em';
              elms[i]._contracted = true;
            }
          }
        }
        return false;
      }
      */

      return true;
    };
    </script>
  </head>
  <body>

<?php

        $lineno = 1;

        while (($data = fgets($fp, TNT_DEBUG_MAX))) {
            if (substr($data, -1) != "\n") {
                trigger_error('Debug line is too long - aborting.', E_USER_ERROR);
                break;
            }

            $rec = unserialize(stripcslashes(substr($data, 0, -1)));

            if (!is_array($rec)) {
                trigger_error('Debug line is invalid - aborting.', E_USER_ERROR);
                break;
            }

            list($usecs, $secs) = explode(' ', $rec['time']);

            echo "<div id=\"line-{$lineno}\" class=\"line\">";
            echo '<span class="line">'.sprintf("%05u", $lineno).'</span>';
            echo '<span class="date">'.htmlentities(date('d/m/y', $secs)).'</span>';
            echo '<span class="time">'.htmlentities(date('h:i:s', $secs).substr($usecs, 1, 4)).'</span>';
            echo '<span class="path">'.htmlentities($rec['path']).'</span>';

            $pop = '';

            $max_i = count($rec['args']);
            for ($i = 0; $i < $max_i; $i++) {
                $id = "{$lineno}-{$i}";
                $arg =& $rec['args'][$i];
                $type = gettype($arg);
                echo "<span class=\"arg{$i} args $type\">";
                if (is_scalar($arg) && strlen($arg) < 512) {
                    echo htmlentities($arg);
                } else {
                    $pop .= "<div id=\"pop-{$id}\" class=\"pop\"><pre>".htmlentities(var_info($arg)).'</pre></div>';
                    echo "<span id=\"win-{$id}\" class=\"win\">$type</span>";
                } 
                echo '</span>';
            }

            echo "</div>{$pop}\n";

            $lineno++;
        }

        echo "</body>\n</html>\n";

        return true;
    }

    /**
     * Return the calling environment as a formatted string.
     *
     * The contents are formatted using HTML formating.
     *
     * @access  public
     * @return  string              formatted backtrace
     */
    /*static*/ function Backtrace() {

        $backtrace = debug_backtrace();

        $output = '<table border="1" cellspacing="0" width="100%"><tr bgcolor="#aaaaaa"><th colspan="3">Call Stack</th></tr><tr bgcolor="#cccccc"><th>#</th><th>Function</th><th>Location</th></tr>';

        foreach ($backtrace as $i => $bt) {
            $args = '';
            foreach ($bt['args'] as $j => $arg) {
                $id = "{$i}-{$j}";
                if ($args != '') {
                    $args .= ', ';
                }
                $var = trim(var_info($arg));
                if (!strstr($var, "\n") && strlen(strip_tags($var)) < 10) {
                    $args .= htmlentities($var);
                } else {
                    $args .= '<span style="position: relative">'.
                             '<div id="pop-'.$id.'" onclick="this.style.display=\'none\';" style="display: none; position: absolute; border: 1px solid black; padding: 5px; background-color: #eeeeee; color: black; overflow: visible;"><pre style="padding: 0px; margin: 0px;">'.htmlentities($var).'</pre></div>'.
                             '<a href="#" onclick="document.getElementById(\'pop-'.$id.'\').style.display = \'block\'; return false;">'.htmlentities(substr($var, 0, 10)).'...</a></span>';
                } 
            }
            @$output .= "<tr><td>{$i}</td><td>{$bt['class']}{$bt['type']}{$bt['function']}($args)</td><td>{$bt['file']}:{$bt['line']}</td></tr>";
        }

        $output .= '</table>';

        return $output;
    }
    
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
