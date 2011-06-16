<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     tnt_mods
 * Purpose:  Load up some module details into the synergy variable
 * -------------------------------------------------------------
 */
function smarty_function_tnt_mods($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    $smarty->tnt_engine->_Command_Mods($params);

    return null;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
