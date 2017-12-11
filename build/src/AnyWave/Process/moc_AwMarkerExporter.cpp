/****************************************************************************
** Meta object code from reading C++ file 'AwMarkerExporter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Process/AwMarkerExporter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwMarkerExporter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwMarkerExporter_t {
    QByteArrayData data[1];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwMarkerExporter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwMarkerExporter_t qt_meta_stringdata_AwMarkerExporter = {
    {
QT_MOC_LITERAL(0, 0, 16) // "AwMarkerExporter"

    },
    "AwMarkerExporter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwMarkerExporter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AwMarkerExporter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AwMarkerExporter::staticMetaObject = {
    { &AwProcess::staticMetaObject, qt_meta_stringdata_AwMarkerExporter.data,
      qt_meta_data_AwMarkerExporter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwMarkerExporter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwMarkerExporter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwMarkerExporter.stringdata0))
        return static_cast<void*>(const_cast< AwMarkerExporter*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessInterface"))
        return static_cast< AwProcess*>(const_cast< AwMarkerExporter*>(this));
    return AwProcess::qt_metacast(_clname);
}

int AwMarkerExporter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_AwMarkerExporterPlugin_t {
    QByteArrayData data[1];
    char stringdata0[23];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwMarkerExporterPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwMarkerExporterPlugin_t qt_meta_stringdata_AwMarkerExporterPlugin = {
    {
QT_MOC_LITERAL(0, 0, 22) // "AwMarkerExporterPlugin"

    },
    "AwMarkerExporterPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwMarkerExporterPlugin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AwMarkerExporterPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AwMarkerExporterPlugin::staticMetaObject = {
    { &AwProcessPlugin::staticMetaObject, qt_meta_stringdata_AwMarkerExporterPlugin.data,
      qt_meta_data_AwMarkerExporterPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwMarkerExporterPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwMarkerExporterPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwMarkerExporterPlugin.stringdata0))
        return static_cast<void*>(const_cast< AwMarkerExporterPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessPluginInterface"))
        return static_cast< AwProcessPlugin*>(const_cast< AwMarkerExporterPlugin*>(this));
    return AwProcessPlugin::qt_metacast(_clname);
}

int AwMarkerExporterPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcessPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
