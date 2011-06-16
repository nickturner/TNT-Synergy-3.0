<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     insert
 * Name:     tnt
 * Purpose:  Insert a predifined TNT-Synergy element
 * -------------------------------------------------------------
 */
function smarty_insert_tnt($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);
    return $smarty->tnt_engine->_Command_Insert($params);
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
