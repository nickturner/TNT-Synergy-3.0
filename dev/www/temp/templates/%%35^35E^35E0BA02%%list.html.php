<?php /* Smarty version 2.6.18, created on 2008-01-18 02:58:08
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cunittests%5Ctnt%5Cwui%5Ctests/list.html */ ?>
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
_smarty_load_plugins(array('plugins' => array(array('block', 'TNTWuiListBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/list.html', 127, false),array('block', 'TNTWuiToggleList', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/list.html', 191, false),)), $this); ?>
<?php $this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%35^35E^35E0BA02%%list.html.inc'] = '5ae2c597ae3d55a6602654d594755531'; ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Welcome to the List Controls Widget test suite</title>
<script ctrl="text/javascript" src="~/scripts/main.js"></script>
<style>
<?php echo '
body { font-size: 8pt; font-weight: normal; text-decoration: }
h1 { font-size: 12pt; font-weight: bold; text-decoration: underline; }
h2 { font-size: 10pt; font-weight: bold; text-decoration: none; margin-bottom: 4px; }
th { font-size: 10pt; font-weight: bold; text-align: center; padding: 2px; background-color: #ccccff; }
.widget { background-color: #EEEEEE; }
'; ?>

</style>
</head>
<body>
<h1>Welcome to the List Controls Widget test suite</h1>

<p><a href="index.html">Test Index</a></p>

<?php 

global $flat_items;

$flat_items = array(
  array('id' =>  1, 'pid' => 0,  'label' => 'Root'),
  array('id' =>  2, 'pid' => 1,  'label' => 'England'),
  array('id' =>  3, 'pid' => 1,  'label' => 'Scotland'),
  array('id' =>  4, 'pid' => 1,  'label' => 'Wales'),

  array('id' =>  5, 'pid' => 2,  'label' => 'South West'),
  array('id' =>  6, 'pid' => 2,  'label' => 'South East'),
  array('id' =>  7, 'pid' => 2,  'label' => 'Midlands'),
  array('id' =>  8, 'pid' => 2,  'label' => 'North West'),
  array('id' =>  9, 'pid' => 2,  'label' => 'North East'),

  array('id' => 10, 'pid' => 3,  'label' => 'Lowlands'),
  array('id' => 11, 'pid' => 3,  'label' => 'Hightlands'),

  array('id' => 12, 'pid' => 4,  'label' => 'North'),
  array('id' => 13, 'pid' => 4,  'label' => 'South'),

  array('id' => 14, 'pid' => 5,  'label' => 'Somerset'),
  array('id' => 15, 'pid' => 5,  'label' => 'Devon'),
  array('id' => 16, 'pid' => 5,  'label' => 'Cornwall'),
  array('id' => 17, 'pid' => 5,  'label' => 'North Somerset'),
  array('id' => 18, 'pid' => 5,  'label' => 'Dorset'),
  array('id' => 19, 'pid' => 5,  'label' => 'Wiltshire'),

  array('id' => 20, 'pid' => 6,  'label' => 'Essex'),
  array('id' => 21, 'pid' => 6,  'label' => 'London'),
  array('id' => 22, 'pid' => 6,  'label' => 'Hampshire'),
  array('id' => 23, 'pid' => 6,  'label' => 'Norfolk'),
  array('id' => 24, 'pid' => 6,  'label' => 'East Anglia'),

  array('id' => 25, 'pid' => 7,  'label' => 'Lincolnshire'),
  array('id' => 26, 'pid' => 7,  'label' => 'Nottinghamshire'),
  array('id' => 27, 'pid' => 7,  'label' => 'Birmingham'),
  array('id' => 28, 'pid' => 7,  'label' => 'Leicestershire'),
  array('id' => 29, 'pid' => 7,  'label' => 'Cheshire'),

  array('id' => 30, 'pid' => 8,  'label' => 'Lancashire'),
  array('id' => 31, 'pid' => 8,  'label' => 'Stafordshire'),
  array('id' => 32, 'pid' => 8,  'label' => 'Manchester'),

  array('id' => 33, 'pid' => 9,  'label' => 'Northumberland'),
  array('id' => 34, 'pid' => 9,  'label' => 'Durham'),
  array('id' => 35, 'pid' => 9,  'label' => 'Yorkshire')
);

$this->assign('flat_items', $flat_items);

$tree_items = array(
array('id' =>  1, 'pid' => 0, 'label' => 'Root', 'group' => 1, 'items' => array(
array('id' =>  2, 'pid' => 1, 'label' => 'England', 'group' => 1, 'items' => array(
array('id' =>  5, 'pid' => 2, 'label' => 'South West', 'group' => 1, 'items' => array(
        array('id' => 14, 'pid' => 5,  'label' => 'Somerset'),
        array('id' => 15, 'pid' => 5,  'label' => 'Devon'),
        array('id' => 16, 'pid' => 5,  'label' => 'Cornwall'),
        array('id' => 17, 'pid' => 5,  'label' => 'North Somerset'),
        array('id' => 18, 'pid' => 5,  'label' => 'Dorset'),
        array('id' => 19, 'pid' => 5,  'label' => 'Wiltshire'))),
        array('id' =>  6, 'pid' => 2, 'label' => 'South East', 'group' => 1, 'items' => array(
        array('id' => 20, 'pid' => 6,  'label' => 'Essex'),
        array('id' => 21, 'pid' => 6,  'label' => 'London'),
        array('id' => 22, 'pid' => 6,  'label' => 'Hampshire'),
        array('id' => 23, 'pid' => 6,  'label' => 'Norfolk'),
        array('id' => 24, 'pid' => 6,  'label' => 'East Anglia'))),
        array('id' =>  7, 'pid' => 2, 'label' => 'Midlands', 'group' => 1, 'items' => array(
        array('id' => 25, 'pid' => 7,  'label' => 'Lincolnshire'),
        array('id' => 26, 'pid' => 7,  'label' => 'Nottinghamshire'),
        array('id' => 27, 'pid' => 7,  'label' => 'Birmingham'),
        array('id' => 28, 'pid' => 7,  'label' => 'Leicestershire'),
        array('id' => 29, 'pid' => 7,  'label' => 'Cheshire'))),
        array('id' =>  8, 'pid' => 2, 'label' => 'North West', 'group' => 1, 'items' => array(
        array('id' => 30, 'pid' => 8,  'label' => 'Lancashire'),
        array('id' => 31, 'pid' => 8,  'label' => 'Stafordshire'),
        array('id' => 32, 'pid' => 8,  'label' => 'Manchester'))),
        array('id' =>  9, 'pid' => 2, 'label' => 'North East', 'group' => 1, 'items' => array(
        array('id' => 33, 'pid' => 9,  'label' => 'Northumberland'),
        array('id' => 34, 'pid' => 9,  'label' => 'Durham'),
        array('id' => 35, 'pid' => 9,  'label' => 'Yorkshire'))))),

        array('id' =>  3, 'pid' => 1, 'label' => 'Scotland', 'group' => 1, 'items' => array(
      array('id' => 10, 'pid' => 3,  'label' => 'Lowlands'),
      array('id' => 11, 'pid' => 3,  'label' => 'Hightlands'))),

      array('id' =>  4, 'pid' => 1, 'label' => 'Wales', 'group' => 1, 'items' => array(
      array('id' => 12, 'pid' => 4, 'label' => 'North'),
      array('id' => 13, 'pid' => 4, 'label' => 'South'))),
    ))
);

$this->assign('tree_items', $tree_items);

 ?>

<table width="100%" cellpadding="20" border="1">
  <col width="50%" align="left" valign="top" />
  <col width="50%" align="left" valign="top" />
  <thead><tr><th>Flat Options</th><th>Tree Options</th></tr></thead>
  <tbody><tr><td>

<h2>1) Drop Down List Box Control widget</h2>
<div id="widget1" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#0}'; endif;$this->_tag_stack[] = array('TNTWuiListBox', array('name' => 'list1','size' => '1','value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#0}'; endif;?>
</div>

</td><td>

<h2>2) Drop Down List Box Control widget</h2>
<div id="widget2" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#1}'; endif;$this->_tag_stack[] = array('TNTWuiListBox', array('name' => 'list2','size' => '1','indent' => "&raquo;",'value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#1}'; endif;?>
</div>

</td></tr><tr><td>

<h2>3) Drop Down List Box Control widget (Multi-Select)</h2>
<div id="widget3" class="widget">
  <listbox name="list3" size="1" multiple=true
    value="Devon" items=$flat_items />
</div>

</td><td>

<h2>4) Drop Down List Box Control widget (Multi-Select)</h2>
<div id="widget4" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#2}'; endif;$this->_tag_stack[] = array('TNTWuiListBox', array('name' => 'list4','size' => '1','indent' => "&raquo;",'multiple' => true,'value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#2}'; endif;?>
</div>

</td></tr><tr><td>

<h2>5) Expanded List Box Control widget</h2>
<div id="widget5" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#3}'; endif;$this->_tag_stack[] = array('TNTWuiListBox', array('name' => 'list5','size' => '10','value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#3}'; endif;?>
</div>

</td><td>

<h2>6) Expanded List Box Control widget</h2>
<div id="widget6" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#4}'; endif;$this->_tag_stack[] = array('TNTWuiListBox', array('name' => 'list6','size' => '10','indent' => "&raquo;",'value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#4}'; endif;?>
</div>

</td></tr><tr><td>

<h2>7) Expanded List Box Control widget (Multi-Select)</h2>
<div id="widget7" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#5}'; endif;$this->_tag_stack[] = array('TNTWuiListBox', array('name' => 'list7','size' => '10','multiple' => true,'value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#5}'; endif;?>
</div>

</td><td>

<h2>8) Expanded List Box Control widget (Multi-Select)</h2>
<div id="widget8" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#6}'; endif;$this->_tag_stack[] = array('TNTWuiListBox', array('name' => 'list8','size' => '10','indent' => "&raquo;",'multiple' => true,'value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiListBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#6}'; endif;?>
</div>

</td></tr><tr><td>

<h2>9) Horizontal Toggle List Control widget</h2>
<div id="widget9" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#7}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list9','value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#7}'; endif;?>
</div>

</td><td>

<h2>10) Horizontal Toggle List Control widget</h2>
<div id="widget10" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#8}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list10','value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#8}'; endif;?>
</div>

</td></tr><tr><td>

<h2>11) Horizontal Toggle List Control widget (Multi-Select)</h2>
<div id="widget11" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#9}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list11','multiple' => true,'value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#9}'; endif;?>
</div>

</td><td>

<h2>12) Horizontal Toggle List Control widget (Multi-Select)</h2>
<div id="widget12" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#10}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list12','multiple' => true,'value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#10}'; endif;?>
</div>

</td></tr><tr><td>

<h2>13) Vertical Toggle List Control widget</h2>
<div id="widget13" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#11}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list13','cols' => '1','value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#11}'; endif;?>
</div>

</td><td>

<h2>14) Vertical Toggle List Control widget</h2>
<div id="widget14" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#12}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list14','cols' => '1','value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#12}'; endif;?>
</div>

</td></tr><tr><td>

<h2>15) Vertical Toggle List Control widget (Multi-Select)</h2>
<div id="widget15" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#13}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list15','cols' => '1','multiple' => true,'value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#13}'; endif;?>
</div>

</td><td>

<h2>16) Vertical Toggle List Control widget (Multi-Select)</h2>
<div id="widget16" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#14}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list16','cols' => '1','multiple' => true,'value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#14}'; endif;?>
</div>

</td></tr><tr><td>

<h2>17) Vertical Toggle List Control widget (MultiSelect,3-Col)</h2>
<div id="widget17" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#15}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list17','cols' => '3','multiple' => 'true','style' => "background-color:cyan;",'value' => 'Devon','items' => $this->_tpl_vars['flat_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#15}'; endif;?>
</div>

</td><td>

<h2>18) Vertical Toggle List Control widget (MultiSelect,3-Col)</h2>
<div id="widget18" class="widget">
  <?php if ($this->caching && !$this->_cache_including): echo '{nocache:5ae2c597ae3d55a6602654d594755531#16}'; endif;$this->_tag_stack[] = array('TNTWuiToggleList', array('name' => 'list18','cols' => '3','multiple' => 'true','style' => "background-color:cyan;",'value' => 'Devon','items' => $this->_tpl_vars['tree_items'])); $_block_repeat=true;TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiToggleList::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); if ($this->caching && !$this->_cache_including): echo '{/nocache:5ae2c597ae3d55a6602654d594755531#16}'; endif;?>
</div>

</td></tr></tbody></table>

</body>
</html>
<!-- /* vim: set expandtab tabstop=2 shiftwidth=2: */ -->