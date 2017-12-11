/****************************************************************************
** Meta object code from reading C++ file 'AwSettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Prefs/AwSettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwSettings_t {
    QByteArrayData data[15];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwSettings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwSettings_t qt_meta_stringdata_AwSettings = {
    {
QT_MOC_LITERAL(0, 0, 10), // "AwSettings"
QT_MOC_LITERAL(1, 11, 19), // "markersColorChanged"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 6), // "colors"
QT_MOC_LITERAL(4, 39, 24), // "screenCalibrationChanged"
QT_MOC_LITERAL(5, 64, 1), // "x"
QT_MOC_LITERAL(6, 66, 1), // "y"
QT_MOC_LITERAL(7, 68, 18), // "recentFilesUpdated"
QT_MOC_LITERAL(8, 87, 25), // "timeRepresentationChanged"
QT_MOC_LITERAL(9, 113, 3), // "HMS"
QT_MOC_LITERAL(10, 117, 23), // "setAutoTriggerParsingOn"
QT_MOC_LITERAL(11, 141, 5), // "onoff"
QT_MOC_LITERAL(12, 147, 21), // "savePredefinedMarkers"
QT_MOC_LITERAL(13, 169, 12), // "AwMarkerList"
QT_MOC_LITERAL(14, 182, 7) // "markers"

    },
    "AwSettings\0markersColorChanged\0\0colors\0"
    "screenCalibrationChanged\0x\0y\0"
    "recentFilesUpdated\0timeRepresentationChanged\0"
    "HMS\0setAutoTriggerParsingOn\0onoff\0"
    "savePredefinedMarkers\0AwMarkerList\0"
    "markers"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwSettings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    2,   47,    2, 0x06 /* Public */,
       7,    1,   52,    2, 0x06 /* Public */,
       8,    1,   55,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   58,    2, 0x0a /* Public */,
      12,    1,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    5,    6,
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void, QMetaType::Bool,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void AwSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwSettings *_t = static_cast<AwSettings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markersColorChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->screenCalibrationChanged((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 2: _t->recentFilesUpdated((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: _t->timeRepresentationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setAutoTriggerParsingOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->savePredefinedMarkers((*reinterpret_cast< const AwMarkerList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMarkerList >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwSettings::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSettings::markersColorChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwSettings::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSettings::screenCalibrationChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwSettings::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSettings::recentFilesUpdated)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwSettings::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSettings::timeRepresentationChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject AwSettings::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwSettings.data,
      qt_meta_data_AwSettings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwSettings.stringdata0))
        return static_cast<void*>(const_cast< AwSettings*>(this));
    return QObject::qt_metacast(_clname);
}

int AwSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AwSettings::markersColorChanged(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwSettings::screenCalibrationChanged(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwSettings::recentFilesUpdated(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AwSettings::timeRepresentationChanged(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
