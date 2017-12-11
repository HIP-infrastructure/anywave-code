/****************************************************************************
** Meta object code from reading C++ file 'AwDisplay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Display/AwDisplay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwDisplay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwDisplay_t {
    QByteArrayData data[72];
    char stringdata0[986];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwDisplay_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwDisplay_t qt_meta_stringdata_AwDisplay = {
    {
QT_MOC_LITERAL(0, 0, 9), // "AwDisplay"
QT_MOC_LITERAL(1, 10, 9), // "newMarker"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 3), // "pos"
QT_MOC_LITERAL(4, 25, 8), // "duration"
QT_MOC_LITERAL(5, 34, 23), // "selectedChannelsChanged"
QT_MOC_LITERAL(6, 58, 14), // "AwChannelList&"
QT_MOC_LITERAL(7, 73, 9), // "selection"
QT_MOC_LITERAL(8, 83, 17), // "gainLevelsUpdated"
QT_MOC_LITERAL(9, 101, 3), // "eeg"
QT_MOC_LITERAL(10, 105, 4), // "seeg"
QT_MOC_LITERAL(11, 110, 3), // "meg"
QT_MOC_LITERAL(12, 114, 3), // "ecg"
QT_MOC_LITERAL(13, 118, 7), // "trigger"
QT_MOC_LITERAL(14, 126, 16), // "clickedAtLatency"
QT_MOC_LITERAL(15, 143, 7), // "latency"
QT_MOC_LITERAL(16, 151, 24), // "displayedChannelsChanged"
QT_MOC_LITERAL(17, 176, 8), // "channels"
QT_MOC_LITERAL(18, 185, 15), // "resetMarkerMode"
QT_MOC_LITERAL(19, 201, 12), // "setupChanged"
QT_MOC_LITERAL(20, 214, 15), // "AwDisplaySetup*"
QT_MOC_LITERAL(21, 230, 5), // "setup"
QT_MOC_LITERAL(22, 236, 5), // "flags"
QT_MOC_LITERAL(23, 242, 24), // "mappingTimeSelectionDone"
QT_MOC_LITERAL(24, 267, 12), // "QTSModeEnded"
QT_MOC_LITERAL(25, 280, 22), // "updateSelectedChannels"
QT_MOC_LITERAL(26, 303, 11), // "updateSetup"
QT_MOC_LITERAL(27, 315, 14), // "executeCommand"
QT_MOC_LITERAL(28, 330, 7), // "command"
QT_MOC_LITERAL(29, 338, 4), // "args"
QT_MOC_LITERAL(30, 343, 27), // "synchronizeMappingCursorPos"
QT_MOC_LITERAL(31, 371, 8), // "position"
QT_MOC_LITERAL(32, 380, 16), // "addNewSignalView"
QT_MOC_LITERAL(33, 397, 10), // "removeView"
QT_MOC_LITERAL(34, 408, 19), // "alignViewsVerticaly"
QT_MOC_LITERAL(35, 428, 21), // "alignViewsHorizontaly"
QT_MOC_LITERAL(36, 450, 16), // "synchronizeViews"
QT_MOC_LITERAL(37, 467, 19), // "synchronizeOnCursor"
QT_MOC_LITERAL(38, 487, 20), // "synchronizeCursorPos"
QT_MOC_LITERAL(39, 508, 19), // "showPositionInViews"
QT_MOC_LITERAL(40, 528, 15), // "highlightMarker"
QT_MOC_LITERAL(41, 544, 9), // "AwMarker*"
QT_MOC_LITERAL(42, 554, 1), // "m"
QT_MOC_LITERAL(43, 556, 12), // "captureViews"
QT_MOC_LITERAL(44, 569, 28), // "changeChannelsSelectionState"
QT_MOC_LITERAL(45, 598, 4), // "name"
QT_MOC_LITERAL(46, 603, 8), // "selected"
QT_MOC_LITERAL(47, 612, 18), // "makeChannelVisible"
QT_MOC_LITERAL(48, 631, 5), // "label"
QT_MOC_LITERAL(49, 637, 29), // "setSelectedChannelsFromLabels"
QT_MOC_LITERAL(50, 667, 6), // "labels"
QT_MOC_LITERAL(51, 674, 18), // "addVirtualChannels"
QT_MOC_LITERAL(52, 693, 14), // "AwChannelList*"
QT_MOC_LITERAL(53, 708, 21), // "removeVirtualChannels"
QT_MOC_LITERAL(54, 730, 17), // "removeICAChannels"
QT_MOC_LITERAL(55, 748, 11), // "setChannels"
QT_MOC_LITERAL(56, 760, 7), // "montage"
QT_MOC_LITERAL(57, 768, 18), // "processHasFinished"
QT_MOC_LITERAL(58, 787, 20), // "addMarkerModeChanged"
QT_MOC_LITERAL(59, 808, 2), // "on"
QT_MOC_LITERAL(60, 811, 17), // "cursorModeChanged"
QT_MOC_LITERAL(61, 829, 10), // "setQTSMode"
QT_MOC_LITERAL(62, 840, 18), // "changeCurrentSetup"
QT_MOC_LITERAL(63, 859, 8), // "newSetup"
QT_MOC_LITERAL(64, 868, 18), // "updateMarkersColor"
QT_MOC_LITERAL(65, 887, 6), // "colors"
QT_MOC_LITERAL(66, 894, 13), // "updateDisplay"
QT_MOC_LITERAL(67, 908, 16), // "setMappingModeOn"
QT_MOC_LITERAL(68, 925, 21), // "showICAMapOverChannel"
QT_MOC_LITERAL(69, 947, 4), // "flag"
QT_MOC_LITERAL(70, 952, 24), // "updateICAChannelRejected"
QT_MOC_LITERAL(71, 977, 8) // "rejected"

    },
    "AwDisplay\0newMarker\0\0pos\0duration\0"
    "selectedChannelsChanged\0AwChannelList&\0"
    "selection\0gainLevelsUpdated\0eeg\0seeg\0"
    "meg\0ecg\0trigger\0clickedAtLatency\0"
    "latency\0displayedChannelsChanged\0"
    "channels\0resetMarkerMode\0setupChanged\0"
    "AwDisplaySetup*\0setup\0flags\0"
    "mappingTimeSelectionDone\0QTSModeEnded\0"
    "updateSelectedChannels\0updateSetup\0"
    "executeCommand\0command\0args\0"
    "synchronizeMappingCursorPos\0position\0"
    "addNewSignalView\0removeView\0"
    "alignViewsVerticaly\0alignViewsHorizontaly\0"
    "synchronizeViews\0synchronizeOnCursor\0"
    "synchronizeCursorPos\0showPositionInViews\0"
    "highlightMarker\0AwMarker*\0m\0captureViews\0"
    "changeChannelsSelectionState\0name\0"
    "selected\0makeChannelVisible\0label\0"
    "setSelectedChannelsFromLabels\0labels\0"
    "addVirtualChannels\0AwChannelList*\0"
    "removeVirtualChannels\0removeICAChannels\0"
    "setChannels\0montage\0processHasFinished\0"
    "addMarkerModeChanged\0on\0cursorModeChanged\0"
    "setQTSMode\0changeCurrentSetup\0newSetup\0"
    "updateMarkersColor\0colors\0updateDisplay\0"
    "setMappingModeOn\0showICAMapOverChannel\0"
    "flag\0updateICAChannelRejected\0rejected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwDisplay[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  214,    2, 0x06 /* Public */,
       5,    1,  219,    2, 0x06 /* Public */,
       8,    5,  222,    2, 0x06 /* Public */,
      14,    1,  233,    2, 0x06 /* Public */,
      16,    1,  236,    2, 0x06 /* Public */,
      18,    0,  239,    2, 0x06 /* Public */,
      19,    2,  240,    2, 0x06 /* Public */,
      23,    2,  245,    2, 0x06 /* Public */,
      24,    0,  250,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      25,    0,  251,    2, 0x0a /* Public */,
      26,    2,  252,    2, 0x0a /* Public */,
      27,    2,  257,    2, 0x0a /* Public */,
      30,    1,  262,    2, 0x0a /* Public */,
      32,    0,  265,    2, 0x0a /* Public */,
      33,    0,  266,    2, 0x0a /* Public */,
      34,    0,  267,    2, 0x0a /* Public */,
      35,    0,  268,    2, 0x0a /* Public */,
      36,    1,  269,    2, 0x0a /* Public */,
      37,    1,  272,    2, 0x0a /* Public */,
      38,    1,  275,    2, 0x0a /* Public */,
      39,    1,  278,    2, 0x0a /* Public */,
      40,    1,  281,    2, 0x0a /* Public */,
      43,    0,  284,    2, 0x0a /* Public */,
      44,    2,  285,    2, 0x0a /* Public */,
      47,    1,  290,    2, 0x0a /* Public */,
      49,    1,  293,    2, 0x0a /* Public */,
      51,    1,  296,    2, 0x0a /* Public */,
      53,    1,  299,    2, 0x0a /* Public */,
      54,    0,  302,    2, 0x0a /* Public */,
      55,    1,  303,    2, 0x0a /* Public */,
      57,    0,  306,    2, 0x0a /* Public */,
      58,    1,  307,    2, 0x0a /* Public */,
      60,    1,  310,    2, 0x0a /* Public */,
      61,    1,  313,    2, 0x0a /* Public */,
      62,    1,  316,    2, 0x0a /* Public */,
      64,    1,  319,    2, 0x0a /* Public */,
      66,    0,  322,    2, 0x0a /* Public */,
      67,    1,  323,    2, 0x0a /* Public */,
      68,    1,  326,    2, 0x0a /* Public */,
      70,    2,  329,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float,    9,   10,   11,   12,   13,
    QMetaType::Void, QMetaType::Float,   15,
    QMetaType::Void, 0x80000000 | 6,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20, QMetaType::Int,   21,   22,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20, QMetaType::Int,   21,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::QVariantList,   28,   29,
    QMetaType::Void, QMetaType::Float,   31,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   31,
    QMetaType::Void, QMetaType::Float,   31,
    QMetaType::Void, QMetaType::Float,   31,
    QMetaType::Void, QMetaType::Float,   31,
    QMetaType::Void, 0x80000000 | 41,   42,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   45,   46,
    QMetaType::Void, QMetaType::QString,   48,
    QMetaType::Void, QMetaType::QStringList,   50,
    QMetaType::Void, 0x80000000 | 52,   17,
    QMetaType::Void, 0x80000000 | 52,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,   56,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   59,
    QMetaType::Void, QMetaType::Bool,   59,
    QMetaType::Void, QMetaType::Bool,   59,
    QMetaType::Void, 0x80000000 | 20,   63,
    QMetaType::Void, QMetaType::QStringList,   65,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   59,
    QMetaType::Void, QMetaType::Bool,   69,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   48,   71,

       0        // eod
};

void AwDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwDisplay *_t = static_cast<AwDisplay *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newMarker((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->selectedChannelsChanged((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 2: _t->gainLevelsUpdated((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< float(*)>(_a[5]))); break;
        case 3: _t->clickedAtLatency((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 4: _t->displayedChannelsChanged((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 5: _t->resetMarkerMode(); break;
        case 6: _t->setupChanged((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->mappingTimeSelectionDone((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 8: _t->QTSModeEnded(); break;
        case 9: _t->updateSelectedChannels(); break;
        case 10: _t->updateSetup((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 11: _t->executeCommand((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QVariantList(*)>(_a[2]))); break;
        case 12: _t->synchronizeMappingCursorPos((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 13: _t->addNewSignalView(); break;
        case 14: _t->removeView(); break;
        case 15: _t->alignViewsVerticaly(); break;
        case 16: _t->alignViewsHorizontaly(); break;
        case 17: _t->synchronizeViews((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 18: _t->synchronizeOnCursor((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 19: _t->synchronizeCursorPos((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 20: _t->showPositionInViews((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 21: _t->highlightMarker((*reinterpret_cast< AwMarker*(*)>(_a[1]))); break;
        case 22: _t->captureViews(); break;
        case 23: _t->changeChannelsSelectionState((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 24: _t->makeChannelVisible((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 25: _t->setSelectedChannelsFromLabels((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 26: _t->addVirtualChannels((*reinterpret_cast< AwChannelList*(*)>(_a[1]))); break;
        case 27: _t->removeVirtualChannels((*reinterpret_cast< AwChannelList*(*)>(_a[1]))); break;
        case 28: _t->removeICAChannels(); break;
        case 29: _t->setChannels((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 30: _t->processHasFinished(); break;
        case 31: _t->addMarkerModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: _t->cursorModeChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 33: _t->setQTSMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->changeCurrentSetup((*reinterpret_cast< AwDisplaySetup*(*)>(_a[1]))); break;
        case 35: _t->updateMarkersColor((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 36: _t->updateDisplay(); break;
        case 37: _t->setMappingModeOn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 38: _t->showICAMapOverChannel((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 39: _t->updateICAChannelRejected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwDisplay::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::newMarker)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)(AwChannelList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::selectedChannelsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)(float , float , float , float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::gainLevelsUpdated)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::clickedAtLatency)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)(AwChannelList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::displayedChannelsChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::resetMarkerMode)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)(AwDisplaySetup * , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::setupChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::mappingTimeSelectionDone)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (AwDisplay::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwDisplay::QTSModeEnded)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject AwDisplay::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwDisplay.data,
      qt_meta_data_AwDisplay,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwDisplay.stringdata0))
        return static_cast<void*>(const_cast< AwDisplay*>(this));
    return QObject::qt_metacast(_clname);
}

int AwDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 40;
    }
    return _id;
}

// SIGNAL 0
void AwDisplay::newMarker(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwDisplay::selectedChannelsChanged(AwChannelList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwDisplay::gainLevelsUpdated(float _t1, float _t2, float _t3, float _t4, float _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AwDisplay::clickedAtLatency(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AwDisplay::displayedChannelsChanged(AwChannelList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AwDisplay::resetMarkerMode()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void AwDisplay::setupChanged(AwDisplaySetup * _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void AwDisplay::mappingTimeSelectionDone(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AwDisplay::QTSModeEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 8, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
