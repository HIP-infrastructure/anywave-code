#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(REGISTERINJECT_LIB)
#  define REGISTERINJECT_EXPORT Q_DECL_EXPORT
# else
#  define REGISTERINJECT_EXPORT Q_DECL_IMPORT
# endif
#else
# define REGISTERINJECT_EXPORT
#endif
