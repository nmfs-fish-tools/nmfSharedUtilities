#ifndef NMFDATA_GLOBAL_H
#define NMFDATA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NMFDATA_LIBRARY)
#  define NMFDATASHARED_EXPORT Q_DECL_EXPORT
#else
#  define NMFDATASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NMFDATA_GLOBAL_H
