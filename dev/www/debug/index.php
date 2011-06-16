<?

/*****************************************************************************/
/* E-BETTING: Online Betting System                                  v1.0.00 */
/* ================================                                          */
/*                                                                           */
/* Copyright (c) 2002 etote Ltd.                                             */
/* mail:info@e-tote.ltd.uk                                                   */
/* http://www.e-tote.ltd.uk/                                                 */
/*                                                                           */
/* E-BETTING is an online betting system allowing partner sites to offer     */
/* a range of betting options from the etote Ltd website.                    */
/*                                                                           */
/* etote Ltd. is a wholly owned subsidiary of DataTote (England) Ltd.        */
/*                                                                           */
/*****************************************************************************/

require_once('../includes/libs/tnt/tnt.inc');
require_once('../includes/libs/tnt/core/tnt.class.inc');
require_once('../includes/libs/tnt/core/tntloader.class.inc');
require_once('../includes/libs/tnt/core/tntdebug.class.inc');

$error = null;

if (!empty($_REQUEST['file'])) {
    $file = $_REQUEST['file'];
    $op = @$_REQUEST['op'];
    if (substr($file, -strlen(TNT_DEBUG_EXT)) != TNT_DEBUG_EXT) {
        die('Invalid file name - not a debug file');
    }
    if (!file_exists($file)) {
        $error = 'No such file: '.$file;
    }
    switch ($op) {
        case 'delete':
            if (@unlink($file)) {
                header('Location: '.$_SERVER['PHP_SELF']);
                exit();
            }
            $error = 'Can not delete: '.$file;
            break;
        case 'view':
            if (TNTDebug::Display($file)) {
                exit();
            }
            $error = 'Can not view: '.$file;
        default:
            $error = 'Invalid option: '.$op;
            break;
    }
}

$title = htmlentities('TNT Debug Display');

echo <<<EOS
<html>
  <head>
    <title>{$title}</title>
    <style>
    .error {
      color: red;
      border: 1px solid red;
      margin: 5px;
      padding: 5px;
    }
    TABLE {
        margin-left: 20px;
    }
    IMG {
        width: 16px;
        height: 16px;
        border: 0px;
    }
    </style>
  </head>
  <body>

EOS;

  $files = TNTDebug::Files();

  if ($error) {
      echo "    <p class=\"error\">".htmlentities($error)."</p>\n";
  }

  if (!$files) {
      echo "    <p><b>No debug files available.</b></p>\n";
  } else {
      echo "    <p>Please select from the following debug files;</p>\n";
      echo "      <ul>\n";
      foreach ($files as $file) {
          $href = $_SERVER['PHP_SELF'].'?file='.urlencode($file);
          $name = basename($file);
          echo "        <li><a href=\"{$href}&op=view\">{$name}</a>&nbsp;<a href=\"{$href}&op=delete\" style=\"color: red;\">[Delete]</a></li>\n";
      }
      echo "      </ul>\n";
  }

echo <<<EOS
  </body>
</html>

EOS;

/* vim: set expandtab: */

?>
