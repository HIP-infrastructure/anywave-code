#ifndef MERGEFILES_GLOBAL_H
#define MERGEFILES_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MERGEFILES_LIB
# define MERGEFILES_EXPORT Q_DECL_EXPORT
#else
# define MERGEFILES_EXPORT Q_DECL_IMPORT
#endif

#endif // MERGEFILES_GLOBAL_H
