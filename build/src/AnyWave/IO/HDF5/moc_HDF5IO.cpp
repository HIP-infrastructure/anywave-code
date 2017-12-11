/****************************************************************************
** Meta object code from reading C++ file 'HDF5IO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/AnyWave/IO/HDF5/HDF5IO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HDF5IO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AHDF5IO_t {
    QByteArrayData data[1];
    char stringdata0[8];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AHDF5IO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AHDF5IO_t qt_meta_stringdata_AHDF5IO = {
    {
QT_MOC_LITERAL(0, 0, 7) // "AHDF5IO"

    },
    "AHDF5IO"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AHDF5IO[] = {

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

void AHDF5IO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AHDF5IO::staticMetaObject = {
    { &AwFileIO::staticMetaObject, qt_meta_stringdata_AHDF5IO.data,
      qt_meta_data_AHDF5IO,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AHDF5IO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AHDF5IO::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AHDF5IO.stringdata0))
        return static_cast<void*>(const_cast< AHDF5IO*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterface"))
        return static_cast< AwFileIO*>(const_cast< AHDF5IO*>(this));
    return AwFileIO::qt_metacast(_clname);
}

int AHDF5IO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_AHDF5IOPlugin_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AHDF5IOPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AHDF5IOPlugin_t qt_meta_stringdata_AHDF5IOPlugin = {
    {
QT_MOC_LITERAL(0, 0, 13) // "AHDF5IOPlugin"

    },
    "AHDF5IOPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AHDF5IOPlugin[] = {

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

void AHDF5IOPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AHDF5IOPlugin::staticMetaObject = {
    { &AwFileIOPlugin::staticMetaObject, qt_meta_stringdata_AHDF5IOPlugin.data,
      qt_meta_data_AHDF5IOPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AHDF5IOPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AHDF5IOPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AHDF5IOPlugin.stringdata0))
        return static_cast<void*>(const_cast< AHDF5IOPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterfacePlugin"))
        return static_cast< AwFileIOPlugin*>(const_cast< AHDF5IOPlugin*>(this));
    return AwFileIOPlugin::qt_metacast(_clname);
}

int AHDF5IOPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIOPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
