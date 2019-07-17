#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "php_firstest.h"

#include "firsturl.h"

const zend_function_entry firsturl_functions[] = {
	PHP_FE(first_url_parse, arginfo_url_parse)
	PHP_FE_END	/* Must be the last line in firstest_functions[] */
};

// PHP_FUNCTION(first_url_parse)
// {
//     char *str;
//     size_t str_len;
//     zend_long key = -1;
//     php_url *resource;

//     if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|l", &str, &str_len, &key) == FAILURE) {
//         return;
//     }

//     resource = php_url_parse_ex(str, str_len);
//     if (resource == NULL) {
//         RETURN_FALSE;
//     }
// 	php_printf("scheme : %s\n", resource->scheme);

//     zval val;
//     HashTable *ht;
//     ht = emalloc(sizeof(HashTable));
//     zend_hash_init(ht, 3, NULL, ZVAL_PTR_DTOR, 0);

//     if (resource->scheme != NULL) {
//         char *scheme = "scheme";
//         ZVAL_STR(&val,zend_string_init(resource->scheme, strlen(resource->scheme), 1));
//         zend_hash_str_add_new(ht, scheme, strlen(scheme), &val);
//     }
	
//     if (resource->host != NULL) {
//         char *host = "host";
//         ZVAL_STR(&val, zend_string_init(resource->host, strlen(resource->host), 1));
//         zend_hash_str_add_new(ht, host, strlen(host), &val);
//     }

	
//     if (resource->port != 0) {
//         char *port = "port";
// 		char p[5];
// 		sprintf(p, "%d", resource->port);
//         ZVAL_STR(&val, zend_string_init(p, strlen(p), 1));
//         zend_hash_str_add_new(ht, port, strlen(port), &val);
//     }

//     if (resource->user != NULL) {
//         char *user = "user";
//         ZVAL_STR(&val, zend_string_init(resource->user, strlen(resource->user), 1));
//         zend_hash_str_add_new(ht, user, strlen(user), &val);
//     }

//     if (resource->pass != NULL) {
//         char *pass = "pass";
//         ZVAL_STR(&val, zend_string_init(resource->pass, strlen(resource->pass), 1));
//         zend_hash_str_add_new(ht, pass, strlen(pass), &val);
//     }

//     if (resource->path != NULL) {
//         char *path = "path";
//         ZVAL_STR(&val, zend_string_init(resource->path, strlen(resource->path), 1));
//         zend_hash_str_add_new(ht, path, strlen(path), &val);
//     }

// 	if (resource->query != NULL) {
// 		char *query = "query";
// 		ZVAL_STR(&val, zend_string_init(resource->query, strlen(resource->query), 1));
// 		zend_hash_str_add_new(ht, query, strlen(query), &val);
// 	}

// 	if (resource->fragment != NULL) {
// 		char *fragment = "fragment";
// 		ZVAL_STR(&val, zend_string_init(resource->fragment, strlen(resource->query), 1));
// 		zend_hash_str_add_new(ht, fragment, strlen(fragment), &val);
// 	}

// 	RETURN_ARR(ht);
// }

FIRSTURL_STARTUP_FUNCTION(firsturl) {
    return SUCCESS;
}

char const *first_url_parse_ex()
{
    return "this is first_url_parse_ex function return \n";
}