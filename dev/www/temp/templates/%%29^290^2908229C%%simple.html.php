<?php /* Smarty version 2.6.18, created on 2008-01-21 17:45:39
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cunittests%5Ctnt%5Cwui%5Ctests/simple.html */ ?>
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
_smarty_load_plugins(array('plugins' => array(array('block', 'TNTWuiWidget', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/simple.html', 23, false),)), $this); ?>
<?php $this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%29^290^2908229C%%simple.html.inc'] = '6110dcc16dec6c9d53354edd09d95412'; ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Welcome to the Simple Widget test suite</title>
<style>
<?php echo '
body { font-size: 8pt; font-weight: normal; text-decoration: }
h1 { font-size: 12pt; font-weight: bold; text-decoration: underline; }
h2 { font-size: 10pt; font-weight: bold; text-decoration: none; margin-bottom: 4px; }
#widget1 { background-color: gray; }
#widget2 { background-color: gray; }
#widget3 { background-color: gray; }
'; ?>

</style>
</head>
<body>
<h1>Welcome to the Simple Widget test suite</h1>

<p><a href="index.html">Test Index</a></p>

<h2>1) Inline widget</h2>
<div id="widget1"><?php if ($this->caching && !$this->_cache_including): echo '{nocache:6110dcc16dec6c9d53354edd09d95412#0}'; endif;$this->_tag_stack[] = array('TNTWuiWidget', array()); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:6110dcc16dec6c9d53354edd09d95412#0}'; endif;?></div>

<h2>2) Block widget</h2>
<div id="widget2"><?php if ($this->caching && !$this->_cache_including): echo '{nocache:6110dcc16dec6c9d53354edd09d95412#1}'; endif;$this->_tag_stack[] = array('TNTWuiWidget', array()); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>Widgets Contents<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:6110dcc16dec6c9d53354edd09d95412#1}'; endif;?></div>

<h2>3) Hierarchy</h2>
<?php if ($this->caching && !$this->_cache_including): echo '{nocache:6110dcc16dec6c9d53354edd09d95412#2}'; endif;$this->_tag_stack[] = array('TNTWuiWidget', array('id' => 'w1')); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
  <?php $this->_tag_stack[] = array('TNTWuiWidget', array('id' => 'w11')); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
    <?php $this->_tag_stack[] = array('TNTWuiWidget', array('id' => 'w111')); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>
    <?php $this->_tag_stack[] = array('TNTWuiWidget', array('id' => 'w112')); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>
    <?php $this->_tag_stack[] = array('TNTWuiWidget', array('id' => 'w113')); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
      <?php $this->_tag_stack[] = array('TNTWuiWidget', array('id' => 'w1131')); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
      <div><code>$w1131->Path() = <?php echo $this->_tpl_vars['w1131']->Path(); ?>
</code></div>
        <div><code>$self->Path() = <?php echo $this->_tpl_vars['self']->Path(); ?>
</code></div>
      <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>
    <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>
  <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>
  <?php $this->_tag_stack[] = array('TNTWuiWidget', array('id' => 'w12')); $_block_repeat=true;TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWidget::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:6110dcc16dec6c9d53354edd09d95412#2}'; endif;?>
</body>
</html>

<!-- /* vim: set expandtab tabstop=4 shiftwidth=4: */ -->
