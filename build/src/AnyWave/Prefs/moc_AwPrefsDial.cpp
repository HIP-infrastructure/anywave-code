/****************************************************************************
** Meta object code from reading C++ file 'AwPrefsDial.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Prefs/AwPrefsDial.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwPrefsDial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwPrefsDial_t {
    QByteArrayData data[23];
    char stringdata0[352];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwPrefsDial_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwPrefsDial_t qt_meta_stringdata_AwPrefsDial = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AwPrefsDial"
QT_MOC_LITERAL(1, 12, 24), // "screenCalibrationChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 9), // "xPixPerCm"
QT_MOC_LITERAL(4, 48, 9), // "yPixPerCm"
QT_MOC_LITERAL(5, 58, 19), // "markersColorChanged"
QT_MOC_LITERAL(6, 78, 6), // "colors"
QT_MOC_LITERAL(7, 85, 6), // "accept"
QT_MOC_LITERAL(8, 92, 27), // "restoreDefaultMarkersColors"
QT_MOC_LITERAL(9, 120, 26), // "restoreDefaultCursorColors"
QT_MOC_LITERAL(10, 147, 27), // "restoreDefaultMappingColors"
QT_MOC_LITERAL(11, 175, 17), // "selectCursorColor"
QT_MOC_LITERAL(12, 193, 18), // "selectMappingColor"
QT_MOC_LITERAL(13, 212, 14), // "editCursorFont"
QT_MOC_LITERAL(14, 227, 15), // "editMappingFont"
QT_MOC_LITERAL(15, 243, 14), // "editMarkerFont"
QT_MOC_LITERAL(16, 258, 16), // "changeMarkerType"
QT_MOC_LITERAL(17, 275, 5), // "index"
QT_MOC_LITERAL(18, 281, 17), // "chooseCustomColor"
QT_MOC_LITERAL(19, 299, 16), // "pickMatlabFolder"
QT_MOC_LITERAL(20, 316, 13), // "pickMCRFolder"
QT_MOC_LITERAL(21, 330, 10), // "pickGARDEL"
QT_MOC_LITERAL(22, 341, 10) // "pickPython"

    },
    "AwPrefsDial\0screenCalibrationChanged\0"
    "\0xPixPerCm\0yPixPerCm\0markersColorChanged\0"
    "colors\0accept\0restoreDefaultMarkersColors\0"
    "restoreDefaultCursorColors\0"
    "restoreDefaultMappingColors\0"
    "selectCursorColor\0selectMappingColor\0"
    "editCursorFont\0editMappingFont\0"
    "editMarkerFont\0changeMarkerType\0index\0"
    "chooseCustomColor\0pickMatlabFolder\0"
    "pickMCRFolder\0pickGARDEL\0pickPython"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwPrefsDial[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   99,    2, 0x06 /* Public */,
       5,    1,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  107,    2, 0x0a /* Public */,
       8,    0,  108,    2, 0x0a /* Public */,
       9,    0,  109,    2, 0x0a /* Public */,
      10,    0,  110,    2, 0x0a /* Public */,
      11,    0,  111,    2, 0x0a /* Public */,
      12,    0,  112,    2, 0x0a /* Public */,
      13,    0,  113,    2, 0x0a /* Public */,
      14,    0,  114,    2, 0x0a /* Public */,
      15,    0,  115,    2, 0x0a /* Public */,
      16,    1,  116,    2, 0x0a /* Public */,
      18,    0,  119,    2, 0x0a /* Public */,
      19,    0,  120,    2, 0x0a /* Public */,
      20,    0,  121,    2, 0x0a /* Public */,
      21,    0,  122,    2, 0x0a /* Public */,
      22,    0,  123,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void, QMetaType::QStringList,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwPrefsDial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwPrefsDial *_t = static_cast<AwPrefsDial *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->screenCalibrationChanged((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->markersColorChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->accept(); break;
        case 3: _t->restoreDefaultMarkersColors(); break;
        case 4: _t->restoreDefaultCursorColors(); break;
        case 5: _t->restoreDefaultMappingColors(); break;
        case 6: _t->selectCursorColor(); break;
        case 7: _t->selectMappingColor(); break;
        case 8: _t->editCursorFont(); break;
        case 9: _t->editMappingFont(); break;
        case 10: _t->editMarkerFont(); break;
        case 11: _t->changeMarkerType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->chooseCustomColor(); break;
        case 13: _t->pickMatlabFolder(); break;
        case 14: _t->pickMCRFolder(); break;
        case 15: _t->pickGARDEL(); break;
        case 16: _t->pickPython(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwPrefsDial::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPrefsDial::screenCalibrationChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwPrefsDial::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwPrefsDial::markersColorChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwPrefsDial::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AwPrefsDial.data,
      qt_meta_data_AwPrefsDial,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwPrefsDial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwPrefsDial::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwPrefsDial.stringdata0))
        return static_cast<void*>(const_cast< AwPrefsDial*>(this));
    if (!strcmp(_clname, "DialPrefsClass"))
        return static_cast< DialPrefsClass*>(const_cast< AwPrefsDial*>(this));
    return QDialog::qt_metacast(_clname);
}

int AwPrefsDial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void AwPrefsDial::screenCalibrationChanged(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwPrefsDial::markersColorChanged(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
