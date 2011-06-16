<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     modifier
 * Name:     join
 * Purpose:  join an array of strings
 * -------------------------------------------------------------
 */
function smarty_modifier_join($array)
{
	$args = func_get_args();
	$sep = (isset($args[1]) ? $args[1] : ',');
	TNT_Error('array=', $array);
        $string = ($array ? join($sep, $array) : '');
	TNT_Error('string=', $string);
        return $string;
}

?>
