<?php /* Smarty version 2.6.14, created on 2006-10-29 01:38:39
         compiled from parse_math.tpl */ ?>
<?php $_from = $this->_tpl_vars['items']; if (!is_array($_from) && !is_object($_from)) { settype($_from, 'array'); }$this->_foreach['loop'] = array('total' => count($_from), 'iteration' => 0);
if ($this->_foreach['loop']['total'] > 0):
    foreach ($_from as $this->_tpl_vars['i']):
        $this->_foreach['loop']['iteration']++;
?>
<?php echo $this->_foreach['loop']['iteration']+2; ?>

<?php echo $this->_foreach['loop']['iteration']+$this->_tpl_vars['flt']; ?>

<?php echo $this->_foreach['loop']['iteration']+$this->_tpl_vars['obj']->six(); ?>

<?php echo $this->_foreach['loop']['iteration']+$this->_tpl_vars['obj']->ten; ?>

<?php endforeach; endif; unset($_from); ?>
<?php echo $this->_tpl_vars['obj']->ten+$this->_tpl_vars['flt']; ?>

<?php echo $this->_tpl_vars['obj']->ten*$this->_tpl_vars['flt']; ?>

<?php echo $this->_tpl_vars['obj']->six()+$this->_tpl_vars['obj']->ten; ?>

<?php echo $this->_tpl_vars['obj']->ten+$this->_tpl_vars['obj']->ten; ?>

<?php echo $this->_tpl_vars['obj']->six()+$this->_tpl_vars['flt']; ?>

<?php echo $this->_tpl_vars['obj']->six()+$this->_tpl_vars['items']['0']; ?>
