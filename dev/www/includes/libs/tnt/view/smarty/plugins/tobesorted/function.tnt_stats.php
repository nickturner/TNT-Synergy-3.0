<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     tnt_stats
 * Purpose:  Load up some stats into the synergy variable
 * -------------------------------------------------------------
 */
function smarty_function_tnt_stats($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    $smarty->tnt_engine->_Command_Stats($params);

    return null;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
