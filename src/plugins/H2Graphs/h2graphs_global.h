#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(H2GRAPHS_LIB)
#  define H2GRAPHS_EXPORT Q_DECL_EXPORT
# else
#  define H2GRAPHS_EXPORT Q_DECL_IMPORT
# endif
#else
# define H2GRAPHS_EXPORT
#endif
