dnl ##### HEPMC #####
AC_DEFUN([THEPEG_CHECK_HEPMC],
[
  AC_MSG_CHECKING([for HepMC location])
  HEPMCINCLUDE=""
  HEPMCLIBS=""
  AC_ARG_WITH(hepmc,
  AC_HELP_STRING([--with-hepmc=DIR],[Location of HepMC installation @<:@default=system libs@:>@]),
  [],
  [with_hepmc=no])
  if test "x$with_hepmc" = "xno"; then
    AC_MSG_RESULT([not found])
  else
    AC_MSG_RESULT([$with_hepmc])
    HEPMCINCLUDE=-I$with_hepmc/include
    HEPMCLIBS="-L$with_hepmc/lib -lHepMCfio -lHepMC"
    HEPMCLIBDIR="$with_hepmc/lib"
  fi
  if test "x$with_hepmc" != "xno"; then
    # Now lets see if the libraries work properly
    oldLIBS="$LIBS"
    oldLDFLAGS="$LDFLAGS"
    oldCPPFLAGS="$CPPFLAGS"
    LIBS="$LIBS `echo $HEPMCLIBS | sed -e 's! -R.* ! !'`"
    LDFLAGS="$LDFLAGS"
    CPPFLAGS="$CPPFLAGS $HEPMCINCLUDE"
    AC_CHECK_HEADERS([HepMC/HepMCDefs.h],[],[AC_MSG_ERROR([Defs...])])
    AC_CHECK_HEADERS([HepMC/IO_GenEvent.h],[],[AC_MSG_ERROR([Need HepMC with GenEvent support.])])
    # check HepMC
    #AC_MSG_CHECKING([that HepMC works])
    #AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <HepMC/GenEvent.h>
    #]],[[HepMC::GenEvent();]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no])
    #AC_MSG_ERROR([Use '--with-hepmc=' to set a path or use '--without-hepmc'.])
    #])
    #AC_CHECK_HEADERS([HepMC/PdfInfo.h],[],[AC_MSG_ERROR([Need HepMC with PdfInfo support.])],[
    #include <algorithm>
    #include <ostream>
    #include <istream>
    #])
    LIBS="$oldLIBS"
    LDFLAGS="$oldLDFLAGS"
    CPPFLAGS="$oldCPPFLAGS"
  fi
  AM_CONDITIONAL(HAVE_HEPMC,[test "x$with_hepmc" != "xno"])
  AC_SUBST(HEPMCINCLUDE)
  AC_SUBST(HEPMCLIBS)
  AC_SUBST(HEPMCLIBDIR)
  AC_SUBST(CREATE_HEPMC)
  AC_SUBST(LOAD_HEPMC)
])
