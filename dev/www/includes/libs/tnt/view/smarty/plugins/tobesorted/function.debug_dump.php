<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     debug_dump
 * Purpose:  dump a smarty variable
 * -------------------------------------------------------------
 */
function smarty_function_debug_dump($params, &$smarty)
{

    $content = '<pre style="width:600px; max-width: 600px; height:300px; max-height:300px; overflow: auto;">';

    if ($params['var']) {
        if ($params['var'] == '__keys__') {
            $content .= 'tpl_vars='.var_info(array_keys($smarty->_tpl_vars));
        } else if (isset($smarty->_tpl_vars[$params['var']])) {
            $content .= "{$params['var']}=".var_info($smarty->_tpl_vars[$params['var']]);
        } else {
            $content .= "No such variable: {$params['var']}";
        }
    } else {
        $content .= "tpl_vars=".var_info($smarty->_tpl_vars);
    }

    $content .= '</pre>';

    if (!empty($params['assign'])) {
        $smarty->assign($params['assign'], $content);
        return;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
