/****************************************************************************
** Meta object code from reading C++ file 'AwDisplaySetupToolBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Widgets/AwDisplaySetupToolBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDisplaySetupToolBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDisplaySetupToolBar_t {
    QByteArrayData data[8];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDisplaySetupToolBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDisplaySetupToolBar_t qt_meta_stringdata_AwDisplaySetupToolBar = {
    {
QT_MOC_LITERAL(0, 0, 21), // "AwDisplaySetupToolBar"
QT_MOC_LITERAL(1, 22, 12), // "setupChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 4), // "name"
QT_MOC_LITERAL(4, 41, 23), // "currentSetupSaveRequest"
QT_MOC_LITERAL(5, 65, 25), // "currentSetupWillBeDeteted"
QT_MOC_LITERAL(6, 91, 15), // "setCurrentSetup"
QT_MOC_LITERAL(7, 107, 11) // "retranslate"

    },
    "AwDisplaySetupToolBar\0setupChanged\0\0"
    "name\0currentSetupSaveRequest\0"
    "currentSetupWillBeDeteted\0setCurrentSetup\0"
    "retranslate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDisplaySetupToolBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    0,   42,    2, 0x06 /* Public */,
       5,    0,   43,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   44,    2, 0x0a /* Public */,
       7,    0,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void AwDisplaySetupToolBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDisplaySetupToolBar *_t = static_cast<AwDisplaySetupToolBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setupChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->currentSetupSaveRequest(); break;
        case 2: _t->currentSetupWillBeDeteted(); break;
        case 3: _t->setCurrentSetup((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->retranslate(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwDisplaySetupToolBar::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplaySetupToolBar::setupChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwDisplaySetupToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplaySetupToolBar::currentSetupSaveRequest)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwDisplaySetupToolBar::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplaySetupToolBar::currentSetupWillBeDeteted)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwDisplaySetupToolBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwDisplaySetupToolBar.data,
      qt_meta_data_AwDisplaySetupToolBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDisplaySetupToolBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDisplaySetupToolBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDisplaySetupToolBar.stringdata0))
        return static_cast<void*>(const_cast< AwDisplaySetupToolBar*>(this));
    if (!strcmp(_clname, "Ui::AwDisplaySetupToolBarUI"))
        return static_cast< Ui::AwDisplaySetupToolBarUI*>(const_cast< AwDisplaySetupToolBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwDisplaySetupToolBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AwDisplaySetupToolBar::setupChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwDisplaySetupToolBar::currentSetupSaveRequest()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AwDisplaySetupToolBar::currentSetupWillBeDeteted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
