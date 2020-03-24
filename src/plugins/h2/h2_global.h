#ifndef H2_GLOBAL_H
#define H2_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef H2_LIB
# define H2_EXPORT Q_DECL_EXPORT
#else
# define H2_EXPORT Q_DECL_IMPORT
#endif

#endif // H2_GLOBAL_H
