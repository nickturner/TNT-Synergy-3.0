<!--*
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
 * LinkLine Widget Template 
 * ~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * A LinkLine widget draws an array of links as a navigational menu.
 *
 * @package TNT.View
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
*-->
<!-- template name="tntwuilinkline" type="Smarty" version="1.0" langauge="en" basedir="." -->
<div class="{$class} tnt-wui-linkline" style="{$style}">
  {strip}
  {foreach name="linkline" item='link' from=$links}
    <a href="{$link->href|default:'#'}" title="{$link->tooltip}" {if $link->active}class="active"{/if} {$link->extras}>{$link->content}</a>{if ! $smarty.foreach.linkline.last}<span class="separator">{$separator|default:"&nbsp;&gt;&nbsp;"}</span>{/if}
  {/foreach}
  {/strip}
</div>
<!-- /template tntlinkline -->
