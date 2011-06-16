<?php /* Smarty version 2.6.18, created on 2008-01-21 23:30:53
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cincludes%5Clibs%5Ctnt%5Cform%5Ctests/control.html */ ?>
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
  1 => 
  array (
    0 => 'taglib',
    1 => 
    array (
      'prefix' => 'wui',
      'name' => 'tntwui',
    ),
  ),
), $this);
_smarty_load_plugins(array('plugins' => array(array('block', 'TNTFormCell', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\form\\tests/control.html', 24, false),array('block', 'TNTWuiEditBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\includes\\libs\\tnt\\form\\tests/control.html', 25, false),)), $this); ?>
<?php $this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%F9^F95^F950DCF3%%control.html.inc'] = '62df975cf5decb1fa303ddc6650d1f8c'; ?>

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
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:62df975cf5decb1fa303ddc6650d1f8c#0}'; endif;$this->_tag_stack[] = array('TNTFormCell', array()); $_block_repeat=true;TNTFormCell::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
    <?php $this->_tag_stack[] = array('TNTWuiEditBox', array('name' => 'widget1','cols' => '10','minLength' => '1','maxlength' => '35','mandatory' => 'mandatory')); $_block_repeat=true;TNTWuiEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>
    <label>Edit Box Control widget</label>
    <value>1\n2\n3\n4\n5\n6\n</value>
    <hint>Test single line edit box</hint>
    <help>Test single line edit box with a minlength of 1 and a maxlength of 35</help>
    <alert>Validation error</alert>
  <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTFormCell::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:62df975cf5decb1fa303ddc6650d1f8c#0}'; endif;?>
</div>



</body>
</html>

<!-- /* vim: set expandtab tabstop=2 shiftwidth=2: */ -->
