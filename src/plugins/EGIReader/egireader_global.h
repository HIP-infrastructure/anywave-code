#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EGIREADER_LIB)
#  define EGIREADER_EXPORT Q_DECL_EXPORT
# else
#  define EGIREADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define EGIREADER_EXPORT
#endif
