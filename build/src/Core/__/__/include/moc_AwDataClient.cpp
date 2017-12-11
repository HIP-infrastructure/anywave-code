/****************************************************************************
** Meta object code from reading C++ file 'AwDataClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../include/AwDataClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDataClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDataClient_t {
    QByteArrayData data[10];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDataClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDataClient_t qt_meta_stringdata_AwDataClient = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwDataClient"
QT_MOC_LITERAL(1, 13, 8), // "needData"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 14), // "AwChannelList*"
QT_MOC_LITERAL(4, 38, 8), // "channels"
QT_MOC_LITERAL(5, 47, 5), // "start"
QT_MOC_LITERAL(6, 53, 8), // "duration"
QT_MOC_LITERAL(7, 62, 7), // "rawData"
QT_MOC_LITERAL(8, 70, 9), // "AwMarker*"
QT_MOC_LITERAL(9, 80, 6) // "marker"

    },
    "AwDataClient\0needData\0\0AwChannelList*\0"
    "channels\0start\0duration\0rawData\0"
    "AwMarker*\0marker"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDataClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   34,    2, 0x06 /* Public */,
       1,    3,   43,    2, 0x26 /* Public | MethodCloned */,
       1,    3,   50,    2, 0x06 /* Public */,
       1,    2,   57,    2, 0x26 /* Public | MethodCloned */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Float, QMetaType::Float, QMetaType::Bool,    4,    5,    6,    7,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Float, QMetaType::Float,    4,    5,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 8, QMetaType::Bool,    4,    9,    7,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 8,    4,    9,

       0        // eod
};

void AwDataClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDataClient *_t = static_cast<AwDataClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->needData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->needData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 2: _t->needData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< AwMarker*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 3: _t->needData((*reinterpret_cast< AwChannelList*(*)>(_a[1])),(*reinterpret_cast< AwMarker*(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwDataClient::*_t)(AwChannelList * , float , float , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDataClient::needData)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwDataClient::*_t)(AwChannelList * , AwMarker * , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDataClient::needData)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwDataClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwDataClient.data,
      qt_meta_data_AwDataClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDataClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDataClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDataClient.stringdata0))
        return static_cast<void*>(const_cast< AwDataClient*>(this));
    return QObject::qt_metacast(_clname);
}

int AwDataClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void AwDataClient::needData(AwChannelList * _t1, float _t2, float _t3, bool _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void AwDataClient::needData(AwChannelList * _t1, AwMarker * _t2, bool _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
