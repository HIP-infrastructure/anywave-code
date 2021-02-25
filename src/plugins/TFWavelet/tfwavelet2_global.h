#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(TFWAVELET2_LIB)
#  define TFWAVELET2_EXPORT Q_DECL_EXPORT
# else
#  define TFWAVELET2_EXPORT Q_DECL_IMPORT
# endif
#else
# define TFWAVELET2_EXPORT
#endif
