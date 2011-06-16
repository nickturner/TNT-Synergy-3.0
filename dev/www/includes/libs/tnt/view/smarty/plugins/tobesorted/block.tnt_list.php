<?php

TNTDriverTemplate {
    var $content = null;

    // Display a page
    function DisplayPage($content) {
        global $tnt;
        TNT_Debug("TNTDriverTemplate::DisplayPage($content)");

        $this->content = $content;

        return true;
    }

    // Display an error
    function DisplayError($title, $error, $url) {
        global $tnt;
        TNT_Debug("TNTDriverTemplate::DisplayError($title, $error, $url)");

        $content = TNT_HtmlErrorBox($title, $error, $url, 'center');

        return $this->DisplayPage($content);
    }
    
    function Dispatch($op) {
        global $tnt;
        TNT_Debug("TNTDriverTemplate::Dispatch($op)");

        // Attempt to handle the op natively
        if (parent::Dispatch($op)) {
            return true;
        }

        TNT_Debug('TNTDriverTemplate::Dispatch() - op was not handled by driver ');

        return false;
    }

    function TNTDriverTemplate() {
        global $tnt;
        TNT_Debug('TNTDriverTemplate::TNTDriverTemplate()');

        $this->_hierachy[] = 'TNTDriverTemplate';

    }
}

/*
 * Smarty plugin
 * -------------------------------------------------------------
 * Type:     block
 * Name:     tnt_list
 * Purpose:  render the content as an interactive list
 * -------------------------------------------------------------
 */
function smarty_block_tnt_list($params, $content, &$smarty)
{
    if (!$content) {
        return null;
    }

    $driver_data = $params;
    unset($driver_data['ctl_class']);

    extract($params);

    if (empty($ctl_class)) {
        $ctl_class = 'TNTListDbDriver';
    }

    TNT_RequireClass($ctl_class);
    
    $driver =& new $ctl_class($content);
    $driver->SetParent(&$this);

    if (!$driver->Load($driver_data)) {
        TNT_Debug('smarty_block_tnt_list - failed to load driver');
        return false;
    }

    if (!$driver->Build()) {
        TNT_Debug('smarty_block_tnt_list - failed to build the driver');
        return false;
    }

    $driver->DisplayPage();

    $content = $driver->content;

    if (!empty($assign)) {
        $smarty->assign($assign, $content);
        return null;
    }

    return $content;
}

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>
