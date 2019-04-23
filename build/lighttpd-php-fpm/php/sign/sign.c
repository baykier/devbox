/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Author: zhaoshunyao@baidu.com                                        |
  +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_sign.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#define SIGN_VERSION	"1.0"

zend_function_entry sign_functions[] = {
	ZEND_FE(sign64, NULL)
	{NULL, NULL, NULL}
};

zend_module_entry sign_module_entry = {
	STANDARD_MODULE_HEADER,
	"sign",
	sign_functions,
	ZEND_MODULE_STARTUP_N(sign),
	ZEND_MODULE_SHUTDOWN_N(sign),
	ZEND_MODULE_ACTIVATE_N(sign),
    ZEND_MODULE_DEACTIVATE_N(sign),
	ZEND_MODULE_INFO_N(sign),
	SIGN_VERSION,
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_SIGN
ZEND_GET_MODULE(sign)
#endif

ZEND_INI_BEGIN()
	ZEND_INI_ENTRY("sign.type", "murmurhash2", ZEND_INI_ALL, NULL)
ZEND_INI_END()

// 64-bit hash for 64-bit platforms
uint64_t MurmurHash64A(const void *key, int len)
{
	const uint64_t m = 0xc6a4a7935bd1e995;
	const uint32_t seed = 5381;
	const int r = 47;

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)key;
	const uint64_t * end = data + (len/8);

	while(data != end)
	{
		uint64_t k = *data++;

		k *= m; 
		k ^= k >> r; 
		k *= m; 
		
		h ^= k;
		h *= m; 
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7)
	{
		case 7: h ^= (uint64_t)data2[6] << 48;
		case 6: h ^= (uint64_t)data2[5] << 40;
		case 5: h ^= (uint64_t)data2[4] << 32;
		case 4: h ^= (uint64_t)data2[3] << 24;
		case 3: h ^= (uint64_t)data2[2] << 16;
		case 2: h ^= (uint64_t)data2[1] << 8;
		case 1: h ^= (uint64_t)data2[0];
	        h *= m;
	};
 
	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

//在模块第一次加载时被调用
ZEND_MODULE_STARTUP_D(sign)
{
	REGISTER_INI_ENTRIES();
	
	return SUCCESS;
}

//在模块卸载关闭时被引擎调用，通常用于注销INI条目。
ZEND_MODULE_SHUTDOWN_D(sign)
{
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}

//在每次PHP请求开始，请求前启动函数被调用，通常用于管理请求前逻辑。
ZEND_MODULE_ACTIVATE_D(sign)
{
	return SUCCESS;
}

ZEND_MODULE_DEACTIVATE_D(sign)
{
	return SUCCESS;
}

//模块输出信息
ZEND_MODULE_INFO_D(sign)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "sign support", "enabled");
	php_info_print_table_row(2, "sign version", SIGN_VERSION);
	php_info_print_table_end();
}

//模块外部接口
ZEND_FUNCTION(sign64)
{
	char		*str   = NULL;
	int			str_len;
	uint64_t	id = 0;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &str, &str_len) == FAILURE)
	{
		zend_error(E_ERROR, "parameters failed");
	}

	//RETURN_STRING("hellow", 1);

	id  = MurmurHash64A(str, str_len);
	if(id)
	{
		RETURN_LONG(id);
	}
	else
	{
		RETURN_FALSE;
	}
}
