#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ZH0_LIB)
#  define ZH0_EXPORT Q_DECL_EXPORT
# else
#  define ZH0_EXPORT Q_DECL_IMPORT
# endif
#else
# define ZH0_EXPORT
#endif
