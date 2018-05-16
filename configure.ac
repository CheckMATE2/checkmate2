#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(CheckMATE, 2.0.0, daschm@uni-bonn.de)
AM_INIT_AUTOMAKE
# Checks for programs.
AC_PROG_CXX
#AC_PROG_RANLIB
#AC_ENABLE_SHARED
#AC_DISABLE_STATIC
LT_INIT()

AC_LANG([C++])
AC_HEADER_STDC
AC_CHECK_HEADERS([iostream])
AC_CHECK_HEADERS([ostream])
AC_CHECK_HEADERS([fstream])

AX_CXX_COMPILE_STDCXX_11()

# Checks for libraries.
THEPEG_CHECK_HEPMC([2], ,AC_MSG_ERROR("No HepMC... but why?") )
CHECK_PYTHIA([6], ,AC_MSG_ERROR("No Pythia... but why?") )
# first command allows for --with-python but doesn't check the version. This is why we have two checks.
PY_PROG_PYTHON([2.7.3], ,AC_MSG_ERROR(You need Python 2.7.3+ to run CheckMATE. Use --with-python and refer to a valid python binary.) )
AM_PATH_PYTHON([2.7.3], ,AC_MSG_ERROR(You need Python 2.7.3+ to run CheckMATE. Use --with-python and refer to a valid python binary.) )
ROOT_PATH([5.20], , AC_MSG_ERROR(ROOT could not be initialised. Try using --with-rootsys and check whether you have ver. 5.34/06 or newer.))
ROOT_FEATURE(python, ,AC_MSG_ERROR(ROOT could be initialised but apparently it has been configured without Python functionality. Checkout the manual to find out how to add it.))
ROOT_FEATURE(minuit2, ,AC_MSG_ERROR(ROOT could be initialised but apparently it has been configured without Minuit2 functionality. Checkout the manual to find out how to add it.))
#ROOT_FEATURE(roofit, ,AC_MSG_ERROR(ROOT could be initialised but apparently it has been configured without RooFit functionality. Checkout the manual to find out how to add it.))
AC_SUBST(ROOTSYS)
AC_SUBST(LD_LIBRARY_PATH)
AC_SUBST(DYLD_LIBRARY_PATH)
AC_SUBST(LD_RUN_PATH)
CHECK_DELPHES([6], ,AC_MSG_ERROR("No Delphes... but why?") )
CHECK_MADGRAPH([6], ,AC_MSG_ERROR("No MadGraph... but why?") )

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

#AC_CONFIG_SUBDIRS([tools/delphes/Delphes-3.1.10X])
                   
AC_CONFIG_FILES([Makefile
                 tools/analysis/Makefile
                 tools/fritz/Makefile])
AC_OUTPUT

AC_MSG_RESULT([])
AC_MSG_RESULT([=== Subconfigurations Done ===])
AC_MSG_RESULT([Configuration of CheckMATE finished. Run 'make' to compile. If you have more than one core, you can use -j2 or even -j4 to speed up the compilation process.])
AC_MSG_RESULT([])

if test "x$with_madgraph" = "xno"; then
  AC_MSG_RESULT([])
  AC_MSG_RESULT([=== IMPORANT! ===])
  AC_MSG_RESULT([You did not provide any link to MG5_aMC@NLO using the --with-madgraph parameter. If you continue you will not be able to])
  AC_MSG_RESULT([   - Simulate parton events using MG5_aMC@NLO])
fi

if test "x$with_pythia" = "xno"; then
  AC_MSG_RESULT([])
  AC_MSG_RESULT([=== IMPORANT! ===])
  AC_MSG_RESULT([You did not provide any link to Pythia8 using the --with-pythia parameter. If you continue you will not be able to])
  AC_MSG_RESULT([   - Simulate events using Pythia])
  AC_MSG_RESULT([   - Shower .lhe events using Pythia])
  AC_MSG_RESULT([   - Simulate parton events using MG5_aMC@NLO (even if you provide --with-madgraph!)])
fi

if test "x$with_hepmc" = "xno"; then
  AC_MSG_RESULT([])
  AC_MSG_RESULT([=== IMPORANT! ===])
  AC_MSG_RESULT([You did not provide any link to HepMC using the --with-hepmc parameter. If you continue you will not be able to])
  AC_MSG_RESULT([   - Create .hepmc output events using Pythia])
fi
