#ifndef REWRITE4D_GLOBAL_H
#define REWRITE4D_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef REWRITE_4DNI_LIB
# define REWRITE4D_EXPORT Q_DECL_EXPORT
#else
# define REWRITE4D_EXPORT Q_DECL_IMPORT
#endif

#endif // REWRITE4D_GLOBAL_H
