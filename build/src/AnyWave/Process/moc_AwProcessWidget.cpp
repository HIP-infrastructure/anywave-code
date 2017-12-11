/****************************************************************************
** Meta object code from reading C++ file 'AwProcessWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Process/AwProcessWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwProcessWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwProcessWidget_t {
    QByteArrayData data[19];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwProcessWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwProcessWidget_t qt_meta_stringdata_AwProcessWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwProcessWidget"
QT_MOC_LITERAL(1, 16, 7), // "removed"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 13), // "removeClicked"
QT_MOC_LITERAL(4, 39, 16), // "AwProcessWidget*"
QT_MOC_LITERAL(5, 56, 6), // "widget"
QT_MOC_LITERAL(6, 63, 7), // "setIdle"
QT_MOC_LITERAL(7, 71, 10), // "setRunning"
QT_MOC_LITERAL(8, 82, 11), // "setFinished"
QT_MOC_LITERAL(9, 94, 10), // "setMessage"
QT_MOC_LITERAL(10, 105, 7), // "message"
QT_MOC_LITERAL(11, 113, 14), // "setProgression"
QT_MOC_LITERAL(12, 128, 5), // "value"
QT_MOC_LITERAL(13, 134, 4), // "stop"
QT_MOC_LITERAL(14, 139, 11), // "showResults"
QT_MOC_LITERAL(15, 151, 9), // "updateLog"
QT_MOC_LITERAL(16, 161, 7), // "showLog"
QT_MOC_LITERAL(17, 169, 8), // "removeMe"
QT_MOC_LITERAL(18, 178, 16) // "makeShowLogBlink"

    },
    "AwProcessWidget\0removed\0\0removeClicked\0"
    "AwProcessWidget*\0widget\0setIdle\0"
    "setRunning\0setFinished\0setMessage\0"
    "message\0setProgression\0value\0stop\0"
    "showResults\0updateLog\0showLog\0removeMe\0"
    "makeShowLogBlink"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwProcessWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   83,    2, 0x0a /* Public */,
       7,    0,   84,    2, 0x0a /* Public */,
       8,    0,   85,    2, 0x0a /* Public */,
       9,    1,   86,    2, 0x0a /* Public */,
      11,    1,   89,    2, 0x0a /* Public */,
      13,    0,   92,    2, 0x0a /* Public */,
      14,    0,   93,    2, 0x0a /* Public */,
      15,    0,   94,    2, 0x0a /* Public */,
      16,    0,   95,    2, 0x0a /* Public */,
      17,    0,   96,    2, 0x0a /* Public */,
      18,    0,   97,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwProcessWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwProcessWidget *_t = static_cast<AwProcessWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->removed(); break;
        case 1: _t->removeClicked((*reinterpret_cast< AwProcessWidget*(*)>(_a[1]))); break;
        case 2: _t->setIdle(); break;
        case 3: _t->setRunning(); break;
        case 4: _t->setFinished(); break;
        case 5: _t->setMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->setProgression((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->stop(); break;
        case 8: _t->showResults(); break;
        case 9: _t->updateLog(); break;
        case 10: _t->showLog(); break;
        case 11: _t->removeMe(); break;
        case 12: _t->makeShowLogBlink(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwProcessWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwProcessWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcessWidget::removed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwProcessWidget::*_t)(AwProcessWidget * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwProcessWidget::removeClicked)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwProcessWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwProcessWidget.data,
      qt_meta_data_AwProcessWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwProcessWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwProcessWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwProcessWidget.stringdata0))
        return static_cast<void*>(const_cast< AwProcessWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwProcessWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void AwProcessWidget::removed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwProcessWidget::removeClicked(AwProcessWidget * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
