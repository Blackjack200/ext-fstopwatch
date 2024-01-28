/* fstopwatch extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <pthread.h>
#include "php.h"
#include "ext/standard/info.h"
#include "php_fstopwatch.h"
#include "fstopwatch_arginfo.h"

ZEND_BEGIN_MODULE_GLOBALS(fstopwatch)
    zend_long call_depth;
ZEND_END_MODULE_GLOBALS(fstopwatch)

ZEND_DECLARE_MODULE_GLOBALS(fstopwatch)

#define FSTOPWATCH_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(fstopwatch,v)

typedef void(*zend_execute_ex_function)(zend_execute_data *);

static zend_execute_ex_function zend_execute_ex_hook = NULL;

#define BILLION  1000000000L

static inline void print(zend_function *fbc) {
    if (fbc->common.scope) {
        printf("[->] THREAD=%s DEPTH=%lld SCOPE=%s::%s()\n",
               pthread_main_np() != 0 ? "MAIN" : "OTHER",
               FSTOPWATCH_G(call_depth),
               ZSTR_VAL(fbc->common.scope->name),
               ZSTR_VAL(fbc->common.function_name)
        );
    } else {
        printf("[->] THREAD=%s DEPTH=%lld SCOPE=%s()\n",
               pthread_main_np() != 0 ? "MAIN" : "OTHER",
               FSTOPWATCH_G(call_depth),
               ZSTR_VAL(fbc->common.function_name)
        );
    }
}

static inline void print_time(zend_function *fbc, double time) {
    if (fbc->common.scope) {
        printf("[->] THREAD=%s DEPTH=%lld SCOPE=%s::%s() TIME=%d\n",
               pthread_main_np() != 0 ? "MAIN" : "OTHER",
               FSTOPWATCH_G(call_depth),
               ZSTR_VAL(fbc->common.scope->name),
               ZSTR_VAL(fbc->common.function_name),
               time
        );
    } else {
        printf("[->] THREAD=%s DEPTH=%lld SCOPE=%s() TIME=%d\n",
               pthread_main_np() != 0 ? "MAIN" : "OTHER",
               FSTOPWATCH_G(call_depth),
               ZSTR_VAL(fbc->common.function_name),
               time
        );
    }
}

static void guard_execute_ex(zend_execute_data *execute_data) {
    struct timespec start, stop;
    double accum;
    zend_function *f;

    if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
        zend_throw_error(NULL, "clock_gettime errored.");
        return;
    }

    FSTOPWATCH_G(call_depth)++;

    f = execute_data->func;
    if (f != NULL && f->common.function_name != NULL) {
        print(f);
    }

    zend_execute_ex_hook(execute_data);
    FSTOPWATCH_G(call_depth)--;

    if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
        zend_throw_error(NULL, "clock_gettime errored.");
        return;
    }

    accum = ((double) stop.tv_sec - (double) start.tv_sec)
            + (double) (stop.tv_nsec - start.tv_nsec)
              / (double) BILLION;

    f = execute_data->func;
    if (f != NULL && f->common.function_name != NULL) {
        print_time(f, accum);
    }
}

PHP_MINIT_FUNCTION (fstopwatch) {
    zend_execute_ex_hook = zend_execute_ex;
    zend_execute_ex = guard_execute_ex;
    return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION (fstopwatch) {
#if defined(ZTS) && defined(COMPILE_DL_FSTOPWATCH)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    FSTOPWATCH_G(call_depth)++;
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION (fstopwatch) {
    php_info_print_table_start();
    php_info_print_table_header(2, "fstopwatch support", "enabled");
    php_info_print_table_end();
}
/* }}} */

/* {{{ fstopwatch_module_entry */
zend_module_entry fstopwatch_module_entry = {
        STANDARD_MODULE_HEADER,
        "fstopwatch",                    /* Extension name */
        NULL,                    /* zend_function_entry */
        PHP_MINIT(fstopwatch),                            /* PHP_MINIT - Module initialization */
        NULL,                            /* PHP_MSHUTDOWN - Module shutdown */
        PHP_RINIT(fstopwatch),            /* PHP_RINIT - Request initialization */
        NULL,                            /* PHP_RSHUTDOWN - Request shutdown */
        PHP_MINFO(fstopwatch),            /* PHP_MINFO - Module info */
        PHP_FSTOPWATCH_VERSION,        /* Version */
        STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_FSTOPWATCH
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(fstopwatch)
#endif
