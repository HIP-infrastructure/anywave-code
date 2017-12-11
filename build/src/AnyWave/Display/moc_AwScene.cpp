/****************************************************************************
** Meta object code from reading C++ file 'AwScene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Display/AwScene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwScene_t {
    QByteArrayData data[8];
    char stringdata0[88];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwScene_t qt_meta_stringdata_AwScene = {
    {
QT_MOC_LITERAL(0, 0, 7), // "AwScene"
QT_MOC_LITERAL(1, 8, 10), // "newMontage"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 13), // "updateMarkers"
QT_MOC_LITERAL(4, 34, 17), // "setSelectionAsBad"
QT_MOC_LITERAL(5, 52, 15), // "setChannelAsBad"
QT_MOC_LITERAL(6, 68, 5), // "label"
QT_MOC_LITERAL(7, 74, 13) // "launchProcess"

    },
    "AwScene\0newMontage\0\0updateMarkers\0"
    "setSelectionAsBad\0setChannelAsBad\0"
    "label\0launchProcess"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwScene[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   40,    2, 0x0a /* Public */,
       4,    0,   41,    2, 0x0a /* Public */,
       5,    1,   42,    2, 0x0a /* Public */,
       7,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,

       0        // eod
};

void AwScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwScene *_t = static_cast<AwScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newMontage(); break;
        case 1: _t->updateMarkers(); break;
        case 2: _t->setSelectionAsBad(); break;
        case 3: _t->setChannelAsBad((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->launchProcess(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwScene::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwScene::newMontage)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwScene::staticMetaObject = {
    { &AwGraphicsScene::staticMetaObject, qt_meta_stringdata_AwScene.data,
      qt_meta_data_AwScene,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwScene::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwScene.stringdata0))
        return static_cast<void*>(const_cast< AwScene*>(this));
    return AwGraphicsScene::qt_metacast(_clname);
}

int AwScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AwScene::newMontage()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
