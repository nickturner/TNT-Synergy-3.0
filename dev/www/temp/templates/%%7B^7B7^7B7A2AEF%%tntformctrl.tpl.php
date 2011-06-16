<?php /* Smarty version 2.6.18, created on 2008-01-21 16:28:43
         compiled from includes/libs/tnt/form/templates/tntformctrl.tpl */ ?>

<?php echo '
<style>
label {
  display: inline-block;
  height: 18px;
  vertical-align: top;
  font-size: 10pt;
}
input {
  display: inline-block;
  height: 22px;
  font-size: 10pt;
}
span.tntformctrl-mandatory {
  color: red;
  vertical-align: super;
}
span.tntformctrl-help {
  color: blue;
  font-size: 150%;
  font-weight: bold;
  vertical-align: top;
}
span.tntformctrl-alert {
  color: red;
  vertical-align: top;
}
</style>
'; ?>

<?php $this->assign('id', $this->_tpl_vars['self']->GetId()); ?>
<span id="<?php echo $this->_tpl_vars['id']; ?>
-ctrl" class=" <?php echo $this->_tpl_vars['self']->GetClass(); ?>
" style="<?php echo $this->_tpl_vars['self']->GetStyle(); ?>
"
  title="<?php echo $this->_tpl_vars['self']->GetHint(); ?>
">
  <label id="<?php echo $this->_tpl_vars['id']; ?>
-label" class="tntformctrl-label" for="<?php echo $this->_tpl_vars['id']; ?>
-input"><?php echo $this->_tpl_vars['self']->GetLabel(); ?>
</label>
  <?php if ($this->_tpl_vars['self']->GetMandatory()): ?>
  <span id="<?php echo $this->_tpl_vars['id']; ?>
-mandatory" class="tntformctrl-mandatory" title="This field is mandatory">*</span>
  <?php endif; ?>
  <?php echo $this->_tpl_vars['self']->RenderCtrl(); ?>

  <?php if ($this->_tpl_vars['self']->GetHelp()): ?>
  <span id="<?php echo $this->_tpl_vars['id']; ?>
-help" class="tntformctrl-help" title="<?php echo $this->_tpl_vars['self']->GetHelp(); ?>
">?</span>
  <?php endif; ?>
  <?php if ($this->_tpl_vars['self']->GetAlert()): ?>
  <span id="<?php echo $this->_tpl_vars['id']; ?>
-alert" class="tntformctrl-alert" title="<?php echo $this->_tpl_vars['self']->GetAlert(); ?>
" style="visibility:hidden;">!</span>
  <?php endif; ?>
</span>
