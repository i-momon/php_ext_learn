
#ifndef FIRSTURL_H
#define FIRSTURL_H

// ZEND_BEGIN_ARG_INFO(arginfo_url_parse, 0)
// 	ZEND_ARG_INFO(0, url)
// ZEND_END_ARG_INFO()

char const *first_url_parse_ex();

PHP_FUNCTION(first_url_parse);

#define FIRSTURL_STARTUP_FUNCTION(module)    ZEND_MINIT_FUNCTION(foo_##module)
#define FIRSTURL_STARTUP(module)             ZEND_MODULE_STARTUP_N(foo_##module)(INIT_FUNC_ARGS_PASSTHRU)
#define FIRSTURL_SHUTDOWN_FUNCTION(module)   ZEND_MSHUTDOWN_FUNCTION(foo_##module)
#define FIRSTURL_SHUTDOWN(module)            ZEND_MODULE_SHUTDOWN_N(foo_##module)(SHUTDOWN_FUNC_ARGS_PASSTHRU)

#endif /* FIRSTURL_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 */
