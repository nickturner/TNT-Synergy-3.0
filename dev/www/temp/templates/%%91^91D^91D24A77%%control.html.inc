<?php /* Smarty version 2.6.18, created on 2008-01-21 16:28:43
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cunittests%5Ctnt%5Cform%5Ctests/control.html */

$this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%91^91D^91D24A77%%control.html.inc'] = '5c7a19aba4e4204a8f0749cf35b8f412';

?><?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
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
<?php
function _smarty_tplfunc_5c7a19aba4e4204a8f0749cf35b8f412_0(&$this)
{
$this->_tag_stack[] = array('TNTFormEditBox', array('name' => 'widget1','cols' => '10','minLength' => '1','maxlength' => '35','mandatory' => 'mandatory')); $_block_repeat=true;TNTFormEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
    <label>Edit Box Control widget</label>
    <value>1\n2\n3\n4\n5\n6\n</value>
    <hint>Test single line edit box</hint>
    <help>Test single line edit box with a minlength of 1 and a maxlength of 35</help>
    <alert>Validation error</alert>
  <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTFormEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}



?>
