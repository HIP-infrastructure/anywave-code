/****************************************************************************
** Meta object code from reading C++ file 'SST.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/SST/SST.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SST.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_SST_t {
    QByteArrayData data[1];
    char stringdata0[4];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SST_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SST_t qt_meta_stringdata_SST = {
    {
QT_MOC_LITERAL(0, 0, 3) // "SST"

    },
    "SST"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SST[] = {

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

void SST::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject SST::staticMetaObject = {
    { &AwGUIProcess::staticMetaObject, qt_meta_stringdata_SST.data,
      qt_meta_data_SST,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SST::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SST::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SST.stringdata0))
        return static_cast<void*>(const_cast< SST*>(this));
    if (!strcmp(_clname, "AnyWave.GUIProcessInterface"))
        return static_cast< AwGUIProcess*>(const_cast< SST*>(this));
    return AwGUIProcess::qt_metacast(_clname);
}

int SST::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwGUIProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_SSTPlugin_t {
    QByteArrayData data[1];
    char stringdata0[10];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SSTPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SSTPlugin_t qt_meta_stringdata_SSTPlugin = {
    {
QT_MOC_LITERAL(0, 0, 9) // "SSTPlugin"

    },
    "SSTPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SSTPlugin[] = {

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

void SSTPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject SSTPlugin::staticMetaObject = {
    { &AwProcessPlugin::staticMetaObject, qt_meta_stringdata_SSTPlugin.data,
      qt_meta_data_SSTPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *SSTPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SSTPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_SSTPlugin.stringdata0))
        return static_cast<void*>(const_cast< SSTPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessPluginInterface"))
        return static_cast< AwProcessPlugin*>(const_cast< SSTPlugin*>(this));
    return AwProcessPlugin::qt_metacast(_clname);
}

int SSTPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    0xa0, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x8c, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x1e, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'P',  'r',  'o',  'c',  'e',  's', 
    's',  'P',  'l',  'u',  'g',  'i',  'n',  'I', 
    'n',  't',  'e',  'r',  'f',  'a',  'c',  'e', 
    0x1b, 0x09, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x09, 0x00, 'S',  'S',  'T',  'P',  'l',  'u', 
    'g',  'i',  'n',  0x00, ':',  0xe0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
    0x05, 0x00, 'd',  'e',  'b',  'u',  'g',  0x00,
    0x15, 0x10, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    'p',  0x00, 0x00, 0x00, '8',  0x00, 0x00, 0x00,
    'd',  0x00, 0x00, 0x00, 'T',  0x00, 0x00, 0x00
};

#else // QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0xa0, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x8c, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
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
    'a',  'm',  'e',  0x00, 0x09, 0x00, 'S',  'S', 
    'T',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    '1',  0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, ':',  0xe0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    '8',  0x00, 0x00, 0x00, 'T',  0x00, 0x00, 0x00,
    'p',  0x00, 0x00, 0x00, '|',  0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(SSTPlugin, SSTPlugin)

QT_END_MOC_NAMESPACE
