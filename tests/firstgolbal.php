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

echo "############# _SERVER test #############".PHP_EOL;
if (class_exists('request')) {
    echo "request class ok".PHP_EOL;
} else {
    echo "request class  not exists".PHP_EOL;
}
$request = new request();
$str = $request->getServer('SCRIPT_NAME');
print_r($str);
echo PHP_EOL;

$str = $request->getServer('REQUEST_URI');
var_dump($str);

echo "############# _GET test #############".PHP_EOL;
$_GET['golbal_test_key'] = "read golbal key success";
print_r($_GET);
$arr = $request->getQuery('golbal_test_key');
print_r($arr);


echo "############# _REQUEST test #############".PHP_EOL;
$_REQUEST['test'] = 'aaaaa';
print_r($_REQUEST);
$arr = $request->getRequest("test");
var_dump($arr);