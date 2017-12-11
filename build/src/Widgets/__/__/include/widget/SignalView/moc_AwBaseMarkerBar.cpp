/****************************************************************************
** Meta object code from reading C++ file 'AwBaseMarkerBar.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../include/widget/SignalView/AwBaseMarkerBar.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwBaseMarkerBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwBaseMarkerBar_t {
    QByteArrayData data[21];
    char stringdata0[239];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwBaseMarkerBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwBaseMarkerBar_t qt_meta_stringdata_AwBaseMarkerBar = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwBaseMarkerBar"
QT_MOC_LITERAL(1, 16, 18), // "showMarkersClicked"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 4), // "flag"
QT_MOC_LITERAL(4, 41, 17), // "showMarkerClicked"
QT_MOC_LITERAL(5, 59, 9), // "AwMarker*"
QT_MOC_LITERAL(6, 69, 6), // "marker"
QT_MOC_LITERAL(7, 76, 15), // "positionChanged"
QT_MOC_LITERAL(8, 92, 3), // "pos"
QT_MOC_LITERAL(9, 96, 5), // "clean"
QT_MOC_LITERAL(10, 102, 15), // "setPageDuration"
QT_MOC_LITERAL(11, 118, 1), // "d"
QT_MOC_LITERAL(12, 120, 17), // "setPositionInFile"
QT_MOC_LITERAL(13, 138, 10), // "setMarkers"
QT_MOC_LITERAL(14, 149, 12), // "AwMarkerList"
QT_MOC_LITERAL(15, 162, 7), // "markers"
QT_MOC_LITERAL(16, 170, 13), // "setAllMarkers"
QT_MOC_LITERAL(17, 184, 11), // "hideMarkers"
QT_MOC_LITERAL(18, 196, 11), // "showMarkers"
QT_MOC_LITERAL(19, 208, 15), // "switchToClassic"
QT_MOC_LITERAL(20, 224, 14) // "switchToGlobal"

    },
    "AwBaseMarkerBar\0showMarkersClicked\0\0"
    "flag\0showMarkerClicked\0AwMarker*\0"
    "marker\0positionChanged\0pos\0clean\0"
    "setPageDuration\0d\0setPositionInFile\0"
    "setMarkers\0AwMarkerList\0markers\0"
    "setAllMarkers\0hideMarkers\0showMarkers\0"
    "switchToClassic\0switchToGlobal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwBaseMarkerBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,
       7,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   83,    2, 0x0a /* Public */,
      10,    1,   84,    2, 0x0a /* Public */,
      12,    1,   87,    2, 0x0a /* Public */,
      13,    1,   90,    2, 0x0a /* Public */,
      16,    1,   93,    2, 0x0a /* Public */,
      17,    0,   96,    2, 0x09 /* Protected */,
      18,    0,   97,    2, 0x09 /* Protected */,
      19,    0,   98,    2, 0x09 /* Protected */,
      20,    0,   99,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Float,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, QMetaType::Float,    8,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwBaseMarkerBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwBaseMarkerBar *_t = static_cast<AwBaseMarkerBar *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showMarkersClicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->showMarkerClicked((*reinterpret_cast< AwMarker*(*)>(_a[1]))); break;
        case 2: _t->positionChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->clean(); break;
        case 4: _t->setPageDuration((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: _t->setPositionInFile((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->setMarkers((*reinterpret_cast< const AwMarkerList(*)>(_a[1]))); break;
        case 7: _t->setAllMarkers((*reinterpret_cast< const AwMarkerList(*)>(_a[1]))); break;
        case 8: _t->hideMarkers(); break;
        case 9: _t->showMarkers(); break;
        case 10: _t->switchToClassic(); break;
        case 11: _t->switchToGlobal(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMarkerList >(); break;
            }
            break;
        case 7:
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
            typedef void (AwBaseMarkerBar::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseMarkerBar::showMarkersClicked)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwBaseMarkerBar::*_t)(AwMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseMarkerBar::showMarkerClicked)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwBaseMarkerBar::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwBaseMarkerBar::positionChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwBaseMarkerBar::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_AwBaseMarkerBar.data,
      qt_meta_data_AwBaseMarkerBar,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwBaseMarkerBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwBaseMarkerBar::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwBaseMarkerBar.stringdata0))
        return static_cast<void*>(const_cast< AwBaseMarkerBar*>(this));
    return QFrame::qt_metacast(_clname);
}

int AwBaseMarkerBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void AwBaseMarkerBar::showMarkersClicked(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwBaseMarkerBar::showMarkerClicked(AwMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwBaseMarkerBar::positionChanged(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
