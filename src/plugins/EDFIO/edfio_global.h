#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EDFIO_LIB)
#  define EDFIO_EXPORT Q_DECL_EXPORT
# else
#  define EDFIO_EXPORT Q_DECL_IMPORT
# endif
#else
# define EDFIO_EXPORT
#endif
