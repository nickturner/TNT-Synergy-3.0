///////////////////////////////////////////////////////////////////////////////
// TNT-JUIC: Javascript User Interface Components                    v2.0.01 //
// ==============================================                            //
//                                                                           //
// Copyright (c) 2003 by Nick Turner                                         //
// mail:info@tnt-juic.com                                                    //
// http://www.tnt-juic.com/                                                  //
//                                                                           //
// TNT-JUIC is an Javascript library of advanced user interface components   //
// for use on web based forms.                                               //
//                                                                           //
// WARNING: This package is protected by copyright law and international     //
// treaties. Unauthorized reproduction or distribution of this package,      //
// or any portion of it, may result in severe civil and criminal penalties,  //
// and will be prosecuted to the maximum extent possible under the law.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/**
 * TNT JUIC Javascript
 * ~~~~~~~~~~~~~~~~~~~
 *
 * Global include class to include the JUIC javascript code.
 *
 * @package TNT.JUIC
 * @author  Nick Turner <nick@tnt-juic.com>
 * @since   21/10/03
 * @version 2.0.01
 */

/*
 * Internal method to include package files.
 *
 * @access  public
 * @param   string      filename        name of file to include relative to this
 * @return  void
 */
var TNT = {
    /**
     * URL to root of installation (ie. where this file is)
     * @access  private
     * @var     string                  root path
     */
    rootUrl = null,

    /*
     * Internal method to include package files.
     *
     * @access  public
     * @param   string      filename        name of file to include relative to this
     * @return  void
     */
    function $include(file) {
      if (typeof TNT.rootUrl == 'undefined') {
        // Initialize the root directory by finding which directory this file is in
        var self = 'scripts/juic.js';
        var scripts = document.getElementsByTagName('script');
        var max_i = scripts.length;
        for (var i = 0; i < max_i; i++) {
          if (scripts[i].src.match(self)) {
            TNT.rootUrl = scripts[i].src.replace(self, '');
            break;
          }
        }
        if (typeof TNT.rootUrl == 'undefined') {
          alert('Can not find path to "juic.js" script');
          TNT.rootUrl = null;
          return;
        }
      }
      if (TNT.rootUrl) {
        if (file.substr(file.length - 3) == '.js') {
          document.write('<script src="' + TNT.rootUrl + file + '" language="javascript"></' + 'script>');
        } else if (file.substr(file.length - 4) == '.css') {
          document.write('<link href="' + TNT.rootUrl + file + '" rel="stylesheet" type="text/css" />');
        }
      }
    }
}

// Include the core stylesheets
$include('styles/winxp/juic.css');

// Include the core files
$include('scripts/core/tnt.class.js');
$include('scripts/core/juic.class.js');
$include('scripts/core/function.class.js');
$include('scripts/core/string.class.js');
$include('scripts/core/array.class.js');
$include('scripts/core/hash.class.js');
$include('scripts/core/object.class.js');
$include('scripts/core/objectcollection.class.js');
$include('scripts/core/browser.class.js');

// Include the WebForms implementation

// Include the UI
//$include('scripts/ui/attrs.class.js');
//$include('scripts/ui/styles.class.js');
$include('scripts/ui/widget.class.js');
$include('scripts/ui/control.class.js');

// Include the UI HTML elements
//$include('scripts/html/div.class.js');
//$include('scripts/html/table.class.js');
//$include('scripts/html/tr.class.js');
//$include('scripts/html/td.class.js');
//$include('scripts/html/img.class.js');
//$include('scripts/html/span.class.js');

// Include the UI Controls
$include('scripts/ui/controls/listview.class.js');

document.writeln('\
<!-- ActiveWidgets stylesheet and scripts -->\
<link href="' + TNT.rootUrl + '../contrib/activewidgets-2-0-b3/ActiveWidgets/source/styles/xp/aw.css" rel="stylesheet" type="text/css" ></link>\
<script src="' + TNT.rootUrl + '../contrib/activewidgets-2-0-b3/ActiveWidgets/source/lib/aw.js"></script>\
');


/* vim: set expandtab tabstop=2 shiftwidth=2: */
