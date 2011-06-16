<?php /* Smarty version 2.6.18, created on 2008-01-17 22:38:21
         compiled from d:/my+webs+1/tnt-synergy-3.0/dev/www/data/pages/index.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('block', 'wrap', 'd:/my webs 1/tnt-synergy-3.0/dev/www/data/pages/index.tpl', 2, false),)), $this); ?>

<?php $this->_tag_stack[] = array('wrap', array('file' => "chrome/frame",'title' => 'Welcome')); $_block_repeat=true;smarty_block_wrap($this->_tag_stack[count($this->_tag_stack)-1][1], null, $this, $_block_repeat);while ($_block_repeat) { ob_start(); ?>
<h2>Hi Nick</h2>
<?php echo $this->_tpl_vars['content']; ?>

<?php $_block_content = ob_get_contents(); ob_end_clean(); $_block_repeat=false;echo smarty_block_wrap($this->_tag_stack[count($this->_tag_stack)-1][1], $_block_content, $this, $_block_repeat); }  array_pop($this->_tag_stack); ?>

<!-- /* vim: set expandtab tabstop=2 shiftwidth=2: */ -->

