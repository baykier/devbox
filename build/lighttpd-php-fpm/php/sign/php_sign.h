/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Author: zhaoshunyao@baidu.com                                        |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_SIGN_H
#define PHP_SIGN_H

#ifdef PHP_WIN32
#define SIGN_API __declspec(dllexport)
#else
#define SIGN_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

extern zend_module_entry sign_module_entry;
#define phpext_sign_ptr &sign_module_entry

ZEND_MODULE_STARTUP_D(sign);
ZEND_MODULE_SHUTDOWN_D(sign);
ZEND_MODULE_ACTIVATE_D(sign);
ZEND_MODULE_DEACTIVATE_D(sign);
ZEND_MODULE_INFO_D(sign);

ZEND_FUNCTION(sign64);

#endif	/* PHP_SIGN_H */
