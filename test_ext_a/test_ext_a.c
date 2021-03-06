#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test_ext_a.h"

static function_entry test_ext_a_functions[] = {
	{NULL, NULL, NULL}
};

zend_module_entry test_ext_a_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_TEST_EXT_A_EXTNAME,
	test_ext_a_functions,
	PHP_MINIT(test_ext_a),
	PHP_MSHUTDOWN(test_ext_a),
	PHP_RINIT(test_ext_a),
	NULL,
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_TEST_EXT_A_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_TEST_EXT_A
ZEND_GET_MODULE(test_ext_a)
#endif

int sum(int x, int y)
{
	return x + y;
}

int safe_sum(int x, int y)
{
	if(x > 0 && y > 0)
	{
		if(INT_MAX - x < y)
		{
#ifdef TEST_DEBUG
			php_printf("Overflow error!\n");
#endif
			return 0;
		}
	}
	else if(x < 0 && y < 0)
	{
		if(INT_MIN - x > y)
		{
#ifdef TEST_DEBUG
			php_printf("Overflow error!\n");
#endif
			return 0;
		}
	}

	return x + y;
}

PHP_RINIT_FUNCTION(test_ext_a)
{
#ifdef TEST_DEBUG
	php_printf("RINIT test ext a\n");
#endif

	return SUCCESS;
}

PHP_MINIT_FUNCTION(test_ext_a)
{
	SAMPLE_EXT_API ext_old, ext_new;
	
	ext_old.sum = sum;
	ext_old.code = 9876;

	ext_new.sum = safe_sum;
	ext_new.code = 1234;

	zend_eapi_register("eapi_test", "1.0.0.0", (void *)&ext_old, sizeof(ext_old));
	zend_eapi_register("eapi_test", "1.1.0.0", (void *)&ext_new, sizeof(ext_new));
	
	/* To test find_versions */
	zend_eapi_register("eapi_test2", "1.0.0.1", (void *)&ext_old, sizeof(ext_old));
	zend_eapi_register("eapi_test2", "1.0.1.2", (void *)&ext_old, sizeof(ext_old));
	zend_eapi_register("eapi_test2", "1.0.2.3", (void *)&ext_old, sizeof(ext_old));
	zend_eapi_register("eapi_test2", "1.0.0.0", (void *)&ext_old, sizeof(ext_old));
	zend_eapi_register("eapi_test2", "2.0.1.1", (void *)&ext_old, sizeof(ext_old));
	zend_eapi_register("eapi_test2", "2.0.2.2", (void *)&ext_old, sizeof(ext_old));
	zend_eapi_register("eapi_test2", "2.0.0.3", (void *)&ext_old, sizeof(ext_old));
	
	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(test_ext_a)
{
	return SUCCESS;
}
