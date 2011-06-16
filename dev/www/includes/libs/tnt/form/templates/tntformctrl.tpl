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
 * TNTFormCtrl Template 
 * ~~~~~~~~~~~~~~~~~~~~
 *
 * A FormCtrl renders the basic control layout.
 *
 * @package TNT.Form
 * @access  public
 * @author  Nick Turner <nick@tnt-synergy.com>
 *
*-->
{*html_include type="text/css" src="./../css/tntformctrl.css"*}
{*html_include type="text/javascript" src="./../js/tntformctrl.js"*}
{literal}
<style>
label {
  display: inline-block;
  height: 18px;
  vertical-align: top;
  font-size: 10pt;
}
input {
  display: inline-block;
  height: 22px;
  font-size: 10pt;
}
span.tntformctrl-mandatory {
  color: red;
  vertical-align: super;
}
span.tntformctrl-help {
  color: blue;
  font-size: 150%;
  font-weight: bold;
  vertical-align: top;
}
span.tntformctrl-alert {
  color: red;
  vertical-align: top;
}
</style>
{/literal}
{assign var="id" value=$self->GetId()}
<span id="{$id}-ctrl" class="{*$self->GetCssClasses()*} {$self->GetClass()}" style="{$self->GetStyle()}"
  title="{$self->GetHint()}">
  <label id="{$id}-label" class="tntformctrl-label" for="{$id}-input">{$self->GetLabel()}</label>
  {if $self->GetMandatory()}
  <span id="{$id}-mandatory" class="tntformctrl-mandatory" title="This field is mandatory">*</span>
  {/if}
  {$self->RenderCtrl()}
  {if $self->GetHelp()}
  <span id="{$id}-help" class="tntformctrl-help" title="{$self->GetHelp()}">?</span>
  {/if}
  {if $self->GetAlert()}
  <span id="{$id}-alert" class="tntformctrl-alert" title="{$self->GetAlert()}" style="visibility:hidden;">!</span>
  {/if}
</span>
<!--* /* vim: set expandtab tabstop=4 shiftwidth=4: */ *-->
