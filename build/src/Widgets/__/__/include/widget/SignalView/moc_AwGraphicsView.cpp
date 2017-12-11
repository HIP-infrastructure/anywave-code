/****************************************************************************
** Meta object code from reading C++ file 'AwGraphicsView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../include/widget/SignalView/AwGraphicsView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwGraphicsView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwGraphicsView_t {
    QByteArrayData data[14];
    char stringdata0[162];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwGraphicsView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwGraphicsView_t qt_meta_stringdata_AwGraphicsView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AwGraphicsView"
QT_MOC_LITERAL(1, 15, 17), // "newMarkersBarMode"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 4), // "mode"
QT_MOC_LITERAL(4, 39, 19), // "pageDurationChanged"
QT_MOC_LITERAL(5, 59, 8), // "duration"
QT_MOC_LITERAL(6, 68, 11), // "layoutItems"
QT_MOC_LITERAL(7, 80, 13), // "applySettings"
QT_MOC_LITERAL(8, 94, 15), // "AwViewSettings*"
QT_MOC_LITERAL(9, 110, 8), // "settings"
QT_MOC_LITERAL(10, 119, 14), // "updateSettings"
QT_MOC_LITERAL(11, 134, 5), // "flags"
QT_MOC_LITERAL(12, 140, 17), // "setPositionInFile"
QT_MOC_LITERAL(13, 158, 3) // "pos"

    },
    "AwGraphicsView\0newMarkersBarMode\0\0"
    "mode\0pageDurationChanged\0duration\0"
    "layoutItems\0applySettings\0AwViewSettings*\0"
    "settings\0updateSettings\0flags\0"
    "setPositionInFile\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwGraphicsView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   50,    2, 0x0a /* Public */,
       7,    1,   51,    2, 0x0a /* Public */,
      10,    2,   54,    2, 0x0a /* Public */,
      12,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Float,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   11,
    QMetaType::Void, QMetaType::Float,   13,

       0        // eod
};

void AwGraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwGraphicsView *_t = static_cast<AwGraphicsView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newMarkersBarMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->pageDurationChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: _t->layoutItems(); break;
        case 3: _t->applySettings((*reinterpret_cast< AwViewSettings*(*)>(_a[1]))); break;
        case 4: _t->updateSettings((*reinterpret_cast< AwViewSettings*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->setPositionInFile((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwViewSettings* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwViewSettings* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwGraphicsView::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsView::newMarkersBarMode)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwGraphicsView::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsView::pageDurationChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwGraphicsView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_AwGraphicsView.data,
      qt_meta_data_AwGraphicsView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwGraphicsView.stringdata0))
        return static_cast<void*>(const_cast< AwGraphicsView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int AwGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
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
void AwGraphicsView::newMarkersBarMode(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwGraphicsView::pageDurationChanged(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
