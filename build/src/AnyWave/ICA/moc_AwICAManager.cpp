/****************************************************************************
** Meta object code from reading C++ file 'AwICAManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/ICA/AwICAManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwICAManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwICAManager_t {
    QByteArrayData data[11];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwICAManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwICAManager_t qt_meta_stringdata_AwICAManager = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwICAManager"
QT_MOC_LITERAL(1, 13, 16), // "componentsLoaded"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17), // "filteringSwitched"
QT_MOC_LITERAL(4, 49, 6), // "active"
QT_MOC_LITERAL(5, 56, 21), // "icaComponentsUnloaded"
QT_MOC_LITERAL(6, 78, 23), // "componentsFiltersLoaded"
QT_MOC_LITERAL(7, 102, 2), // "lp"
QT_MOC_LITERAL(8, 105, 2), // "hp"
QT_MOC_LITERAL(9, 108, 15), // "setICAFiletring"
QT_MOC_LITERAL(10, 124, 2) // "on"

    },
    "AwICAManager\0componentsLoaded\0\0"
    "filteringSwitched\0active\0icaComponentsUnloaded\0"
    "componentsFiltersLoaded\0lp\0hp\0"
    "setICAFiletring\0on"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwICAManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    1,   40,    2, 0x06 /* Public */,
       5,    0,   43,    2, 0x06 /* Public */,
       6,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    7,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   10,

       0        // eod
};

void AwICAManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwICAManager *_t = static_cast<AwICAManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->componentsLoaded(); break;
        case 1: _t->filteringSwitched((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->icaComponentsUnloaded(); break;
        case 3: _t->componentsFiltersLoaded((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 4: _t->setICAFiletring((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwICAManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwICAManager::componentsLoaded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwICAManager::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwICAManager::filteringSwitched)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwICAManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwICAManager::icaComponentsUnloaded)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwICAManager::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwICAManager::componentsFiltersLoaded)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject AwICAManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwICAManager.data,
      qt_meta_data_AwICAManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwICAManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwICAManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwICAManager.stringdata0))
        return static_cast<void*>(const_cast< AwICAManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AwICAManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AwICAManager::componentsLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwICAManager::filteringSwitched(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwICAManager::icaComponentsUnloaded()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void AwICAManager::componentsFiltersLoaded(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
