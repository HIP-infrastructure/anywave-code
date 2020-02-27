#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(MFV_LIB)
#  define MFV_EXPORT Q_DECL_EXPORT
# else
#  define MFV_EXPORT Q_DECL_IMPORT
# endif
#else
# define MFV_EXPORT
#endif
