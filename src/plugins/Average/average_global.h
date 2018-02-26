#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(AVERAGE_LIB)
#  define AVERAGE_EXPORT Q_DECL_EXPORT
# else
#  define AVERAGE_EXPORT Q_DECL_IMPORT
# endif
#else
# define AVERAGE_EXPORT
#endif
