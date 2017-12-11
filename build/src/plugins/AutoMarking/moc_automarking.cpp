/****************************************************************************
** Meta object code from reading C++ file 'automarking.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/AutoMarking/automarking.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'automarking.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AutoMarking_t {
    QByteArrayData data[5];
    char stringdata0[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AutoMarking_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AutoMarking_t qt_meta_stringdata_AutoMarking = {
    {
QT_MOC_LITERAL(0, 0, 11), // "AutoMarking"
QT_MOC_LITERAL(1, 12, 10), // "newMarkers"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 12), // "AwMarkerList"
QT_MOC_LITERAL(4, 37, 7) // "markers"

    },
    "AutoMarking\0newMarkers\0\0AwMarkerList\0"
    "markers"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutoMarking[] = {

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
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void AutoMarking::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AutoMarking *_t = static_cast<AutoMarking *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newMarkers((*reinterpret_cast< const AwMarkerList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMarkerList >(); break;
            }
            break;
        }
    }
}

const QMetaObject AutoMarking::staticMetaObject = {
    { &AwGUIProcess::staticMetaObject, qt_meta_stringdata_AutoMarking.data,
      qt_meta_data_AutoMarking,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AutoMarking::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AutoMarking::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AutoMarking.stringdata0))
        return static_cast<void*>(const_cast< AutoMarking*>(this));
    if (!strcmp(_clname, "AnyWave.GUIProcessInterface"))
        return static_cast< AwGUIProcess*>(const_cast< AutoMarking*>(this));
    return AwGUIProcess::qt_metacast(_clname);
}

int AutoMarking::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwGUIProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_AutoMarkingPlugin_t {
    QByteArrayData data[1];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AutoMarkingPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AutoMarkingPlugin_t qt_meta_stringdata_AutoMarkingPlugin = {
    {
QT_MOC_LITERAL(0, 0, 17) // "AutoMarkingPlugin"

    },
    "AutoMarkingPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutoMarkingPlugin[] = {

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

void AutoMarkingPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AutoMarkingPlugin::staticMetaObject = {
    { &AwProcessPlugin::staticMetaObject, qt_meta_stringdata_AutoMarkingPlugin.data,
      qt_meta_data_AutoMarkingPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AutoMarkingPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AutoMarkingPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AutoMarkingPlugin.stringdata0))
        return static_cast<void*>(const_cast< AutoMarkingPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessPluginInterface"))
        return static_cast< AwProcessPlugin*>(const_cast< AutoMarkingPlugin*>(this));
    return AwProcessPlugin::qt_metacast(_clname);
}

int AutoMarkingPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcessPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}

QT_PLUGIN_METADATA_SECTION const uint qt_section_alignment_dummy = 42;

#ifdef QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0xa8, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x94, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x1e, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'P',  'r',  'o',  'c',  'e',  's', 
    's',  'P',  'l',  'u',  'g',  'i',  'n',  'I', 
    'n',  't',  'e',  'r',  'f',  'a',  'c',  'e', 
    0x1b, 0x09, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x11, 0x00, 'A',  'u',  't',  'o',  'M',  'a', 
    'r',  'k',  'i',  'n',  'g',  'P',  'l',  'u', 
    'g',  'i',  'n',  0x00, ':',  0xe0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
    0x05, 0x00, 'd',  'e',  'b',  'u',  'g',  0x00,
    0x15, 0x11, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    'x',  0x00, 0x00, 0x00, '8',  0x00, 0x00, 0x00,
    'l',  0x00, 0x00, 0x00, '\\', 0x00, 0x00, 0x00
};

#else // QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0xa8, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x94, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x1e, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'P',  'r',  'o',  'c',  'e',  's', 
    's',  'P',  'l',  'u',  'g',  'i',  'n',  'I', 
    'n',  't',  'e',  'r',  'f',  'a',  'c',  'e', 
    0x15, 0x09, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x9b, 0x0c, 0x00, 0x00,
    0x09, 0x00, 'c',  'l',  'a',  's',  's',  'N', 
    'a',  'm',  'e',  0x00, 0x11, 0x00, 'A',  'u', 
    't',  'o',  'M',  'a',  'r',  'k',  'i',  'n', 
    'g',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    '1',  0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, ':',  0xe0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    '8',  0x00, 0x00, 0x00, 'T',  0x00, 0x00, 0x00,
    'x',  0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(AutoMarkingPlugin, AutoMarkingPlugin)

QT_END_MOC_NAMESPACE
