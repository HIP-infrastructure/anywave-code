/****************************************************************************
** Meta object code from reading C++ file 'AwScriptManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Script/AwScriptManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwScriptManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwScriptManager_t {
    QByteArrayData data[5];
    char stringdata0[50];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwScriptManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwScriptManager_t qt_meta_stringdata_AwScriptManager = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwScriptManager"
QT_MOC_LITERAL(1, 16, 9), // "runScript"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "scriptPath"
QT_MOC_LITERAL(4, 38, 11) // "endOfScript"

    },
    "AwScriptManager\0runScript\0\0scriptPath\0"
    "endOfScript"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwScriptManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       1,    1,   30,    2, 0x0a /* Public */,
       4,    0,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void AwScriptManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwScriptManager *_t = static_cast<AwScriptManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->runScript(); break;
        case 1: _t->runScript((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->endOfScript(); break;
        default: ;
        }
    }
}

const QMetaObject AwScriptManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwScriptManager.data,
      qt_meta_data_AwScriptManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwScriptManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwScriptManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwScriptManager.stringdata0))
        return static_cast<void*>(const_cast< AwScriptManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AwScriptManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
