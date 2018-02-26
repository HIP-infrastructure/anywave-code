#ifndef PYTHONMODULE_GLOBAL_H
#define PYTHONMODULE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef PYTHONSUPPORT_LIB
# define PYTHONMODULE_EXPORT Q_DECL_EXPORT
#else
# define PYTHONMODULE_EXPORT Q_DECL_IMPORT
#endif

#endif // PYTHONMODULE_GLOBAL_H
