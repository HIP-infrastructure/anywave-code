/****************************************************************************
** Meta object code from reading C++ file 'AwPluginManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Plugin/AwPluginManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwPluginManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwPluginManager_t {
    QByteArrayData data[11];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwPluginManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwPluginManager_t qt_meta_stringdata_AwPluginManager = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwPluginManager"
QT_MOC_LITERAL(1, 16, 3), // "log"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 15), // "showPluginsDial"
QT_MOC_LITERAL(4, 37, 9), // "newReader"
QT_MOC_LITERAL(5, 47, 9), // "AwFileIO*"
QT_MOC_LITERAL(6, 57, 15), // "AwFileIOPlugin*"
QT_MOC_LITERAL(7, 73, 6), // "plugin"
QT_MOC_LITERAL(8, 80, 20), // "deleteReaderInstance"
QT_MOC_LITERAL(9, 101, 2), // "fr"
QT_MOC_LITERAL(10, 104, 9) // "newWriter"

    },
    "AwPluginManager\0log\0\0showPluginsDial\0"
    "newReader\0AwFileIO*\0AwFileIOPlugin*\0"
    "plugin\0deleteReaderInstance\0fr\0newWriter"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwPluginManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   42,    2, 0x0a /* Public */,
       4,    1,   43,    2, 0x0a /* Public */,
       8,    1,   46,    2, 0x0a /* Public */,
      10,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    0x80000000 | 5, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 5,    9,
    0x80000000 | 5, 0x80000000 | 6,    7,

       0        // eod
};

void AwPluginManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwPluginManager *_t = static_cast<AwPluginManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->log((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->showPluginsDial(); break;
        case 2: { AwFileIO* _r = _t->newReader((*reinterpret_cast< AwFileIOPlugin*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< AwFileIO**>(_a[0]) = _r; }  break;
        case 3: _t->deleteReaderInstance((*reinterpret_cast< AwFileIO*(*)>(_a[1]))); break;
        case 4: { AwFileIO* _r = _t->newWriter((*reinterpret_cast< AwFileIOPlugin*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< AwFileIO**>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwPluginManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPluginManager::log)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwPluginManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwPluginManager.data,
      qt_meta_data_AwPluginManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwPluginManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwPluginManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwPluginManager.stringdata0))
        return static_cast<void*>(const_cast< AwPluginManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AwPluginManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void AwPluginManager::log(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
