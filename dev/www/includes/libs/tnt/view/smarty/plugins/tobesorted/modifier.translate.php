<?php

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     modifier
 * Name:     translate
 * Purpose:  translate a string in the TNT-Synergy system
 * -------------------------------------------------------------
 */
function smarty_modifier_translate($string)
{
        if (function_exists(translate)) {
                $string = translate($string);
        }
        return $string;
}

?>
