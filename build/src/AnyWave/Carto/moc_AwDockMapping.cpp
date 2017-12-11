/****************************************************************************
** Meta object code from reading C++ file 'AwDockMapping.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Carto/AwDockMapping.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDockMapping.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDockMapping_t {
    QByteArrayData data[14];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDockMapping_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDockMapping_t qt_meta_stringdata_AwDockMapping = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AwDockMapping"
QT_MOC_LITERAL(1, 14, 13), // "mappingClosed"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 21), // "selectedLabelsChanged"
QT_MOC_LITERAL(4, 51, 6), // "labels"
QT_MOC_LITERAL(5, 58, 20), // "newMappingAtPosition"
QT_MOC_LITERAL(6, 79, 3), // "pos"
QT_MOC_LITERAL(7, 83, 19), // "newMappingSelection"
QT_MOC_LITERAL(8, 103, 9), // "durartion"
QT_MOC_LITERAL(9, 113, 12), // "setBadLabels"
QT_MOC_LITERAL(10, 126, 12), // "QStringList&"
QT_MOC_LITERAL(11, 139, 19), // "setSelectedChannels"
QT_MOC_LITERAL(12, 159, 14), // "AwChannelList&"
QT_MOC_LITERAL(13, 174, 8) // "channels"

    },
    "AwDockMapping\0mappingClosed\0\0"
    "selectedLabelsChanged\0labels\0"
    "newMappingAtPosition\0pos\0newMappingSelection\0"
    "durartion\0setBadLabels\0QStringList&\0"
    "setSelectedChannels\0AwChannelList&\0"
    "channels"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDockMapping[] = {

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
       1,    0,   44,    2, 0x06 /* Public */,
       3,    1,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   48,    2, 0x0a /* Public */,
       7,    2,   51,    2, 0x0a /* Public */,
       9,    1,   56,    2, 0x0a /* Public */,
      11,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    6,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    6,    8,
    QMetaType::Void, 0x80000000 | 10,    4,
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void AwDockMapping::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDockMapping *_t = static_cast<AwDockMapping *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mappingClosed(); break;
        case 1: _t->selectedLabelsChanged((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 2: _t->newMappingAtPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 3: _t->newMappingSelection((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 4: _t->setBadLabels((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 5: _t->setSelectedChannels((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwDockMapping::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDockMapping::mappingClosed)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwDockMapping::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDockMapping::selectedLabelsChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwDockMapping::staticMetaObject = {
    { &QDockWidget::staticMetaObject, qt_meta_stringdata_AwDockMapping.data,
      qt_meta_data_AwDockMapping,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDockMapping::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDockMapping::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDockMapping.stringdata0))
        return static_cast<void*>(const_cast< AwDockMapping*>(this));
    return QDockWidget::qt_metacast(_clname);
}

int AwDockMapping::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AwDockMapping::mappingClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwDockMapping::selectedLabelsChanged(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
