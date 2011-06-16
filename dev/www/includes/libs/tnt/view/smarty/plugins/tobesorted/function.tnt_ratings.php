<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     tnt_ratings
 * Purpose:  perform a ratings lookup
 * -------------------------------------------------------------
 */
function smarty_function_tnt_ratings($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    if (empty($params['objtype'])) {
        $smarty->trigger_error("tnt_ratings: missing 'objtype' parameter");
        return;
    }

    if (empty($params['objid'])) {
        $smarty->trigger_error("tnt_ratings: missing 'objid' parameter");
        return;
    }

    TNT_RequireClass('TNTRatings');

    $ratings = &new TNTRatings($params['objtype'], $params['objid']);

    $tpl['objtype'] = $ratings->objtype;
    $tpl['objid'] = $ratings->objid;
    $tpl['enabled'] = false;
    $tpl['stats'] = null;
    $tpl['images'] = null;
    $tpl['range'] = null;
    $tpl['action'] = null;

    if ($ratings->Enabled()) {
        $tpl['enabled'] = true;
        $tpl['stats'] = $ratings->Stats();
        $tpl['images'] = $ratings->Images();
        $tpl['range'] = $ratings->Range();
        if (!empty($params['rater'])) {
            $tpl['action'] = $ratings->Action($params['rater']);
        } else {
            $tpl['action'] = null;
        }
        foreach (array('min', 'avg', 'max') as $var) {
            $val = $var['stats'][$var];
            $tpl[$var] = array(
               'value' => $val,
               'image' => '/synergy/'.$ratings->Image($val));
        }
    }

    TNT_Debug('ratings=', $tpl);

    $assign = (!empty($params['assign']) ? $params['assign'] : '_ratings');

    $smarty->assign_by_ref($assign, $tpl);

    return '';
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
