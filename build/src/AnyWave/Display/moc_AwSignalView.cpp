/****************************************************************************
** Meta object code from reading C++ file 'AwSignalView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Display/AwSignalView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwSignalView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwSignalView_t {
    QByteArrayData data[30];
    char stringdata0[421];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwSignalView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwSignalView_t qt_meta_stringdata_AwSignalView = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AwSignalView"
QT_MOC_LITERAL(1, 13, 19), // "markersGUIRequested"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 24), // "displayedChannelsUpdated"
QT_MOC_LITERAL(4, 59, 14), // "AwChannelList&"
QT_MOC_LITERAL(5, 74, 8), // "channels"
QT_MOC_LITERAL(6, 83, 13), // "badChannelSet"
QT_MOC_LITERAL(7, 97, 5), // "label"
QT_MOC_LITERAL(8, 103, 10), // "reloadData"
QT_MOC_LITERAL(9, 114, 7), // "refresh"
QT_MOC_LITERAL(10, 122, 20), // "updatePositionInFile"
QT_MOC_LITERAL(11, 143, 3), // "pos"
QT_MOC_LITERAL(12, 147, 21), // "synchronizeOnPosition"
QT_MOC_LITERAL(13, 169, 8), // "position"
QT_MOC_LITERAL(14, 178, 12), // "showPosition"
QT_MOC_LITERAL(15, 191, 20), // "centerViewOnPosition"
QT_MOC_LITERAL(16, 212, 18), // "updatePageDuration"
QT_MOC_LITERAL(17, 231, 8), // "duration"
QT_MOC_LITERAL(18, 240, 14), // "updateSettings"
QT_MOC_LITERAL(19, 255, 15), // "AwViewSettings*"
QT_MOC_LITERAL(20, 271, 8), // "settings"
QT_MOC_LITERAL(21, 280, 5), // "flags"
QT_MOC_LITERAL(22, 286, 10), // "newFilters"
QT_MOC_LITERAL(23, 297, 7), // "goToPos"
QT_MOC_LITERAL(24, 305, 24), // "selectChannelsFromLabels"
QT_MOC_LITERAL(25, 330, 6), // "labels"
QT_MOC_LITERAL(26, 337, 18), // "addVirtualChannels"
QT_MOC_LITERAL(27, 356, 21), // "removeVirtualChannels"
QT_MOC_LITERAL(28, 378, 17), // "removeICAChannels"
QT_MOC_LITERAL(29, 396, 24) // "makeMontageFromSelection"

    },
    "AwSignalView\0markersGUIRequested\0\0"
    "displayedChannelsUpdated\0AwChannelList&\0"
    "channels\0badChannelSet\0label\0reloadData\0"
    "refresh\0updatePositionInFile\0pos\0"
    "synchronizeOnPosition\0position\0"
    "showPosition\0centerViewOnPosition\0"
    "updatePageDuration\0duration\0updateSettings\0"
    "AwViewSettings*\0settings\0flags\0"
    "newFilters\0goToPos\0selectChannelsFromLabels\0"
    "labels\0addVirtualChannels\0"
    "removeVirtualChannels\0removeICAChannels\0"
    "makeMontageFromSelection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwSignalView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x06 /* Public */,
       3,    1,  105,    2, 0x06 /* Public */,
       6,    1,  108,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  111,    2, 0x0a /* Public */,
       9,    0,  112,    2, 0x0a /* Public */,
      10,    1,  113,    2, 0x0a /* Public */,
      12,    1,  116,    2, 0x0a /* Public */,
      14,    1,  119,    2, 0x0a /* Public */,
      15,    1,  122,    2, 0x0a /* Public */,
      16,    1,  125,    2, 0x0a /* Public */,
      18,    2,  128,    2, 0x0a /* Public */,
      22,    0,  133,    2, 0x0a /* Public */,
      23,    1,  134,    2, 0x0a /* Public */,
      24,    1,  137,    2, 0x0a /* Public */,
      26,    1,  140,    2, 0x0a /* Public */,
      27,    1,  143,    2, 0x0a /* Public */,
      28,    0,  146,    2, 0x0a /* Public */,
      29,    0,  147,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   11,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   13,
    QMetaType::Void, QMetaType::Float,   17,
    QMetaType::Void, 0x80000000 | 19, QMetaType::Int,   20,   21,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::QStringList,   25,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AwSignalView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwSignalView *_t = static_cast<AwSignalView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->markersGUIRequested(); break;
        case 1: _t->displayedChannelsUpdated((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 2: _t->badChannelSet((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->reloadData(); break;
        case 4: _t->refresh(); break;
        case 5: _t->updatePositionInFile((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->synchronizeOnPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 7: _t->showPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->centerViewOnPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->updatePageDuration((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 10: _t->updateSettings((*reinterpret_cast< AwViewSettings*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->newFilters(); break;
        case 12: _t->goToPos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->selectChannelsFromLabels((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 14: _t->addVirtualChannels((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 15: _t->removeVirtualChannels((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 16: _t->removeICAChannels(); break;
        case 17: _t->makeMontageFromSelection(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwViewSettings* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwSignalView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSignalView::markersGUIRequested)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwSignalView::*_t)(AwChannelList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSignalView::displayedChannelsUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwSignalView::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwSignalView::badChannelSet)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject AwSignalView::staticMetaObject = {
    { &AwBaseSignalView::staticMetaObject, qt_meta_stringdata_AwSignalView.data,
      qt_meta_data_AwSignalView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwSignalView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwSignalView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwSignalView.stringdata0))
        return static_cast<void*>(const_cast< AwSignalView*>(this));
    return AwBaseSignalView::qt_metacast(_clname);
}

int AwSignalView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwBaseSignalView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void AwSignalView::markersGUIRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void AwSignalView::displayedChannelsUpdated(AwChannelList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwSignalView::badChannelSet(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
