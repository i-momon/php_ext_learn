#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_firstest.h"
#include "ext/standard/php_var.h"

/* __construct 函数参数 */
ZEND_BEGIN_ARG_INFO_EX(baby_arg_info, 0, 0, 1)  // name, pass_rest_by_reference, return_reference, required_num_args
	ZEND_ARG_INFO(0, age)
    ZEND_ARG_INFO(0, area)
ZEND_END_ARG_INFO()

const zend_function_entry baby_method[] = {
    PHP_ME(baby, __construct, baby_arg_info, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(baby, hello, NULL, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

PHP_METHOD(baby, __construct)
{
    zval *age, *area;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zz", &age, &area) == FAILURE) {  // 
        return;
    }
    zend_class_entry *baby_ce;
    baby_ce = Z_OBJCE_P(getThis());  // 获取

    // 属性赋值
    php_printf("set property age \n");
    zend_update_property(baby_ce, getThis(), "age", sizeof("age")-1, age);
    php_printf("set static property age \n");
    zend_update_static_property(baby_ce, "area", sizeof("area")-1, area);
    php_printf("set static property area end \n");
}

PHP_METHOD(baby, hello)
{
    zval *age, *area;
    zend_class_entry *baby_ce;

    baby_ce = Z_OBJCE_P(getThis());  // 获取对象句柄

    zend_read_property(baby_ce, getThis(), "age", sizeof("age")-1, 0, age);
    php_printf("hello get property age end\n");

    area = zend_read_static_property(baby_ce, "area", sizeof("area")-1, 0);
    php_printf("hello get static property area end : %s\n", Z_STRVAL_P(area));  // 获取ZVAL结构值
    
    // RETURN_ZVAL(area);  // 了解返回结构
    ZVAL_STR(return_value, Z_STR_P(area));
}

void baby_init()
{
    // 注册类
    INIT_CLASS_ENTRY(baby_ce, "baby", baby_method);
    baby_ce_ptr = zend_register_internal_class_ex(&baby_ce, NULL);

    // 添加属性
    zend_declare_property_null(baby_ce_ptr, "age", strlen("age"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(baby_ce_ptr, "area", strlen("area"), ZEND_ACC_STATIC|ZEND_ACC_PUBLIC);

}
