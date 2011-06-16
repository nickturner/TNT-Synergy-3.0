<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     block
 * Name:     skip
 * Purpose:  skip over (suppress) the blocks contents
 *           (same as {if 0})
 * -------------------------------------------------------------
 */
function smarty_block_skip($params, $content, &$smarty)
{
    return null;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
