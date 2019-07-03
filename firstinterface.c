#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_firstest.h"


/* demo函数参数 */
ZEND_BEGIN_ARG_INFO_EX(demo_arg_info, 0, 0, 1)  // name, pass_rest_by_reference, return_reference, required_num_args
	ZEND_ARG_INFO(0, name)
    ZEND_ARG_ARRAY_INFO(0, arr, 0)
ZEND_END_ARG_INFO()


const zend_function_entry firstest_interface_method[] = {
    ZEND_ABSTRACT_ME(firstest_interface, demo, demo_arg_info) // 接口名，方法名，参数
    ZEND_FE_END
};

// 实现interface_demo接口
const zend_function_entry parentclass_demo_method[] = {
    PHP_ME(parentclass_demo, demo, demo_arg_info, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

PHP_METHOD(parentclass_demo, demo)
{
    char *name = NULL;
    size_t name_len;
    zend_string *strg;

    zval *arr;
    int count;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "sa", &name, &name_len, &arr) == FAILURE) {
        return;
    }
    php_printf("Hello ,this is you first parentclass_demo extension\n");
    
    /* 测试参数是数组 */
    count = zend_hash_num_elements(Z_ARRVAL_P(arr));
    strg = strpprintf(0, "%s => %s, len => %d, elements => %d", "name", name, strlen(name), count);

	RETURN_STR(strg);
}

void firstest_interface_init()
{
    INIT_CLASS_ENTRY(firstest_interface_ce, "interface_demo", firstest_interface_method);
    firstest_interface_ce_ptr = zend_register_internal_interface(&firstest_interface_ce TSRMLS_CC); // 注册内部接口 到这步接口定义完成

    /* 实现接口 */
    INIT_CLASS_ENTRY(parentclass_demo_ce, "parentclass_demo", parentclass_demo_method);
    parentclass_demo_ce_ptr = zend_register_internal_class(&parentclass_demo_ce TSRMLS_CC);
    zend_class_implements(parentclass_demo_ce_ptr TSRMLS_CC, 1, firstest_interface_ce_ptr);
    /* 实现接口结束 */


    /*
    zend_string *_name;
    _name = zend_string_init("interface_demo", sizeof("interface_demo") - 1, 1);
    zend_str_tolower_copy(ZSTR_VAL(_name), "interface_demo", sizeof("interface_demo") - 1);

    zend_string *_interned_name = zend_new_interned_string(_name);

    zend_register_class_alias_ex(ZSTR_VAL(_interned_name), ZSTR_LEN(_interned_name), firstest_interface_ce_ptr);
    */
}
