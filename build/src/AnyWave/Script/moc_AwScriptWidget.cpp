/****************************************************************************
** Meta object code from reading C++ file 'AwScriptWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Script/AwScriptWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwScriptWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwScriptWidget_t {
    QByteArrayData data[15];
    char stringdata0[152];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwScriptWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwScriptWidget_t qt_meta_stringdata_AwScriptWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AwScriptWidget"
QT_MOC_LITERAL(1, 15, 7), // "removed"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 13), // "removeClicked"
QT_MOC_LITERAL(4, 38, 15), // "AwScriptWidget*"
QT_MOC_LITERAL(5, 54, 6), // "widget"
QT_MOC_LITERAL(6, 61, 10), // "setRunning"
QT_MOC_LITERAL(7, 72, 11), // "setFinished"
QT_MOC_LITERAL(8, 84, 10), // "setMessage"
QT_MOC_LITERAL(9, 95, 7), // "message"
QT_MOC_LITERAL(10, 103, 4), // "stop"
QT_MOC_LITERAL(11, 108, 9), // "updateLog"
QT_MOC_LITERAL(12, 118, 7), // "showLog"
QT_MOC_LITERAL(13, 126, 8), // "removeMe"
QT_MOC_LITERAL(14, 135, 16) // "makeShowLogBlink"

    },
    "AwScriptWidget\0removed\0\0removeClicked\0"
    "AwScriptWidget*\0widget\0setRunning\0"
    "setFinished\0setMessage\0message\0stop\0"
    "updateLog\0showLog\0removeMe\0makeShowLogBlink"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwScriptWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   68,    2, 0x0a /* Public */,
       7,    0,   69,    2, 0x0a /* Public */,
       8,    1,   70,    2, 0x0a /* Public */,
      10,    0,   73,    2, 0x0a /* Public */,
      11,    0,   74,    2, 0x0a /* Public */,
      12,    0,   75,    2, 0x0a /* Public */,
      13,    0,   76,    2, 0x0a /* Public */,
      14,    0,   77,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwScriptWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwScriptWidget *_t = static_cast<AwScriptWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->removed(); break;
        case 1: _t->removeClicked((*reinterpret_cast< AwScriptWidget*(*)>(_a[1]))); break;
        case 2: _t->setRunning(); break;
        case 3: _t->setFinished(); break;
        case 4: _t->setMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->stop(); break;
        case 6: _t->updateLog(); break;
        case 7: _t->showLog(); break;
        case 8: _t->removeMe(); break;
        case 9: _t->makeShowLogBlink(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwScriptWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwScriptWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScriptWidget::removed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwScriptWidget::*_t)(AwScriptWidget * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScriptWidget::removeClicked)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwScriptWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwScriptWidget.data,
      qt_meta_data_AwScriptWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwScriptWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwScriptWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwScriptWidget.stringdata0))
        return static_cast<void*>(const_cast< AwScriptWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwScriptWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AwScriptWidget::removed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwScriptWidget::removeClicked(AwScriptWidget * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
