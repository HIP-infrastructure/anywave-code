/****************************************************************************
** Meta object code from reading C++ file 'AwPythonScriptPlugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Process/AwPythonScriptPlugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwPythonScriptPlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwPythonScriptProcess_t {
    QByteArrayData data[6];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwPythonScriptProcess_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwPythonScriptProcess_t qt_meta_stringdata_AwPythonScriptProcess = {
    {
QT_MOC_LITERAL(0, 0, 21), // "AwPythonScriptProcess"
QT_MOC_LITERAL(1, 22, 12), // "pythonOutput"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 11), // "pythonError"
QT_MOC_LITERAL(4, 48, 5), // "error"
QT_MOC_LITERAL(5, 54, 22) // "QProcess::ProcessError"

    },
    "AwPythonScriptProcess\0pythonOutput\0\0"
    "pythonError\0error\0QProcess::ProcessError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwPythonScriptProcess[] = {

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
       3,    0,   30,    2, 0x0a /* Public */,
       4,    1,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    4,

       0        // eod
};

void AwPythonScriptProcess::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwPythonScriptProcess *_t = static_cast<AwPythonScriptProcess *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pythonOutput(); break;
        case 1: _t->pythonError(); break;
        case 2: _t->error((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AwPythonScriptProcess::staticMetaObject = {
    { &AwScriptProcess::staticMetaObject, qt_meta_stringdata_AwPythonScriptProcess.data,
      qt_meta_data_AwPythonScriptProcess,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwPythonScriptProcess::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwPythonScriptProcess::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwPythonScriptProcess.stringdata0))
        return static_cast<void*>(const_cast< AwPythonScriptProcess*>(this));
    return AwScriptProcess::qt_metacast(_clname);
}

int AwPythonScriptProcess::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwScriptProcess::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_AwPythonScriptPlugin_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwPythonScriptPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwPythonScriptPlugin_t qt_meta_stringdata_AwPythonScriptPlugin = {
    {
QT_MOC_LITERAL(0, 0, 20) // "AwPythonScriptPlugin"

    },
    "AwPythonScriptPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwPythonScriptPlugin[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AwPythonScriptPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AwPythonScriptPlugin::staticMetaObject = {
    { &AwScriptPlugin::staticMetaObject, qt_meta_stringdata_AwPythonScriptPlugin.data,
      qt_meta_data_AwPythonScriptPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwPythonScriptPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwPythonScriptPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwPythonScriptPlugin.stringdata0))
        return static_cast<void*>(const_cast< AwPythonScriptPlugin*>(this));
    return AwScriptPlugin::qt_metacast(_clname);
}

int AwPythonScriptPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwScriptPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
