/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/standard/url.h"
#include "ext/pcre/php_pcre.h"

#include "zend_API.h"
#include "php_firstest.h"

// 
ZEND_BEGIN_ARG_INFO(arginfo_custom_ini_get, 0)
	ZEND_ARG_INFO(0, arg)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_url_parse, 0)
	ZEND_ARG_INFO(0, url)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_uri, 0)
	ZEND_ARG_INFO(0, uri)
ZEND_END_ARG_INFO()

/* If you declare any globals in php_firstest.h uncomment this: */
ZEND_DECLARE_MODULE_GLOBALS(firstest)

/* True global resources - no need for thread safety here */
static int le_firstest;


/* 调用自定义PHP函数 */
ZEND_BEGIN_ARG_INFO_EX(call_php_function_arg_info, 0, 0, 0)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()
/* 调用自定义的PHP函数 结束*/

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("firstest.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_firstest_globals, firstest_globals)
    STD_PHP_INI_ENTRY("firstest.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_firstest_globals, firstest_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_firstest_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_firstest_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "firstest", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


PHP_FUNCTION(first_url_parse)
{
    char *str;
    size_t str_len;
    zend_long key = -1;
    php_url *resource;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|l", &str, &str_len, &key) == FAILURE) {
        return;
    }

    resource = php_url_parse_ex(str, str_len);
    if (resource == NULL) {
        RETURN_FALSE;
    }
	php_printf("scheme : %s\n", resource->scheme);

    zval val;
    HashTable *ht;
    ht = emalloc(sizeof(HashTable));
    zend_hash_init(ht, 3, NULL, ZVAL_PTR_DTOR, 0);

    if (resource->scheme != NULL) {
        char *scheme = "scheme";
        ZVAL_STR(&val,zend_string_init(resource->scheme, strlen(resource->scheme), 1));
        zend_hash_str_add_new(ht, scheme, strlen(scheme), &val);
    }
	
    if (resource->host != NULL) {
        char *host = "host";
        ZVAL_STR(&val, zend_string_init(resource->host, strlen(resource->host), 1));
        zend_hash_str_add_new(ht, host, strlen(host), &val);
    }

	
    if (resource->port != 0) {
        char *port = "port";
		char p[5];
		sprintf(p, "%d", resource->port);
        ZVAL_STR(&val, zend_string_init(p, strlen(p), 1));
        zend_hash_str_add_new(ht, port, strlen(port), &val);
    }

    if (resource->user != NULL) {
        char *user = "user";
        ZVAL_STR(&val, zend_string_init(resource->user, strlen(resource->user), 1));
        zend_hash_str_add_new(ht, user, strlen(user), &val);
    }

    if (resource->pass != NULL) {
        char *pass = "pass";
        ZVAL_STR(&val, zend_string_init(resource->pass, strlen(resource->pass), 1));
        zend_hash_str_add_new(ht, pass, strlen(pass), &val);
    }

    if (resource->path != NULL) {
        char *path = "path";
        ZVAL_STR(&val, zend_string_init(resource->path, strlen(resource->path), 1));
        zend_hash_str_add_new(ht, path, strlen(path), &val);
    }

	if (resource->query != NULL) {
		char *query = "query";
		ZVAL_STR(&val, zend_string_init(resource->query, strlen(resource->query), 1));
		zend_hash_str_add_new(ht, query, strlen(query), &val);
	}

	if (resource->fragment != NULL) {
		char *fragment = "fragment";
		ZVAL_STR(&val, zend_string_init(resource->fragment, strlen(resource->query), 1));
		zend_hash_str_add_new(ht, fragment, strlen(fragment), &val);
	}

	RETURN_ARR(ht);
}


/**
 * 调用PHP函数
 * 
 * 
		<?php
		// 调用一个可变参函数，输出函数名和参数，返回调用后的结果
		function calltask(callable $func[, mixed $arg1[, ...]]);
		// 原型可变参写法
		function calltask(... $args) {
				$func = array_shift($args);
				if (!is_callable($func)) {
						printf("the function is not callable:%s\n", $func);
						return false;
				}
				// 输出
				printf("you call func:%s\n", $func);
				foreach ($args as $i=>$arg) {
						printf("arg[%d] type=%d len=%d val=%s \n", $i, typeof($arg), strlen($arg), strval($arg));
				}

				return $func(... $args);
		}
 */ 
PHP_FUNCTION(call_php_function) {
	int i, status, argc = ZEND_NUM_ARGS();
	zval *args;
	zval retval;

#ifndef FAST_ZPP
	if (zend_pasre_parameters(ZEND_NUM_ARGS(), "+", &args, &argc) == FAILURE) {  // 
		return;
	}
#else
	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_VARIADIC("+", args, argc)
	ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);
#endif

	if (argc < 1) {
		WRONG_PARAM_COUNT;  // 参数个数不对抛出错误
	}

	// 第一参数为函数名 取出来
	zval *func = &(args[0]);
	php_printf("you will call:%s argc=%d is_callble=%d\n", Z_STRVAL(*func), argc, zend_is_callable(func, IS_CALLABLE_CHECK_NO_ACCESS, NULL));
	if (!zend_is_callable(func, IS_CALLABLE_CHECK_NO_ACCESS, NULL)) {
		php_printf("the function is not callable:%s\n", Z_STRVAL(*func));
		RETURN_FALSE;
	}

	// 第二个参数开始, 把参数打印出来
	for (i=0; i<argc; i++) {
		convert_to_string_ex(&args[i]);
		php_printf("arg[%d] type=%d len = %d val = %s \n", i, Z_TYPE(args[i]), Z_STRLEN(args[i]), Z_STRVAL(args[i]));
	}

	// 调用函数
	status = call_user_function_ex(EG(function_table), NULL, func, &retval, argc-1, (argc > 1 ? args + 1:NULL), 0, NULL);
	
	// 判断返回值
	if (status == SUCCESS && !Z_ISUNDEF(retval)) {
		php_printf("the func success:%d\n", status);
	} else {
		php_printf("the func failed:%d\n", status);
	}
	php_printf("the call over:%s argc=%d retval = %s\n", Z_STRVAL(*func), argc, &(retval).value.str); //  &(retval).value.str 获取不到值 TODO

	
	RETURN_ZVAL(&retval,1,1); // TODO RETVAL_STR 与 RETURN_STR的区别
}

/**
 *  本函数功能：调用扩展内类实例化对象方法、静态方法
 * 
 */
PHP_FUNCTION(test_object_call)
{
	// __construct 需要的参数
	/*
	char age, area;
	size_t age_len, area_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ss", &age, &age_len, &area, &area_len) == FAILURE) {
		return;
	}
	*/

	// TRACE("test_object_all start");
	zend_string *babyClass = zend_string_init("baby", sizeof("baby")-1, 0);
	zend_class_entry *babyHandle = zend_lookup_class(babyClass);  // zend_lookup_class 查找hashtable里的类
	zend_string_release(babyClass);  // 释放内存

	// new a babyHandle object
	zval obj;
	object_init_ex(&obj, babyHandle);


	zval func_name, retval, func_params[2];
	char *func = "__construct";
	ZVAL_STR(&func_name, zend_string_init(func, strlen(func),0));

	zend_string *call_func_str;
	call_func_str = zend_string_init("18岁呼!", sizeof("18岁呼!")-1,0);
	ZVAL_STR(&func_params[0], call_func_str);

	call_func_str = zend_string_init("imomon", sizeof("imomon")-1,0);
	ZVAL_STR(&func_params[1], call_func_str);

	if(SUCCESS != call_user_function(&(babyHandle->function_table), &obj, &func_name, &retval, 2, func_params)){
        RETURN_FALSE;
  }

	zval fun_name, retv, fun_params;
	char *fun = "hello";
	ZVAL_STR(&fun_name, zend_string_init(fun, strlen(fun), 0));
	if(SUCCESS != call_user_function(&(babyHandle->function_table), &obj, &fun_name, &retv, 0, &fun_params)){
        RETURN_FALSE;
  }

	php_printf("get hello function %v\n", retv);

	// 需要释放掉zend_string结构
	zend_string_release(call_func_str);

	// TODO 函数调用返回值
}

/* 生成如下对象  
class baby
{
		public $age;
		public $static $area;
    public function __construct($age, $area)
    {
        $this->age = $age;
				self::$area = $area;
    }   

    public function hello()
    {
        var_dump($this->age, self::$area);
    }
}

function test_call()
{
    $obj = new baby();
    $obj->hello();
}
*/
//TODO
/* 生成对象结束*/

PHP_FUNCTION(regex_compiled_test)
{
	zval *servers, *server, *empty;
    // zend_long limit = -1;
    // zend_string *what = NULL;
	// int replace_count;

    servers = &PG(http_globals)[TRACK_VARS_SERVER];
	zend_string *k = zend_string_init("SCRIPT_FILENAME", strlen("SCRIPT_FILENAME"), 0);
	php_printf("servers …… \n");
	php_printf("k : %s \n", ZSTR_VAL(k));
    server = zend_hash_find(Z_ARRVAL_P(servers), k);
	if (!server) {
		RETURN_TRUE;
	}
	if (Z_TYPE_P(server) == IS_STRING) {
		php_printf("server is string : %s \n", Z_STRVAL_P(server));
	}
	if (Z_TYPE_P(server) == IS_LONG) {
		php_printf("int : %d", Z_STRVAL_P(server));
	}
	if (Z_TYPE_P(server) == IS_UNDEF) {
		php_printf("server undef \n");
	}

	//字符串正则替换
	zend_long limit = -1;
	zend_string *result;
	char *subject = "/8634:tests/firsturl_parse_path.php";
	char *re = "![0-9\\.]{4,}$!";
	
	zend_string *subject_str = zend_string_init(subject, strlen(subject), 0);
	zend_string *regex = zend_string_init(re, strlen(re), 0);

	zval replace_val;
	ZVAL_STR(&replace_val, zend_string_init("9999", strlen("9999"), 0));

	pcre_cache_entry *pce;
	/* Compile regex or get it from cache. */
	if ((pce = pcre_get_compiled_regex_cache(regex)) == NULL) {
		php_printf("regex compiled fail\n");
		RETURN_FALSE;
	} 

	/* if subject is not an array */
	if ((result = php_pcre_replace(regex, subject_str, subject, strlen(subject), &replace_val, -1, 0, 0)) != NULL) {
		php_printf("success result = : %s \n", ZSTR_VAL(result));
	}
    // if (!server) {
    //     HUBBLE_MAKE_STD_ZVAL(empty);
    //     RETURN_ZVAL(empty,0,1);
    // }
	// zend_string *tmp = php_trim(ZVAL_STR(server), (what ? ZSTR_VAL(what) : NULL), (what ? ZSTR_LEN(what) : 0), 3);
	// php_printf("tmp : %s", ZSTR_VAL(tmp));


	/* 
    zval *ret;
    replace_count = preg_replace_impl(ret, "![0-9\\.]{4,}$!", '', server, limit, 0, 0);
    php_trim(ZVAL_STR(ret), (what ? ZSTR_VAL(what) : NULL), (what ? ZSTR_LEN(what) : 0), 3);

    // 兼容Moa
    zval data;
    array_init(&data);

    zend_string *delim_string = zend_string_init(":", strlen(":"), 1);
    php_explode(delim_string, ZSTR_VAL(ret), &data, 100);
    if (Z_ARR_P(&data) == IS_ARRAY && Z_ARR_P(&data)->nNumUsed == 0) {
        add_index_null(&data, 0);
        add_index_null(&data, 0);
        return RETURN_ZVAL(&data, 1, 1);
    }
    zval *domain, *business, *action, *slice;
    domain = zend_hash_index_find(Z_ARR_P(&data), 0);
    business = zend_hash_index_find(Z_ARR_P(&data), 1);
    
    if (Z_ARR_P(&data)->nNumUsed <= 2) {
        ZVAL_NEW_STR(action, zend_string_init("default", strlen("default"), 1));
    } else {
        slice = zend_hash_index_find(Z_ARR_P(&data), 3);
        ZVAL_NEW_STR(action, zend_string_init("default", strlen("default"), 1));
        // 这里需要再分解
    }

    if (!business) {
        ZVAL_NEW_STR(business, zend_string_init("default", strlen("default"), 1));
    }

    HashTable *ht;
    ht = emalloc(sizeof(HashTable));
    zend_hash_init(ht, 3, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_index_add_new(ht, 1, domain);
    zend_hash_index_add_new(ht, 2, business);
    zend_hash_index_add_new(ht, 3, action);
    RETURN_ARR(ht);    
	*/
}

/* 测试各种类型返回值实例 */
PHP_FUNCTION(test_return_result)
{
	// 返回null
	//RETURN_NULL(); 

	// 返回整型  参数是整型
	//int ilong = 100;
	//RETURN_LONG(ilong);

	// 返回bool  参数是bool
	//RETURN_BOOL(false);

	// 返回double  参数是double 和 float型
	//double idouble = 0.45;
	//RETURN_DOUBLE(idouble);

	// 返回str  参数是zend_string
	//zend_string *_str;
	//char *str = "this is string";
	//_str = zend_string_init(str, strlen(str), 1);
	//RETURN_STR(_str);

	// 返回内部字符串 参数是zend_string
	//zend_string *_str;
	//char *str = "function";
	//_str = zend_string_init(str, strlen(str), 1);
	//RETURN_INTERNED_STR(_str);

	// 返回字符串  复制一个新值
	//zend_string *_str;
	//char *str = "this is copy str , refcount ++";
	//_str = zend_string_init(str, strlen(str), 1);
	//RETURN_STR_COPY(_str);

	// 返回字符串 生成一个新的字符串 参数为字符串
	// char *str = "this is new string!"; 
	// RETURN_STRING(str);

	// 返回字符串  
	//char *str = "this is new string , params with len";
	//RETURN_STRINGL(str, strlen(str));

	// 返回空字符串  
	// RETURN_EMPTY_STRING();

	// 返回资源类型 TODO

	// 返回数组  参数是zend_array
	/*
	HashTable *ht;
	ht = emalloc(sizeof(HashTable));
	zend_hash_init(ht, 3, NULL, ZVAL_PTR_DTOR, 0);

	char *str = "this is arr val";
	zend_string *kval = zend_string_init(str, strlen(str), 1);
	zval _kval;
	ZVAL_STR(&_kval, kval);
	zend_hash_str_add_new(ht, "ky", strlen("ky"), &_kval);
	zend_hash_index_add_new(ht, 1, &_kval);
	zend_hash_index_add(ht, 2, &_kval);
	RETURN_ARR(ht);
	*/
	/*
	第二种返回方法
	zval res;
  array_init(&res);
  add_assoc_string(&res, "indicator", "indicator");
	RETURN_ZVAL(&res, 1, 1);
	*/


	// 返回zval  参数为zval, copy, dtor
	/*
	zval res;
	char *str = "this is ZVAL_STR init string";
	zend_string *kval = zend_string_init(str, strlen(str), 1);
	ZVAL_STR(&res, kval);
	RETURN_ZVAL(&res, 1, 1);  // (zv, copy, dtor)
	*/

	// RETURN_FALSE;
	RETURN_TRUE;
	
	// 返回对象
	// RETURN_OBJ

}
/* 测试各种类型返回值实例 结束 */


/* 函数内获取ini配置*/
PHP_FUNCTION(custom_ini_get)
{
	char *arg = NULL;
	size_t arg_len;
	const char *ini_string;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	// ini_string = INI_STR("firstest.custom_string");
	// ini_string = INI_STR(arg);  // 此方法获取不到配置值 待定位
	// php_printf("%s => %s\n",arg, ini_string);
	ini_string = zend_ini_string(arg, (uint)arg_len, 0);
	RETURN_STRING(ini_string);

}

/* {{{ 变量定义
*/
PHP_FUNCTION(define_var)
{
	/* 定义变量开始 */
	zval var_value; // 变量的值
	zend_string *var_name = NULL; // 变量的名称
	// 创建整型变量
	ZVAL_LONG(&var_value, 2);
	zend_set_local_var_str("lng", 3, &var_value, 0);
	/* 定义变量开始 结束 */

	RETURN_TRUE;
}
/* }}}
*/


/* {{{  PHP函数翻写 
	uri = /a/b/c/d?id=1 
*/
PHP_FUNCTION(uri_process)
{
		zend_string *uri;
		char *delim = ":";
		zend_long limit = 1;

		ZEND_PARSE_PARAMETERS_START(1, 1)
				Z_PARAM_STR(uri)
		ZEND_PARSE_PARAMETERS_END();

		php_printf("uri:%s", ZSTR_VAL(uri));
		zval data;
		array_init(&data);

		zend_string *delim_string = zend_string_init(delim, strlen(delim), 1);

		php_printf("delim_string : %s \n", ZSTR_VAL(delim_string));

		php_explode(delim_string, uri, &data, 100);

		php_printf("data nNumUsed:%d \n", (Z_ARR_P(&data))->nNumUsed);

		//zend_string *service = zend_string_init("service", strlen("service"), 1);

		if (Z_TYPE_P(&data) == IS_ARRAY && Z_ARR_P(&data)->nNumUsed > 3) {
			zval *service, *method;
			service = zend_hash_index_find(Z_ARR_P(&data), 1);
			method = zend_hash_index_find(Z_ARR_P(&data), 3);
			php_printf("service = %s, method = %s \n", Z_STRVAL_P(service), Z_STRVAL_P(method));

			zval serviceArr;
			array_init(&serviceArr);		

			zend_string *de = zend_string_init("/", strlen("/"), 1);
			php_explode(de, Z_STR_P(service), &serviceArr, 100);

			php_printf("serviceArr nNumUsed:%d \n", (Z_ARR_P(&serviceArr))->nNumUsed);
			if (Z_ARR_P(&serviceArr)->nNumUsed > 1) {
				zval res;
				zend_string *delim_str = zend_string_init(".", strlen("."), 1);
				php_implode(delim_str, &serviceArr, &res);
				php_printf("new service : %s \n", Z_STRVAL_P(&res));


				zval *tmp;
				zend_string *str;
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&serviceArr), tmp){
						str = zval_get_string(tmp);
						php_printf("val : %s \n", ZSTR_VAL(str));
				}ZEND_HASH_FOREACH_END();
			}
		}

		

		// 遍历新的数组, 
    zend_string *skey;
    zend_ulong nkey;
    zval *newEntry;
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(&data), nkey, skey, newEntry){
        if (skey == NULL && zend_hash_index_exists(Z_ARR_P(&data), nkey)) {
            php_printf("\n num_key : %d, entry : %s \n", nkey, Z_STRVAL_P(newEntry));
        } else {
            php_printf("\n skey :%s, entry : %s \n", ZSTR_VAL(skey), Z_STRVAL_P(newEntry));
        }
    }ZEND_HASH_FOREACH_END();

/*

		zval *ret;
    array_init(ret);

		zend_string *delim_string = zend_string_init(delim, strlen(delim), 1);
		php_explode(delim_string, uri, ret, 1);
		if (Z_TYPE_P(ret) == IS_ARRAY) {
			php_printf("Array count ");
		} else {
			php_printf("not Array ");
		}
		*/

		RETURN_TRUE;

}
/* }}}
*/

PHP_FUNCTION(golbal_var)
{
	RETURN_STRINGL(FIRSTEST_G(global_string), strlen(FIRSTEST_G(global_string)));
}

/* {{{ php_firstest_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_firstest_init_globals(zend_firstest_globals *firstest_globals)
{
	firstest_globals->global_value = 0;
	firstest_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(firstest)
{
	/* If you have INI entries, uncomment these lines */
	REGISTER_INI_ENTRIES();
	
	firstest_init(); // 资源初始化
	firstest_interface_init(); // 资源初始化
	baby_init();

	array_example_init(); 

	FIRSTEST_G(global_string) = "This is global var";

	return SUCCESS;
}
/* }}} */


/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(firstest)
{
	/* uncomment this line if you have INI entries */
	UNREGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(firstest)
{
#if defined(COMPILE_DL_FIRSTEST) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(firstest)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(firstest)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "firstest support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ firstest_functions[]
 *
 * Every user visible function must have an entry in firstest_functions[].
 */
const zend_function_entry firstest_functions[] = {
	PHP_FE(confirm_firstest_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(test_object_call, NULL) // 测试类 对象调用
	PHP_FE(test_return_result, NULL) // 测试各种返回值情况
	PHP_FE(custom_ini_get, arginfo_custom_ini_get)
	// PHP_FE(test_object_call, NULL) // 对象调用
	PHP_FE(call_php_function, NULL) // 调用php 函数
	PHP_FE(define_var, NULL) // 变量定义
	PHP_FE(golbal_var, NULL) // 获取全局变量
	PHP_FE(uri_process, arginfo_uri) // uri分成数组 组成
	PHP_FE(first_url_parse, arginfo_url_parse)
	PHP_FE(regex_compiled_test, NULL) // 正则匹配测试
	PHP_FE_END	/* Must be the last line in firstest_functions[] */
};
/* }}} */

/* {{{ firstest_module_entry
 */
zend_module_entry firstest_module_entry = {
	STANDARD_MODULE_HEADER,
	"firstest",
	firstest_functions,
	PHP_MINIT(firstest),
	PHP_MSHUTDOWN(firstest),
	PHP_RINIT(firstest),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(firstest),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(firstest),
	PHP_FIRSTEST_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */



#ifdef COMPILE_DL_FIRSTEST
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(firstest)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
