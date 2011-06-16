<?php /* Smarty version 2.6.18, created on 2008-01-21 23:59:47
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cincludes%5Clibs%5Ctnt%5Cwui%5Ctests/control.html */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
_smarty_load_directives(array (
  0 => 
  array (
    0 => 'taglib',
    1 => 
    array (
      'prefix' => 'wui',
      'name' => 'tntwui',
    ),
  ),
), $this);
_smarty_load_plugins(array('plugins' => array(array('block', 'TNTWuiEditBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 28, false),array('block', 'TNTWuiTextBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 55, false),array('block', 'TNTWuiCheckBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 72, false),array('block', 'TNTWuiRadioBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 80, false),array('block', 'TNTWuiButton', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 88, false),array('block', 'TNTWuiPasswordBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 97, false),array('block', 'TNTWuiNumberBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 104, false),array('block', 'TNTWuiUploadBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 111, false),array('block', 'TNTWuiTickBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\wui\\tests/control.html', 125, false),)), $this); ?>
<?php $this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%2A^2A8^2A849F91%%control.html.inc'] = '266b36e71855825b411aa3b3e1fcc0c8'; ?>
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

<table width="100%" cellpadding="20" border="1">
  <col width="50%" align="left" valign="top" />
  <col width="50%" align="left" valign="top" />
  <tbody><tr><td>

<h2>1) Edit Box Control widget</h2>
<div id="widget1" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#0}'; endif;$this->_tag_stack[] = array('TNTWuiEditBox', array('name' => 'widget1','cols' => '20','minLength' => '1','maxlength' => '35')); $_block_repeat=true;TNTWuiEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
    <label>Edit Box Control widget</label>
    <value>1\n2\n3\n4\n5\n6\n</value>
    <hint>Test single line edit box</hint>
    <help>Test single line edit box with a minlength of 1 and a maxlength of 35</help>
    <alert>Validation error</alert>
      <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#0}'; endif;?>
</div>

</td><td>

<h2>2) Text Box Control (Single-Line) widget</h2>
<div id="widget2" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#1}'; endif;$this->_tag_stack[] = array('TNTWuiTextBox', array('name' => 'widget2','cols' => '20','minLength' => '1','maxlength' => '35','value' => "1\n2\n3\n4\n5\n6\n")); $_block_repeat=true;TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#1}'; endif;?>
</div>

</td></tr><tr><td>

<h2>3) Text Box Control (Multi-Line) widget</h2>
<div id="widget3" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#2}'; endif;$this->_tag_stack[] = array('TNTWuiTextBox', array('name' => 'widget3','cols' => '20','rows' => '5','minLength' => '1','maxlength' => '35','value' => "1\n2\n3\n4\n5\n6\n")); $_block_repeat=true;TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#2}'; endif;?>
</div>

</td><td>

<h2>4) Check Box Control widget</h2>
<div id="widget4" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#3}'; endif;$this->_tag_stack[] = array('TNTWuiCheckBox', array('name' => 'widget4','value' => 'male','label' => 'Male','labelalign' => 'left')); $_block_repeat=true;TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#3}'; endif;?>
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#4}'; endif;$this->_tag_stack[] = array('TNTWuiCheckBox', array('name' => 'widget4','value' => 'female','label' => 'Female','labelalign' => 'left')); $_block_repeat=true;TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#4}'; endif;?>
</div>

</td></tr><tr><td>

<h2>5) Radio Box Control widget</h2>
<div id="widget5" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#5}'; endif;$this->_tag_stack[] = array('TNTWuiRadioBox', array('name' => 'widget5','value' => 'yes','label' => 'Yes','labelalign' => 'right')); $_block_repeat=true;TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#5}'; endif;?>
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#6}'; endif;$this->_tag_stack[] = array('TNTWuiRadioBox', array('name' => 'widget5','value' => 'no','label' => 'No','labelalign' => 'right')); $_block_repeat=true;TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#6}'; endif;?>
</div>

</td><td>

<h2>6) Button Control widget</h2>
<div id="widget6" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#7}'; endif;$this->_tag_stack[] = array('TNTWuiButton', array('name' => 'widget6','type' => 'submit','label' => 'Submit')); $_block_repeat=true;TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#7}'; endif;?>
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#8}'; endif;$this->_tag_stack[] = array('TNTWuiButton', array('name' => 'widget6','type' => 'cancel','label' => 'Cancel')); $_block_repeat=true;TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#8}'; endif;?>
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#9}'; endif;$this->_tag_stack[] = array('TNTWuiButton', array('name' => 'widget6','type' => 'reset','label' => 'Reset')); $_block_repeat=true;TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#9}'; endif;?>
</div>

</td></tr><tr><td>

<h2>7) Password Edit widget</h2>
<div id="widget7" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#10}'; endif;$this->_tag_stack[] = array('TNTWuiPasswordBox', array('name' => 'widget7','value' => '123456')); $_block_repeat=true;TNTWuiPasswordBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiPasswordBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#10}'; endif;?>
</div>

</td><td>

<h2>8) Number Edit widget</h2>
<div id="widget8" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#11}'; endif;$this->_tag_stack[] = array('TNTWuiNumberBox', array('name' => 'widget8','value' => '123456')); $_block_repeat=true;TNTWuiNumberBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiNumberBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#11}'; endif;?>
</div>

</td></tr><tr><td>

<h2>9) File Upload Control widget</h2>
<div id="widget9" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#12}'; endif;$this->_tag_stack[] = array('TNTWuiUploadBox', array('name' => 'widget9','value' => '123456')); $_block_repeat=true;TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#12}'; endif;?>
</div>

</td><td>

<h2>10) Image Upload Control widget</h2>
<div id="widget10" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#13}'; endif;$this->_tag_stack[] = array('TNTWuiUploadBox', array('name' => 'widget10','value' => '123456')); $_block_repeat=true;TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#13}'; endif;?>
</div>

</td></tr><tr><td>

<h2>11) Tick Box Control widget</h2>
<div id="widget11" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:266b36e71855825b411aa3b3e1fcc0c8#14}'; endif;$this->_tag_stack[] = array('TNTWuiTickBox', array('name' => 'widget11','label' => "Tick me!",'value' => '123456')); $_block_repeat=true;TNTWuiTickBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiTickBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:266b36e71855825b411aa3b3e1fcc0c8#14}'; endif;?>
</div>

</td><td>

</td></tr></tbody></table>

</body>
</html>

<!-- /* vim: set expandtab tabstop=2 shiftwidth=2: */ -->
