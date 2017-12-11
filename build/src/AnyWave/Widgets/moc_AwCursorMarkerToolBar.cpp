/****************************************************************************
** Meta object code from reading C++ file 'AwCursorMarkerToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Widgets/AwCursorMarkerToolBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwCursorMarkerToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwCursorMarkerToolBar_t {
    QByteArrayData data[12];
    char stringdata0[154];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwCursorMarkerToolBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwCursorMarkerToolBar_t qt_meta_stringdata_AwCursorMarkerToolBar = {
    {
QT_MOC_LITERAL(0, 0, 21), // "AwCursorMarkerToolBar"
QT_MOC_LITERAL(1, 22, 17), // "markerModeChanged"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 17), // "cursorModeChanged"
QT_MOC_LITERAL(4, 59, 14), // "QTSModeChanged"
QT_MOC_LITERAL(5, 74, 5), // "reset"
QT_MOC_LITERAL(6, 80, 15), // "setCursorModeOn"
QT_MOC_LITERAL(7, 96, 4), // "flag"
QT_MOC_LITERAL(8, 101, 15), // "setMarkerModeOn"
QT_MOC_LITERAL(9, 117, 12), // "setQTSModeOn"
QT_MOC_LITERAL(10, 130, 11), // "retranslate"
QT_MOC_LITERAL(11, 142, 11) // "stopQTSMode"

    },
    "AwCursorMarkerToolBar\0markerModeChanged\0"
    "\0cursorModeChanged\0QTSModeChanged\0"
    "reset\0setCursorModeOn\0flag\0setMarkerModeOn\0"
    "setQTSModeOn\0retranslate\0stopQTSMode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwCursorMarkerToolBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       3,    1,   62,    2, 0x06 /* Public */,
       4,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   68,    2, 0x0a /* Public */,
       6,    1,   69,    2, 0x0a /* Public */,
       8,    1,   72,    2, 0x0a /* Public */,
       9,    1,   75,    2, 0x0a /* Public */,
      10,    0,   78,    2, 0x0a /* Public */,
      11,    0,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwCursorMarkerToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwCursorMarkerToolBar *_t = static_cast<AwCursorMarkerToolBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markerModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->cursorModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->QTSModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->reset(); break;
        case 4: _t->setCursorModeOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setMarkerModeOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->setQTSModeOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->retranslate(); break;
        case 8: _t->stopQTSMode(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwCursorMarkerToolBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwCursorMarkerToolBar::markerModeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwCursorMarkerToolBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwCursorMarkerToolBar::cursorModeChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwCursorMarkerToolBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwCursorMarkerToolBar::QTSModeChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwCursorMarkerToolBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwCursorMarkerToolBar.data,
      qt_meta_data_AwCursorMarkerToolBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwCursorMarkerToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwCursorMarkerToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwCursorMarkerToolBar.stringdata0))
        return static_cast<void*>(const_cast< AwCursorMarkerToolBar*>(this));
    if (!strcmp(_clname, "Ui::AwCursorMarkerToolBarClass"))
        return static_cast< Ui::AwCursorMarkerToolBarClass*>(const_cast< AwCursorMarkerToolBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwCursorMarkerToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AwCursorMarkerToolBar::markerModeChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwCursorMarkerToolBar::cursorModeChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwCursorMarkerToolBar::QTSModeChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
