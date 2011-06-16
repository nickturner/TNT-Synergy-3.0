<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     modifier
 * Name:     conv
 * Purpose:  conv a string in the TNT-Synergy system
 * -------------------------------------------------------------
 */
function smarty_modifier_conv($string, $type = null)
{
    $args = func_get_args();

    unset($args[1]);

    switch ($type) {
        case 'currency':
	    $string = call_user_func_array('TNT_ConvNumberToCurrency', $args);
            break;
        case 'time_duration':
	    $args[0] = abs(time() - (int)$args[0]);
	    // Drop through to duration
        case 'duration':
        case 'period':
	    $string = call_user_func_array('TNT_ConvDurationToText', $args);
            break;
        case 'timestamp':
            $args[0] = (int)$args[0]; // Ensure argument is numeric
	    // Drop through to time 
        case 'time':
	    $string = call_user_func_array('TNT_ConvTimeToText', $args);
            break;
        case 'counter':
	    $string = call_user_func_array('TNT_ConvCounterToText', $args);
            break;
        case 'position':
	    $string = call_user_func_array('TNT_ConvPositionToText', $args);
            break;
        case 'rating':
	    $string = call_user_func_array('TNT_ConvRatingToImageTag', $args);
            break;
	case 'html':
	    $string = call_user_func_array('TNT_ConvTextToHtml', $args);
            break;
	case 'post':
	    $string = call_user_func_array('TNT_ConvPostToHtml', $args);
            break;
	case 'size':
	    $string = call_user_func_array('TNT_ConvSizeToText', $args);
            break;
    }
    return $string;
}

?>
