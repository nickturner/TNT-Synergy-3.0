<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     tnt_render
 * Purpose:  render an object
 * -------------------------------------------------------------
 */
function smarty_function_tnt_render($params, &$smarty)
{
    extract($params);

    if (empty($widget)) {
        $smarty->trigger_error("tnt_render: missing 'widget' parameter");
        return;
    }

    // The widget can either be the name of a global object or the name of
    // a template variable. We will check for a global object first
    if (TNTObject1::Object($widget)) {
        $widget =& TNTObject1::Object($widget);
    } else if (isset($smarty->_tpl_vars[$widget])) {
        $widget =& $smarty->_tpl_vars[$widget];
    }

    if (!$widget) {
        $smarty->trigger_error('tnt_render: there is no object with the id or template variable with the name: '.$widget);
        return;
    }

    if (!is_a($widget, 'tntwidget')) {
        $smarty->trigger_error("tnt_render: 'widget' parameter is not a TNTWidget derived object");
        return;
    }

    $attrs = $params;
    unset($attrs['widget']);
    unset($attrs['assign']);

    $widget->SetMembers($attrs, true);

    TNT_RequireClass('TNTWidget');

    $content = $widget->Render();

    if (!empty($assign)) {
        $smarty->assign($assign, $content);
        return;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
