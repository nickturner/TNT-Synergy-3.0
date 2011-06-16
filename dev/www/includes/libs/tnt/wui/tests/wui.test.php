<?php

require_once(dirname(__FILE__).'/../../tnt.inc');


/*
 * Globals
 */

/*
 * Defines
 */

/* Report all errors and warnings */
error_reporting(E_ALL);

/*
 * Disable session.use_trans_sid to mitigate performance-penalty
 * (do it before any output is started)
 */
if (!defined('SID')) {
    @ini_set('session.use_trans_sid', 0);
}

TNT::Initialise('test', 1);

TNT::Timestamp('Test Starting');

TNTLoader::RequireClass('TNTFileSystem');
TNTLoader::RequireClass('TNTCtrlHttpResponse');

TNT::Timestamp('Constructing Response');

$response =& TNTCtrlHttpResponse::Singleton();

$response->NoCache();

$path = rtrim(dirname($_SERVER['SCRIPT_FILENAME']), '/');
if (DIRECTORY_SEPARATOR != '/') {
    $path = str_replace(DIRECTORY_SEPARATOR, '/', $path);
}

if (!empty($_SERVER['PATH_INFO'])) {
    $path = dirname(__FILE__).$_SERVER['PATH_INFO'];
} else {
    $response->Redirect(TNT_REQUEST_URI.'/index.html');
    die(TNT_REQUEST_URI);
}

// Does the path exist - if not return a '404 Not Found' status
if (!file_exists($path) || !is_readable($path)) {
    echo 'path does not exist: '.$path;
    header('HTTP/1.1 404 Not Found');
    TNT::Shutdown();
}

// Is it the path to a template file - if not send it directly
$ext = extname(strtolower($path));

if ($ext != 'html') {
    switch ($ext) {
        case 'gif':
            header('content-type: image/gif');
            break;
         case 'gif':
            header('content-type: image/jpeg');
            break;
         case 'png':
            header('content-type: image/png');
            break;
     }
     passthru($path);
     TNT::Shutdown();
     break;
}


TNT::Timestamp('Constructing Page');

TNTLoader::RequireClass('TNTWuiWidget');

$page =& new TNTWuiWidget();

TNT::Timestamp('Initialising Page');

if (!$page->SetTemplate($path)) {
    TNT::Abort('Failed to set the page template', $page->_GetError());
}

TNT::Timestamp('Rendering page');

$output = $page->Render();

if (is_null($output)) {
    TNT::Abort('Failed to render page', $page->_GetError());
}

$response->Output($output);

$response->Close();

TNT::Timestamp('Test Finished');

TNT::Shutdown();

/* vim: set expandtab tabstop=4 shiftwidth=4: */

?>

