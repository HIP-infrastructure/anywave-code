/****************************************************************************
** Meta object code from reading C++ file 'MATLABIO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/AnyWave/IO/MATLAB/MATLABIO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MATLABIO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MATLABIO_t {
    QByteArrayData data[1];
    char stringdata0[9];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MATLABIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MATLABIO_t qt_meta_stringdata_MATLABIO = {
    {
QT_MOC_LITERAL(0, 0, 8) // "MATLABIO"

    },
    "MATLABIO"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MATLABIO[] = {

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

void MATLABIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MATLABIO::staticMetaObject = {
    { &AwFileIO::staticMetaObject, qt_meta_stringdata_MATLABIO.data,
      qt_meta_data_MATLABIO,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MATLABIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MATLABIO::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MATLABIO.stringdata0))
        return static_cast<void*>(const_cast< MATLABIO*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterface"))
        return static_cast< AwFileIO*>(const_cast< MATLABIO*>(this));
    return AwFileIO::qt_metacast(_clname);
}

int MATLABIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_MATLABIOPlugin_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MATLABIOPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MATLABIOPlugin_t qt_meta_stringdata_MATLABIOPlugin = {
    {
QT_MOC_LITERAL(0, 0, 14) // "MATLABIOPlugin"

    },
    "MATLABIOPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MATLABIOPlugin[] = {

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

void MATLABIOPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MATLABIOPlugin::staticMetaObject = {
    { &AwFileIOPlugin::staticMetaObject, qt_meta_stringdata_MATLABIOPlugin.data,
      qt_meta_data_MATLABIOPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MATLABIOPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MATLABIOPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MATLABIOPlugin.stringdata0))
        return static_cast<void*>(const_cast< MATLABIOPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterfacePlugin"))
        return static_cast< AwFileIOPlugin*>(const_cast< MATLABIOPlugin*>(this));
    return AwFileIOPlugin::qt_metacast(_clname);
}

int MATLABIOPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIOPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
