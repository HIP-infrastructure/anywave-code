/****************************************************************************
** Meta object code from reading C++ file 'AwProcessesWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Process/AwProcessesWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwProcessesWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwProcessesWidget_t {
    QByteArrayData data[6];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwProcessesWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwProcessesWidget_t qt_meta_stringdata_AwProcessesWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AwProcessesWidget"
QT_MOC_LITERAL(1, 18, 12), // "removeWidget"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "AwProcessWidget*"
QT_MOC_LITERAL(4, 49, 2), // "pw"
QT_MOC_LITERAL(5, 52, 9) // "clearList"

    },
    "AwProcessesWidget\0removeWidget\0\0"
    "AwProcessWidget*\0pw\0clearList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwProcessesWidget[] = {

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
       5,    0,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void AwProcessesWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwProcessesWidget *_t = static_cast<AwProcessesWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->removeWidget((*reinterpret_cast< AwProcessWidget*(*)>(_a[1]))); break;
        case 1: _t->clearList(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwProcessWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject AwProcessesWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwProcessesWidget.data,
      qt_meta_data_AwProcessesWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwProcessesWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwProcessesWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwProcessesWidget.stringdata0))
        return static_cast<void*>(const_cast< AwProcessesWidget*>(this));
    if (!strcmp(_clname, "Ui::AwProcessesWidgetUi"))
        return static_cast< Ui::AwProcessesWidgetUi*>(const_cast< AwProcessesWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwProcessesWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
