/****************************************************************************
** Meta object code from reading C++ file 'AwGraphicsObjects.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../include/graphics/AwGraphicsObjects.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwGraphicsObjects.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwGraphicsItem_t {
    QByteArrayData data[7];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwGraphicsItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwGraphicsItem_t qt_meta_stringdata_AwGraphicsItem = {
    {
QT_MOC_LITERAL(0, 0, 14), // "AwGraphicsItem"
QT_MOC_LITERAL(1, 15, 11), // "sizeChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "filtersChanged"
QT_MOC_LITERAL(4, 43, 6), // "execUi"
QT_MOC_LITERAL(5, 50, 10), // "setNewSize"
QT_MOC_LITERAL(6, 61, 4) // "size"

    },
    "AwGraphicsItem\0sizeChanged\0\0filtersChanged\0"
    "execUi\0setNewSize\0size"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwGraphicsItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   36,    2, 0x0a /* Public */,
       5,    1,   37,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Int,
    QMetaType::Void, QMetaType::QSize,    6,

       0        // eod
};

void AwGraphicsItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwGraphicsItem *_t = static_cast<AwGraphicsItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sizeChanged(); break;
        case 1: _t->filtersChanged(); break;
        case 2: { int _r = _t->execUi();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: _t->setNewSize((*reinterpret_cast< const QSize(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwGraphicsItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsItem::sizeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwGraphicsItem::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsItem::filtersChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwGraphicsItem::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwGraphicsItem.data,
      qt_meta_data_AwGraphicsItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwGraphicsItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwGraphicsItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwGraphicsItem.stringdata0))
        return static_cast<void*>(const_cast< AwGraphicsItem*>(this));
    return QObject::qt_metacast(_clname);
}

int AwGraphicsItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void AwGraphicsItem::sizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwGraphicsItem::filtersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
struct qt_meta_stringdata_AwGraphicsCursorItem_t {
    QByteArrayData data[4];
    char stringdata0[44];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwGraphicsCursorItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwGraphicsCursorItem_t qt_meta_stringdata_AwGraphicsCursorItem = {
    {
QT_MOC_LITERAL(0, 0, 20), // "AwGraphicsCursorItem"
QT_MOC_LITERAL(1, 21, 17), // "setPositionInFile"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 3) // "pos"

    },
    "AwGraphicsCursorItem\0setPositionInFile\0"
    "\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwGraphicsCursorItem[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Float,    3,

       0        // eod
};

void AwGraphicsCursorItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwGraphicsCursorItem *_t = static_cast<AwGraphicsCursorItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setPositionInFile((*reinterpret_cast< float(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AwGraphicsCursorItem::staticMetaObject = {
    { &AwGraphicsItem::staticMetaObject, qt_meta_stringdata_AwGraphicsCursorItem.data,
      qt_meta_data_AwGraphicsCursorItem,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwGraphicsCursorItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwGraphicsCursorItem::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwGraphicsCursorItem.stringdata0))
        return static_cast<void*>(const_cast< AwGraphicsCursorItem*>(this));
    if (!strcmp(_clname, "QGraphicsRectItem"))
        return static_cast< QGraphicsRectItem*>(const_cast< AwGraphicsCursorItem*>(this));
    return AwGraphicsItem::qt_metacast(_clname);
}

int AwGraphicsCursorItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwGraphicsItem::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
