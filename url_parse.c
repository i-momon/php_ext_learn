#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_firstest.h"

PHP_FUNCTION(url_parse_ext)
{
    php_printf("this is url parse function \n")
	
    RETURN_TRUE();
}