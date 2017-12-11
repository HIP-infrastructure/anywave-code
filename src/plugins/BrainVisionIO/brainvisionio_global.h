#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(BRAINVISIONIO_LIB)
#  define BRAINVISIONIO_EXPORT Q_DECL_EXPORT
# else
#  define BRAINVISIONIO_EXPORT Q_DECL_IMPORT
# endif
#else
# define BRAINVISIONIO_EXPORT
#endif
