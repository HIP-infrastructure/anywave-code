/****************************************************************************
** Meta object code from reading C++ file 'AwICAComponents.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/ICA/AwICAComponents.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwICAComponents.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwICAComponents_t {
    QByteArrayData data[12];
    char stringdata0[153];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwICAComponents_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwICAComponents_t qt_meta_stringdata_AwICAComponents = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwICAComponents"
QT_MOC_LITERAL(1, 16, 17), // "componentRejected"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 5), // "index"
QT_MOC_LITERAL(4, 41, 14), // "componentAdded"
QT_MOC_LITERAL(5, 56, 16), // "filteringChecked"
QT_MOC_LITERAL(6, 73, 7), // "checked"
QT_MOC_LITERAL(7, 81, 9), // "showPanel"
QT_MOC_LITERAL(8, 91, 13), // "updateFilters"
QT_MOC_LITERAL(9, 105, 17), // "switchFilteringOn"
QT_MOC_LITERAL(10, 123, 20), // "updateComponentState"
QT_MOC_LITERAL(11, 144, 8) // "rejected"

    },
    "AwICAComponents\0componentRejected\0\0"
    "index\0componentAdded\0filteringChecked\0"
    "checked\0showPanel\0updateFilters\0"
    "switchFilteringOn\0updateComponentState\0"
    "rejected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwICAComponents[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    1,   52,    2, 0x06 /* Public */,
       5,    1,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   58,    2, 0x0a /* Public */,
       8,    0,   59,    2, 0x0a /* Public */,
       9,    0,   60,    2, 0x09 /* Protected */,
      10,    2,   61,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Bool,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,   11,

       0        // eod
};

void AwICAComponents::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwICAComponents *_t = static_cast<AwICAComponents *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->componentRejected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->componentAdded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->filteringChecked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->showPanel(); break;
        case 4: _t->updateFilters(); break;
        case 5: _t->switchFilteringOn(); break;
        case 6: _t->updateComponentState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwICAComponents::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwICAComponents::componentRejected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwICAComponents::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwICAComponents::componentAdded)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwICAComponents::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwICAComponents::filteringChecked)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwICAComponents::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwICAComponents.data,
      qt_meta_data_AwICAComponents,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwICAComponents::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwICAComponents::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwICAComponents.stringdata0))
        return static_cast<void*>(const_cast< AwICAComponents*>(this));
    return QObject::qt_metacast(_clname);
}

int AwICAComponents::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void AwICAComponents::componentRejected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwICAComponents::componentAdded(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwICAComponents::filteringChecked(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
