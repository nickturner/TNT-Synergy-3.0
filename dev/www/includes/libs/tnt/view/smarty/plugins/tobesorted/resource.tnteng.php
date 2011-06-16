<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     resource
 * Name:     tnteng
 * Purpose:  Retrieve template from parent template engine
 * -------------------------------------------------------------
 */
function smarty_resource_tnteng_source($name, &$source, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    $source = $smarty->tnt_engine->template->content;
    return true;
}

function smarty_resource_tnteng_timestamp($name, &$timestamp, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    $timestamp = $smarty->tnt_engine->template->mtime;
    return true;
}

function smarty_resource_tnteng_secure($name, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    return true;
}

function smarty_resource_tnteng_trusted($name, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    return true;
}


/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
