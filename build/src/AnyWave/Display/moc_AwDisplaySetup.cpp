/****************************************************************************
** Meta object code from reading C++ file 'AwDisplaySetup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Display/AwDisplaySetup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDisplaySetup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDisplaySetup_t {
    QByteArrayData data[7];
    char stringdata0[55];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDisplaySetup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDisplaySetup_t qt_meta_stringdata_AwDisplaySetup = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AwDisplaySetup"
QT_MOC_LITERAL(1, 15, 4), // "load"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "loadFromFile"
QT_MOC_LITERAL(4, 34, 4), // "path"
QT_MOC_LITERAL(5, 39, 4), // "save"
QT_MOC_LITERAL(6, 44, 10) // "saveToFile"

    },
    "AwDisplaySetup\0load\0\0loadFromFile\0"
    "path\0save\0saveToFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDisplaySetup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x0a /* Public */,
       3,    1,   35,    2, 0x0a /* Public */,
       5,    0,   38,    2, 0x0a /* Public */,
       6,    1,   39,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString,    4,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString,    4,

       0        // eod
};

void AwDisplaySetup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDisplaySetup *_t = static_cast<AwDisplaySetup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { bool _r = _t->load();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 1: { bool _r = _t->loadFromFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 2: { bool _r = _t->save();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->saveToFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject AwDisplaySetup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwDisplaySetup.data,
      qt_meta_data_AwDisplaySetup,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDisplaySetup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDisplaySetup::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDisplaySetup.stringdata0))
        return static_cast<void*>(const_cast< AwDisplaySetup*>(this));
    return QObject::qt_metacast(_clname);
}

int AwDisplaySetup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
