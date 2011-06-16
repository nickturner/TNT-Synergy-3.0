<?php /* Smarty version 2.6.18, created on 2008-01-21 16:28:43
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cunittests%5Ctnt%5Cform%5Ctests/control.html */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
_smarty_load_directives(array (
  0 => 
  array (
    0 => 'taglib',
    1 => 
    array (
      'prefix' => 'form',
      'name' => 'tntform',
    ),
  ),
), $this);
_smarty_load_plugins(array('plugins' => array(array('block', 'TNTFormEditBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\form\\tests/control.html', 23, false),)), $this); ?>
<?php $this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%91^91D^91D24A77%%control.html.inc'] = '5c7a19aba4e4204a8f0749cf35b8f412'; ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Welcome to the Simple Controls Widget test suite</title>
<script ctrl="text/javascript" src="~/scripts/main.js"></script>
<style>
<?php echo '
body { font-size: 8pt; font-weight: normal; text-decoration: }
h1 { font-size: 12pt; font-weight: bold; text-decoration: underline; }
h2 { font-size: 10pt; font-weight: bold; text-decoration: none; margin-bottom: 4px; }
.widget { background-color: gray; }
'; ?>

</style>
</head>
<body>
<h1>Welcome to the Simple Controls Widget test suite</h1>

<p><a href="index.html">Test Index</a></p>

<h2>1) Edit Box Control widget</h2>
<div id="widget1" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5c7a19aba4e4204a8f0749cf35b8f412#0}'; endif;$this->_tag_stack[] = array('TNTFormEditBox', array('name' => 'widget1','cols' => '10','minLength' => '1','maxlength' => '35','mandatory' => 'mandatory')); $_block_repeat=true;TNTFormEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
    <label>Edit Box Control widget</label>
    <value>1\n2\n3\n4\n5\n6\n</value>
    <hint>Test single line edit box</hint>
    <help>Test single line edit box with a minlength of 1 and a maxlength of 35</help>
    <alert>Validation error</alert>
  <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTFormEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5c7a19aba4e4204a8f0749cf35b8f412#0}'; endif;?>
</div>

<!--*

<h2>2) Text Box Control (Single-Line) widget</h2>
<div id="widget2" class="widget">
  <wui:textbox name="widget2" cols="20" minLength="1" maxlength="35"
  value="1\n2\n3\n4\n5\n6\n" />
</div>

<h2>3) Text Box Control (Multi-Line) widget</h2>
<div id="widget3" class="widget">
  <wui:textbox name="widget3" cols="20" rows="5"
    minLength="1" maxlength="35"
    value="1\n2\n3\n4\n5\n6\n" />
</div>

<h2>4) Check Box Control widget</h2>
<div id="widget4" class="widget">
  <wui:checkbox name="widget4" value="male" label="Male" labelalign='left' />
  <wui:checkbox name="widget4" value="female" label="Female" labelalign='left' />
</div>

<h2>5) Radio Box Control widget</h2>
<div id="widget5" class="widget">
  <wui:radiobox name="widget5" value='yes' label='Yes' labelalign='right' />
  <wui:radiobox name="widget5" value='no' label='No' labelalign='right' />
</div>

<h2>6) Button Control widget</h2>
<div id="widget6" class="widget">
  <wui:button name="widget6" type="submit" label="Submit" />
  <wui:button name="widget6" type="cancel" label="Cancel" />
  <wui:button name="widget6" type="reset" label="Reset" />
</div>

<h2>7) Password Edit widget</h2>
<div id="widget7" class="widget">
  <wui:passwordbox name="widget7" value='123456' />
</div>

<h2>8) Number Edit widget</h2>
<div id="widget8" class="widget">
  <wui:numberbox name="widget8" value='123456' />
</div>

<h2>9) File Upload Control widget</h2>
<div id="widget9" class="widget">
  <wui:uploadbox name="widget9" value="123456" />
</div>

<h2>10) Image Upload Control widget</h2>
<div id="widget10" class="widget">
  <wui:uploadbox name="widget10" value="123456" />
</div>

<h2>11) Tick Box Control widget</h2>
<div id="widget11" class="widget">
  <wui:tickbox name="widget11" label="Tick me!" value="123456" />
</div>

</body>
</html>

<!-- /* vim: set expandtab tabstop=2 shiftwidth=2: */ -->
