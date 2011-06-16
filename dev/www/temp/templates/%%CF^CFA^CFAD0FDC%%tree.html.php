<?php /* Smarty version 2.6.18, created on 2008-01-21 18:11:00
         compiled from d:%5Cmy+webs+1%5Ctnt-synergy-3.0%5Cdev%5Cwww%5Cunittests%5Ctnt%5Cwui%5Ctests/tree.html */ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Welcome to the Tree Controls Widget test suite</title>
<script type="text/javascript">
var app_win = window.top;
var skinName = window.top.skinName || 'winclassic';
var serverDir = '/synergy';
var iconDir = '/synergy/media/images/icons/16x16' + '/';
</script>
<script ctrl="text/javascript" src="~/scripts/main.js"></script>
<style>
<?php echo '
body { font-size: 8pt; font-weight: normal; text-decoration: }
h1 { font-size: 12pt; font-weight: bold; text-decoration: underline; }
h2 { font-size: 10pt; font-weight: bold; text-decoration: none; margin-bottom: 4px; }
th { font-size: 10pt; font-weight: bold; text-align: center; padding: 2px; background-color: #ccccff; }
.widget { }
'; ?>

</style>
</head>
<body>
<h1>Welcome to the Tree Controls Widget test suite</h1>

<p><a href="index.html">Test Index</a></p>

<?php 

$tree_items = array(
array('id' =>  1, 'pid' => 0, 'label' => 'Root', 'items' => array(
array('id' =>  2, 'pid' => 1, 'label' => 'England', 'items' => array(
array('id' =>  5, 'pid' => 2, 'label' => 'South West', 'items' => array(
        array('id' => 14, 'pid' => 5,  'label' => 'Somerset'),
        array('id' => 15, 'pid' => 5,  'label' => 'Devon'),
        array('id' => 16, 'pid' => 5,  'label' => 'Cornwall'),
        array('id' => 17, 'pid' => 5,  'label' => 'North Somerset'),
        array('id' => 18, 'pid' => 5,  'label' => 'Dorset'),
        array('id' => 19, 'pid' => 5,  'label' => 'Wiltshire'))),
        array('id' =>  6, 'pid' => 2, 'label' => 'South East', 'items' => array(
        array('id' => 20, 'pid' => 6,  'label' => 'Essex'),
        array('id' => 21, 'pid' => 6,  'label' => 'London'),
        array('id' => 22, 'pid' => 6,  'label' => 'Hampshire'),
        array('id' => 23, 'pid' => 6,  'label' => 'Norfolk'),
        array('id' => 24, 'pid' => 6,  'label' => 'East Anglia'))),
        array('id' =>  7, 'pid' => 2, 'label' => 'Midlands', 'items' => array(
        array('id' => 25, 'pid' => 7,  'label' => 'Lincolnshire'),
        array('id' => 26, 'pid' => 7,  'label' => 'Nottinghamshire'),
        array('id' => 27, 'pid' => 7,  'label' => 'Birmingham'),
        array('id' => 28, 'pid' => 7,  'label' => 'Leicestershire'),
        array('id' => 29, 'pid' => 7,  'label' => 'Cheshire'))),
        array('id' =>  8, 'pid' => 2, 'label' => 'North West', 'items' => array(
        array('id' => 30, 'pid' => 8,  'label' => 'Lancashire'),
        array('id' => 31, 'pid' => 8,  'label' => 'Stafordshire'),
        array('id' => 32, 'pid' => 8,  'label' => 'Manchester'))),
      array('id' =>  9, 'pid' => 2, 'label' => 'North East', 'items' => array(
        array('id' => 33, 'pid' => 9,  'label' => 'Northumberland'),
        array('id' => 34, 'pid' => 9,  'label' => 'Durham'),
        array('id' => 35, 'pid' => 9,  'label' => 'Yorkshire'))))),

        array('id' =>  3, 'pid' => 1, 'label' => 'Scotland', 'items' => array(
      array('id' => 10, 'pid' => 3,  'label' => 'Lowlands'),
      array('id' => 11, 'pid' => 3,  'label' => 'Hightlands'))),

      array('id' =>  4, 'pid' => 1, 'label' => 'Wales', 'items' => array(
      array('id' => 12, 'pid' => 4, 'label' => 'North'),
      array('id' => 13, 'pid' => 4, 'label' => 'South'))),
    ))
);

$this->assign('tree_items', $tree_items);

 ?>

<table width="100%" cellpadding="20" border="1">
  <col width="50%" align="left" valign="top" />
  <col width="50%" align="left" valign="top" />
  <tbody><tr><td>

<h2>1) Classic Tree widget</h2>
<div id="widget1" class="widget">
  <tnt:tree items=$tree_items />
</div>

</td><td>

<h2>2) Explorer Tree widget</h2>
<div id="widget2" class="widget">
  <tnt:tree behaviour="explorer" items=$tree_items />
</div>

</td></tr><tr><td>

<?php echo '
<script>
  function showSelection(tree, div) {
    var s = \'\';
    for (var i in tree.selectedItems) {
      if (s != \'\') s += \',\';
      s += tree.selectedItems[i].label;
    }
    window.document.all[div].innerHTML = s;
  }
</script>
'; ?>


<h2>3) Classic Tree widget (MultiSelect)</h2>
<div id="widget3" class="widget">
  <tnt:tree multiple=true items=$tree_items
    onchange="showSelection(this, 'tree3_selected');" />
  <div id="tree3_selected">&nbsp;</div>
</div>

</td><td>

<h2>4) Explorer Tree widget (MultiSelect)</h2>
<div id="widget4" class="widget">
  <tnt:tree behaviour="explorer" multiple=true items=$tree_items
    onchange="showSelection(this, 'tree4_selected');" />
  <div id="tree4_selected">&nbsp;</div>
</div>

</td></tr><tr><td>

<h2>5) Tree widget (SingleExpand)</h2>
<div id="widget5" class="widget">
  <tnt:tree singleExpand=true items=$tree_items />
</div>

</td><td>

<h2>6) Tree widget (NoIcons)</h2>
<div id="widget6" class="widget">
  <tnt:tree showIcons=false items=$tree_items />
</div>

</td></tr><tr><td>

<h2>7) Tree widget (Check)</h2>
<form>
<div id="widget7" class="widget">
  <tnt:tree type="check" items=$tree_items />
</div>
<input type="checkbox" value="1" />Check All
</form>

</td><td>

<h2>8) Tree widget (Radio)</h2>
<div id="widget8" class="widget">
  <tnt:tree type="radio" items=$tree_items />
</div>

</td></tr><tr><td>

<h2>9) Tree Control widget</h2>
<div id="widget9" class="widget">
  <tnt:treebox name="tree5"
    value="Devon" items=$tree_items />
</div>

</td><td>

<h2>10) Tree Control widget (MultiSelect)</h2>
<div id="widget10" class="widget">
  <tnt:tree name="tree6" multiple=true
    value="Devon" items=$tree_items />
</div>

</td></tr></tbody></table>

</body>
</html>
<!-- /* vim: set expandtab tabstop=2 shiftwidth=2: */ -->