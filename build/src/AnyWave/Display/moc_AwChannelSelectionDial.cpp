/****************************************************************************
** Meta object code from reading C++ file 'AwChannelSelectionDial.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Display/AwChannelSelectionDial.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwChannelSelectionDial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwChannelSelectionDial_t {
    QByteArrayData data[11];
    char stringdata0[123];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwChannelSelectionDial_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwChannelSelectionDial_t qt_meta_stringdata_AwChannelSelectionDial = {
    {
QT_MOC_LITERAL(0, 0, 22), // "AwChannelSelectionDial"
QT_MOC_LITERAL(1, 23, 9), // "selectAll"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 11), // "unselectAll"
QT_MOC_LITERAL(4, 46, 15), // "invertSelection"
QT_MOC_LITERAL(5, 62, 9), // "selectMEG"
QT_MOC_LITERAL(6, 72, 9), // "selectEEG"
QT_MOC_LITERAL(7, 82, 10), // "selectSEEG"
QT_MOC_LITERAL(8, 93, 13), // "selectTrigger"
QT_MOC_LITERAL(9, 107, 11), // "contextMenu"
QT_MOC_LITERAL(10, 119, 3) // "pos"

    },
    "AwChannelSelectionDial\0selectAll\0\0"
    "unselectAll\0invertSelection\0selectMEG\0"
    "selectEEG\0selectSEEG\0selectTrigger\0"
    "contextMenu\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwChannelSelectionDial[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x0a /* Public */,
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    0,   59,    2, 0x0a /* Public */,
       8,    0,   60,    2, 0x0a /* Public */,
       9,    1,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   10,

       0        // eod
};

void AwChannelSelectionDial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwChannelSelectionDial *_t = static_cast<AwChannelSelectionDial *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectAll(); break;
        case 1: _t->unselectAll(); break;
        case 2: _t->invertSelection(); break;
        case 3: _t->selectMEG(); break;
        case 4: _t->selectEEG(); break;
        case 5: _t->selectSEEG(); break;
        case 6: _t->selectTrigger(); break;
        case 7: _t->contextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AwChannelSelectionDial::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AwChannelSelectionDial.data,
      qt_meta_data_AwChannelSelectionDial,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwChannelSelectionDial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwChannelSelectionDial::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwChannelSelectionDial.stringdata0))
        return static_cast<void*>(const_cast< AwChannelSelectionDial*>(this));
    if (!strcmp(_clname, "AwChannelSelectionDialClass"))
        return static_cast< AwChannelSelectionDialClass*>(const_cast< AwChannelSelectionDial*>(this));
    return QDialog::qt_metacast(_clname);
}

int AwChannelSelectionDial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
