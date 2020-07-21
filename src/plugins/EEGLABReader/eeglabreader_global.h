#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(EEGLABREADER_LIB)
#  define EEGLABREADER_EXPORT Q_DECL_EXPORT
# else
#  define EEGLABREADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define EEGLABREADER_EXPORT
#endif
