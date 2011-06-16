<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     tnt_ratings_image
 * Purpose:  perform a ratings image lookup
 * -------------------------------------------------------------
 */
function smarty_function_tnt_ratings_image($params, &$smarty)
{
    _TNT_Smarty_Valid($smarty);

    if (empty($params['rating'])) {
        $smarty->trigger_error("tnt_ratings: missing 'rating' parameter");
        return;
    }

    TNT_RequireClass('TNTRatings');

    $ratings = &new TNTRatings(' ', 1);

    if ($ratings->Enabled()) {
        $image = $ratings->Image($params['rating']);
    } else {
        $image = '';
    }

    if (!empty($params['assign'])) {
        $smarty->assign_by_ref($params['assign'], $image);
        return '';
    }

    return $image;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
