<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     resource
 * Name:     tnttpl
 * Purpose:  Retrieve template contents from template
 * -------------------------------------------------------------
 */
function &smarty_resource_tnttpl($name, &$smarty) {
    global $tnt;
    _TNT_Smarty_Valid($smarty);

    $tpl = $tnt['theme']->Template($name);

    if (!$tpl) {
        $smarty->trigger_error('can\'t find: tnttpl:'.$name);
        return $tpl;
    }

    return $tpl;
}

function smarty_resource_tnttpl_source($name, &$source, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    if (!($tpl =& smarty_resource_tnttpl($name, $smarty))) {
        return false;
    }
    $source = $tpl->content;
    return true;
}

function smarty_resource_tnttpl_timestamp($name, &$timestamp, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    if (!($tpl =& smarty_resource_tnttpl($name, $smarty))) {
        return false;
    }
    $timestamp = $tpl->mtime;
    return true;
}

function smarty_resource_tnttpl_secure($name, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    return true;
}

function smarty_resource_tnttpl_trusted($name, &$smarty) {
    _TNT_Smarty_Valid($smarty);
    return true;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
