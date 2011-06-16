<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     function
 * Name:     join
 * Purpose:  join strings
 *	{join [sep="sep"] [assign="varname"]
 *        str1="string" [str2="string" ...]}
 * -------------------------------------------------------------
 */
 
function smarty_function_join($params, &$smarty)
{
	$sep = (isset($params['sep']) ? $params['sep'] : '');

    $strs = null;
    $data['strs'] =& $strs;

    $cb = create_function('$val, $key, &$data', 'if (!empty($val) && strncasecmp(\'str\', $key, 3) == 0) { $n = (int)(substr($key, 3)); $data[\'strs\'][$n] = $val; }');
    array_walk($params, $cb, $data);

    $content = '';
    if (is_array($strs) && count($strs)) {
        ksort($strs);
        $content = join($sep, $strs);
    }

    if (!empty($args['escape'])) {
        $content = Smart_Escape($content, $args['escape']);
    }

	if (isset($params['assign'])) {
		$smarty->assign($params['assign'], $content);
        return;
	}

	return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
