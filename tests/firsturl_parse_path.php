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

echo "############# _REQUEST_URI parse test #############".PHP_EOL;

$_SERVER['REQUEST_URI'] = "http://api.immomon.com:8639/v2/vchat/room/join?vid=1";
print_r($_SERVER);

$url = regex_compiled_test();
echo $url.PHP_EOL;

$uri = "/8634:tests/firsturl_parse_path.php";
$uri = preg_replace('![0-9\\.]{4,}$!', '', $uri);

$uri = regex_compiled_test()

echo $uri.PHP_EOL;


