<?php /* Smarty version 2.6.14, created on 2006-10-29 01:38:39
         compiled from parse_obj_meth.tpl */ ?>
<?php echo $this->_tpl_vars['obj']->meth($this->_tpl_vars['foo'],2.5); ?>

<?php echo $this->_tpl_vars['obj']->meth(2.5,$this->_tpl_vars['foo']); ?>

<?php echo $this->_tpl_vars['obj']->meth(2.5); ?>

<?php echo $this->_tpl_vars['obj']->meth($this->_tpl_vars['obj']->val,'foo'); ?>

<?php echo $this->_tpl_vars['obj']->meth('foo',$this->_tpl_vars['obj']->val); ?>

<?php echo $this->_tpl_vars['obj']->meth('foo',$this->_tpl_vars['foo']); ?>

<?php echo $this->_tpl_vars['obj']->meth($this->_tpl_vars['obj']->arr['one'],2); ?>

<?php echo $this->_tpl_vars['obj']->meth($this->_tpl_vars['obj']->meth('foo',$this->_tpl_vars['foo'])); ?>
