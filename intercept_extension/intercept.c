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

  $Id: intercept.c,v 1.5 2005/05/28 21:58:24 gabe Exp $ 
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_intercept.h"

ZEND_DLEXPORT void intercept_execute_internal(zend_execute_data *execute_data_ptr, int return_value_used TSRMLS_DC);
ZEND_DLEXPORT void (*intercept_old_zend_execute_internal)(zend_execute_data *execute_data_ptr, int return_value_used TSRMLS_DC);

ZEND_DLEXPORT void intercept_execute(zend_op_array *op_array TSRMLS_DC);
ZEND_DLEXPORT void (*intercept_old_execute)(zend_op_array *op_array TSRMLS_DC);

ZEND_DECLARE_MODULE_GLOBALS(intercept)

/* True global resources - no need for thread safety here */
static int le_intercept;

/* {{{ intercept_functions[]
 *
 * Every user visible function must have an entry in intercept_functions[].
 */
function_entry intercept_functions[] = {
	PHP_FE(intercept_add,	NULL)
	{NULL, NULL, NULL}
};
/* }}} */

/* {{{ intercept_module_entry
 */
zend_module_entry intercept_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"intercept",
	intercept_functions,
	PHP_MINIT(intercept),
	PHP_MSHUTDOWN(intercept),
	PHP_RINIT(intercept),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(intercept),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(intercept),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1",
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_INTERCEPT
ZEND_GET_MODULE(intercept)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
	STD_PHP_INI_ENTRY("intercept.global_value",	  "42", PHP_INI_ALL, OnUpdateInt, global_value, zend_intercept_globals, intercept_globals)
	STD_PHP_INI_ENTRY("intercept.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_intercept_globals, intercept_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_intercept_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_intercept_init_globals(zend_intercept_globals *intercept_globals)
{
	intercept_globals->global_value = 0;
	intercept_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(intercept)
{
	/* If you have INI entries, uncomment these lines 
	ZEND_INIT_MODULE_GLOBALS(intercept, php_intercept_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	*/

	REGISTER_LONG_CONSTANT("PRE_INTERCEPT", PRE_INTERCEPT, CONST_CS | CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("POST_INTERCEPT", POST_INTERCEPT, CONST_CS | CONST_PERSISTENT);

	intercept_old_execute = zend_execute;
	zend_execute = intercept_execute;

	intercept_old_zend_execute_internal = zend_execute_internal;
	zend_execute_internal = intercept_execute_internal;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(intercept)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/

	zend_execute = intercept_old_execute;
	zend_execute_internal = intercept_old_zend_execute_internal;

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(intercept)
{
	MAKE_STD_ZVAL(IntG(pre_intercept_handlers));
	array_init(IntG(pre_intercept_handlers));
	MAKE_STD_ZVAL(IntG(post_intercept_handlers));
	array_init(IntG(post_intercept_handlers));

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(intercept)
{
	zval_ptr_dtor(&IntG(pre_intercept_handlers));
	zval_ptr_dtor(&IntG(post_intercept_handlers));
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(intercept)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "intercept support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* {{{ proto bool intercept_add(string target, string handler, long flags)
   Add an intercept handler */
PHP_FUNCTION(intercept_add)
{
	zval *handler, *handler_copy;
	zval *target, *target_copy;
	char *handler_name, *target_name;
	long flags;
	zval *intercept_handlers;
	zend_function *func;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "zz|l", &target, &handler, &flags ) == FAILURE) {
		return;
	}

	// an array means a user passed in an object/method pair
	if (Z_TYPE_P(target) != IS_ARRAY) {
		convert_to_string(target);
	} else {
		// perhaps increment refcount if array element 1 is an object
		// instead of a class name

		// maybe do this after the following validity check
	}

	if (!zend_is_callable(target, 0, &target_name)) {
		if (strstr(target_name, "->") == NULL) {
			php_error_docref1(NULL TSRMLS_CC, target_name, E_WARNING, "%s is not a valid function", target_name);
			efree(target_name);
			RETURN_FALSE;
		}
	}

	if (flags & POST_INTERCEPT) {
		intercept_handlers = IntG(post_intercept_handlers);
	} else {
		intercept_handlers = IntG(pre_intercept_handlers);
	}

	if( zend_hash_exists(Z_ARRVAL_P(intercept_handlers), target_name, strlen(target_name) + 1) ) {
		php_error_docref1(NULL TSRMLS_CC, target_name, E_WARNING,
						  "%s already has a registered intercept", target_name);
		efree(target_name);
		RETURN_FALSE;
	}

   	convert_to_string(handler);
	if (!zend_is_callable(handler, 0, &handler_name)) {
		php_error_docref1(NULL TSRMLS_CC, handler_name, E_WARNING,
						  "%s is not a valid handler function", handler_name);
		efree(handler_name);
		efree(target_name);
		RETURN_FALSE;
	} else {
		if( zend_hash_find(EG(function_table), handler_name, strlen(handler_name) + 1, (void **) &func) != FAILURE ) {
			if (func->type != ZEND_USER_FUNCTION) {
				php_error_docref1(NULL TSRMLS_CC, handler_name, E_WARNING,
								  "%s is not a user-defined handler function", handler_name);
				efree(handler_name);
				efree(target_name);
				RETURN_FALSE;
			}
		}
	}
	add_assoc_string(intercept_handlers, target_name, handler_name, 1);
	efree(handler_name);
	efree(target_name);

	RETURN_TRUE;
}
/* }}} */


/* borrowed from APD - apd_get_active_function_name() */
char *intercept_get_active_function_name(zend_op_array *op_array TSRMLS_DC)
{
	char *funcname = NULL;
	int curSize = 0;
	zend_execute_data *execd = NULL;
	char *tmpfname;
	char *classname;
	int classnameLen;
	int tmpfnameLen;
  
	execd = EG(current_execute_data);
	if(execd) {
		tmpfname = execd->function_state.function->common.function_name;
		if(tmpfname) {
			tmpfnameLen = strlen(tmpfname);
			if(execd->object) {
				classname = Z_OBJCE(*execd->object)->name;
				classnameLen = strlen(classname);
				funcname = (char *)emalloc(classnameLen + tmpfnameLen + 3);
				snprintf(funcname, classnameLen + tmpfnameLen + 3, "%s->%s",
						 classname, tmpfname);
			}
			else if(execd->function_state.function->common.scope) {
				classname = execd->function_state.function->common.scope->name;
				classnameLen = strlen(classname);
				funcname = (char *)emalloc(classnameLen + tmpfnameLen + 3);
				snprintf(funcname, classnameLen + tmpfnameLen + 3, "%s::%s",
						 classname, tmpfname);
			}
			else {
				funcname = estrdup(tmpfname);
			}
		} 
		else {
			switch (execd->opline->op2.u.constant.value.lval) {
			case ZEND_EVAL:
				funcname = estrdup("eval");
				break;
			case ZEND_INCLUDE:
				funcname = estrdup("include");
				break;
			case ZEND_REQUIRE:
				funcname = estrdup("require");
				break;
			case ZEND_INCLUDE_ONCE:
				funcname = estrdup("include_once");
				break;
			case ZEND_REQUIRE_ONCE:
				funcname = estrdup("require_once");
				break;
			default:
				funcname = estrdup("???");
				break;
			}
		}
	} 
	else {
		funcname = estrdup("main");
	}
	return funcname;
}

#if sadfasdf
{
	char *funcname = NULL;
	int curSize = 0;
	zend_execute_data *execd;
	char *tmpfname;
	int tmpfnameLen;
	char *classname;
	int classnameLen;

	execd = EG(current_execute_data);

	if(execd) {
		tmpfname = execd->function_state.function->common.function_name;
		if(tmpfname) {
			tmpfnameLen = strlen(tmpfname);
			if(execd->ce) {
				classname = execd->ce->name;
				classnameLen = strlen(classname);
				funcname = (char *)emalloc(classnameLen + tmpfnameLen + 3);
				snprintf(funcname, classnameLen + tmpfnameLen + 3, "%s->%s",
						 classname, tmpfname);
			}
			else if(execd->object.ptr) {
				classname = estrdup(execd->object.ptr->value.obj.ce->name);
				classnameLen = strlen(classname);
				funcname = (char *)emalloc(classnameLen + tmpfnameLen + 3);
				snprintf(funcname, classnameLen + tmpfnameLen + 3, "%s::%s",
						 classname, tmpfname);
			}
			else {
				funcname = estrdup(tmpfname);
			}
		}
		else {
			switch (execd->opline->op2.u.constant.value.lval) {
			case ZEND_EVAL:
				funcname = estrdup("eval");
				break;
			case ZEND_INCLUDE:
				funcname = estrdup("include");
				break;
			case ZEND_REQUIRE:
				funcname = estrdup("require");
				break;
			case ZEND_INCLUDE_ONCE:
				funcname = estrdup("include_once");
				break;
			case ZEND_REQUIRE_ONCE:
				funcname = estrdup("require_once");
				break;
			default:
				funcname = estrdup("???");
				break;
			}
		}
	}
	else {
		funcname = estrdup("main");
	}
	return funcname;
}
#endif

void prepareArgs(zval **args, zend_op_array *op_array TSRMLS_DC) {
	//Prepare arguments that will be passed to interceptor
	//Current object
	MAKE_STD_ZVAL(args[0]);
	args[0] = EG(current_execute_data)->object;
	//Called method arguments
	int arg_count = op_array->num_args;
	zval *method_args_ptr;
	ALLOC_ZVAL(method_args_ptr);
	INIT_PZVAL(method_args_ptr);
	array_init_size(method_args_ptr, arg_count);
	if (zend_copy_parameters_array(arg_count, method_args_ptr TSRMLS_CC) == FAILURE) {
		zval_dtor(method_args_ptr);
	}
	args[1] = method_args_ptr;
}

ZEND_API void intercept_execute(zend_op_array *op_array TSRMLS_DC)
{
	char *fname = NULL;
	int cb_res = NULL;
	zval *retval = NULL;
	zval **func_name;
	zval *args[2];
	
	//Get name of called methid
	fname = intercept_get_active_function_name(op_array TSRMLS_CC);
	//Exec pre-interceptors
	if( zend_hash_find(Z_ARRVAL_P(IntG(pre_intercept_handlers)), fname, strlen(fname) + 1, (void **) &func_name) != FAILURE ) {
		MAKE_STD_ZVAL(retval);
		//Prepare arguments that will be passed to interceptor
		prepareArgs(args, op_array);
		//Pass two arguments to interceptor: current object and array with method params
		cb_res = call_user_function(EG(function_table),	NULL, *func_name, retval, 2, args TSRMLS_CC);
		FREE_ZVAL(retval);
	}
	//Exec method
	intercept_old_execute(op_array TSRMLS_CC);
	//Exec post interceptors
	if( zend_hash_find(Z_ARRVAL_P(IntG(post_intercept_handlers)), fname, strlen(fname) + 1, (void **) &func_name) != FAILURE ) {
		MAKE_STD_ZVAL(retval);
		//Prepare arguments that will be passed to interceptor
		prepareArgs(args, op_array);
		//Pass two arguments to interceptor: current object and array with method params
		cb_res = call_user_function(EG(function_table),	NULL, *func_name, retval, 2, args TSRMLS_CC);
		FREE_ZVAL(retval);
	}
	
	efree(fname);
}

ZEND_API void intercept_execute_internal(zend_execute_data *execute_data_ptr, int return_value_used TSRMLS_DC)
{
	char *fname = NULL;
	int cb_res = NULL;
	zval *retval = NULL;
	zval *args[1];
	zval **func_name;
	zend_execute_data *execd;

	execd = EG(current_execute_data);
	fname = intercept_get_active_function_name(execd->op_array TSRMLS_CC);

	if( zend_hash_find(Z_ARRVAL_P(IntG(pre_intercept_handlers)), fname, strlen(fname) + 1, (void **) &func_name) != FAILURE ) {
		MAKE_STD_ZVAL(args[0]);
		MAKE_STD_ZVAL(retval);
		cb_res = call_user_function(EG(function_table),
									NULL,
									*func_name,
									retval, 0, args TSRMLS_CC);

		zval_dtor(retval);
		zval_dtor(args[0]);
		efree(retval);
		efree(args[0]);
	}


	if (!intercept_old_zend_execute_internal) {
		execute_internal(execute_data_ptr, return_value_used TSRMLS_CC);
	} else {
		intercept_old_zend_execute_internal(execute_data_ptr, return_value_used TSRMLS_CC);
	}

	if( zend_hash_find(Z_ARRVAL_P(IntG(post_intercept_handlers)), fname, strlen(fname) + 1, (void **) &func_name) != FAILURE ) {
		MAKE_STD_ZVAL(args[0]);
		MAKE_STD_ZVAL(retval);
		cb_res = call_user_function(EG(function_table),
									NULL,
									*func_name,
									retval, 0, args TSRMLS_CC);

		zval_dtor(retval);
		zval_dtor(args[0]);
		efree(retval);
		efree(args[0]);
	}

	efree(fname);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
