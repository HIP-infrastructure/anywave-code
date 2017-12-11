/****************************************************************************
** Meta object code from reading C++ file 'AwExporter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Process/AwExporter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwExporter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwExporter_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwExporter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwExporter_t qt_meta_stringdata_AwExporter = {
    {
QT_MOC_LITERAL(0, 0, 10) // "AwExporter"

    },
    "AwExporter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwExporter[] = {

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

void AwExporter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AwExporter::staticMetaObject = {
    { &AwProcess::staticMetaObject, qt_meta_stringdata_AwExporter.data,
      qt_meta_data_AwExporter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwExporter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwExporter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwExporter.stringdata0))
        return static_cast<void*>(const_cast< AwExporter*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessInterface"))
        return static_cast< AwProcess*>(const_cast< AwExporter*>(this));
    return AwProcess::qt_metacast(_clname);
}

int AwExporter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_AwExporterPlugin_t {
    QByteArrayData data[1];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwExporterPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwExporterPlugin_t qt_meta_stringdata_AwExporterPlugin = {
    {
QT_MOC_LITERAL(0, 0, 16) // "AwExporterPlugin"

    },
    "AwExporterPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwExporterPlugin[] = {

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

void AwExporterPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AwExporterPlugin::staticMetaObject = {
    { &AwProcessPlugin::staticMetaObject, qt_meta_stringdata_AwExporterPlugin.data,
      qt_meta_data_AwExporterPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwExporterPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwExporterPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwExporterPlugin.stringdata0))
        return static_cast<void*>(const_cast< AwExporterPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessPluginInterface"))
        return static_cast< AwProcessPlugin*>(const_cast< AwExporterPlugin*>(this));
    return AwProcessPlugin::qt_metacast(_clname);
}

int AwExporterPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcessPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
