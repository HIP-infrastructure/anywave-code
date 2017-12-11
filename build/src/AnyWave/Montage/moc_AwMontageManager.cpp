/****************************************************************************
** Meta object code from reading C++ file 'AwMontageManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Montage/AwMontageManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwMontageManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwMontageManager_t {
    QByteArrayData data[29];
    char stringdata0[378];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwMontageManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwMontageManager_t qt_meta_stringdata_AwMontageManager = {
    {
QT_MOC_LITERAL(0, 0, 16), // "AwMontageManager"
QT_MOC_LITERAL(1, 17, 14), // "montageChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 14), // "AwChannelList&"
QT_MOC_LITERAL(4, 48, 8), // "channels"
QT_MOC_LITERAL(5, 57, 20), // "quickMontagesUpdated"
QT_MOC_LITERAL(6, 78, 14), // "badChannelsSet"
QT_MOC_LITERAL(7, 93, 6), // "labels"
QT_MOC_LITERAL(8, 100, 13), // "showInterface"
QT_MOC_LITERAL(9, 114, 18), // "saveCurrentMontage"
QT_MOC_LITERAL(10, 133, 16), // "loadQuickMontage"
QT_MOC_LITERAL(11, 150, 4), // "name"
QT_MOC_LITERAL(12, 155, 11), // "loadMontage"
QT_MOC_LITERAL(13, 167, 4), // "path"
QT_MOC_LITERAL(14, 172, 22), // "buildQuickMontagesList"
QT_MOC_LITERAL(15, 195, 17), // "addChannelsByName"
QT_MOC_LITERAL(16, 213, 23), // "buildNewMontageFomNames"
QT_MOC_LITERAL(17, 237, 5), // "names"
QT_MOC_LITERAL(18, 243, 10), // "newFilters"
QT_MOC_LITERAL(19, 254, 22), // "addChannelToAsRecorded"
QT_MOC_LITERAL(20, 277, 10), // "AwChannel*"
QT_MOC_LITERAL(21, 288, 7), // "channel"
QT_MOC_LITERAL(22, 296, 16), // "markChannelAsBad"
QT_MOC_LITERAL(23, 313, 11), // "channelName"
QT_MOC_LITERAL(24, 325, 3), // "bad"
QT_MOC_LITERAL(25, 329, 17), // "markChannelsAsBad"
QT_MOC_LITERAL(26, 347, 11), // "setChannels"
QT_MOC_LITERAL(27, 359, 13), // "addNewSources"
QT_MOC_LITERAL(28, 373, 4) // "type"

    },
    "AwMontageManager\0montageChanged\0\0"
    "AwChannelList&\0channels\0quickMontagesUpdated\0"
    "badChannelsSet\0labels\0showInterface\0"
    "saveCurrentMontage\0loadQuickMontage\0"
    "name\0loadMontage\0path\0buildQuickMontagesList\0"
    "addChannelsByName\0buildNewMontageFomNames\0"
    "names\0newFilters\0addChannelToAsRecorded\0"
    "AwChannel*\0channel\0markChannelAsBad\0"
    "channelName\0bad\0markChannelsAsBad\0"
    "setChannels\0addNewSources\0type"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwMontageManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   99,    2, 0x06 /* Public */,
       5,    0,  102,    2, 0x06 /* Public */,
       6,    1,  103,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  106,    2, 0x0a /* Public */,
       9,    0,  107,    2, 0x0a /* Public */,
      10,    1,  108,    2, 0x0a /* Public */,
      12,    1,  111,    2, 0x0a /* Public */,
      14,    0,  114,    2, 0x0a /* Public */,
      15,    1,  115,    2, 0x0a /* Public */,
      16,    1,  118,    2, 0x0a /* Public */,
      18,    0,  121,    2, 0x0a /* Public */,
      19,    1,  122,    2, 0x0a /* Public */,
      22,    2,  125,    2, 0x0a /* Public */,
      22,    1,  130,    2, 0x2a /* Public | MethodCloned */,
      25,    1,  133,    2, 0x0a /* Public */,
      26,    1,  136,    2, 0x0a /* Public */,
      27,    1,  139,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Bool, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QStringList,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   23,   24,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void, QMetaType::QStringList,    7,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,   28,

       0        // eod
};

void AwMontageManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwMontageManager *_t = static_cast<AwMontageManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->montageChanged((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 1: _t->quickMontagesUpdated(); break;
        case 2: _t->badChannelsSet((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 3: _t->showInterface(); break;
        case 4: _t->saveCurrentMontage(); break;
        case 5: _t->loadQuickMontage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: { bool _r = _t->loadMontage((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: _t->buildQuickMontagesList(); break;
        case 8: _t->addChannelsByName((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 9: _t->buildNewMontageFomNames((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 10: _t->newFilters(); break;
        case 11: _t->addChannelToAsRecorded((*reinterpret_cast< AwChannel*(*)>(_a[1]))); break;
        case 12: _t->markChannelAsBad((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 13: _t->markChannelAsBad((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->markChannelsAsBad((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 15: _t->setChannels((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 16: _t->addNewSources((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwMontageManager::*_t)(AwChannelList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMontageManager::montageChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwMontageManager::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMontageManager::quickMontagesUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwMontageManager::*_t)(const QStringList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwMontageManager::badChannelsSet)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwMontageManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwMontageManager.data,
      qt_meta_data_AwMontageManager,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwMontageManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwMontageManager::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwMontageManager.stringdata0))
        return static_cast<void*>(const_cast< AwMontageManager*>(this));
    return QObject::qt_metacast(_clname);
}

int AwMontageManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void AwMontageManager::montageChanged(AwChannelList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwMontageManager::quickMontagesUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void AwMontageManager::badChannelsSet(const QStringList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
