dnl ##### LHAPDF #####
AC_DEFUN([CHECK_LHAPDF],
[
  AC_MSG_CHECKING([for LHAPDF6 location])
  LHAPDFINCLUDE=""
  LHAPDFLIBS=""
  AC_ARG_WITH(lhapdf,     
  AC_HELP_STRING([--with-lhapdf=DIR],[Location of LHAPDF6 installation]),
    [],
    [with_lhapdf=no])
  if test "x$with_lhapdf" = "xno"; then
    AC_MSG_RESULT("not found") 
  else
    AC_MSG_RESULT([$with_lhapdf])
    LHAPDFINCLUDE=-I$with_lhapdf/include
    LHAPDFLIBS="-L$with_lhapdf/lib -lLHAPDF"
    LHAPDFLIBDIR="$with_lhapdf/lib"
  fi

  AM_CONDITIONAL(HAVE_LHAPDF,[test "x$with_lhapdf" != "xno"])
  AC_SUBST(LHAPDFINCLUDE)
  AC_SUBST(LHAPDFLIBS)
  AC_SUBST(LHAPDFLIBDIR)
])
