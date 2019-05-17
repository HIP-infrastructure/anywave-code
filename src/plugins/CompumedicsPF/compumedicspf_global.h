#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(COMPUMEDICSPF_LIB)
#  define COMPUMEDICSPF_EXPORT Q_DECL_EXPORT
# else
#  define COMPUMEDICSPF_EXPORT Q_DECL_IMPORT
# endif
#else
# define COMPUMEDICSPF_EXPORT
#endif
