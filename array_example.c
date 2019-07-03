#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_firstest.h"
#include "ext/standard/php_string.h"



/* demo函数参数 */
ZEND_BEGIN_ARG_INFO_EX(array_exmaple_args_info, 0, 0, 1)  // name, pass_rest_by_reference, return_reference, required_num_args
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

const zend_function_entry array_example_methods[] = {
    PHP_ME(array_example, stringToArray, array_exmaple_args_info, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

int array_example_init()
{
    INIT_CLASS_ENTRY(array_example_ce, "array_example", array_example_methods);
    array_example_ce_ptr = zend_register_internal_class_ex(&array_example_ce, NULL);

    return SUCCESS;
}

PHP_METHOD(array_example, stringToArray)
{
    zend_string *str, *delim;
    zend_long limit = ZEND_LONG_MAX;

#ifndef FAST_ZPP
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "SS|l", &delim, &str, &limit) == FAILURE) {
        return;
    }
#else
    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_STR(delim)
        Z_PARAM_STR(str)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(limit)
    ZEND_PARSE_PARAMETERS_END();
#endif

    php_printf("params : delim = %s, str = %s, limit = %d", ZSTR_VAL(delim), ZSTR_VAL(str), limit);

    if (ZSTR_LEN(delim) == 0) {
        php_error_docref(NULL, E_WARNING, "Empty delimiter");
        RETURN_FALSE;
    }

    zval tmp, *ret;
    array_init(return_value);
    if (ZSTR_LEN(str) == 0) {
        if (limit >= 0) {
            ZVAL_EMPTY_STRING(&tmp);
            zend_hash_index_add_new(Z_ARRVAL_P(return_value), 0, &tmp);
        }
    }

    if (limit > 1) {  // 建议参考explode PHP函数
		php_explode(delim, str, return_value, limit);
	} else {
		ZVAL_STR_COPY(&tmp, str);
		zend_hash_index_add_new(Z_ARRVAL_P(return_value), 0, &tmp);
	}

    zend_string *string_key;
    zend_ulong num_key;
    zval *entry;

    HashTable *ht;
	ht = emalloc(sizeof(HashTable));
	zend_hash_init(ht, 1, NULL, ZVAL_PTR_DTOR, 0);
   
   // 遍历数组  num_key 为数字索引， string_key 为字符串索引  entry为value值
   // 并将分隔数组val，加入到新的数组中， 规则是新数组key、val均用val  如 array('a','b') => array('a' => 'a', 'b' => 'b‘)
    ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(return_value), num_key, string_key, entry) {
        if (string_key == NULL && zend_hash_index_exists(Z_ARRVAL_P(return_value), num_key)) {
            php_printf("\n string_key : %s, num_key : %d, entry : %s \n", NULL, num_key, Z_STRVAL_P(entry));
            zend_hash_str_add_new(ht, Z_STRVAL_P(entry), strlen(Z_STRVAL_P(entry)), entry);  // 插到一个新数组，下面再遍历
        } else {
            php_printf("\n string_key :%s, num_key : %d, entry : %s \n", "aa", num_key, Z_STRVAL_P(entry));
        }
    }ZEND_HASH_FOREACH_END();


    // 遍历新的数组, 
    zend_string *skey;
    zend_ulong nkey;
    zval *newEntry;
    ZEND_HASH_FOREACH_KEY_VAL(ht, nkey, skey, newEntry){
        if (string_key == NULL && zend_hash_index_exists(ht, nkey)) {
            php_printf("\n num_key : %d, entry : %s \n", nkey, Z_STRVAL_P(newEntry));
        } else {
            php_printf("\n string_key :%s, entry : %s \n", ZSTR_VAL(skey), Z_STRVAL_P(newEntry));
        }
    }ZEND_HASH_FOREACH_END();

    // zval *entry;
    // zend_string *string_key;
    // zend_ulong num_key;
    //ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(ret), num_key, string_key, entry) {
        // php_printf("key : %s, val : %s\n", ZSTR_VAL(string_key), Z_STRVAL_P(entry));
        // php_printf("key : %s, val : %s\n", "key", "val");
    //}ZEND_HASH_FOREACH_END();
}
