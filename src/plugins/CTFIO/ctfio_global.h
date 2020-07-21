#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CTFIO_LIB)
#  define CTFIO_EXPORT Q_DECL_EXPORT
# else
#  define CTFIO_EXPORT Q_DECL_IMPORT
# endif
#else
# define CTFIO_EXPORT
#endif
