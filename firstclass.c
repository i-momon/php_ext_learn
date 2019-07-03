#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_firstest.h"


const zend_function_entry firstclass_methods[] = {
    PHP_ME(firstclass, getIv, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
    PHP_FE_END
};

PHP_METHOD(firstclass, getIv)
{
    php_printf("Hello ,this is you first construct extension\n");
    RETURN_TRUE;
}

void firstest_init()
{
    INIT_CLASS_ENTRY(firstclass_ce, "First\\Firstest", firstclass_methods);
    firstclass_ce_ptr = zend_register_internal_class_ex(&firstclass_ce, NULL);

    zend_string *_name;
    _name = zend_string_init("firstclass", sizeof("firstclass") - 1, 1);
    zend_str_tolower_copy(ZSTR_VAL(_name), "firstclass", sizeof("firstclass") - 1);

    zend_string *_interned_name = zend_new_interned_string(_name);

    zend_register_class_alias_ex(ZSTR_VAL(_interned_name), ZSTR_LEN(_interned_name), firstclass_ce_ptr);

    /* 类 成员变量定义 */
    zend_declare_property_string(firstclass_ce_ptr, ZEND_STRL("name"), "my name is imomon", ZEND_ACC_PUBLIC);
    zend_declare_property_bool(firstclass_ce_ptr, ZEND_STRL("sex"), 1, ZEND_ACC_PUBLIC);
    /* 类 成员变量定义结束*/

    /* 类 静态成员定义 */
    zend_declare_class_constant_string(firstclass_ce_ptr, ZEND_STRL("AGE"), "100岁咯!" TSRMLS_DC);
    /* 类 静态成员定义结束*/
}
