/*
  +----------------------------------------------------------------------+
  | Intercept Extension                                                  |
  +----------------------------------------------------------------------+
  | Copyright (c) 2005 Gabriel Ricard                                    |
  +----------------------------------------------------------------------+
  | This source file is subject to version 2.02 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available at through the world-wide-web at                           |
  | http://www.php.net/license/2_02.txt.                                 |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Gabriel Ricard <gabe@php.net>                                |
  +----------------------------------------------------------------------+

  $Id: php_intercept.h,v 1.2 2005/02/27 01:57:59 gabe Exp $ 
*/

#ifndef PHP_INTERCEPT_H
#define PHP_INTERCEPT_H

extern zend_module_entry intercept_module_entry;
#define phpext_intercept_ptr &intercept_module_entry

#ifdef PHP_WIN32
#define PHP_INTERCEPT_API __declspec(dllexport)
#else
#define PHP_INTERCEPT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(intercept);
PHP_MSHUTDOWN_FUNCTION(intercept);
PHP_RINIT_FUNCTION(intercept);
PHP_RSHUTDOWN_FUNCTION(intercept);
PHP_MINFO_FUNCTION(intercept);

PHP_FUNCTION(intercept_add);

ZEND_BEGIN_MODULE_GLOBALS(intercept)
	zval *pre_intercept_handlers;
	zval *post_intercept_handlers;
ZEND_END_MODULE_GLOBALS(intercept)

#ifdef ZTS
#define IntG(v) TSRMG(intercept_globals_id, zend_intercept_globals *, v)
#else
#define IntG(v) (intercept_globals.v)
#endif

#define PRE_INTERCEPT 1
#define POST_INTERCEPT 2

#endif	/* PHP_INTERCEPT_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: t
 * End:
 */
