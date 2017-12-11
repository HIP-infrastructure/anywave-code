/****************************************************************************
** Meta object code from reading C++ file 'AwDataConnection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Data/AwDataConnection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDataConnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDataConnection_t {
    QByteArrayData data[13];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDataConnection_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDataConnection_t qt_meta_stringdata_AwDataConnection = {
    {
QT_MOC_LITERAL(0, 0, 16), // "AwDataConnection"
QT_MOC_LITERAL(1, 17, 11), // "sendingData"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "AwChannelList*"
QT_MOC_LITERAL(4, 45, 8), // "channels"
QT_MOC_LITERAL(5, 54, 9), // "endOfData"
QT_MOC_LITERAL(6, 64, 11), // "outOfMemory"
QT_MOC_LITERAL(7, 76, 8), // "loadData"
QT_MOC_LITERAL(8, 85, 5), // "start"
QT_MOC_LITERAL(9, 91, 8), // "duration"
QT_MOC_LITERAL(10, 100, 7), // "rawData"
QT_MOC_LITERAL(11, 108, 9), // "AwMarker*"
QT_MOC_LITERAL(12, 118, 6) // "marker"

    },
    "AwDataConnection\0sendingData\0\0"
    "AwChannelList*\0channels\0endOfData\0"
    "outOfMemory\0loadData\0start\0duration\0"
    "rawData\0AwMarker*\0marker"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDataConnection[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    0,   62,    2, 0x06 /* Public */,
       6,    0,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    4,   64,    2, 0x0a /* Public */,
       7,    3,   73,    2, 0x2a /* Public | MethodCloned */,
       7,    3,   80,    2, 0x0a /* Public */,
       7,    2,   87,    2, 0x2a /* Public | MethodCloned */,
       7,    4,   92,    2, 0x0a /* Public */,
       7,    3,  101,    2, 0x2a /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Float, QMetaType::Float, QMetaType::Bool,    4,    8,    9,   10,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Float, QMetaType::Float,    4,    8,    9,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 11, QMetaType::Bool,    4,   12,   10,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 11,    4,   12,
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong, QMetaType::ULongLong, QMetaType::Bool,    4,    8,    9,   10,
    QMetaType::Void, 0x80000000 | 3, QMetaType::ULongLong, QMetaType::ULongLong,    4,    8,    9,

       0        // eod
};

void AwDataConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDataConnection *_t = static_cast<AwDataConnection *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendingData((*reinterpret_cast< AwChannelList*(*)>(_a[1]))); break;
        case 1: _t->endOfData(); break;
        case 2: _t->outOfMemory(); break;
        case 3: _t->loadData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 4: _t->loadData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 5: _t->loadData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< AwMarker*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 6: _t->loadData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< AwMarker*(*)>(_a[2]))); break;
        case 7: _t->loadData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< quint64(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 8: _t->loadData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< quint64(*)>(_a[2])),(*reinterpret_cast< quint64(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwDataConnection::*_t)(AwChannelList * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDataConnection::sendingData)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwDataConnection::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDataConnection::endOfData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwDataConnection::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDataConnection::outOfMemory)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwDataConnection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwDataConnection.data,
      qt_meta_data_AwDataConnection,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDataConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDataConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDataConnection.stringdata0))
        return static_cast<void*>(const_cast< AwDataConnection*>(this));
    return QObject::qt_metacast(_clname);
}

int AwDataConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void AwDataConnection::sendingData(AwChannelList * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwDataConnection::endOfData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AwDataConnection::outOfMemory()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
