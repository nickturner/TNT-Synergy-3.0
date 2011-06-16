<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     block
 * Name:     tnt_widget
 * Purpose:  render the content as a widget
 * -------------------------------------------------------------
 */
function smarty_block_tnt_widget($params, $content, &$smarty)
{
    if (!$content) {
        return null;
    }

    $driver_data = $params;
    unset($driver_data['ctl_class']);

    extract($params);

    if (empty($class)) {
        $class = 'TNTSmartList';
    }

    TNT_RequireClass($class);
    
    $widget =& new $class();

    if (!$widget->Construct($content)) {
        TNT_Debug('smarty_block_tnt_widget - failed to load widget: '.$class);
        return false;
    }

    $content = $widget->Render();

    if (!empty($assign)) {
        $smarty->assign($assign, $content);
        return null;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
