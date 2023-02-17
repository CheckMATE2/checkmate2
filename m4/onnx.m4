dnl ##### ONNXRuntime #####
AC_DEFUN([CHECK_ONNX],
[
  AC_MSG_CHECKING([for ONNX location])
  ONNXINCLUDE=""
  ONNXLIBDIR=""
  ONNXLIBS=""
  AC_ARG_WITH(onnx,     
  AC_HELP_STRING([--with-onnx=DIR],[Location of ONNXRuntime installation]),
    [],
    [with_onnx=no])
  if test "x$with_onnx" = "xno"; then
    AC_MSG_RESULT(not found) 
  else
    AC_MSG_RESULT([$with_onnx])
    ONNXINCLUDE="-I$with_onnx/include"
    ONNXLIBDIR="$with_onnx/lib"
    ONNXLIBS="-L$with_onnx/lib -lonnxruntime"
  fi
  AM_CONDITIONAL(HAVE_ONNX,[test "x$with_onnx" != "xno"])
  AC_SUBST(ONNXINCLUDE)
  AC_SUBST(ONNXLIBDIR)
  AC_SUBST(ONNXLIBS)
])
