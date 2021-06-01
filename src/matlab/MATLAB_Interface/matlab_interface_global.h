#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MATLAB_INTERFACE_LIB)
#  define MATLAB_INTERFACE_EXPORT Q_DECL_EXPORT
# else
#  define MATLAB_INTERFACE_EXPORT Q_DECL_IMPORT
# endif
#else
# define MATLAB_INTERFACE_EXPORT
#endif
