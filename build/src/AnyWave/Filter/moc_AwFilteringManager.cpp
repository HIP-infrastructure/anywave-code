/****************************************************************************
** Meta object code from reading C++ file 'AwFilteringManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Filter/AwFilteringManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwFilteringManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwFilteringManager_t {
    QByteArrayData data[20];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwFilteringManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwFilteringManager_t qt_meta_stringdata_AwFilteringManager = {
    {
QT_MOC_LITERAL(0, 0, 18), // "AwFilteringManager"
QT_MOC_LITERAL(1, 19, 14), // "filtersChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 15), // "noActiveFilters"
QT_MOC_LITERAL(4, 51, 14), // "newICASettings"
QT_MOC_LITERAL(5, 66, 4), // "type"
QT_MOC_LITERAL(6, 71, 2), // "lp"
QT_MOC_LITERAL(7, 74, 2), // "hp"
QT_MOC_LITERAL(8, 77, 17), // "newSourceSettings"
QT_MOC_LITERAL(9, 95, 12), // "filtersReset"
QT_MOC_LITERAL(10, 108, 9), // "setFilter"
QT_MOC_LITERAL(11, 118, 2), // "LP"
QT_MOC_LITERAL(12, 121, 2), // "HP"
QT_MOC_LITERAL(13, 124, 14), // "setICASettings"
QT_MOC_LITERAL(14, 139, 17), // "setSourceSettings"
QT_MOC_LITERAL(15, 157, 8), // "setNotch"
QT_MOC_LITERAL(16, 166, 5), // "notch"
QT_MOC_LITERAL(17, 172, 5), // "reset"
QT_MOC_LITERAL(18, 178, 13), // "switchFilters"
QT_MOC_LITERAL(19, 192, 2) // "On"

    },
    "AwFilteringManager\0filtersChanged\0\0"
    "noActiveFilters\0newICASettings\0type\0"
    "lp\0hp\0newSourceSettings\0filtersReset\0"
    "setFilter\0LP\0HP\0setICASettings\0"
    "setSourceSettings\0setNotch\0notch\0reset\0"
    "switchFilters\0On"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwFilteringManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    3,   71,    2, 0x06 /* Public */,
       8,    3,   78,    2, 0x06 /* Public */,
       9,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    3,   86,    2, 0x0a /* Public */,
      13,    3,   93,    2, 0x0a /* Public */,
      14,    3,  100,    2, 0x0a /* Public */,
      15,    2,  107,    2, 0x0a /* Public */,
      17,    0,  112,    2, 0x0a /* Public */,
      18,    1,  113,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    5,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    5,    6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    5,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    5,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    5,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    5,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   19,

       0        // eod
};

void AwFilteringManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwFilteringManager *_t = static_cast<AwFilteringManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filtersChanged(); break;
        case 1: _t->noActiveFilters(); break;
        case 2: _t->newICASettings((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 3: _t->newSourceSettings((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 4: _t->filtersReset(); break;
        case 5: _t->setFilter((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 6: _t->setICASettings((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 7: _t->setSourceSettings((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 8: _t->setNotch((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 9: _t->reset(); break;
        case 10: _t->switchFilters((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwFilteringManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilteringManager::filtersChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwFilteringManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilteringManager::noActiveFilters)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwFilteringManager::*_t)(int , float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilteringManager::newICASettings)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwFilteringManager::*_t)(int , float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilteringManager::newSourceSettings)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwFilteringManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilteringManager::filtersReset)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject AwFilteringManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwFilteringManager.data,
      qt_meta_data_AwFilteringManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwFilteringManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwFilteringManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwFilteringManager.stringdata0))
        return static_cast<void*>(const_cast< AwFilteringManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AwFilteringManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AwFilteringManager::filtersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwFilteringManager::noActiveFilters()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AwFilteringManager::newICASettings(int _t1, float _t2, float _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AwFilteringManager::newSourceSettings(int _t1, float _t2, float _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AwFilteringManager::filtersReset()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
