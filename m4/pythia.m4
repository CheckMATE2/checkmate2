dnl ##### PYTHIA #####
AC_DEFUN([CHECK_PYTHIA],
[
  AC_MSG_CHECKING([for Pythia8 location])
  PYTHIAINCLUDE=""
  PYTHIALIBS="-lpythia8"
  AC_ARG_WITH(pythia,     
  AC_HELP_STRING([--with-pythia=DIR],[Location of Pythia8 installation]),
    [],
    [with_pythia=no])
  if test "x$with_pythia" = "xno"; then
    AC_MSG_RESULT("not found") 
  else
    AC_MSG_RESULT([$with_pythia])
    PYTHIAINCLUDE=-I$with_pythia/include
    PYTHIALIBS="-L$with_pythia/lib -lpythia8 -ldl -lm -lz"
    PYTHIALIBDIR="$with_pythia/lib"
    PYTHIAXMLDIR="\"$with_pythia/xmldir\""
#    PYTHIAGZIPLIB="-DGZIPSUPPORT -I/usr/include/ -L/lib/x86_64-linux-gnu -Wl,-rpath,/lib/x86_64-linux-gnu -lz"
  fi
  AC_MSG_CHECKING([for gzip include (optional)])
  AC_ARG_WITH(gzipinc,     
  AC_HELP_STRING([--with-gzipinc=DIR],[Location of gzip include]),
    [],
    [with_gzipinc=no])
  if test "x$with_gzipinc" = "xno"; then
    AC_MSG_RESULT(not found) 
  else
    AC_MSG_RESULT([$with_gzipinc])
    GZIPINC="-DGZIPSUPPORT -I$with_gzipinc"
  fi 
  AC_MSG_CHECKING([for gzip library (optional)]) 
  AC_ARG_WITH(gziplib,     
  AC_HELP_STRING([--with-gziplib=DIR],[Location of gzip library]),
    [],
    [with_gziplib=no])
  if test "x$with_gziplib" = "xno"; then
    AC_MSG_RESULT(not found) 
  else
    AC_MSG_RESULT([$with_gziplib])
    GZIPLIB="-L$with_gziplib -Wl,-rpath,$with_gziplib -lz"
  fi 
  if test "x$with_pythia" != "xno"; then
  # Now lets see if the libraries work properly
    oldLIBS="$LIBS"
    oldLDFLAGS="$LDFLAGS"
    oldCPPFLAGS="$CPPFLAGS"
    LIBS="$LIBS $HEPMCLIBS `echo $PYTHIALIBS | sed -e 's! -R.* ! !'`"
    LDFLAGS="$LDFLAGS"
    CPPFLAGS="$CPPFLAGS $PYTHIAINCLUDE $HEPMCINCLUDE"
    AC_MSG_RESULT([$LDFLAGS])
    # check HepMC
    #AC_MSG_CHECKING([that HepMC works])
    #AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <HepMC/GenEvent.h>
    #]],[[HepMC::GenEvent();]])],[AC_MSG_RESULT([yes])],[AC_MSG_RESULT([no])
    #AC_MSG_ERROR([Use '--with-hepmc=' to set a path or use '--without-hepmc'.])
    #])               
    AC_CHECK_HEADERS([Pythia8/Pythia.h],[],[AC_MSG_ERROR([Cannot find Pythia header])])
    if test "x$with_hepmc" != "xno"; then
       AC_CHECK_HEADERS([Pythia8Plugins/HepMC2.h],[],[AC_MSG_ERROR([Cannot find Pythia8ToHepMC header])])
    fi
  fi
  AM_CONDITIONAL(HAVE_PYTHIA,[test "x$with_pythia" != "xno"])
  AM_CONDITIONAL(HAVE_GZIPINC,[test "x$with_gzipinc" != "xno"])
  AM_CONDITIONAL(HAVE_GZIPLIB,[test "x$with_gziplib" != "xno"])
  AC_SUBST(PYTHIAINCLUDE)
  AC_SUBST(PYTHIALIBS)
  AC_SUBST(PYTHIALIBDIR)
  AC_SUBST(GZIPINC)
  AC_SUBST(GZIPLIB)
  # AC_DEFINE_UNQUOTED(PYTHIAXMLDIR, $PYTHIAXMLDIR) Can maybe be useful later
])
