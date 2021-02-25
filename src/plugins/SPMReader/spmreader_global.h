#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SPMREADER_LIB)
#  define SPMREADER_EXPORT Q_DECL_EXPORT
# else
#  define SPMREADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define SPMREADER_EXPORT
#endif
