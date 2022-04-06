#pragma once
#include <qglobal.h>

#ifdef NIEXPORTER_LIB
# define NIEXPORTER_EXPORT Q_DECL_EXPORT
#else
# define NIEXPORTER_EXPORT Q_DECL_IMPORT
#endif
