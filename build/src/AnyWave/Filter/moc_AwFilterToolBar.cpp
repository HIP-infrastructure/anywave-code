/****************************************************************************
** Meta object code from reading C++ file 'AwFilterToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Filter/AwFilterToolBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwFilterToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwFilterToolBar_t {
    QByteArrayData data[16];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwFilterToolBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwFilterToolBar_t qt_meta_stringdata_AwFilterToolBar = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwFilterToolBar"
QT_MOC_LITERAL(1, 16, 19), // "filterButtonClicked"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 16), // "filterICAClicked"
QT_MOC_LITERAL(4, 54, 6), // "toggle"
QT_MOC_LITERAL(5, 61, 16), // "ICASwitchChanged"
QT_MOC_LITERAL(6, 78, 5), // "onoff"
QT_MOC_LITERAL(7, 84, 12), // "applyFilters"
QT_MOC_LITERAL(8, 97, 18), // "enableICAFiltering"
QT_MOC_LITERAL(9, 116, 9), // "closeFile"
QT_MOC_LITERAL(10, 126, 15), // "setICAFiltering"
QT_MOC_LITERAL(11, 142, 10), // "setICAMode"
QT_MOC_LITERAL(12, 153, 6), // "filter"
QT_MOC_LITERAL(13, 160, 18), // "changeICAFiltering"
QT_MOC_LITERAL(14, 179, 6), // "button"
QT_MOC_LITERAL(15, 186, 7) // "checked"

    },
    "AwFilterToolBar\0filterButtonClicked\0"
    "\0filterICAClicked\0toggle\0ICASwitchChanged\0"
    "onoff\0applyFilters\0enableICAFiltering\0"
    "closeFile\0setICAFiltering\0setICAMode\0"
    "filter\0changeICAFiltering\0button\0"
    "checked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwFilterToolBar[] = {

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
       1,    0,   59,    2, 0x06 /* Public */,
       3,    1,   60,    2, 0x06 /* Public */,
       5,    1,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   66,    2, 0x0a /* Public */,
       8,    0,   67,    2, 0x0a /* Public */,
       9,    0,   68,    2, 0x0a /* Public */,
      10,    1,   69,    2, 0x0a /* Public */,
      11,    1,   72,    2, 0x0a /* Public */,
      13,    2,   75,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   14,   15,

       0        // eod
};

void AwFilterToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwFilterToolBar *_t = static_cast<AwFilterToolBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filterButtonClicked(); break;
        case 1: _t->filterICAClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->ICASwitchChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->applyFilters(); break;
        case 4: _t->enableICAFiltering(); break;
        case 5: _t->closeFile(); break;
        case 6: _t->setICAFiltering((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->setICAMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->changeICAFiltering((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwFilterToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilterToolBar::filterButtonClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwFilterToolBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilterToolBar::filterICAClicked)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwFilterToolBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilterToolBar::ICASwitchChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwFilterToolBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwFilterToolBar.data,
      qt_meta_data_AwFilterToolBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwFilterToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwFilterToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwFilterToolBar.stringdata0))
        return static_cast<void*>(const_cast< AwFilterToolBar*>(this));
    if (!strcmp(_clname, "Ui::AwFilterToolBarClass"))
        return static_cast< Ui::AwFilterToolBarClass*>(const_cast< AwFilterToolBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwFilterToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AwFilterToolBar::filterButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwFilterToolBar::filterICAClicked(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwFilterToolBar::ICASwitchChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
