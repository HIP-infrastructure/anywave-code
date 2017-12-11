/****************************************************************************
** Meta object code from reading C++ file 'AwDisplayToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Widgets/AwDisplayToolBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDisplayToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDisplayToolBar_t {
    QByteArrayData data[18];
    char stringdata0[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDisplayToolBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDisplayToolBar_t qt_meta_stringdata_AwDisplayToolBar = {
    {
QT_MOC_LITERAL(0, 0, 16), // "AwDisplayToolBar"
QT_MOC_LITERAL(1, 17, 14), // "addViewClicked"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 10), // "removeView"
QT_MOC_LITERAL(4, 44, 13), // "alignHClicked"
QT_MOC_LITERAL(5, 58, 13), // "alignVClicked"
QT_MOC_LITERAL(6, 72, 12), // "setupChanged"
QT_MOC_LITERAL(7, 85, 15), // "AwDisplaySetup*"
QT_MOC_LITERAL(8, 101, 5), // "setup"
QT_MOC_LITERAL(9, 107, 5), // "flags"
QT_MOC_LITERAL(10, 113, 14), // "captureClicked"
QT_MOC_LITERAL(11, 128, 11), // "updateSetup"
QT_MOC_LITERAL(12, 140, 11), // "setNewSetup"
QT_MOC_LITERAL(13, 152, 7), // "addView"
QT_MOC_LITERAL(14, 160, 15), // "setSynchronized"
QT_MOC_LITERAL(15, 176, 1), // "f"
QT_MOC_LITERAL(16, 178, 15), // "changeAlignment"
QT_MOC_LITERAL(17, 194, 5) // "index"

    },
    "AwDisplayToolBar\0addViewClicked\0\0"
    "removeView\0alignHClicked\0alignVClicked\0"
    "setupChanged\0AwDisplaySetup*\0setup\0"
    "flags\0captureClicked\0updateSetup\0"
    "setNewSetup\0addView\0setSynchronized\0"
    "f\0changeAlignment\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDisplayToolBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,
       4,    0,   71,    2, 0x06 /* Public */,
       5,    0,   72,    2, 0x06 /* Public */,
       6,    2,   73,    2, 0x06 /* Public */,
      10,    0,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    2,   79,    2, 0x0a /* Public */,
      12,    1,   84,    2, 0x0a /* Public */,
      13,    0,   87,    2, 0x0a /* Public */,
      14,    1,   88,    2, 0x09 /* Protected */,
      16,    1,   91,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int,    8,    9,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int,    8,    9,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   15,
    QMetaType::Void, QMetaType::Int,   17,

       0        // eod
};

void AwDisplayToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDisplayToolBar *_t = static_cast<AwDisplayToolBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addViewClicked(); break;
        case 1: _t->removeView(); break;
        case 2: _t->alignHClicked(); break;
        case 3: _t->alignVClicked(); break;
        case 4: _t->setupChanged((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->captureClicked(); break;
        case 6: _t->updateSetup((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->setNewSetup((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1]))); break;
        case 8: _t->addView(); break;
        case 9: _t->setSynchronized((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->changeAlignment((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwDisplaySetup* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwDisplaySetup* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwDisplaySetup* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwDisplayToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplayToolBar::addViewClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwDisplayToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplayToolBar::removeView)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwDisplayToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplayToolBar::alignHClicked)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwDisplayToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplayToolBar::alignVClicked)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwDisplayToolBar::*_t)(AwDisplaySetup * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplayToolBar::setupChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AwDisplayToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplayToolBar::captureClicked)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject AwDisplayToolBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwDisplayToolBar.data,
      qt_meta_data_AwDisplayToolBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDisplayToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDisplayToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDisplayToolBar.stringdata0))
        return static_cast<void*>(const_cast< AwDisplayToolBar*>(this));
    if (!strcmp(_clname, "Ui::AwDisplayToolBarClassv2"))
        return static_cast< Ui::AwDisplayToolBarClassv2*>(const_cast< AwDisplayToolBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwDisplayToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void AwDisplayToolBar::addViewClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwDisplayToolBar::removeView()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AwDisplayToolBar::alignHClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void AwDisplayToolBar::alignVClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void AwDisplayToolBar::setupChanged(AwDisplaySetup * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AwDisplayToolBar::captureClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
