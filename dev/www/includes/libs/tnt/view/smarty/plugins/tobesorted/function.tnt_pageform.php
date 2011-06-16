<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     tnt_pageform
 * Purpose:  render a page selection form
 * -------------------------------------------------------------
 */
function smarty_function_tnt_pageform($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    $content =  $smarty->tnt_engine->_Command_PageForm($params);

    if (!empty($params['assign'])) {
        $smarty->assign_by_ref($params['assign'],
           $smarty->tnt_engine->_variables[$params['assign']]);
        return;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
