#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(FIFFIO_LIB)
#  define FIFFIO_EXPORT Q_DECL_EXPORT
# else
#  define FIFFIO_EXPORT Q_DECL_IMPORT
# endif
#else
# define FIFFIO_EXPORT
#endif
