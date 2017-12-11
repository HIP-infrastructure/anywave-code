/****************************************************************************
** Meta object code from reading C++ file 'AwChannelListModel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Montage/AwChannelListModel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwChannelListModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwChannelListModel_t {
    QByteArrayData data[5];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwChannelListModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwChannelListModel_t qt_meta_stringdata_AwChannelListModel = {
    {
QT_MOC_LITERAL(0, 0, 18), // "AwChannelListModel"
QT_MOC_LITERAL(1, 19, 15), // "channelsDropped"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 6), // "labels"
QT_MOC_LITERAL(4, 43, 8) // "beginRow"

    },
    "AwChannelListModel\0channelsDropped\0\0"
    "labels\0beginRow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwChannelListModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList, QMetaType::Int,    3,    4,

       0        // eod
};

void AwChannelListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwChannelListModel *_t = static_cast<AwChannelListModel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->channelsDropped((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwChannelListModel::*_t)(const QStringList & , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwChannelListModel::channelsDropped)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwChannelListModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_AwChannelListModel.data,
      qt_meta_data_AwChannelListModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwChannelListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwChannelListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwChannelListModel.stringdata0))
        return static_cast<void*>(const_cast< AwChannelListModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int AwChannelListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void AwChannelListModel::channelsDropped(const QStringList & _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_AwChannelListModelAsRecorded_t {
    QByteArrayData data[5];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwChannelListModelAsRecorded_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwChannelListModelAsRecorded_t qt_meta_stringdata_AwChannelListModelAsRecorded = {
    {
QT_MOC_LITERAL(0, 0, 28), // "AwChannelListModelAsRecorded"
QT_MOC_LITERAL(1, 29, 13), // "badChannelSet"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 5), // "label"
QT_MOC_LITERAL(4, 50, 15) // "badChannelUnset"

    },
    "AwChannelListModelAsRecorded\0badChannelSet\0"
    "\0label\0badChannelUnset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwChannelListModelAsRecorded[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       4,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void AwChannelListModelAsRecorded::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwChannelListModelAsRecorded *_t = static_cast<AwChannelListModelAsRecorded *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->badChannelSet((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->badChannelUnset((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwChannelListModelAsRecorded::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwChannelListModelAsRecorded::badChannelSet)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwChannelListModelAsRecorded::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwChannelListModelAsRecorded::badChannelUnset)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject AwChannelListModelAsRecorded::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_AwChannelListModelAsRecorded.data,
      qt_meta_data_AwChannelListModelAsRecorded,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwChannelListModelAsRecorded::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwChannelListModelAsRecorded::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwChannelListModelAsRecorded.stringdata0))
        return static_cast<void*>(const_cast< AwChannelListModelAsRecorded*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int AwChannelListModelAsRecorded::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AwChannelListModelAsRecorded::badChannelSet(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwChannelListModelAsRecorded::badChannelUnset(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_AsRecordedProxyModel_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AsRecordedProxyModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AsRecordedProxyModel_t qt_meta_stringdata_AsRecordedProxyModel = {
    {
QT_MOC_LITERAL(0, 0, 20) // "AsRecordedProxyModel"

    },
    "AsRecordedProxyModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AsRecordedProxyModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void AsRecordedProxyModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AsRecordedProxyModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_AsRecordedProxyModel.data,
      qt_meta_data_AsRecordedProxyModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AsRecordedProxyModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AsRecordedProxyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AsRecordedProxyModel.stringdata0))
        return static_cast<void*>(const_cast< AsRecordedProxyModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int AsRecordedProxyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_MontageProxyModel_t {
    QByteArrayData data[1];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MontageProxyModel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MontageProxyModel_t qt_meta_stringdata_MontageProxyModel = {
    {
QT_MOC_LITERAL(0, 0, 17) // "MontageProxyModel"

    },
    "MontageProxyModel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MontageProxyModel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MontageProxyModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MontageProxyModel::staticMetaObject = {
    { &QSortFilterProxyModel::staticMetaObject, qt_meta_stringdata_MontageProxyModel.data,
      qt_meta_data_MontageProxyModel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MontageProxyModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MontageProxyModel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MontageProxyModel.stringdata0))
        return static_cast<void*>(const_cast< MontageProxyModel*>(this));
    return QSortFilterProxyModel::qt_metacast(_clname);
}

int MontageProxyModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSortFilterProxyModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_AwChannelListDelegate_t {
    QByteArrayData data[3];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwChannelListDelegate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwChannelListDelegate_t qt_meta_stringdata_AwChannelListDelegate = {
    {
QT_MOC_LITERAL(0, 0, 21), // "AwChannelListDelegate"
QT_MOC_LITERAL(1, 22, 22), // "commitAndCloseComboBox"
QT_MOC_LITERAL(2, 45, 0) // ""

    },
    "AwChannelListDelegate\0commitAndCloseComboBox\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwChannelListDelegate[] = {

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
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void AwChannelListDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwChannelListDelegate *_t = static_cast<AwChannelListDelegate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commitAndCloseComboBox(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject AwChannelListDelegate::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_AwChannelListDelegate.data,
      qt_meta_data_AwChannelListDelegate,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwChannelListDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwChannelListDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwChannelListDelegate.stringdata0))
        return static_cast<void*>(const_cast< AwChannelListDelegate*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int AwChannelListDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_AwChannelListDelegateAsRecorded_t {
    QByteArrayData data[3];
    char stringdata0[56];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwChannelListDelegateAsRecorded_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwChannelListDelegateAsRecorded_t qt_meta_stringdata_AwChannelListDelegateAsRecorded = {
    {
QT_MOC_LITERAL(0, 0, 31), // "AwChannelListDelegateAsRecorded"
QT_MOC_LITERAL(1, 32, 22), // "commitAndCloseComboBox"
QT_MOC_LITERAL(2, 55, 0) // ""

    },
    "AwChannelListDelegateAsRecorded\0"
    "commitAndCloseComboBox\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwChannelListDelegateAsRecorded[] = {

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
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void AwChannelListDelegateAsRecorded::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwChannelListDelegateAsRecorded *_t = static_cast<AwChannelListDelegateAsRecorded *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->commitAndCloseComboBox(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject AwChannelListDelegateAsRecorded::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_AwChannelListDelegateAsRecorded.data,
      qt_meta_data_AwChannelListDelegateAsRecorded,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwChannelListDelegateAsRecorded::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwChannelListDelegateAsRecorded::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwChannelListDelegateAsRecorded.stringdata0))
        return static_cast<void*>(const_cast< AwChannelListDelegateAsRecorded*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int AwChannelListDelegateAsRecorded::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
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
