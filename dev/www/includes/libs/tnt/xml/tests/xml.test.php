<?php
/**
 * Test cases for tnt/xml library
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
  <title>XML Tests</title>
</head>
<body>

<?php
global $tnt;

include '../../../../../includes/tntsynergy.inc';

TNTDebug::SetFlags(TNT_DEBUG_FLAGS_TOSCREEN);

TNTLoader::RequireClass('TNTXmlParser');

$file = './test.xml';
$opts = (TNT_TAGPARSER_OPTS_CASELOWER | TNT_TAGPARSER_OPTS_SKIPALL | TNT_TAGPARSER_OPTS_TRIM);


echo '<h1>Basic XML Parser Tests</h1>';

$xmlParser =& new TNTXmlParser();

if (!$xmlParser->ParseFile($file, $opts)) {
    TNTError::Error('Failed to parse the xml file: '.$xmlParser->Error());
} else {
    TNTDebug::Warning('Parsed data=', $xmlParser->Root());
    TNTDebug::Warning('Reduced data=', $xmlParser->Reduce());
    TNTDebug::Warning('Condensed data=', $xmlParser->Condense());
    TNTDebug::Warning('Group data=', $xmlParser->Group());
    TNTDebug::Warning('Merge data=', $xmlParser->Merge());
}

?>

</body>
</html>

<!-- /* vim: set expandtab tabstop=4 shiftwidth=4: */ -->

