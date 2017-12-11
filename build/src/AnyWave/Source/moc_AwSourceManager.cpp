/****************************************************************************
** Meta object code from reading C++ file 'AwSourceManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Source/AwSourceManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwSourceManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwSourceManager_t {
    QByteArrayData data[14];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwSourceManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwSourceManager_t qt_meta_stringdata_AwSourceManager = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwSourceManager"
QT_MOC_LITERAL(1, 16, 13), // "sourcesLoaded"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17), // "newSourcesCreated"
QT_MOC_LITERAL(4, 49, 4), // "type"
QT_MOC_LITERAL(5, 54, 13), // "setBeamformer"
QT_MOC_LITERAL(6, 68, 4), // "mat&"
QT_MOC_LITERAL(7, 73, 6), // "matrix"
QT_MOC_LITERAL(8, 80, 8), // "awLabels"
QT_MOC_LITERAL(9, 89, 6), // "labels"
QT_MOC_LITERAL(10, 96, 2), // "lp"
QT_MOC_LITERAL(11, 99, 2), // "hp"
QT_MOC_LITERAL(12, 102, 4), // "load"
QT_MOC_LITERAL(13, 107, 4) // "path"

    },
    "AwSourceManager\0sourcesLoaded\0\0"
    "newSourcesCreated\0type\0setBeamformer\0"
    "mat&\0matrix\0awLabels\0labels\0lp\0hp\0"
    "load\0path"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwSourceManager[] = {

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
       3,    1,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    6,   38,    2, 0x0a /* Public */,
      12,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 6, QMetaType::QStringList, QMetaType::QStringList, QMetaType::Float, QMetaType::Float,    4,    7,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QString,   13,

       0        // eod
};

void AwSourceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwSourceManager *_t = static_cast<AwSourceManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sourcesLoaded(); break;
        case 1: _t->newSourcesCreated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setBeamformer((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< mat(*)>(_a[2])),(*reinterpret_cast< const QStringList(*)>(_a[3])),(*reinterpret_cast< const QStringList(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5])),(*reinterpret_cast< float(*)>(_a[6]))); break;
        case 3: _t->load((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwSourceManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSourceManager::sourcesLoaded)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwSourceManager::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSourceManager::newSourcesCreated)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwSourceManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwSourceManager.data,
      qt_meta_data_AwSourceManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwSourceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwSourceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwSourceManager.stringdata0))
        return static_cast<void*>(const_cast< AwSourceManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AwSourceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void AwSourceManager::sourcesLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwSourceManager::newSourcesCreated(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
