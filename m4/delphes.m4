dnl ##### DELPHES #####
AC_DEFUN([CHECK_DELPHES],
[
  AC_MSG_CHECKING([for Delphes location])
  DELPHESINCLUDE=""
  DELPHESLIBS="-ldelphes"
  AC_ARG_WITH(delphes,     
  AC_HELP_STRING([--with-delphes=DIR],[Location of Delphes installation]),
    [],
    [with_delphes=no])
  if test "x$with_delphes" = "xno"; then
    AC_MSG_RESULT("not found") 
    AC_MSG_ERROR([Delphes could not be found. Try --with-delphes])
  else
    AC_MSG_RESULT([$with_delphes])
    DELPHESCFLAGS="-I$with_delphes -I$with_delphes/external"
    DELPHESLIBS="-lEG -lDelphes"
    DELPHESLIBDIR="$with_delphes"
  fi
  if test "x$with_pythia" != "xno"; then
    # TODO: Check if Delphes works properly
    CPPFLAGS="$CPPFLAGS $DELPHESCFLAGS $ROOTCFLAGS"
    LIBS="$LIBS -L$DELPHESLIBDIR -L$ROOTLIBDIR $ROOTGLIBS $ROOTLIBS $DELPHESLIBS"
    LDFLAGS="$LDFLAGS"             
    AC_CHECK_HEADERS([classes/DelphesClasses.h],[],[AC_MSG_ERROR([Cannot find DelphesClasses header])],
    [#include "TRef.h"
#include "TObject.h"
#include "TRefArray.h"
#include "TLorentzVector.h"
#include "classes/SortableObject.h"
])
  fi
  AM_CONDITIONAL(HAVE_DELPHES,[test "x$with_delphes" != "xno"])
  AC_SUBST(DELPHESCFLAGS)
  AC_SUBST(DELPHESINCLUDE)
  AC_SUBST(DELPHESLIBS)
  AC_SUBST(DELPHESLIBDIR)
])
