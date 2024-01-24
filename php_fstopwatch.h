/* fstopwatch extension for PHP */

#ifndef PHP_FSTOPWATCH_H
# define PHP_FSTOPWATCH_H

extern zend_module_entry fstopwatch_module_entry;
# define phpext_fstopwatch_ptr &fstopwatch_module_entry

# define PHP_FSTOPWATCH_VERSION "0.1.0"

# if defined(ZTS) && defined(COMPILE_DL_FSTOPWATCH)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_FSTOPWATCH_H */
