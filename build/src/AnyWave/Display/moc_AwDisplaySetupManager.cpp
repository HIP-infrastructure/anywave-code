/****************************************************************************
** Meta object code from reading C++ file 'AwDisplaySetupManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Display/AwDisplaySetupManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDisplaySetupManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDisplaySetupManager_t {
    QByteArrayData data[19];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDisplaySetupManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDisplaySetupManager_t qt_meta_stringdata_AwDisplaySetupManager = {
    {
QT_MOC_LITERAL(0, 0, 21), // "AwDisplaySetupManager"
QT_MOC_LITERAL(1, 22, 16), // "newSetupSelected"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 15), // "AwDisplaySetup*"
QT_MOC_LITERAL(4, 56, 5), // "setup"
QT_MOC_LITERAL(5, 62, 12), // "setupChanged"
QT_MOC_LITERAL(6, 75, 5), // "flags"
QT_MOC_LITERAL(7, 81, 14), // "changeSetupDir"
QT_MOC_LITERAL(8, 96, 4), // "path"
QT_MOC_LITERAL(9, 101, 11), // "changeSetup"
QT_MOC_LITERAL(10, 113, 8), // "newSetup"
QT_MOC_LITERAL(11, 122, 12), // "saveSettings"
QT_MOC_LITERAL(12, 135, 11), // "updateSetup"
QT_MOC_LITERAL(13, 147, 4), // "view"
QT_MOC_LITERAL(14, 152, 16), // "saveCurrentSetup"
QT_MOC_LITERAL(15, 169, 9), // "saveAsNew"
QT_MOC_LITERAL(16, 179, 18), // "deleteCurrentSetup"
QT_MOC_LITERAL(17, 198, 9), // "loadSetup"
QT_MOC_LITERAL(18, 208, 14) // "resetToDefault"

    },
    "AwDisplaySetupManager\0newSetupSelected\0"
    "\0AwDisplaySetup*\0setup\0setupChanged\0"
    "flags\0changeSetupDir\0path\0changeSetup\0"
    "newSetup\0saveSettings\0updateSetup\0"
    "view\0saveCurrentSetup\0saveAsNew\0"
    "deleteCurrentSetup\0loadSetup\0"
    "resetToDefault"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDisplaySetupManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    2,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   77,    2, 0x0a /* Public */,
       9,    1,   80,    2, 0x0a /* Public */,
      11,    0,   83,    2, 0x0a /* Public */,
      12,    2,   84,    2, 0x0a /* Public */,
      14,    0,   89,    2, 0x0a /* Public */,
      15,    0,   90,    2, 0x0a /* Public */,
      16,    0,   91,    2, 0x0a /* Public */,
      17,    1,   92,    2, 0x0a /* Public */,
      18,    0,   95,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    4,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,   13,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,

       0        // eod
};

void AwDisplaySetupManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDisplaySetupManager *_t = static_cast<AwDisplaySetupManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newSetupSelected((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1]))); break;
        case 1: _t->setupChanged((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->changeSetupDir((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->changeSetup((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->saveSettings(); break;
        case 5: _t->updateSetup((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->saveCurrentSetup(); break;
        case 7: _t->saveAsNew(); break;
        case 8: _t->deleteCurrentSetup(); break;
        case 9: _t->loadSetup((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->resetToDefault(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwDisplaySetupManager::*_t)(AwDisplaySetup * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplaySetupManager::newSetupSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwDisplaySetupManager::*_t)(AwDisplaySetup * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplaySetupManager::setupChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwDisplaySetupManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwDisplaySetupManager.data,
      qt_meta_data_AwDisplaySetupManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDisplaySetupManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDisplaySetupManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDisplaySetupManager.stringdata0))
        return static_cast<void*>(const_cast< AwDisplaySetupManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AwDisplaySetupManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void AwDisplaySetupManager::newSetupSelected(AwDisplaySetup * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwDisplaySetupManager::setupChanged(AwDisplaySetup * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
