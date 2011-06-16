<?php
/**
 * Test cases for tnt/xml library
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>CSV Tests</title>
</head>
<body>

<?php
global $tnt;

include '../../../../../includes/tntsynergy.inc';

TNTDebug::SetFlags(TNT_DEBUG_FLAGS_TOSCREEN);

echo '<h1>CSV Format Tests</h1>';

TNTLoader::RequireClass('tntcsv');

$csv =& new TNTCsv();

$csv_string = 'this is a test,12,13,14,"of "","" files",-1';
$csv_array = array('this is a test',12,13,14,'of "csv" files',-1);

TNTDebug::Warning('$csv->Set($csv_string)=', $csv->Set($csv_string));
TNTDebug::Warning('$csv->Get()=', $csv->Get());
TNTDebug::Warning('$csv->Get(\'string\')=', $csv->Get('string'));
TNTDebug::Warning('$csv->Get(\'array\')=', $csv->Get('array'));
TNTDebug::Warning('array_diff($csv->Get(\'array\'),$csv_array)=', array_diff($csv->Get('array'), $csv_array));

TNTDebug::Warning('$csv->Set($csv_array)=', $csv->Set($csv_array));
TNTDebug::Warning('$csv->Get()=', $csv->Get());
TNTDebug::Warning('$csv->Get(\'string\')=', $csv->Get('string'));
TNTDebug::Warning('$csv->Get(\'array\')=', $csv->Get('array'));
TNTDebug::Warning('$csv->Get(\'string\') === $csv_string =', $csv->Get('string') === $csv_string);

echo '<h1>URI Format Tests</h1>';

TNTLoader::RequireClass('tnturi');

$uri =& new TNTUri();

$uri_string = 'http://user:pass@localhost:80/tnt-synergy-2.0/includes/libs/tnt/util/tests/util.test.php?arg1=val1&arg2=va2#fragment';
$uri_array = array(
  'scheme' => 'http',
  'host' => 'localhost',
  'port' => '80',
  'user' => 'user',
  'pass' => 'pass',
  'path' => '/tnt-synergy-2.0/includes/libs/tnt/util/tests/util.test.php',
  'query' => 'arg1=val1&arg2=va2',
  'fragment' => 'fragment',
  );

TNTDebug::Warning('$uri->Set($uri_string)=', $uri->Set($uri_string));
TNTDebug::Warning('$uri->Get()=', $uri->Get());
TNTDebug::Warning('$uri->Get(\'string\')=', $uri->Get('string'));
TNTDebug::Warning('$uri->Get(\'array\')=', $uri->Get('array'));
TNTDebug::Warning('array_diff($uri->Get(\'array\'),$uri_array)=', array_diff($uri->Get('array'), $uri_array));

TNTDebug::Warning('$uri->Set($uri_array)=', $uri->Set($uri_array));
TNTDebug::Warning('$uri->Get()=', $uri->Get());
TNTDebug::Warning('$uri->Get(\'string\')=', $uri->Get('string'));
TNTDebug::Warning('$uri->Get(\'array\')=', $uri->Get('array'));
TNTDebug::Warning('$uri->Get(\'string\') === $uri_string =', $uri->Get('string') === $uri_string);

echo '<h1>PATH Format Tests</h1>';

TNTLoader::RequireClass('tntpath');

$path =& new TNTPath();

$path_string = '/tnt-synergy-2.0/includes/libs/tnt/util/tests/util.test.php';
$path_array = array(
  'dirname' => '/tnt-synergy-2.0/includes/libs/tnt/util/tests',
  'filename' => 'util.test',
  'extenstion' => 'php',
  );

TNTDebug::Warning('$path->Set($path_string)=', $path->Set($path_string));
TNTDebug::Warning('$path->Get()=', $path->Get());
TNTDebug::Warning('$path->Get(\'string\')=', $path->Get('string'));
TNTDebug::Warning('$path->Get(\'array\')=', $path->Get('array'));
TNTDebug::Warning('array_diff($path->Get(\'array\'),$path_array)=', array_diff($path->Get('array'), $path_array));

TNTDebug::Warning('$path->Set($path_array)=', $path->Set($path_array));
TNTDebug::Warning('$path->Get()=', $path->Get());
TNTDebug::Warning('$path->Get(\'string\')=', $path->Get('string'));
TNTDebug::Warning('$path->Get(\'array\')=', $path->Get('array'));
TNTDebug::Warning('$path->Get(\'string\') === $path_string =', $path->Get('string') === $path_string);

echo '<h1>Time Format Tests</h1>';

TNTLoader::RequireClass('tnttime');

$tm =& new TNTTime();

$tm_string = '15/06/2004 AD 11:53:04 GMT';

$tm_array = array('day' => 15, 'month' => 6, 'year' => 2004, 'period' => 'AD',
                  'hour' => 11, 'minute' => 53, 'second' => 4, 'tz' => 'GMT');

TNTDebug::Warning('$tm->Set($tm_string)=', $tm->Set($tm_string));
TNTDebug::Warning('$tm->Get()=', $tm->Get());
TNTDebug::Warning('$tm->Get(\'string\')=', $tm->Get('string'));
TNTDebug::Warning('$tm->Get(\'array\')=', $tm->Get('array'));
TNTDebug::Warning('array_diff($tm->Get(\'array\'),$tm_array)=', array_diff($tm->Get('array'), $tm_array));

TNTDebug::Warning('$tm->Set($tm_array)=', $tm->Set($tm_array));
TNTDebug::Warning('$tm->Get()=', $tm->Get());
TNTDebug::Warning('$tm->Get(\'string\')=', $tm->Get('string'));
TNTDebug::Warning('$tm->Get(\'array\')=', $tm->Get('array'));
TNTDebug::Warning('$tm->Get(\'string\') === $tm_string =', $tm->Get('string') === $tm_string);

echo '<h1>Ini Tests</h1>';

TNTLoader::RequireClass('tntini');

$ini =& new TNTIni();

TNTDebug::Warning('$ini->LoadFile(\'test-ini.ini\')=', $ini->LoadFile('test-ini.ini'));
TNTDebug::Warning('$tm->Items()=', $ini->Items());
TNTDebug::Warning('$ini->SaveFile(\'test-ini-tmp.ini\')=', $ini->SaveFile('test-ini-tmp.ini'));

?>

</body>
</html>

<!-- /* vim: set expandtab tabstop=4 shiftwidth=4: */ -->

