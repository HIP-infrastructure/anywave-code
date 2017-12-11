/****************************************************************************
** Meta object code from reading C++ file 'AwProcessLog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Process/AwProcessLog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwProcessLog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwProcessLog_t {
    QByteArrayData data[9];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwProcessLog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwProcessLog_t qt_meta_stringdata_AwProcessLog = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwProcessLog"
QT_MOC_LITERAL(1, 13, 8), // "finished"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "addError"
QT_MOC_LITERAL(4, 32, 7), // "message"
QT_MOC_LITERAL(5, 40, 10), // "addWarning"
QT_MOC_LITERAL(6, 51, 10), // "addMessage"
QT_MOC_LITERAL(7, 62, 17), // "addProcessMessage"
QT_MOC_LITERAL(8, 80, 8) // "closeLog"

    },
    "AwProcessLog\0finished\0\0addError\0message\0"
    "addWarning\0addMessage\0addProcessMessage\0"
    "closeLog"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwProcessLog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   45,    2, 0x08 /* Private */,
       5,    1,   48,    2, 0x08 /* Private */,
       6,    1,   51,    2, 0x08 /* Private */,
       7,    1,   54,    2, 0x08 /* Private */,
       8,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,

       0        // eod
};

void AwProcessLog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwProcessLog *_t = static_cast<AwProcessLog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finished(); break;
        case 1: _t->addError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->addWarning((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->addMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->addProcessMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->closeLog(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwProcessLog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcessLog::finished)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwProcessLog::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwProcessLog.data,
      qt_meta_data_AwProcessLog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwProcessLog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwProcessLog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwProcessLog.stringdata0))
        return static_cast<void*>(const_cast< AwProcessLog*>(this));
    return QObject::qt_metacast(_clname);
}

int AwProcessLog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AwProcessLog::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
