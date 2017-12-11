/****************************************************************************
** Meta object code from reading C++ file 'AwMapWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../include/widget/AwMapWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwMapWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwMapWidget_t {
    QByteArrayData data[31];
    char stringdata0[321];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwMapWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwMapWidget_t qt_meta_stringdata_AwMapWidget = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AwMapWidget"
QT_MOC_LITERAL(1, 12, 28), // "sensorSelectionStatusChanged"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 5), // "label"
QT_MOC_LITERAL(4, 48, 4), // "flag"
QT_MOC_LITERAL(5, 53, 21), // "selectedLabelsChanged"
QT_MOC_LITERAL(6, 75, 6), // "labels"
QT_MOC_LITERAL(7, 82, 11), // "topoChanged"
QT_MOC_LITERAL(8, 94, 12), // "AwMapWidget*"
QT_MOC_LITERAL(9, 107, 1), // "w"
QT_MOC_LITERAL(10, 109, 6), // "float*"
QT_MOC_LITERAL(11, 116, 6), // "values"
QT_MOC_LITERAL(12, 123, 16), // "badLabelsChanged"
QT_MOC_LITERAL(13, 140, 10), // "setLatency"
QT_MOC_LITERAL(14, 151, 3), // "lat"
QT_MOC_LITERAL(15, 155, 3), // "HMS"
QT_MOC_LITERAL(16, 159, 5), // "start"
QT_MOC_LITERAL(17, 165, 3), // "end"
QT_MOC_LITERAL(18, 169, 11), // "showSensors"
QT_MOC_LITERAL(19, 181, 12), // "showContours"
QT_MOC_LITERAL(20, 194, 12), // "showColorMap"
QT_MOC_LITERAL(21, 207, 14), // "changeColorMap"
QT_MOC_LITERAL(22, 222, 8), // "colormap"
QT_MOC_LITERAL(23, 231, 13), // "selectSensors"
QT_MOC_LITERAL(24, 245, 16), // "updateBadSensors"
QT_MOC_LITERAL(25, 262, 6), // "update"
QT_MOC_LITERAL(26, 269, 6), // "widget"
QT_MOC_LITERAL(27, 276, 9), // "updateMap"
QT_MOC_LITERAL(28, 286, 7), // "latency"
QT_MOC_LITERAL(29, 294, 14), // "QVector<float>"
QT_MOC_LITERAL(30, 309, 11) // "arma::fvec&"

    },
    "AwMapWidget\0sensorSelectionStatusChanged\0"
    "\0label\0flag\0selectedLabelsChanged\0"
    "labels\0topoChanged\0AwMapWidget*\0w\0"
    "float*\0values\0badLabelsChanged\0"
    "setLatency\0lat\0HMS\0start\0end\0showSensors\0"
    "showContours\0showColorMap\0changeColorMap\0"
    "colormap\0selectSensors\0updateBadSensors\0"
    "update\0widget\0updateMap\0latency\0"
    "QVector<float>\0arma::fvec&"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwMapWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   89,    2, 0x06 /* Public */,
       5,    1,   94,    2, 0x06 /* Public */,
       7,    3,   97,    2, 0x06 /* Public */,
      12,    2,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    2,  109,    2, 0x0a /* Public */,
      13,    3,  114,    2, 0x0a /* Public */,
      18,    1,  121,    2, 0x0a /* Public */,
      19,    1,  124,    2, 0x0a /* Public */,
      20,    1,  127,    2, 0x0a /* Public */,
      21,    1,  130,    2, 0x0a /* Public */,
      23,    1,  133,    2, 0x0a /* Public */,
      24,    1,  136,    2, 0x0a /* Public */,
      25,    1,  139,    2, 0x0a /* Public */,
      27,    3,  142,    2, 0x0a /* Public */,
      27,    3,  149,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    3,    4,
    QMetaType::Void, QMetaType::QStringList,    6,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QStringList, 0x80000000 | 10,    9,    6,   11,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QStringList,    9,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Bool,   14,   15,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Bool,   16,   17,   15,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Int,   22,
    QMetaType::Void, QMetaType::QStringList,    6,
    QMetaType::Void, QMetaType::QStringList,    6,
    QMetaType::Void, 0x80000000 | 8,   26,
    QMetaType::Void, QMetaType::Float, 0x80000000 | 29, QMetaType::QStringList,   28,   11,    6,
    QMetaType::Void, QMetaType::Float, 0x80000000 | 30, QMetaType::QStringList,   28,   11,    6,

       0        // eod
};

void AwMapWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwMapWidget *_t = static_cast<AwMapWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sensorSelectionStatusChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->selectedLabelsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->topoChanged((*reinterpret_cast< AwMapWidget*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        case 3: _t->badLabelsChanged((*reinterpret_cast< AwMapWidget*(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 4: _t->setLatency((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->setLatency((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 6: _t->showSensors((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->showContours((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->showColorMap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->changeColorMap((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->selectSensors((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 11: _t->updateBadSensors((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 12: _t->update((*reinterpret_cast< AwMapWidget*(*)>(_a[1]))); break;
        case 13: _t->updateMap((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< const QVector<float>(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 14: _t->updateMap((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< arma::fvec(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
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
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMapWidget* >(); break;
            }
            break;
        case 12:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMapWidget* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwMapWidget::*_t)(const QString & , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMapWidget::sensorSelectionStatusChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwMapWidget::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMapWidget::selectedLabelsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwMapWidget::*_t)(AwMapWidget * , const QStringList & , float * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMapWidget::topoChanged)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwMapWidget::*_t)(AwMapWidget * , const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMapWidget::badLabelsChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject AwMapWidget::staticMetaObject = {
    { &QVTKWidget::staticMetaObject, qt_meta_stringdata_AwMapWidget.data,
      qt_meta_data_AwMapWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwMapWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwMapWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwMapWidget.stringdata0))
        return static_cast<void*>(const_cast< AwMapWidget*>(this));
    return QVTKWidget::qt_metacast(_clname);
}

int AwMapWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QVTKWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void AwMapWidget::sensorSelectionStatusChanged(const QString & _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwMapWidget::selectedLabelsChanged(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwMapWidget::topoChanged(AwMapWidget * _t1, const QStringList & _t2, float * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AwMapWidget::badLabelsChanged(AwMapWidget * _t1, const QStringList & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
