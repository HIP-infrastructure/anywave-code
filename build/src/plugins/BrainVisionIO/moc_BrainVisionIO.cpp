/****************************************************************************
** Meta object code from reading C++ file 'BrainVisionIO.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/BrainVisionIO/BrainVisionIO.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BrainVisionIO.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BrainVisionIO_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BrainVisionIO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BrainVisionIO_t qt_meta_stringdata_BrainVisionIO = {
    {
QT_MOC_LITERAL(0, 0, 13) // "BrainVisionIO"

    },
    "BrainVisionIO"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BrainVisionIO[] = {

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

void BrainVisionIO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject BrainVisionIO::staticMetaObject = {
    { &AwFileIO::staticMetaObject, qt_meta_stringdata_BrainVisionIO.data,
      qt_meta_data_BrainVisionIO,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BrainVisionIO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BrainVisionIO::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BrainVisionIO.stringdata0))
        return static_cast<void*>(const_cast< BrainVisionIO*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterface"))
        return static_cast< AwFileIO*>(const_cast< BrainVisionIO*>(this));
    return AwFileIO::qt_metacast(_clname);
}

int BrainVisionIO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_BrainVisionIOPlugin_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BrainVisionIOPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BrainVisionIOPlugin_t qt_meta_stringdata_BrainVisionIOPlugin = {
    {
QT_MOC_LITERAL(0, 0, 19) // "BrainVisionIOPlugin"

    },
    "BrainVisionIOPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BrainVisionIOPlugin[] = {

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

void BrainVisionIOPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject BrainVisionIOPlugin::staticMetaObject = {
    { &AwFileIOPlugin::staticMetaObject, qt_meta_stringdata_BrainVisionIOPlugin.data,
      qt_meta_data_BrainVisionIOPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BrainVisionIOPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BrainVisionIOPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BrainVisionIOPlugin.stringdata0))
        return static_cast<void*>(const_cast< BrainVisionIOPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterfacePlugin"))
        return static_cast< AwFileIOPlugin*>(const_cast< BrainVisionIOPlugin*>(this));
    return AwFileIOPlugin::qt_metacast(_clname);
}

int BrainVisionIOPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIOPlugin::qt_metacall(_c, _id, _a);
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
    0xac, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x98, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x1d, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'F',  'i',  'l',  'e',  'I',  'O', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    0x1b, 0x09, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x13, 0x00, 'B',  'r',  'a',  'i',  'n',  'V', 
    'i',  's',  'i',  'o',  'n',  'I',  'O',  'P', 
    'l',  'u',  'g',  'i',  'n',  0x00, 0x00, 0x00,
    ':',  0xe0, 0xa0, 0x00, 0x07, 0x00, 'v',  'e', 
    'r',  's',  'i',  'o',  'n',  0x00, 0x00, 0x00,
    0x11, 0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, 0x95, 0x11, 0x00, 0x00,
    0x08, 0x00, 'M',  'e',  't',  'a',  'D',  'a', 
    't',  'a',  0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, '|',  0x00, 0x00, 0x00,
    '8',  0x00, 0x00, 0x00, 'p',  0x00, 0x00, 0x00,
    '`',  0x00, 0x00, 0x00
};

#else // QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0xac, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x98, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x1d, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'F',  'i',  'l',  'e',  'I',  'O', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    0x15, 0x09, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x9b, 0x0c, 0x00, 0x00,
    0x09, 0x00, 'c',  'l',  'a',  's',  's',  'N', 
    'a',  'm',  'e',  0x00, 0x13, 0x00, 'B',  'r', 
    'a',  'i',  'n',  'V',  'i',  's',  'i',  'o', 
    'n',  'I',  'O',  'P',  'l',  'u',  'g',  'i', 
    'n',  0x00, 0x00, 0x00, '1',  0x00, 0x00, 0x00,
    0x05, 0x00, 'd',  'e',  'b',  'u',  'g',  0x00,
    ':',  0xe0, 0xa0, 0x00, 0x07, 0x00, 'v',  'e', 
    'r',  's',  'i',  'o',  'n',  0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, '8',  0x00, 0x00, 0x00,
    'T',  0x00, 0x00, 0x00, '|',  0x00, 0x00, 0x00,
    0x88, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(BrainVisionIOPlugin, BrainVisionIOPlugin)

QT_END_MOC_NAMESPACE
