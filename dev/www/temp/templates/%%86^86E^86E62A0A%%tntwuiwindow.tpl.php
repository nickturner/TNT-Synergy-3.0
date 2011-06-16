<?php /* Smarty version 2.6.18, created on 2008-01-18 01:26:19
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cincludes%5Clibs%5Ctnt%5Cwui%5Cwidgets/../templates/tntwuiwindow.tpl */ ?>

<table border="0" cellpadding="0" cellspacing="0" class="border" style="<?php echo $this->_tpl_vars['css_border']; ?>
">
  <col width="100%" />
  <thead class="title">
  <tr id="<?php echo $this->_tpl_vars['id']; ?>
_window_head" class="title">
    <td width="100%" align="left" valign="middle" class="title" <?php if ($this->_tpl_vars['collapsible']): ?>onclick="javascript: TNT_ToggleDisplay('<?php echo $this->_tpl_vars['id']; ?>
_window_body'); return false;"<?php endif; ?>>
      <?php echo $this->_tpl_vars['title']; ?>

    </td>
  </tr>
  </thead>
  <tbody class="content">
  <tr id="<?php echo $this->_tpl_vars['id']; ?>
_window_body" class="content">
    <td width="100%" align="left" valign="top" class="content">
      <?php echo $this->_tpl_vars['content']; ?>

    </td>
  </tr>
  </tbody>
</table>