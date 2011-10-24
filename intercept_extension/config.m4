dnl $Id: config.m4,v 1.1.1.1 2005/02/25 16:41:05 gabe Exp $
PHP_ARG_ENABLE(intercept, whether to enable intercept support,
   [  --enable-intercept           Enable intercept support])

if test "$PHP_INTERCEPT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-intercept -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/intercept.h"  # you most likely want to change this
  dnl if test -r $PHP_INTERCEPT/; then # path given as parameter
  dnl   INTERCEPT_DIR=$PHP_INTERCEPT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for intercept files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       INTERCEPT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$INTERCEPT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the intercept distribution])
  dnl fi

  dnl # --with-intercept -> add include path
  dnl PHP_ADD_INCLUDE($INTERCEPT_DIR/include)

  dnl # --with-intercept -> check for lib and symbol presence
  dnl LIBNAME=intercept # you may want to change this
  dnl LIBSYMBOL=intercept # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $INTERCEPT_DIR/lib, INTERCEPT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_INTERCEPTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong intercept lib version or lib not found])
  dnl ],[
  dnl   -L$INTERCEPT_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(INTERCEPT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(intercept, intercept.c, $ext_shared)
fi
