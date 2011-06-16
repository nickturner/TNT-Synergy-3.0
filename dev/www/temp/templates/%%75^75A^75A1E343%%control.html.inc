<?php /* Smarty version 2.6.18, created on 2008-01-18 01:48:19
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cunittests%5Ctnt%5Cwui%5Ctests/control.html */

$this->_cache_serials['d:/my webs 1/tnt-synergy-3.0/dev/www/temp/templates\%%75^75A^75A1E343%%control.html.inc'] = '317b69822587b03fd4e6ffe62143358b';

?><?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
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
_smarty_load_plugins(array('plugins' => array(array('block', 'TNTWuiEditBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 28, false),array('block', 'TNTWuiTextBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 55, false),array('block', 'TNTWuiCheckBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 72, false),array('block', 'TNTWuiRadioBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 80, false),array('block', 'TNTWuiButton', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 88, false),array('block', 'TNTWuiPasswordBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 97, false),array('block', 'TNTWuiNumberBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 104, false),array('block', 'TNTWuiUploadBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 111, false),array('block', 'TNTWuiTickBox', 'd:\\my webs 1\\tnt-synergy-3.0\\dev\\www\\unittests\\tnt\\wui\\tests/control.html', 125, false),)), $this); ?>
<?php
function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_0(&$this)
{
$this->_tag_stack[] = array('TNTWuiEditBox', array('name' => 'widget1','cols' => '20','minLength' => '1','maxlength' => '35')); $_block_repeat=true;TNTWuiEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
    <label>Edit Box Control widget</label>
    <value>1\n2\n3\n4\n5\n6\n</value>
    <hint>Test single line edit box</hint>
    <help>Test single line edit box with a minlength of 1 and a maxlength of 35</help>
    <alert>Validation error</alert>
      <?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiEditBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_1(&$this)
{
$this->_tag_stack[] = array('TNTWuiTextBox', array('name' => 'widget2','cols' => '20','minLength' => '1','maxlength' => '35','value' => "1\n2\n3\n4\n5\n6\n")); $_block_repeat=true;TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_2(&$this)
{
$this->_tag_stack[] = array('TNTWuiTextBox', array('name' => 'widget3','cols' => '20','rows' => '5','minLength' => '1','maxlength' => '35','value' => "1\n2\n3\n4\n5\n6\n")); $_block_repeat=true;TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiTextBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_3(&$this)
{
$this->_tag_stack[] = array('TNTWuiCheckBox', array('name' => 'widget4','value' => 'male','label' => 'Male','labelalign' => 'left')); $_block_repeat=true;TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_4(&$this)
{
$this->_tag_stack[] = array('TNTWuiCheckBox', array('name' => 'widget4','value' => 'female','label' => 'Female','labelalign' => 'left')); $_block_repeat=true;TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiCheckBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_5(&$this)
{
$this->_tag_stack[] = array('TNTWuiRadioBox', array('name' => 'widget5','value' => 'yes','label' => 'Yes','labelalign' => 'right')); $_block_repeat=true;TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_6(&$this)
{
$this->_tag_stack[] = array('TNTWuiRadioBox', array('name' => 'widget5','value' => 'no','label' => 'No','labelalign' => 'right')); $_block_repeat=true;TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiRadioBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_7(&$this)
{
$this->_tag_stack[] = array('TNTWuiButton', array('name' => 'widget6','type' => 'submit','label' => 'Submit')); $_block_repeat=true;TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_8(&$this)
{
$this->_tag_stack[] = array('TNTWuiButton', array('name' => 'widget6','type' => 'cancel','label' => 'Cancel')); $_block_repeat=true;TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_9(&$this)
{
$this->_tag_stack[] = array('TNTWuiButton', array('name' => 'widget6','type' => 'reset','label' => 'Reset')); $_block_repeat=true;TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiButton::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_10(&$this)
{
$this->_tag_stack[] = array('TNTWuiPasswordBox', array('name' => 'widget7','value' => '123456')); $_block_repeat=true;TNTWuiPasswordBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiPasswordBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_11(&$this)
{
$this->_tag_stack[] = array('TNTWuiNumberBox', array('name' => 'widget8','value' => '123456')); $_block_repeat=true;TNTWuiNumberBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiNumberBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_12(&$this)
{
$this->_tag_stack[] = array('TNTWuiUploadBox', array('name' => 'widget9','value' => '123456')); $_block_repeat=true;TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_13(&$this)
{
$this->_tag_stack[] = array('TNTWuiUploadBox', array('name' => 'widget10','value' => '123456')); $_block_repeat=true;TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiUploadBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}


function _smarty_tplfunc_317b69822587b03fd4e6ffe62143358b_14(&$this)
{
$this->_tag_stack[] = array('TNTWuiTickBox', array('name' => 'widget11','label' => "Tick me!",'value' => '123456')); $_block_repeat=true;TNTWuiTickBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?><?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo TNTWuiTickBox::_TemplatePluginBlock($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); 
}



?>
