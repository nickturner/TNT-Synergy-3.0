<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     tnt_smilie
 * Purpose:  render an smilie
 * -------------------------------------------------------------
 */
function smarty_function_tnt_smilie($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    $content =  $smarty->tnt_engine->_Command_Smilie($params);

    if (!empty($params['assign'])) {
        $smarty->assign_by_ref($params['assign'],
           $smarty->tnt_engine->_variables[$params['assign']]);
        return;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
