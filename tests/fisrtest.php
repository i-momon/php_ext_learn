 <?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('firstest')) {
	dl('firstest.' . PHP_SHLIB_SUFFIX);
}
$module = 'firstest';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";
$function = 'confirm_' . $module . '_compiled';
if (extension_loaded($module)) {
	$str = $function($module);
} else {
	$str = "Module $module is not compiled into PHP";
}
echo "$str\n";

use First\Firstest;

$fisrtclass = new firstest();
$str = $fisrtclass->getIv();
echo $str.PHP_EOL;

echo "############### class property ###############".PHP_EOL;
echo "name => ".$fisrtclass->name.PHP_EOL;
echo "sex  => ".$fisrtclass->sex.PHP_EOL;

echo "############### class const ###############".PHP_EOL;
echo "AGE  => ".$fisrtclass::AGE.PHP_EOL;
echo "#######ini\ttest#############".PHP_EOL;
$long = ini_get('firstest.long');
echo $long.PHP_EOL;

ini_set('firstest.long', "99");
$long = ini_get('firstest.long');
echo $long.PHP_EOL;

$string = ini_get('firstest.custom_string');
echo $string.PHP_EOL;


echo "#######custom\tini\ttest#############".PHP_EOL; 
$long = custom_ini_get('firstest.custom_string');
echo $long.PHP_EOL;

echo "############### callback function ###############".PHP_EOL;

$func1 = 'printf';
// $ret = call_php_function($func1, 'a:%s%d', date('Y-m-d H:i:s'), __LINE__);
$ret = call_php_function($func1, 'a-a');
print_r($ret);

// $func2 = 'array_merge';
// $params = array(1);
// $params2 = array(2);
// $ret = call_php_function($func2, $params, $params2);
// print_r($ret);
?>
