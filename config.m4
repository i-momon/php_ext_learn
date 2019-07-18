dnl $Id$
dnl config.m4 for extension firstest

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(firstest, for firstest support,
Make sure that the comment is aligned:
[  --with-firstest             Include firstest support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE 会自动设置好正确的变量以保证扩展能够被PHP_NEW_EXTENSION以共享模式启动
PHP_ARG_ENABLE(firstest, whether to enable firstest support,
[  --enable-firstest           Enable firstest support])

if test "$PHP_FIRSTEST" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-firstest -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/firstest.h"  # you most likely want to change this
  dnl if test -r $PHP_FIRSTEST/$SEARCH_FOR; then # path given as parameter
  dnl   FIRSTEST_DIR=$PHP_FIRSTEST
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for firstest files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       FIRSTEST_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$FIRSTEST_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the firstest distribution])
  dnl fi

  dnl # --with-firstest -> add include path
  dnl PHP_ADD_INCLUDE($FIRSTEST_DIR/include)

  dnl # --with-firstest -> check for lib and symbol presence
  dnl LIBNAME=firstest # you may want to change this
  dnl LIBSYMBOL=firstest # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $FIRSTEST_DIR/$PHP_LIBDIR, FIRSTEST_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_FIRSTESTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong firstest lib version or lib not found])
  dnl ],[
  dnl   -L$FIRSTEST_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(FIRSTEST_SHARED_LIBADD)
  
  dnl PHP_NEW_EXTENSION 的第一个参数是扩展的名称，第二个参数是资源文件，可以是多个资源文件以空格分开。第三个参数 $ext_shared 是由 PHP_ARG_ENABLE/WITH PHP_NEW_EXTENSION 设定的
  PHP_NEW_EXTENSION(firstest, firstest.c firstclass.c firstinterface.c firstobject.c array_example.c firstgolbal.c , $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
