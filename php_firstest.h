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

#ifndef PHP_FIRSTEST_H
#define PHP_FIRSTEST_H


extern zend_module_entry firstest_module_entry;
#define phpext_firstest_ptr &firstest_module_entry

#define PHP_FIRSTEST_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_FIRSTEST_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_FIRSTEST_API __attribute__ ((visibility("default")))
#else
#	define PHP_FIRSTEST_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:*/

ZEND_BEGIN_MODULE_GLOBALS(firstest)
	// zend_long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(firstest)

/* {{{ PHP_INI ini配置定义
*/
PHP_INI_BEGIN()
//STD_ZEND_INI_BOOLEAN("firstest.open", "On", PHP_INI_ALL, OnUpdateBool, gc_enabled, zend_gc_globals, gc_globals)
STD_PHP_INI_ENTRY("firstest.global_value","42", PHP_INI_ALL, OnUpdateLong, gc_enabled, zend_gc_globals, gc_globals)
PHP_INI_ENTRY("firstest.long", "100", PHP_INI_ALL, NULL)  // 创建ini的配置项
PHP_INI_ENTRY("firstest.custom_string", "this is custom string", PHP_INI_ALL, NULL)
PHP_INI_END()

/*
  firstclass_ce 是作什么用的？
  当我们在扩展中对这个类进行操作时，会使用到它，类似于文件句柄
*/
static zend_class_entry firstclass_ce;
static zend_class_entry *firstclass_ce_ptr;
/*
  声明函数
*/
void firstest_init(); 

/*
  声名方法
*/
PHP_METHOD(firstclass, getIv);

/* 接口定义开始 */
static zend_class_entry firstest_interface_ce;
static zend_class_entry *firstest_interface_ce_ptr;
static zend_class_entry parentclass_demo_ce;
static zend_class_entry *parentclass_demo_ce_ptr;
void firstest_interface_init();
PHP_METHOD(parentclass_demo, demo);
/* 接口定义结束 */

/* 对象生成 */
static zend_class_entry baby_ce;
static zend_class_entry *baby_ce_ptr;
void baby_init();
PHP_METHOD(baby, __construct);
PHP_METHOD(baby, hello);
/* 对象生成结束 */

/* 数组实例 开始 */
static zend_class_entry array_example_ce;
static zend_class_entry *array_example_ce_ptr;
int array_example_init();
PHP_METHOD(array_example, stringToArray);
/* 数组实例 结束 */

/* Always refer to the globals in your function as FIRSTEST_G(variable).
   You are encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/
#define FIRSTEST_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(firstest, v)

#if defined(ZTS) && defined(COMPILE_DL_FIRSTEST)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif	/* PHP_FIRSTEST_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
