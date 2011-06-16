<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     random
 * Purpose:  output a random number between $min and $max:
 *	{random [min="min" max="max"] [assign="varname"]}
 * -------------------------------------------------------------
 */
 
function smarty_function_random($params, &$smarty)
{
	srand((double) microtime() * 1000000);

	$random_number = rand(@$params['min'], @$params['max']);

	if (isset($params['assign'])) {
		$smarty->assign($params['assign'], $random_number);
	} else {
		return $random_number;
	}
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
