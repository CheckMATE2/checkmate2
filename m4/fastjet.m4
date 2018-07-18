dnl ##### FASTJET #####
AC_DEFUN([CHECK_FASTJET],
[
  AC_MSG_CHECKING([for FASTJET location])
  FASTJETINCLUDE=""
  FASTJETLIBS=""
  AC_ARG_WITH(fastjet,     
  AC_HELP_STRING([--with-fastjet=DIR],[Location of FASTJET installation]),
    [],
    [with_fastjet=no])
  if test "x$with_fastjet" = "xno"; then
    AC_MSG_RESULT("not found") 
  else
    AC_MSG_RESULT([$with_fastjet])
    FASTJETINCLUDE=-I$with_fastjet/include
    FASTJETLIBS="-L$with_fastjet/lib -lfastjet"
    FASTJETLIBDIR="-Wl,-rpath,$with_fastjet/lib -lm -L$with_fastjet/lib -lfastjettools -lfastjet"
  fi

  AM_CONDITIONAL(HAVE_FASTJET,[test "x$with_fastjet" != "xno"])
  AC_SUBST(FASTJETINCLUDE)
  AC_SUBST(FASTJETLIBS)
  AC_SUBST(FASTJETLIBDIR)
])
