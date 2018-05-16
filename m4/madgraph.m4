dnl ##### MADGRAPH #####
AC_DEFUN([CHECK_MADGRAPH],
[
  AC_MSG_CHECKING([for MadGraph location])
  MADGRAPHDIR=""
  AC_ARG_WITH(madgraph,     
  AC_HELP_STRING([--with-madgraph=DIR],[Location of MadGraph installation]),
    [],
    [with_madgraph=no])
  if test "x$with_madgraph" = "xno"; then
     AC_MSG_RESULT("not found") 
  else
    AC_MSG_RESULT([$with_madgraph])
    MADGRAPHDIR="$with_madgraph"
  fi
  if test "x$with_madgraph" != "xno"; then
    # Check if MadGraph executable is present       
    AC_CHECK_FILE([$MADGRAPHDIR/bin/mg5_aMC],[],[AC_MSG_ERROR([Cannot find MadGraph executable])],
    [#include "TRef.h"
#include "TObject.h"
#include "TRefArray.h"
#include "TLorentzVector.h"
#include "classes/SortableObject.h"
])
  fi
  AM_CONDITIONAL(HAVE_MADGRAPH,[test "x$with_madgraph" != "xno"])
  AC_SUBST(MADGRAPHDIR)
])