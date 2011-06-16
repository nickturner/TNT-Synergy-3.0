<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     translate
 * Purpose:  translate a string in the TNT-Synergy system
 * -------------------------------------------------------------
 */
function smarty_function_sprintf($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    if (empty($params['fmt'])) {
        $smarty->trigger_error("sprintf: missing 'fmt' parameter");
        return;
    }

    $content =  $smarty->tnt_engine->_Command_Sprintf($params);

    if (!empty($params['assign'])) {
        $smarty->assign_by_ref($params['assign'],
           $smarty->tnt_engine->_variables[$params['assign']]);
        return;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
