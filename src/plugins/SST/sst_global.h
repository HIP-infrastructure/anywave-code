#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SST_LIB)
#  define SST_EXPORT Q_DECL_EXPORT
# else
#  define SST_EXPORT Q_DECL_IMPORT
# endif
#else
# define SST_EXPORT
#endif
