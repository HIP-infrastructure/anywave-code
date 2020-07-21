#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(DELTAMEDBINTXTREADER_LIB)
#  define DELTAMEDBINTXTREADER_EXPORT Q_DECL_EXPORT
# else
#  define DELTAMEDBINTXTREADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define DELTAMEDBINTXTREADER_EXPORT
#endif
