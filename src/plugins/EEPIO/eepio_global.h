#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EEPIO_LIB)
#  define EEPIO_EXPORT Q_DECL_EXPORT
# else
#  define EEPIO_EXPORT Q_DECL_IMPORT
# endif
#else
# define EEPIO_EXPORT
#endif
