#ifndef EEGINTO4D_GLOBAL_H
#define EEGINTO4D_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef EEGINTO4D_LIB
# define EEGINTO4D_EXPORT Q_DECL_EXPORT
#else
# define EEGINTO4D_EXPORT Q_DECL_IMPORT
#endif

#endif // EEGINTO4D_GLOBAL_H
