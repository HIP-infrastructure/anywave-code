/****************************************************************************
** Meta object code from reading C++ file 'ADESIO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/AnyWave/IO/ADES/ADESIO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ADESIO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ADESIO_t {
    QByteArrayData data[1];
    char stringdata0[7];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ADESIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ADESIO_t qt_meta_stringdata_ADESIO = {
    {
QT_MOC_LITERAL(0, 0, 6) // "ADESIO"

    },
    "ADESIO"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ADESIO[] = {

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

void ADESIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ADESIO::staticMetaObject = {
    { &AwFileIO::staticMetaObject, qt_meta_stringdata_ADESIO.data,
      qt_meta_data_ADESIO,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ADESIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ADESIO::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ADESIO.stringdata0))
        return static_cast<void*>(const_cast< ADESIO*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterface"))
        return static_cast< AwFileIO*>(const_cast< ADESIO*>(this));
    return AwFileIO::qt_metacast(_clname);
}

int ADESIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_ADESIOPlugin_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ADESIOPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ADESIOPlugin_t qt_meta_stringdata_ADESIOPlugin = {
    {
QT_MOC_LITERAL(0, 0, 12) // "ADESIOPlugin"

    },
    "ADESIOPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ADESIOPlugin[] = {

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

void ADESIOPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ADESIOPlugin::staticMetaObject = {
    { &AwFileIOPlugin::staticMetaObject, qt_meta_stringdata_ADESIOPlugin.data,
      qt_meta_data_ADESIOPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ADESIOPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ADESIOPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ADESIOPlugin.stringdata0))
        return static_cast<void*>(const_cast< ADESIOPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterfacePlugin"))
        return static_cast< AwFileIOPlugin*>(const_cast< ADESIOPlugin*>(this));
    return AwFileIOPlugin::qt_metacast(_clname);
}

int ADESIOPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIOPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
