/****************************************************************************
** Meta object code from reading C++ file 'AwScript.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Script/AwScript.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwScript.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwScript_t {
    QByteArrayData data[15];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwScript_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwScript_t qt_meta_stringdata_AwScript = {
    {
QT_MOC_LITERAL(0, 0, 8), // "AwScript"
QT_MOC_LITERAL(1, 9, 5), // "error"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 7), // "message"
QT_MOC_LITERAL(4, 24, 7), // "warning"
QT_MOC_LITERAL(5, 32, 8), // "finished"
QT_MOC_LITERAL(6, 41, 14), // "processMessage"
QT_MOC_LITERAL(7, 56, 3), // "run"
QT_MOC_LITERAL(8, 60, 10), // "getProcess"
QT_MOC_LITERAL(9, 71, 12), // "QScriptValue"
QT_MOC_LITERAL(10, 84, 4), // "name"
QT_MOC_LITERAL(11, 89, 12), // "getFileInput"
QT_MOC_LITERAL(12, 102, 10), // "runProcess"
QT_MOC_LITERAL(13, 113, 7), // "process"
QT_MOC_LITERAL(14, 121, 9) // "fileInput"

    },
    "AwScript\0error\0\0message\0warning\0"
    "finished\0processMessage\0run\0getProcess\0"
    "QScriptValue\0name\0getFileInput\0"
    "runProcess\0process\0fileInput"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwScript[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       3,    1,   62,    2, 0x06 /* Public */,
       4,    1,   65,    2, 0x06 /* Public */,
       5,    0,   68,    2, 0x06 /* Public */,
       6,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   72,    2, 0x0a /* Public */,
       8,    1,   73,    2, 0x0a /* Public */,
      11,    0,   76,    2, 0x0a /* Public */,
      12,    2,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    0x80000000 | 9, QMetaType::QString,   10,
    0x80000000 | 9,
    QMetaType::Void, 0x80000000 | 9, 0x80000000 | 9,   13,   14,

       0        // eod
};

void AwScript::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwScript *_t = static_cast<AwScript *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->message((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->warning((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->finished(); break;
        case 4: _t->processMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->run(); break;
        case 6: { QScriptValue _r = _t->getProcess((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QScriptValue*>(_a[0]) = _r; }  break;
        case 7: { QScriptValue _r = _t->getFileInput();
            if (_a[0]) *reinterpret_cast< QScriptValue*>(_a[0]) = _r; }  break;
        case 8: _t->runProcess((*reinterpret_cast< QScriptValue(*)>(_a[1])),(*reinterpret_cast< QScriptValue(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwScript::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScript::error)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwScript::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScript::message)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwScript::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScript::warning)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwScript::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScript::finished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwScript::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScript::processMessage)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject AwScript::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwScript.data,
      qt_meta_data_AwScript,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwScript::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwScript::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwScript.stringdata0))
        return static_cast<void*>(const_cast< AwScript*>(this));
    return QObject::qt_metacast(_clname);
}

int AwScript::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void AwScript::error(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwScript::message(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwScript::warning(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AwScript::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void AwScript::processMessage(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
