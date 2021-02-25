#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SPECTRAL_LIB)
#  define SPECTRAL_EXPORT Q_DECL_EXPORT
# else
#  define SPECTRAL_EXPORT Q_DECL_IMPORT
# endif
#else
# define SPECTRAL_EXPORT
#endif
