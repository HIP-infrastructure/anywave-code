/****************************************************************************
** Meta object code from reading C++ file 'AwSEEGWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/Widgets/SEEGViewer/AwSEEGWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwSEEGWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwSEEGWidget_t {
    QByteArrayData data[18];
    char stringdata0[199];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwSEEGWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwSEEGWidget_t qt_meta_stringdata_AwSEEGWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwSEEGWidget"
QT_MOC_LITERAL(1, 13, 6), // "closed"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 14), // "loadElectrodes"
QT_MOC_LITERAL(4, 36, 4), // "file"
QT_MOC_LITERAL(5, 41, 17), // "changeMeshOpacity"
QT_MOC_LITERAL(6, 59, 5), // "value"
QT_MOC_LITERAL(7, 65, 8), // "openMesh"
QT_MOC_LITERAL(8, 74, 8), // "loadMesh"
QT_MOC_LITERAL(9, 83, 19), // "changeFastRendering"
QT_MOC_LITERAL(10, 103, 2), // "on"
QT_MOC_LITERAL(11, 106, 21), // "changeSmoothRendering"
QT_MOC_LITERAL(12, 128, 10), // "computeMap"
QT_MOC_LITERAL(13, 139, 14), // "AwChannelList&"
QT_MOC_LITERAL(14, 154, 8), // "channels"
QT_MOC_LITERAL(15, 163, 7), // "latency"
QT_MOC_LITERAL(16, 171, 6), // "sample"
QT_MOC_LITERAL(17, 178, 20) // "showElectrodesLabels"

    },
    "AwSEEGWidget\0closed\0\0loadElectrodes\0"
    "file\0changeMeshOpacity\0value\0openMesh\0"
    "loadMesh\0changeFastRendering\0on\0"
    "changeSmoothRendering\0computeMap\0"
    "AwChannelList&\0channels\0latency\0sample\0"
    "showElectrodesLabels"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwSEEGWidget[] = {

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
       1,    0,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x0a /* Public */,
       3,    1,   66,    2, 0x0a /* Public */,
       5,    1,   69,    2, 0x0a /* Public */,
       7,    1,   72,    2, 0x0a /* Public */,
       8,    0,   75,    2, 0x0a /* Public */,
       9,    1,   76,    2, 0x0a /* Public */,
      11,    1,   79,    2, 0x0a /* Public */,
      12,    3,   82,    2, 0x0a /* Public */,
      17,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, 0x80000000 | 13, QMetaType::Float, QMetaType::LongLong,   14,   15,   16,
    QMetaType::Void, QMetaType::Bool,   10,

       0        // eod
};

void AwSEEGWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwSEEGWidget *_t = static_cast<AwSEEGWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closed(); break;
        case 1: _t->loadElectrodes(); break;
        case 2: _t->loadElectrodes((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->changeMeshOpacity((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->openMesh((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->loadMesh(); break;
        case 6: _t->changeFastRendering((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->changeSmoothRendering((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->computeMap((*reinterpret_cast< AwChannelList(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 9: _t->showElectrodesLabels((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwSEEGWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSEEGWidget::closed)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwSEEGWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwSEEGWidget.data,
      qt_meta_data_AwSEEGWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwSEEGWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwSEEGWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwSEEGWidget.stringdata0))
        return static_cast<void*>(const_cast< AwSEEGWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwSEEGWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void AwSEEGWidget::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
