/****************************************************************************
** Meta object code from reading C++ file 'AwSEEGViewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../include/widget/AwSEEGViewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwSEEGViewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwSEEGViewer_t {
    QByteArrayData data[16];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwSEEGViewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwSEEGViewer_t qt_meta_stringdata_AwSEEGViewer = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwSEEGViewer"
QT_MOC_LITERAL(1, 13, 17), // "newDataConnection"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 13), // "AwDataClient*"
QT_MOC_LITERAL(4, 46, 14), // "mappingStopped"
QT_MOC_LITERAL(5, 61, 14), // "setMappingMode"
QT_MOC_LITERAL(6, 76, 6), // "active"
QT_MOC_LITERAL(7, 83, 8), // "loadMesh"
QT_MOC_LITERAL(8, 92, 4), // "file"
QT_MOC_LITERAL(9, 97, 14), // "loadElectrodes"
QT_MOC_LITERAL(10, 112, 15), // "setSEEGChannels"
QT_MOC_LITERAL(11, 128, 13), // "AwChannelList"
QT_MOC_LITERAL(12, 142, 8), // "channels"
QT_MOC_LITERAL(13, 151, 15), // "updateMappingAt"
QT_MOC_LITERAL(14, 167, 7), // "latency"
QT_MOC_LITERAL(15, 175, 18) // "handleWidgetClosed"

    },
    "AwSEEGViewer\0newDataConnection\0\0"
    "AwDataClient*\0mappingStopped\0"
    "setMappingMode\0active\0loadMesh\0file\0"
    "loadElectrodes\0setSEEGChannels\0"
    "AwChannelList\0channels\0updateMappingAt\0"
    "latency\0handleWidgetClosed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwSEEGViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    0,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   58,    2, 0x0a /* Public */,
       7,    1,   61,    2, 0x0a /* Public */,
       9,    1,   64,    2, 0x0a /* Public */,
      10,    1,   67,    2, 0x0a /* Public */,
      13,    1,   70,    2, 0x0a /* Public */,
      15,    0,   73,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void,

       0        // eod
};

void AwSEEGViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwSEEGViewer *_t = static_cast<AwSEEGViewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newDataConnection((*reinterpret_cast< AwDataClient*(*)>(_a[1]))); break;
        case 1: _t->mappingStopped(); break;
        case 2: _t->setMappingMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->loadMesh((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->loadElectrodes((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->setSEEGChannels((*reinterpret_cast< const AwChannelList(*)>(_a[1]))); break;
        case 6: _t->updateMappingAt((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->handleWidgetClosed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwDataClient* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwChannelList >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwSEEGViewer::*_t)(AwDataClient * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSEEGViewer::newDataConnection)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwSEEGViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSEEGViewer::mappingStopped)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwSEEGViewer::staticMetaObject = {
    { &AwDataClient::staticMetaObject, qt_meta_stringdata_AwSEEGViewer.data,
      qt_meta_data_AwSEEGViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwSEEGViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwSEEGViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwSEEGViewer.stringdata0))
        return static_cast<void*>(const_cast< AwSEEGViewer*>(this));
    return AwDataClient::qt_metacast(_clname);
}

int AwSEEGViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwDataClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AwSEEGViewer::newDataConnection(AwDataClient * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwSEEGViewer::mappingStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
