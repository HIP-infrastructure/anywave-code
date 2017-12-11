/****************************************************************************
** Meta object code from reading C++ file 'AwMarkerRule.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Marker/AwMarkerRule.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwMarkerRule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwMarkerRule_t {
    QByteArrayData data[5];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwMarkerRule_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwMarkerRule_t qt_meta_stringdata_AwMarkerRule = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwMarkerRule"
QT_MOC_LITERAL(1, 13, 10), // "addElement"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 20), // "AwMarkerRuleElement*"
QT_MOC_LITERAL(4, 46, 4) // "elem"

    },
    "AwMarkerRule\0addElement\0\0AwMarkerRuleElement*\0"
    "elem"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwMarkerRule[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void AwMarkerRule::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwMarkerRule *_t = static_cast<AwMarkerRule *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addElement((*reinterpret_cast< AwMarkerRuleElement*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AwMarkerRule::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwMarkerRule.data,
      qt_meta_data_AwMarkerRule,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwMarkerRule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwMarkerRule::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwMarkerRule.stringdata0))
        return static_cast<void*>(const_cast< AwMarkerRule*>(this));
    return QObject::qt_metacast(_clname);
}

int AwMarkerRule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
