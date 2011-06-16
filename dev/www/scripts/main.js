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

// Determine browser and version.

function TNTBrowser() {

  var ua, s, i;

  this.isIE    = false;
  this.isNS    = false;
  this.version = null;

  ua = navigator.userAgent;

  s = "MSIE";
  if ((i = ua.indexOf(s)) >= 0) {
    this.isIE = true;
    this.version = parseFloat(ua.substr(i + s.length));
    return;
  }

  s = "Netscape6/";
  if ((i = ua.indexOf(s)) >= 0) {
    this.isNS = true;
    this.version = parseFloat(ua.substr(i + s.length));
    return;
  }

  // Treat any other "Gecko" browser as NS 6.1.

  s = "Gecko";
  if ((i = ua.indexOf(s)) >= 0) {
    this.isNS = true;
    this.version = 6.1;
    return;
  }
}

var tnt_browser = new TNTBrowser();

// Write to debug window
function TNT_Debug(msg) {
    if (!window.tnt_debug_win || window.tnt_debug_win.closed) {
        tnt_debug_win = window.open('', 'tnt_debug_win');
        if (!tnt_debug_win) {
            alert('Popup blocker probably prevented debug window from opening');
            return;
        }
        tnt_debug_win.opener = window;
        tnt_debug_win.document.writeln('<style>BODY { font-size: 8pt; color: red; }</style>');
    }

    tnt_debug_win.document.writeln('<div>'+msg+'</div>');

    tnt_debug_win.scrollBy(0, 1000);
}

// Include another script file
function TNT_IncludeOnce(file) {
    if (serverDir && file[0] != '/') {
        file = serverDir + '/' + file;
    }
    var len = file.length;
    if (file.substr(len - 4).toLowerCase() == '.css') {
        document.write('<'+'link');
        document.write(' type="text/css" rel="stylesheet" ');
        document.write(' href="'+file+'"');
        document.write('/'+'>');
    } else if (file.substr(len - 3).toLowerCase() == '.js') {
        document.write('<'+'script');
        document.write(' type="text/javascript" ');
        document.write(' src="'+file+'"');
        document.write('>'+'<'+'/'+'script'+'>');
    } else {
        alert('Unknown file type to include');
    }
}

// Get an element of the given id and tag
function TNT_GetElement(id, tag) {
    var elm = document.getElementById(id);
    if (!elm) {
        return false;
    }

    if (!tag || elm.tag == tag) {
        return elm;
    }

    var elms = document.getElementsByTagName(tag);
    for (var i = 0; i < elms.length; i++) {
        if (elms[i].id == id) {
            return elm;
        }
    }

    return null;
}

// This gets the absolute width of the element
function TNT_GetElementWidth(elm) {
  var width = elm.offsetWidth;
  if (elm.currentStyle && !isNaN(w = parseInt(elm.currentStyle.marginLeft))) {
    width += w;
  }
  if (elm.currentStyle && !isNaN(w = parseInt(elm.currentStyle.marginRight))) {
    width += w;
  }
  return width;
}

// This gets the absolute height of the element
function TNT_GetElementHeight(elm) {
  var height = elm.offsetHeight;
  if (elm.currentStyle && !isNaN(h = parseInt(elm.currentStyle.marginTop))) {
    height += h;
  }
  if (elm.currentStyle && !isNaN(h = parseInt(elm.currentStyle.marginBottom))) {
    height += h;
  }
  return height;
}

// This sets the absolute width of the element
function TNT_SetElementWidth(elm, width) {
  if (elm.offsetWidth != width) {
    elm.style.width = width+'px';
    if (elm.offsetWidth > width) {
      var dw = (elm.offsetWidth - width);
      if ((width - dw) >= 0) {
          elm.style.width = (width - dw)+'px';
      }
    }
  }
}

// This sets the absolute height of the element
function TNT_SetElementHeight(elm, height) {
  if (elm.offsetHeight != height) {
    elm.style.height = height+'px';
    if (elm.offsetHeight > height) {
      var dh = (elm.offsetHeight - height);
      if ((height - dh) >= 0) {
          elm.style.height = (height - dh)+'px';
      }
    }
  }
}

function TNT_InitPopup(name, height, width, chrome) {
    // Ensure the current window is correct popup
    if (!window.opener || (name && window.name != name)) {
        // its not a popup or not popup of correct name

        alert('This window named "'+window.name+'" is supposed to be a popup window named "'+name+'" - we will open a new popup window now and revert this window to the previous page.');

        // Open correct popup
        TNT_OpenPopup(name, window.location, height, width, chrome);
    
        // Send this window back to previous screen
        if (window.history.length > 0) {
            window.history.go(-1);
        } else {
            window.location = '';
        }
    } else {
        // its the correct popup

        // ensure it is the correct size - if this is first time window
        // opened
        if (!window.tnt_init) {
            if (width == 0) {
                width = window.document.body.style.pixelWidth;
            }
            if (height == 0) {
                height = window.document.body.style.pixelHeight;
            }
        
            window.resizeTo(width, height);

            window.tnt_init = true;
        }
    }
}

function TNT_OpenPopup(name, url, height, width, chrome) {

    if (height) {
        if (chrome != '')
            chrome += ',';
        chrome += 'height='+height;
    }

    if (width) {
        if (chrome != '')
            chrome += ',';
        chrome += 'width='+width;
    }

    if (!window.top.tntpopups) {
        window.top.tntpopups = new Array();
    }

    if (!window.top.tntpopups[name] || window.top.tntpopups[name].closed) {
        win = window.open(url, name, chrome);
        win.opener = window.self;
        win.name = name;
        win.top = window.top;
        window.top.tntpopups[name] = win;
    } else {
        win = window.top.tntpopups[name];
    }

    win.location = url;
    win.focus();

    return win;
}

function TNT_ClosePopup(name) {
    if (name) {
        // Close the named window (if a popup)
        if (window.top.tntpopups && window.top.tntpopups[name]) {
            win = window.top.tntpopups[name];
            win.close();
        } else {
            alert('Can not close popup with name '+name);
        }
    } else if (window.opener) {
        // Close the current window (if a popup)
        window.close();
    } else {
        alert('Current window is not a popup and can not be closed');
    }
}

// Toggle whether object with given id is displayed or not
function TNT_ToggleDisplay(id) {
    var elm = TNT_GetElement(id, null);
    if (!elm) {
        return false;
    }
    if (elm.style.display == 'none') {
        elm.style.display = 'block';
    } else {
        elm.style.display = 'none';
    }
    return true;
}

// Submit a form
function TNT_SubmitForm(id) {
    var elm = TNT_GetElement(id, 'form');
    if (!elm) {
        return false;
    }
    return elm.submit();
}

// Click a form button
function TNT_ClickButton(id) {
    var elm = TNT_GetElement(id, 'input');
    if (!elm && !(elm = TNT_GetElement(id, 'button'))) {
        alert('Failed to find a button with id: ' + id);
        return false;
    }
    return elm.click();
}

// Click an anchor
function TNT_ClickAnchor(id) {
    var elm = TNT_GetElement(id, 'a');
    if (!elm) {
        alert('Failed to find an anchor with id: ' + id);
        return false;
    }
    return elm.click();
}

/* Fixup widths for IE controls */
function _TNT_InitControls(frm) {
  /*alert('fixing controls: '+document.compatMode);*/
  /*
   * IE controls sometimes obey the CSS box model and some don't - therefore
   * we want the pixel width to be the actual width of the control including
   * any borders. We do this by lowering the pixelWidth if the actual width
   * is greater. This can't support percentage widths etc. just pixel widths.
   */
  //var a = 'Adjusting control widths';
  for (f = 0; f < document.forms.length; f++) {
    var form = document.forms[f];
    for (c = 0; c < form.elements.length; c++) {
      var control = form.elements(c);
      var width = control.style.pixelWidth;
      //a += 'control before <'+control.tagName+ ' name="'+control.name+'" offsetWidth="'+control.offsetWidth+'", pixelWidth="'+control.style.pixelWidth+'" />\n';

      while (control.style.pixelWidth > 0 && control.offsetWidth > width) {
        control.style.pixelWidth--;
      }

      //a += 'control after <'+control.tagName+ ' name="'+control.name+'" offsetWidth="'+control.offsetWidth+'", pixelWidth="'+control.style.pixelWidth+'" />\n';
    }
  }

  //alert(a);
}

/* This will collapse marquees whos content is less than the marquee size */
function _TNT_InitMarquees() {
  if (!document.getElementsByTagName) {
    // No DOM - quit
    return;
  }

  var marquees = document.getElementsByTagName('marquee');
  if (!marquees || !marquees.length) {
    return;
  }

  for (var i = 0; i < marquees.length; i++) {
    /* This basically makes the marquee behave like a standard DIV if there is
       no need for the content to scroll */
    var marquee = marquees[i];

    /* We only process marquees of class 'tntblockbox-marquee' */
    if (marquee.className != 'tntblockbox-marquee') {
      continue;
    }

    // A marquee is automatically colapsed to the size of the content if it
    // is less than the size of the marquee. If we collapse in the direction
    // that the marquee is travelling then the marquee is disabled effectively
    // becoming static.
    var disable = false;

    if (marquee.direction == 'up' || marquee.direction == 'down') {
        var contentHeight = (marquee.scrollHeight - (marquee.clientHeight * 2));
        var contentWidth = marquee.scrollWidth;
    } else if (marquee.direction == 'left' || marquee.direction == 'right') {
        var contentHeight = marquee.scrollHeight;
        var contentWidth = (marquee.scrollWidth - (marquee.clientWidth * 2));
    }
    //alert('marquee.scrollHeight='+marquee.scrollHeight);
    //alert('marquee.clientHeight='+marquee.clientHeight);
    //alert('contentHeight='+contentHeight);

    //alert('marquee.scrollWidth='+marquee.scrollWidth);
    //alert('marquee.clientWidth='+marquee.clientWidth);
    //alert('contentWidth='+contentWidth);

    if (contentHeight < marquee.clientHeight) {
        // Collapse height
        //alert('Marquee colapsed height to '+contentHeight+' pixels');
        marquee.style.pixelHeight = contentHeight;
        if (marquee.direction == 'up' || marquee.direction == 'down') {
            disable = true;
        }
    }

    if (contentWidth < marquee.clientWidth) {
        // Collapse width
        //alert('Marquee colapsed width to '+contentWidth+' pixels');
        marquee.style.pixelWidth = contentWidth;
        if (marquee.direction == 'left' || marquee.direction == 'right') {
            disable = true;
        }
    }

    if (disable) {
      // This disables the marquee
      //alert('Marquee Disabled');
      marquee.direction='down';
      marquee.behavior='alternate';
      marquee.loop = 1;
    }
  }
}

/* Remember the page position and element with focus when a form is submitted */
function TNT_SmartNavigationSave(form) {
  var elm = form.elements['tnt_smartnavigation'];
  if (elm) {
    var x,y;
    if (self.pageYOffset)
    {
      // all except Explorer
      x = self.pageXOffset;
      y = self.pageYOffset;
    }
    else if (document.documentElement && document.documentElement.scrollTop)
    {
      // Explorer 6 Strict
      x = document.documentElement.scrollLeft;
      y = document.documentElement.scrollTop;
    }
    else if (document.body)
    {
      // all other Explorers
      x = document.body.scrollLeft;
      y = document.body.scrollTop;
    }

    elm.value = x + ',' + y;

    //alert('SmartNavigation saved as ('+x+','+y+')');
  }
  return true;
}

function TNT_SmartNavigationRestore(val) {
  var bits = val.split(',');
  if (bits.length == 2) {
    window.scrollTo(bits[0], bits[1]);
    //alert('SmartNavigation restored to ('+bits[0]+','+bits[1]+')');
  }
}

function _TNT_SmartNavigation() {
  var f, max_f = document.forms.length;  
  for (f = 0; f < max_f; f++) {
    TNT_SmartNavigationSave(document.forms[f]);
  }
}
  
/* Add a string to be evaluated as code to be run at load time */
var _tnt_atLoadFuncs = new Array();
function TNT_AtLoad(func) {
  _tnt_atLoadFuncs[_tnt_atLoadFuncs.length] = func;
}

/* Evaluate the list of strings at load time */
function TNT_OnLoad() {
  for (var i = 0; i < _tnt_atLoadFuncs.length; i++) {
    eval(_tnt_atLoadFuncs[i]);
  }
}

/* Add a string to be evaluated as code to be run at unload time */
var _tnt_atUnLoadFuncs = new Array();
function TNT_AtUnLoad(func) {
  _tnt_atUnLoadFuncs[_tnt_atUnLoadFuncs.length] = func;
}

/* Evaluate the list of strings at unload time */
function TNT_OnUnLoad() {
  for (var i = 0; i < _tnt_atUnLoadFuncs.length; i++) {
    eval(_tnt_atUnLoadFuncs[i]);
  }
}

/* Add function to be ececuted on a given event */
/* Compatible with IE DOM or W3C DOM */
function TNT_AddEventListener(element, event, listener) {
  if (element.attachEvent) {
    element.attachEvent('on'+event, listener);
  } else if (element.addEventListener) {
    element.addEventListener(event, listener, true);
  } else {
    alert('Can\'t add event listener to element: '+element.tagName);
  }
}

/* Remove function to be ececuted on a given event */
/* Compatible with IE DOM or W3C DOM */
function TNT_RemoveEventListener(element, event, listener) {
  if (element.dettachEvent) {
    element.dettachEvent('on'+event, listener);
  } else if (element.removeEventListener) {
    element.removeEventListener(event, listener, true);
  } else {
    alert('Can\'t remove event listener from element: '+element.tagName);
  }
}

/* Some default functions to run at load time */
TNT_AtLoad('_TNT_InitMarquees()');
TNT_AtLoad('_TNT_InitControls()');
TNT_AtUnLoad('_TNT_SmartNavigation()');

window.onload = TNT_OnLoad;
window.onunload = TNT_OnUnLoad;

/* vim: set expandtab: tabstop=2 shiftwidth=2 */
