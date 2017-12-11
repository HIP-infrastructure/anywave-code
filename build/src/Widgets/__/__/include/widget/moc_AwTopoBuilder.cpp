/****************************************************************************
** Meta object code from reading C++ file 'AwTopoBuilder.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../include/widget/AwTopoBuilder.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwTopoBuilder.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwTopoBuilder_t {
    QByteArrayData data[17];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwTopoBuilder_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwTopoBuilder_t qt_meta_stringdata_AwTopoBuilder = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AwTopoBuilder"
QT_MOC_LITERAL(1, 14, 15), // "computationDone"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 13), // "AwTopoWidget*"
QT_MOC_LITERAL(4, 45, 6), // "widget"
QT_MOC_LITERAL(5, 52, 9), // "Aw2DGrid*"
QT_MOC_LITERAL(6, 62, 4), // "grid"
QT_MOC_LITERAL(7, 67, 12), // "AwMapWidget*"
QT_MOC_LITERAL(8, 80, 5), // "error"
QT_MOC_LITERAL(9, 86, 11), // "computeTopo"
QT_MOC_LITERAL(10, 98, 6), // "labels"
QT_MOC_LITERAL(11, 105, 7), // "double*"
QT_MOC_LITERAL(12, 113, 6), // "values"
QT_MOC_LITERAL(13, 120, 6), // "float*"
QT_MOC_LITERAL(14, 127, 16), // "updateBadSensors"
QT_MOC_LITERAL(15, 144, 9), // "badLabels"
QT_MOC_LITERAL(16, 154, 4) // "stop"

    },
    "AwTopoBuilder\0computationDone\0\0"
    "AwTopoWidget*\0widget\0Aw2DGrid*\0grid\0"
    "AwMapWidget*\0error\0computeTopo\0labels\0"
    "double*\0values\0float*\0updateBadSensors\0"
    "badLabels\0stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwTopoBuilder[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       1,    1,   54,    2, 0x06 /* Public */,
       8,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    3,   60,    2, 0x0a /* Public */,
       9,    3,   67,    2, 0x0a /* Public */,
      14,    2,   74,    2, 0x0a /* Public */,
      16,    0,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 7,    4,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::QStringList, 0x80000000 | 11,    4,   10,   12,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QStringList, 0x80000000 | 13,    4,   10,   12,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QStringList,    4,   15,
    QMetaType::Void,

       0        // eod
};

void AwTopoBuilder::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwTopoBuilder *_t = static_cast<AwTopoBuilder *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->computationDone((*reinterpret_cast< AwTopoWidget*(*)>(_a[1])),(*reinterpret_cast< Aw2DGrid*(*)>(_a[2]))); break;
        case 1: _t->computationDone((*reinterpret_cast< AwMapWidget*(*)>(_a[1]))); break;
        case 2: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->computeTopo((*reinterpret_cast< AwTopoWidget*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< double*(*)>(_a[3]))); break;
        case 4: _t->computeTopo((*reinterpret_cast< AwMapWidget*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        case 5: _t->updateBadSensors((*reinterpret_cast< AwMapWidget*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 6: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwTopoWidget* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMapWidget* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwTopoWidget* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMapWidget* >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMapWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwTopoBuilder::*_t)(AwTopoWidget * , Aw2DGrid * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwTopoBuilder::computationDone)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwTopoBuilder::*_t)(AwMapWidget * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwTopoBuilder::computationDone)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwTopoBuilder::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwTopoBuilder::error)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwTopoBuilder::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwTopoBuilder.data,
      qt_meta_data_AwTopoBuilder,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwTopoBuilder::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwTopoBuilder::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwTopoBuilder.stringdata0))
        return static_cast<void*>(const_cast< AwTopoBuilder*>(this));
    return QObject::qt_metacast(_clname);
}

int AwTopoBuilder::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void AwTopoBuilder::computationDone(AwTopoWidget * _t1, Aw2DGrid * _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwTopoBuilder::computationDone(AwMapWidget * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwTopoBuilder::error(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
