/****************************************************************************
** Meta object code from reading C++ file 'AwAmplitudeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../include/widget/AwAmplitudeWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwAmplitudeWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwAmplitudeWidget_t {
    QByteArrayData data[12];
    char stringdata0[154];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwAmplitudeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwAmplitudeWidget_t qt_meta_stringdata_AwAmplitudeWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AwAmplitudeWidget"
QT_MOC_LITERAL(1, 18, 17), // "amplitudesChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 16), // "amplitudeChanged"
QT_MOC_LITERAL(4, 54, 21), // "changeLevelForChannel"
QT_MOC_LITERAL(5, 76, 4), // "type"
QT_MOC_LITERAL(6, 81, 2), // "up"
QT_MOC_LITERAL(7, 84, 4), // "down"
QT_MOC_LITERAL(8, 89, 32), // "changeCurrentChannelTypeAndValue"
QT_MOC_LITERAL(9, 122, 9), // "amplitude"
QT_MOC_LITERAL(10, 132, 15), // "changeAmplitude"
QT_MOC_LITERAL(11, 148, 5) // "value"

    },
    "AwAmplitudeWidget\0amplitudesChanged\0"
    "\0amplitudeChanged\0changeLevelForChannel\0"
    "type\0up\0down\0changeCurrentChannelTypeAndValue\0"
    "amplitude\0changeAmplitude\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwAmplitudeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    2,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   55,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    2,   60,    2, 0x0a /* Public */,
      10,    1,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    5,    9,
    QMetaType::Void, QMetaType::Double,   11,

       0        // eod
};

void AwAmplitudeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwAmplitudeWidget *_t = static_cast<AwAmplitudeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->amplitudesChanged(); break;
        case 1: _t->amplitudeChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 2: _t->changeLevelForChannel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->up(); break;
        case 4: _t->down(); break;
        case 5: _t->changeCurrentChannelTypeAndValue((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 6: _t->changeAmplitude((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwAmplitudeWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwAmplitudeWidget::amplitudesChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwAmplitudeWidget::*_t)(int , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwAmplitudeWidget::amplitudeChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwAmplitudeWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwAmplitudeWidget.data,
      qt_meta_data_AwAmplitudeWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwAmplitudeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwAmplitudeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwAmplitudeWidget.stringdata0))
        return static_cast<void*>(const_cast< AwAmplitudeWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwAmplitudeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void AwAmplitudeWidget::amplitudesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwAmplitudeWidget::amplitudeChanged(int _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
