#ifndef FIFF_READER_GLOBAL_H
#define FIFF_READER_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef FIFF_READER_LIB
# define FIFF_READER_EXPORT Q_DECL_EXPORT
#else
# define FIFF_READER_EXPORT Q_DECL_IMPORT
#endif

#endif // FIFF_READER_GLOBAL_H
