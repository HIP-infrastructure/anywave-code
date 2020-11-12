#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MICROMED_LIB)
#  define MICROMED_EXPORT Q_DECL_EXPORT
# else
#  define MICROMED_EXPORT Q_DECL_IMPORT
# endif
#else
# define MICROMED_EXPORT
#endif
