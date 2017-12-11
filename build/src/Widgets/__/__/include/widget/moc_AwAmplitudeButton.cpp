/****************************************************************************
** Meta object code from reading C++ file 'AwAmplitudeButton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../include/widget/AwAmplitudeButton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwAmplitudeButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwAmplitudeButton_t {
    QByteArrayData data[12];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwAmplitudeButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwAmplitudeButton_t qt_meta_stringdata_AwAmplitudeButton = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AwAmplitudeButton"
QT_MOC_LITERAL(1, 18, 16), // "amplitudeChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 17), // "amplitudesChanged"
QT_MOC_LITERAL(4, 54, 15), // "changeAmplitude"
QT_MOC_LITERAL(5, 70, 7), // "checked"
QT_MOC_LITERAL(6, 78, 13), // "setAmplitudes"
QT_MOC_LITERAL(7, 92, 12), // "setAmplitude"
QT_MOC_LITERAL(8, 105, 2), // "up"
QT_MOC_LITERAL(9, 108, 4), // "down"
QT_MOC_LITERAL(10, 113, 24), // "switchGainLevelToChannel"
QT_MOC_LITERAL(11, 138, 4) // "type"

    },
    "AwAmplitudeButton\0amplitudeChanged\0\0"
    "amplitudesChanged\0changeAmplitude\0"
    "checked\0setAmplitudes\0setAmplitude\0"
    "up\0down\0switchGainLevelToChannel\0type"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwAmplitudeButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       3,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   60,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x0a /* Public */,
       7,    2,   64,    2, 0x0a /* Public */,
       8,    0,   69,    2, 0x0a /* Public */,
       9,    0,   70,    2, 0x0a /* Public */,
      10,    1,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void AwAmplitudeButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwAmplitudeButton *_t = static_cast<AwAmplitudeButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->amplitudeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->amplitudesChanged(); break;
        case 2: _t->changeAmplitude((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setAmplitudes(); break;
        case 4: _t->setAmplitude((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 5: _t->up(); break;
        case 6: _t->down(); break;
        case 7: _t->switchGainLevelToChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwAmplitudeButton::*_t)(int , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwAmplitudeButton::amplitudeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwAmplitudeButton::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwAmplitudeButton::amplitudesChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwAmplitudeButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_AwAmplitudeButton.data,
      qt_meta_data_AwAmplitudeButton,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwAmplitudeButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwAmplitudeButton::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwAmplitudeButton.stringdata0))
        return static_cast<void*>(const_cast< AwAmplitudeButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int AwAmplitudeButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AwAmplitudeButton::amplitudeChanged(int _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwAmplitudeButton::amplitudesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
