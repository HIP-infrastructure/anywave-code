/****************************************************************************
** Meta object code from reading C++ file 'AwDataServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Data/AwDataServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDataServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDataServer_t {
    QByteArrayData data[10];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDataServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDataServer_t qt_meta_stringdata_AwDataServer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwDataServer"
QT_MOC_LITERAL(1, 13, 14), // "openConnection"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 13), // "AwDataClient*"
QT_MOC_LITERAL(4, 43, 6), // "client"
QT_MOC_LITERAL(5, 50, 9), // "AwFileIO*"
QT_MOC_LITERAL(6, 60, 6), // "reader"
QT_MOC_LITERAL(7, 67, 15), // "closeConnection"
QT_MOC_LITERAL(8, 83, 19), // "closeAllConnections"
QT_MOC_LITERAL(9, 103, 17) // "manageOutOfMemory"

    },
    "AwDataServer\0openConnection\0\0AwDataClient*\0"
    "client\0AwFileIO*\0reader\0closeConnection\0"
    "closeAllConnections\0manageOutOfMemory"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDataServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       1,    2,   42,    2, 0x0a /* Public */,
       7,    1,   47,    2, 0x0a /* Public */,
       8,    0,   50,    2, 0x0a /* Public */,
       9,    0,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwDataServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDataServer *_t = static_cast<AwDataServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openConnection((*reinterpret_cast< AwDataClient*(*)>(_a[1]))); break;
        case 1: _t->openConnection((*reinterpret_cast< AwDataClient*(*)>(_a[1])),(*reinterpret_cast< AwFileIO*(*)>(_a[2]))); break;
        case 2: _t->closeConnection((*reinterpret_cast< AwDataClient*(*)>(_a[1]))); break;
        case 3: _t->closeAllConnections(); break;
        case 4: _t->manageOutOfMemory(); break;
        default: ;
        }
    }
}

const QMetaObject AwDataServer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwDataServer.data,
      qt_meta_data_AwDataServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDataServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDataServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDataServer.stringdata0))
        return static_cast<void*>(const_cast< AwDataServer*>(this));
    return QObject::qt_metacast(_clname);
}

int AwDataServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
