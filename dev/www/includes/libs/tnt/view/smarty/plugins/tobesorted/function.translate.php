<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     translate
 * Purpose:  translate a string in the TNT-Synergy system
 * -------------------------------------------------------------
 */
function smarty_function_translate($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    if (empty($params['text'])) {
        $smarty->trigger_error("translate: missing 'text' parameter");
        return;
    }

    $content =  $smarty->tnt_engine->_Command_Translate($params);

    if (!empty($params['assign'])) {
        $smarty->assign($params['assign'],
           $smarty->tnt_engine->_variables[$params['assign']]);
        return;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
