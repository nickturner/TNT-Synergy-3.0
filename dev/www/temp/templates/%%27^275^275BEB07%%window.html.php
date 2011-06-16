<?php /* Smarty version 2.6.18, created on 2008-01-18 01:26:19
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cunittests%5Ctnt%5Cwui%5Ctests/window.html */ ?>
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
_smarty_load_plugins(array('plugins' => array(array('block', 'TNTWuiWindow', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/window.html', 60, false),)), $this); ?>
<?php $this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%27^275^275BEB07%%window.html.inc'] = '4ae8f8169e6a2cb5b819acd4945d0b50'; ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Welcome to the Window Widget test suite</title>
<script type="text/javascript" src="~/scripts/main.js"></script>
<style>
<?php echo '
body { font-size: 8pt; font-weight: normal; text-decoration: }
h1 { font-size: 12pt; font-weight: bold; text-decoration: underline; }
h2 { font-size: 10pt; font-weight: bold; text-decoration: none; margin-bottom: 4px; }
.widget { background-color: gray; }

.tntwuiwindow table.border {
  border: 1px solid;
  border-color:	ThreeDLightShadow ThreeDDarkShadow ThreeDDarkShadow ThreeDLightShadow;
}
.tntwuiwindow td.title {
  height: 15px;
  background-color: blue;
}
.tntwuiwindow td.content {
  border: 1px solid;
  border-color:	ThreeDDarkShadow ThreeDLightShadow ThreeDLightShadow ThreeDDarkShadow;
  background-color: white;
}
'; ?>

</style>
</head>
<body>
<h1>Welcome to the Window Widget test suite</h1>

<?php ob_start(); ?>
<pre>
TNT-SYNERGY is a PHP Application Framework. It encourages application
structured application architectures based on the "Model2" version of the
Model View Controller (MVC) design paradigm. The framework supports the
following key features:
    - Model
        - session persistance (PHP Sessions)
        - permanent persistance (Database library abstraction)
        - authentication (Multi-user)
        - authorization (Access control)
        - application model (Object based view of web page)
    - Control
       - state processing (Page states)
       - event processing (User supplied events)
    - View
       - themes (application and/or user themes)
       - template based presentation (Template library abstraction)
       - internationalisation (Multi-language support)      
</pre>
<?php $this->_smarty_vars['capture']['content'] = ob_get_contents(); ob_end_clean(); ?>
<?php $this->assign('content', $this->_smarty_vars['capture']['content']); ?>

<p><a href="index.html">Test Index</a></p>

<h2>1) Normal Window widget</h2>
<div id="widget1" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#0}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Normal Window','width' => '300px','height' => '100px')); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#0}'; endif;?>
</div>

<h2>2) Left Aligned Hidden Window widget</h2>
<div id="widget2" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#1}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Hidden Scroll Window','width' => '300px','height' => '100px','overflow' => 'hidden','align' => 'left')); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#1}'; endif;?>
</div>

<h2>3) Right Aligned Scroll Window widget</h2>
<div id="widget3" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#2}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Overflow Scroll Window','width' => '300px','height' => '100px','overflow' => 'scroll','align' => 'right')); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#2}'; endif;?>
</div>

<h2>4) Center Aligned Auto Window widget</h2>
<div id="widget4" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#3}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Overflow Auto Window','width' => '300px','height' => '100px','overflow' => 'auto','align' => 'center')); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#3}'; endif;?>
</div>

<h2>5) Marqee Up Window widget</h2>
<div id="widget5" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#4}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Overflow Marquee Up Window','width' => '300px','height' => '100px','overflow' => "marquee-up")); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#4}'; endif;?>
</div>

<h2>6) Marqee Down Window widget</h2>
<div id="widget6" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#5}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Overflow Marquee Down Window','width' => '300px','height' => '100px','overflow' => "marquee-down")); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#5}'; endif;?>
</div>

<h2>7) Marqee Left Window widget</h2>
<div id="widget7" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#6}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Overflow Marquee Left Window','width' => '300px','height' => '100px','overflow' => "marquee-left")); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#6}'; endif;?>
</div>

<h2>8) Marqee Right Window widget</h2>
<div id="widget8" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#7}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => 'Overflow Marquee Right Window','width' => '300px','height' => '100px','overflow' => "marquee-right",'style' => "marquee-speed: 5;")); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#7}'; endif;?>
</div>

<h2>9) Collapsable Window widget</h2>
<div id="widget9" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#8}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => "Collapsable/Overflow Hidden Window",'width' => '300px','overflow' => 'hidden','collapsible' => true)); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#8}'; endif;?>
</div>

<h2>10) Styled Collapsable Window widget</h2>
<p>style="padding: 10px; margin: 10px; border: 10px #cc0000 solid; overflow: hidden; background-color: #cc00cc; color: #00cc00;)</p>
<div id="widget10" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:4ae8f8169e6a2cb5b819acd4945d0b50#9}'; endif;$this->_tag_stack[] = array('TNTWuiWindow', array('title' => "Collapsable/Overflow Hidden Window",'width' => '300px','overflow' => 'hidden','collapsible' => 'true','style' => "padding: 10px; margin: 10px; border: 10px #cc0000 solid; overflow: hidden; background-color: #cc00cc; color: #00cc00;")); $_block_repeat=true;TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php echo $this->_tpl_vars['content']; ?>
<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiWindow::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:4ae8f8169e6a2cb5b819acd4945d0b50#9}'; endif;?>
</div>

 
</body>
</html>

<!-- /* vim: set expandtab tabstop=2 shiftwidth=2: */ -->
