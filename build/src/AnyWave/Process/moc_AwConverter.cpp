/****************************************************************************
** Meta object code from reading C++ file 'AwConverter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Process/AwConverter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwConverter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwConverter_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwConverter_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwConverter_t qt_meta_stringdata_AwConverter = {
    {
QT_MOC_LITERAL(0, 0, 11) // "AwConverter"

    },
    "AwConverter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwConverter[] = {

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

void AwConverter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AwConverter::staticMetaObject = {
    { &AwProcess::staticMetaObject, qt_meta_stringdata_AwConverter.data,
      qt_meta_data_AwConverter,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwConverter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwConverter::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwConverter.stringdata0))
        return static_cast<void*>(const_cast< AwConverter*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessInterface"))
        return static_cast< AwProcess*>(const_cast< AwConverter*>(this));
    return AwProcess::qt_metacast(_clname);
}

int AwConverter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_AwConverterPlugin_t {
    QByteArrayData data[1];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwConverterPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwConverterPlugin_t qt_meta_stringdata_AwConverterPlugin = {
    {
QT_MOC_LITERAL(0, 0, 17) // "AwConverterPlugin"

    },
    "AwConverterPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwConverterPlugin[] = {

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

void AwConverterPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AwConverterPlugin::staticMetaObject = {
    { &AwProcessPlugin::staticMetaObject, qt_meta_stringdata_AwConverterPlugin.data,
      qt_meta_data_AwConverterPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwConverterPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwConverterPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwConverterPlugin.stringdata0))
        return static_cast<void*>(const_cast< AwConverterPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessPluginInterface"))
        return static_cast< AwProcessPlugin*>(const_cast< AwConverterPlugin*>(this));
    return AwProcessPlugin::qt_metacast(_clname);
}

int AwConverterPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcessPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
