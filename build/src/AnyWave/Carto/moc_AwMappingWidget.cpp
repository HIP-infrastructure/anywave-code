/****************************************************************************
** Meta object code from reading C++ file 'AwMappingWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Carto/AwMappingWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwMappingWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwMappingWidget_t {
    QByteArrayData data[20];
    char stringdata0[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwMappingWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwMappingWidget_t qt_meta_stringdata_AwMappingWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwMappingWidget"
QT_MOC_LITERAL(1, 16, 21), // "selectedLabelsChanged"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 6), // "labels"
QT_MOC_LITERAL(4, 46, 10), // "updateMaps"
QT_MOC_LITERAL(5, 57, 7), // "latency"
QT_MOC_LITERAL(6, 65, 14), // "QVector<float>"
QT_MOC_LITERAL(7, 80, 4), // "data"
QT_MOC_LITERAL(8, 85, 13), // "updateMapsPCA"
QT_MOC_LITERAL(9, 99, 5), // "start"
QT_MOC_LITERAL(10, 105, 3), // "end"
QT_MOC_LITERAL(11, 109, 4), // "to2D"
QT_MOC_LITERAL(12, 114, 1), // "f"
QT_MOC_LITERAL(13, 116, 4), // "to3D"
QT_MOC_LITERAL(14, 121, 8), // "showBoth"
QT_MOC_LITERAL(15, 130, 12), // "selectLabels"
QT_MOC_LITERAL(16, 143, 15), // "updateSelection"
QT_MOC_LITERAL(17, 159, 16), // "updateBadSensors"
QT_MOC_LITERAL(18, 176, 14), // "changeColorMap"
QT_MOC_LITERAL(19, 191, 5) // "index"

    },
    "AwMappingWidget\0selectedLabelsChanged\0"
    "\0labels\0updateMaps\0latency\0QVector<float>\0"
    "data\0updateMapsPCA\0start\0end\0to2D\0f\0"
    "to3D\0showBoth\0selectLabels\0updateSelection\0"
    "updateBadSensors\0changeColorMap\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwMappingWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    3,   67,    2, 0x0a /* Public */,
       8,    4,   74,    2, 0x0a /* Public */,
      11,    1,   83,    2, 0x0a /* Public */,
      13,    1,   86,    2, 0x0a /* Public */,
      14,    1,   89,    2, 0x0a /* Public */,
      15,    1,   92,    2, 0x0a /* Public */,
      16,    1,   95,    2, 0x0a /* Public */,
      17,    1,   98,    2, 0x0a /* Public */,
      18,    1,  101,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Float, 0x80000000 | 6, QMetaType::QStringList,    5,    7,    3,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, 0x80000000 | 6, QMetaType::QStringList,    9,   10,    7,    3,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::Int,   19,

       0        // eod
};

void AwMappingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwMappingWidget *_t = static_cast<AwMappingWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectedLabelsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->updateMaps((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< const QVector<float>(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3]))); break;
        case 2: _t->updateMapsPCA((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< const QVector<float>(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4]))); break;
        case 3: _t->to2D((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->to3D((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->showBoth((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->selectLabels((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 7: _t->updateSelection((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 8: _t->updateBadSensors((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 9: _t->changeColorMap((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwMappingWidget::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMappingWidget::selectedLabelsChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwMappingWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwMappingWidget.data,
      qt_meta_data_AwMappingWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwMappingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwMappingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwMappingWidget.stringdata0))
        return static_cast<void*>(const_cast< AwMappingWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwMappingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AwMappingWidget::selectedLabelsChanged(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
