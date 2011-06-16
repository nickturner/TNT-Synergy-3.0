<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     block
 * Name:     tnt_box
 * Purpose:  render the content within a box
 * -------------------------------------------------------------
 */
function smarty_block_tnt_box($params, $content, &$smarty)
{
    if (!$content) {
        return null;
    }

    extract($params);

    TNT_RequireClass('TNTBox');

    if (empty($type)) {
        $type = $GLOBALS['tnt']['theme']->WidgetClass('TNTBox');
    }

    if (!class_exists($type)) {
        $smarty->trigger_error("tnt_box: unknown box type '$type'");
        return null;
    }

    $box = &new $type();

    $attrs = $params;
    unset($attrs['type']);
    unset($attrs['assign']);

    $box->SetMembers($attrs, true);
    $box->Build();

    $box->content = $content;

    $content = $box->Render();

    if (!empty($assign)) {
        $smarty->assign($assign, $content);
        return null;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
