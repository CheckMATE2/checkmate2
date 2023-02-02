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
# removed due to incompatibility with newer root versions (around 6.20); future check should have 'pyroot' instead
#ROOT_FEATURE(python, ,AC_MSG_ERROR(ROOT could be initialised but apparently it has been configured without Python functionality. Checkout the manual to find out how to add it.))
ROOT_FEATURE(minuit2, ,AC_MSG_ERROR(ROOT could be initialised but apparently it has been configured without Minuit2 functionality. Checkout the manual to find out how to add it.))
#ROOT_FEATURE(roofit, ,AC_MSG_ERROR(ROOT could be initialised but apparently it has been configured without RooFit functionality. Checkout the manual to find out how to add it.))
AC_SUBST(ROOTSYS)
AC_SUBST(LD_LIBRARY_PATH)
AC_SUBST(DYLD_LIBRARY_PATH)
AC_SUBST(LD_RUN_PATH)
CHECK_DELPHES([6], ,AC_MSG_ERROR("No Delphes... but why?") )
CHECK_MADGRAPH([6], ,AC_MSG_ERROR("No MadGraph... but why?") )
CHECK_ONNX([6], ,AC_MSG_ERROR("No ONNX... but why?") )

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

#AC_CONFIG_SUBDIRS([tools/delphes/Delphes-3.1.10X])
                   
AC_CONFIG_FILES([Makefile
                 tools/analysis/Makefile
                 tools/fritz/Makefile])
AC_OUTPUT

AC_MSG_RESULT([])
AC_MSG_RESULT([=== Downloading external resources ===])
# BDT weights for atlas_2010_14293
if ! test -f "$srcdir/data/atlas_2010_14293/ZeroLepton2018-SRBDT-weight.tar.gz"
  then
  if ! test -d "$srcdir/data/atlas_2010_14293"
  then 
    mkdir "$srcdir/data/atlas_2010_14293"
  fi  
  `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2010_14293/ZeroLepton2018-SRBDT-weight.tar.gz" "https://doi.org/10.17182/hepdata.95664.v2/r8"`
  `tar -xvzf "$srcdir/data/atlas_2010_14293/ZeroLepton2018-SRBDT-weight.tar.gz" -C "$srcdir/data/atlas_2010_14293"`
fi  
# Likelihoods for atlas_2010_14293
if ! test -f "$srcdir/data/atlas_2010_14293/Likelihoods.tar.gz"
  then
  if ! test -d "$srcdir/data/atlas_2010_14293"
  then 
    mkdir "$srcdir/data/atlas_2010_14293"
  fi  
  `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2010_14293/Likelihoods.tar.gz" "https://doi.org/10.17182/hepdata.95664.v2/r9"`
  `tar -xvzf "$srcdir/data/atlas_2010_14293/Likelihoods.tar.gz" -C "$srcdir/data/atlas_2010_14293"`
fi
# Likelihoods for atlas_2211_08028
# `curl -OJLH "Accept: application/x-tar" https://doi.org/10.17182/hepdata.95928.v1/r5`
# ONNX for atlas_2106_09609
if ! test -f "$srcdir/data/atlas_2106_09609/SUSY-2019-04-ONNX.tgz"
  then
  if ! test -d "$srcdir/data/atlas_2106_09609/"
  then
    mkdir "$srcdir/data/atlas_2106_09609/"
  fi
  `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_2106_09609/SUSY-2019-04-ONNX.tgz" "https://doi.org/10.17182/hepdata.104860.v1/r3"`
  `tar -zxvf "$srcdir/data/atlas_2106_09609/SUSY-2019-04-ONNX.tgz" --strip-components=1 -C "$srcdir/data/atlas_2106_09609/"`
fi  
# Likelihoods for atlas_1908_03122
if ! test -f "$srcdir/data/atlas_1908_03122/HEPData_workspaces.tar.gz"
  then
  `curl -JLH "Accept: application/x-tar" -o "$srcdir/data/atlas_1908_03122/HEPData_workspaces.tar.gz" "https://doi.org/10.17182/hepdata.89408.v3/r2"`
  if ! test -d "$srcdir/data/atlas_1908_03122/Likelihoods/"
  then
    mkdir "$srcdir/data/atlas_1908_03122/Likelihoods"
  fi  
  `tar -xvzf "$srcdir/data/atlas_1908_03122/HEPData_workspaces.tar.gz" -C "$srcdir/data/atlas_1908_03122/Likelihoods/"`
fi  
AC_MSG_RESULT([])

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
