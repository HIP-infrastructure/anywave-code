/****************************************************************************
** Meta object code from reading C++ file 'AwDebugLogWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Debug/AwDebugLogWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDebugLogWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDebugLogWidget_t {
    QByteArrayData data[5];
    char stringdata0[42];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDebugLogWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDebugLogWidget_t qt_meta_stringdata_AwDebugLogWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "AwDebugLogWidget"
QT_MOC_LITERAL(1, 17, 10), // "updateLogs"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 4), // "type"
QT_MOC_LITERAL(4, 34, 7) // "refresh"

    },
    "AwDebugLogWidget\0updateLogs\0\0type\0"
    "refresh"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDebugLogWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    0,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void AwDebugLogWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDebugLogWidget *_t = static_cast<AwDebugLogWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateLogs((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->refresh(); break;
        default: ;
        }
    }
}

const QMetaObject AwDebugLogWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwDebugLogWidget.data,
      qt_meta_data_AwDebugLogWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDebugLogWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDebugLogWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDebugLogWidget.stringdata0))
        return static_cast<void*>(const_cast< AwDebugLogWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwDebugLogWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
