/****************************************************************************
** Meta object code from reading C++ file 'AwProcessInterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../include/AwProcessInterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwProcessInterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwBaseProcess_t {
    QByteArrayData data[19];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwBaseProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwBaseProcess_t qt_meta_stringdata_AwBaseProcess = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AwBaseProcess"
QT_MOC_LITERAL(1, 14, 11), // "sendMarkers"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 13), // "AwMarkerList*"
QT_MOC_LITERAL(4, 41, 7), // "markers"
QT_MOC_LITERAL(5, 49, 10), // "sendMarker"
QT_MOC_LITERAL(6, 60, 9), // "AwMarker*"
QT_MOC_LITERAL(7, 70, 6), // "marker"
QT_MOC_LITERAL(8, 77, 11), // "sendCommand"
QT_MOC_LITERAL(9, 89, 7), // "command"
QT_MOC_LITERAL(10, 97, 4), // "args"
QT_MOC_LITERAL(11, 102, 23), // "dataConnectionRequested"
QT_MOC_LITERAL(12, 126, 13), // "AwDataClient*"
QT_MOC_LITERAL(13, 140, 6), // "client"
QT_MOC_LITERAL(14, 147, 16), // "newDisplayPlugin"
QT_MOC_LITERAL(15, 164, 16), // "AwDisplayPlugin*"
QT_MOC_LITERAL(16, 181, 6), // "plugin"
QT_MOC_LITERAL(17, 188, 4), // "stop"
QT_MOC_LITERAL(18, 193, 5) // "abort"

    },
    "AwBaseProcess\0sendMarkers\0\0AwMarkerList*\0"
    "markers\0sendMarker\0AwMarker*\0marker\0"
    "sendCommand\0command\0args\0"
    "dataConnectionRequested\0AwDataClient*\0"
    "client\0newDisplayPlugin\0AwDisplayPlugin*\0"
    "plugin\0stop\0abort"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwBaseProcess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       5,    1,   52,    2, 0x06 /* Public */,
       8,    2,   55,    2, 0x06 /* Public */,
      11,    1,   60,    2, 0x06 /* Public */,
      14,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,   66,    2, 0x0a /* Public */,
      18,    0,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariantList,    9,   10,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwBaseProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwBaseProcess *_t = static_cast<AwBaseProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMarkers((*reinterpret_cast< AwMarkerList*(*)>(_a[1]))); break;
        case 1: _t->sendMarker((*reinterpret_cast< AwMarker*(*)>(_a[1]))); break;
        case 2: _t->sendCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QVariantList(*)>(_a[2]))); break;
        case 3: _t->dataConnectionRequested((*reinterpret_cast< AwDataClient*(*)>(_a[1]))); break;
        case 4: _t->newDisplayPlugin((*reinterpret_cast< AwDisplayPlugin*(*)>(_a[1]))); break;
        case 5: _t->stop(); break;
        case 6: _t->abort(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwDataClient* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwBaseProcess::*_t)(AwMarkerList * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseProcess::sendMarkers)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwBaseProcess::*_t)(AwMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseProcess::sendMarker)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwBaseProcess::*_t)(int , QVariantList );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseProcess::sendCommand)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwBaseProcess::*_t)(AwDataClient * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseProcess::dataConnectionRequested)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwBaseProcess::*_t)(AwDisplayPlugin * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseProcess::newDisplayPlugin)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject AwBaseProcess::staticMetaObject = {
    { &AwDataClient::staticMetaObject, qt_meta_stringdata_AwBaseProcess.data,
      qt_meta_data_AwBaseProcess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwBaseProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwBaseProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwBaseProcess.stringdata0))
        return static_cast<void*>(const_cast< AwBaseProcess*>(this));
    return AwDataClient::qt_metacast(_clname);
}

int AwBaseProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwDataClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void AwBaseProcess::sendMarkers(AwMarkerList * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwBaseProcess::sendMarker(AwMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwBaseProcess::sendCommand(int _t1, QVariantList _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AwBaseProcess::dataConnectionRequested(AwDataClient * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AwBaseProcess::newDisplayPlugin(AwDisplayPlugin * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
struct qt_meta_stringdata_AwGUIProcess_t {
    QByteArrayData data[11];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwGUIProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwGUIProcess_t qt_meta_stringdata_AwGUIProcess = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwGUIProcess"
QT_MOC_LITERAL(1, 13, 18), // "aboutToBeDestroyed"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 6), // "closed"
QT_MOC_LITERAL(4, 40, 16), // "connectionClosed"
QT_MOC_LITERAL(5, 57, 13), // "AwDataClient*"
QT_MOC_LITERAL(6, 71, 6), // "client"
QT_MOC_LITERAL(7, 78, 4), // "stop"
QT_MOC_LITERAL(8, 83, 10), // "setMarkers"
QT_MOC_LITERAL(9, 94, 12), // "AwMarkerList"
QT_MOC_LITERAL(10, 107, 7) // "markers"

    },
    "AwGUIProcess\0aboutToBeDestroyed\0\0"
    "closed\0connectionClosed\0AwDataClient*\0"
    "client\0stop\0setMarkers\0AwMarkerList\0"
    "markers"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwGUIProcess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    1,   41,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   44,    2, 0x0a /* Public */,
       8,    1,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void AwGUIProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwGUIProcess *_t = static_cast<AwGUIProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->aboutToBeDestroyed(); break;
        case 1: _t->closed(); break;
        case 2: _t->connectionClosed((*reinterpret_cast< AwDataClient*(*)>(_a[1]))); break;
        case 3: _t->stop(); break;
        case 4: _t->setMarkers((*reinterpret_cast< const AwMarkerList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwDataClient* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMarkerList >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwGUIProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGUIProcess::aboutToBeDestroyed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwGUIProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGUIProcess::closed)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwGUIProcess::*_t)(AwDataClient * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGUIProcess::connectionClosed)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwGUIProcess::staticMetaObject = {
    { &AwBaseProcess::staticMetaObject, qt_meta_stringdata_AwGUIProcess.data,
      qt_meta_data_AwGUIProcess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwGUIProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwGUIProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwGUIProcess.stringdata0))
        return static_cast<void*>(const_cast< AwGUIProcess*>(this));
    return AwBaseProcess::qt_metacast(_clname);
}

int AwGUIProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwBaseProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AwGUIProcess::aboutToBeDestroyed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwGUIProcess::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AwGUIProcess::connectionClosed(AwDataClient * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_AwProcess_t {
    QByteArrayData data[13];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwProcess_t qt_meta_stringdata_AwProcess = {
    {
QT_MOC_LITERAL(0, 0, 9), // "AwProcess"
QT_MOC_LITERAL(1, 10, 7), // "started"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 8), // "finished"
QT_MOC_LITERAL(4, 28, 7), // "aborted"
QT_MOC_LITERAL(5, 36, 4), // "idle"
QT_MOC_LITERAL(6, 41, 15), // "progressChanged"
QT_MOC_LITERAL(7, 57, 7), // "percent"
QT_MOC_LITERAL(8, 65, 7), // "message"
QT_MOC_LITERAL(9, 73, 15), // "criticalMessage"
QT_MOC_LITERAL(10, 89, 11), // "outOfMemory"
QT_MOC_LITERAL(11, 101, 4), // "stop"
QT_MOC_LITERAL(12, 106, 5) // "start"

    },
    "AwProcess\0started\0\0finished\0aborted\0"
    "idle\0progressChanged\0percent\0message\0"
    "criticalMessage\0outOfMemory\0stop\0start"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwProcess[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    1,   68,    2, 0x06 /* Public */,
       6,    1,   71,    2, 0x06 /* Public */,
       9,    1,   74,    2, 0x06 /* Public */,
      10,    0,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   78,    2, 0x0a /* Public */,
      12,    0,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwProcess *_t = static_cast<AwProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->started(); break;
        case 1: _t->finished(); break;
        case 2: _t->aborted(); break;
        case 3: _t->idle(); break;
        case 4: _t->progressChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->progressChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->criticalMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->outOfMemory(); break;
        case 8: _t->stop(); break;
        case 9: _t->start(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::started)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::finished)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::aborted)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::idle)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwProcess::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::progressChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AwProcess::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::progressChanged)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AwProcess::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::criticalMessage)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AwProcess::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcess::outOfMemory)) {
                *result = 7;
                return;
            }
        }
    }
}

const QMetaObject AwProcess::staticMetaObject = {
    { &AwBaseProcess::staticMetaObject, qt_meta_stringdata_AwProcess.data,
      qt_meta_data_AwProcess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwProcess.stringdata0))
        return static_cast<void*>(const_cast< AwProcess*>(this));
    return AwBaseProcess::qt_metacast(_clname);
}

int AwProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwBaseProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AwProcess::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwProcess::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AwProcess::aborted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void AwProcess::idle()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void AwProcess::progressChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AwProcess::progressChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AwProcess::criticalMessage(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AwProcess::outOfMemory()
{
    QMetaObject::activate(this, &staticMetaObject, 7, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
