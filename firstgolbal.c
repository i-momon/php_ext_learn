#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_alloc.h"
#include "Zend/zend_compile.h"
#include <fcntl.h>
#include <unistd.h>

#include "php_firstest.h"

// 定义参数类型，使内核自动校验参数类型
ZEND_BEGIN_ARG_INFO_EX(get_server_args, 0, 0, 1)  
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(get_query_args, 0, 0, 1)  
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(get_request_args, 0, 0, 1)  
    ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

const zend_function_entry request_method[] = {
    PHP_ME(request, getServer, get_server_args, ZEND_ACC_PUBLIC)
    PHP_ME(request, getQuery, get_query_args, ZEND_ACC_PUBLIC)
    PHP_ME(request, getRequest, get_request_args, ZEND_ACC_PUBLIC)
    PHP_FE_END
};


PHP_METHOD(request, getServer)
{
    char *name;
    size_t name_len;
    zval *ret;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &name, &name_len) == FAILURE) {
        RETURN_FALSE;
    }
    ret = request_server(TRACK_VARS_SERVER, name, name_len - 1);
    //ret = request_query(TRACK_VARS_SERVER, Z_STRVAL_P(key), Z_STRLEN_P(key));
    php_printf("Hello, Z_TYPE_P(ret) = %d \n", Z_TYPE_P(ret));

    if (Z_TYPE_P(ret) == IS_NULL) {  // TODO 
        RETURN_NULL();
    }
    RETURN_ZVAL(ret, 0, NULL);
}

PHP_METHOD(request, getQuery)
{
    zval *key, *ret;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &key) == FAILURE) {
        RETURN_FALSE;
    }
    ret = request_query(TRACK_VARS_GET, Z_STRVAL_P(key), Z_STRLEN_P(key));
    //ret = request_query(TRACK_VARS_SERVER, Z_STRVAL_P(key), Z_STRLEN_P(key));
    RETURN_ZVAL(ret, 0, NULL);
}

PHP_METHOD(request, getRequest)
{
    zval *key, *ret;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &key) == FAILURE) {
        RETURN_FALSE;
    }
    ret = request_request(TRACK_VARS_REQUEST, Z_STRVAL_P(key), Z_STRLEN_P(key));
    //ret = request_query(TRACK_VARS_SERVER, Z_STRVAL_P(key), Z_STRLEN_P(key));
    RETURN_ZVAL(ret, 0, NULL);
}

void request_init()
{
    INIT_CLASS_ENTRY(request_ce, "request", request_method);
    request_ce_ptr = zend_register_internal_class_ex(&request_ce, NULL);
}

zval * request_server(uint type, char *name, uint len) {

    
    /*  网上有文章说，没有这个就报错了
    if (PG(auto_globals_jit)) {
        zend_string *server_str = zend_string_init(ZEND_STRL("_SERVER"), 0);
        zend_is_auto_global(server_str);
        zend_string_release(server_str);
    }
    */
    zval* result = &PG(http_globals)[type];
    zval *ret;
    //zend_string *k = zend_string_init(ZEND_STRL("SCRIPT_NAME"), 0);
    // zend_string *k = zend_string_init("SCRIPT_NAME", sizeof("SCRIPT_NAME") - 1, 0);
    php_printf("Hello ,name = %s, sizeof(name) = %d, strlen(name) = %d \n", name, sizeof(&name), strlen(name));  // 此sizeof 与 strlen 求值不一致 ，sizeof 计算是编译绶冲区里

    zend_string *k = zend_string_init(name, strlen(name), 0);
    ret = zend_hash_find(Z_ARRVAL_P(result), k);  // 获取数据
    if (!ret) {  // 获取不到key 容错处处理
        // php_printf("Hello, this is KEY not fonund\n");
        zval *empty;
        HUBBLE_MAKE_STD_ZVAL(empty);
        return empty;
    }
    return ret;
}

zval * request_query(uint type, char *name, uint len) {
    zval *ret, *carrier = &PG(http_globals)[type];
    return carrier;

    if (!carrier) {
        ZVAL_NULL(ret);
        return ret;
    }
    php_printf("Hello ,name = %s\n", name);

    zend_string *k = zend_string_init(name, strlen(name), 0);
    ret = zend_hash_find(Z_ARRVAL_P(carrier), k);
    return ret;
}

zval * request_request(uint type, char *name, uint len) {
    zval *ret, *carrier = &PG(http_globals)[type];
    return carrier;

    if (!carrier) {
        HUBBLE_MAKE_STD_ZVAL(ret);
        return ret;
    }
    php_printf("Hello ,name = %s\n", name);

    zend_string *k = zend_string_init(name, strlen(name), 0);
    ret = zend_hash_find(Z_ARRVAL_P(carrier), k);
    return ret;
}
