<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     query
 * Purpose:  perform a database query
 * -------------------------------------------------------------
 */
function smarty_function_query($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    if (empty($params['sql'])) {
        $smarty->trigger_error("sprintf: missing 'sql' parameter");
        return;
    }

    $content =  $smarty->tnt_engine->_Command_Query($params);

    if (!empty($params['assign'])) {
        $smarty->assign_by_ref($params['assign'],
           $smarty->tnt_engine->_variables[$params['assign']]);
        return;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
