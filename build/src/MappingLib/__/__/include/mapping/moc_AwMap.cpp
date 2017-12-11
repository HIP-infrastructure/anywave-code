/****************************************************************************
** Meta object code from reading C++ file 'AwMap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../include/mapping/AwMap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwMap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwMap_t {
    QByteArrayData data[11];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwMap_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwMap_t qt_meta_stringdata_AwMap = {
    {
QT_MOC_LITERAL(0, 0, 5), // "AwMap"
QT_MOC_LITERAL(1, 6, 10), // "mapUpdated"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 6), // "update"
QT_MOC_LITERAL(4, 25, 14), // "QVector<float>"
QT_MOC_LITERAL(5, 40, 4), // "data"
QT_MOC_LITERAL(6, 45, 6), // "labels"
QT_MOC_LITERAL(7, 52, 6), // "float*"
QT_MOC_LITERAL(8, 59, 6), // "values"
QT_MOC_LITERAL(9, 66, 16), // "updateBadSensors"
QT_MOC_LITERAL(10, 83, 11) // "sensorNames"

    },
    "AwMap\0mapUpdated\0\0update\0QVector<float>\0"
    "data\0labels\0float*\0values\0updateBadSensors\0"
    "sensorNames"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwMap[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    2,   40,    2, 0x0a /* Public */,
       3,    2,   45,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       9,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4, QMetaType::QStringList,    5,    6,
    QMetaType::Void, 0x80000000 | 7, QMetaType::QStringList,    8,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,   10,

       0        // eod
};

void AwMap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwMap *_t = static_cast<AwMap *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mapUpdated(); break;
        case 1: _t->update((*reinterpret_cast< const QVector<float>(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 2: _t->update((*reinterpret_cast< float*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 3: _t->update(); break;
        case 4: _t->updateBadSensors((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwMap::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMap::mapUpdated)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwMap::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwMap.data,
      qt_meta_data_AwMap,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwMap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwMap::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwMap.stringdata0))
        return static_cast<void*>(const_cast< AwMap*>(this));
    return QObject::qt_metacast(_clname);
}

int AwMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AwMap::mapUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
