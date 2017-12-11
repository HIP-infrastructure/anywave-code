/****************************************************************************
** Meta object code from reading C++ file 'AwPanelItem.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/ICA/AwPanelItem.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwPanelItem.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwPanelItem_t {
    QByteArrayData data[15];
    char stringdata0[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwPanelItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwPanelItem_t qt_meta_stringdata_AwPanelItem = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AwPanelItem"
QT_MOC_LITERAL(1, 12, 21), // "selectedLabelsChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 6), // "labels"
QT_MOC_LITERAL(4, 42, 16), // "itemStateChanged"
QT_MOC_LITERAL(5, 59, 5), // "index"
QT_MOC_LITERAL(6, 65, 8), // "rejected"
QT_MOC_LITERAL(7, 74, 5), // "label"
QT_MOC_LITERAL(8, 80, 17), // "showSignalClicked"
QT_MOC_LITERAL(9, 98, 8), // "selected"
QT_MOC_LITERAL(10, 107, 19), // "udpateRejectedState"
QT_MOC_LITERAL(11, 127, 10), // "showSignal"
QT_MOC_LITERAL(12, 138, 6), // "select"
QT_MOC_LITERAL(13, 145, 4), // "flag"
QT_MOC_LITERAL(14, 150, 10) // "switch2D3D"

    },
    "AwPanelItem\0selectedLabelsChanged\0\0"
    "labels\0itemStateChanged\0index\0rejected\0"
    "label\0showSignalClicked\0selected\0"
    "udpateRejectedState\0showSignal\0select\0"
    "flag\0switch2D3D"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwPanelItem[] = {

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
       4,    2,   62,    2, 0x06 /* Public */,
       4,    2,   67,    2, 0x06 /* Public */,
       8,    1,   72,    2, 0x06 /* Public */,
       9,    2,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   80,    2, 0x08 /* Private */,
      11,    0,   81,    2, 0x08 /* Private */,
      12,    1,   82,    2, 0x08 /* Private */,
      14,    1,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    7,    6,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    7,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   13,
    QMetaType::Void, QMetaType::Bool,   13,

       0        // eod
};

void AwPanelItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwPanelItem *_t = static_cast<AwPanelItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectedLabelsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->itemStateChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->itemStateChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->showSignalClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->selected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->udpateRejectedState(); break;
        case 6: _t->showSignal(); break;
        case 7: _t->select((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->switch2D3D((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwPanelItem::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPanelItem::selectedLabelsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwPanelItem::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPanelItem::itemStateChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwPanelItem::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPanelItem::itemStateChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwPanelItem::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPanelItem::showSignalClicked)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwPanelItem::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPanelItem::selected)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject AwPanelItem::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwPanelItem.data,
      qt_meta_data_AwPanelItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwPanelItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwPanelItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwPanelItem.stringdata0))
        return static_cast<void*>(const_cast< AwPanelItem*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwPanelItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void AwPanelItem::selectedLabelsChanged(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwPanelItem::itemStateChanged(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwPanelItem::itemStateChanged(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AwPanelItem::showSignalClicked(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AwPanelItem::selected(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
