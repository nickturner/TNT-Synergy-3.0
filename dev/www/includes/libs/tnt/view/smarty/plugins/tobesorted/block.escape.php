<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     block
 * Name:     escape
 * Purpose:  escape the contents of a block
 * -------------------------------------------------------------
 */
function smarty_block_escape($params, $content, &$smarty)
{
    if (empty($params['type'])) {
        $smarty->trigger_error("sprintf: missing 'type' parameter");
        return null;
    }

    if (!$content) {
        return null;
    }

    TNT_RequireCode('tnt_text');

    $content =  TNT_TextEscape($content, $params['type']);

    if (!empty($params['assign'])) {
        $smarty->assign($params['assign'], $content);
        return null;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
