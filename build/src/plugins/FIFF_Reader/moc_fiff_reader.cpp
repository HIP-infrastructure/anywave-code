/****************************************************************************
** Meta object code from reading C++ file 'fiff_reader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/FIFF_Reader/fiff_reader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fiff_reader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FIFF_Reader_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FIFF_Reader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FIFF_Reader_t qt_meta_stringdata_FIFF_Reader = {
    {
QT_MOC_LITERAL(0, 0, 11) // "FIFF_Reader"

    },
    "FIFF_Reader"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FIFF_Reader[] = {

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

void FIFF_Reader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject FIFF_Reader::staticMetaObject = {
    { &AwFileIO::staticMetaObject, qt_meta_stringdata_FIFF_Reader.data,
      qt_meta_data_FIFF_Reader,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FIFF_Reader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FIFF_Reader::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FIFF_Reader.stringdata0))
        return static_cast<void*>(const_cast< FIFF_Reader*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterface"))
        return static_cast< AwFileIO*>(const_cast< FIFF_Reader*>(this));
    return AwFileIO::qt_metacast(_clname);
}

int FIFF_Reader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_FIFF_ReaderPlugin_t {
    QByteArrayData data[1];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FIFF_ReaderPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FIFF_ReaderPlugin_t qt_meta_stringdata_FIFF_ReaderPlugin = {
    {
QT_MOC_LITERAL(0, 0, 17) // "FIFF_ReaderPlugin"

    },
    "FIFF_ReaderPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FIFF_ReaderPlugin[] = {

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

void FIFF_ReaderPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject FIFF_ReaderPlugin::staticMetaObject = {
    { &AwFileIOPlugin::staticMetaObject, qt_meta_stringdata_FIFF_ReaderPlugin.data,
      qt_meta_data_FIFF_ReaderPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FIFF_ReaderPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FIFF_ReaderPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FIFF_ReaderPlugin.stringdata0))
        return static_cast<void*>(const_cast< FIFF_ReaderPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterfacePlugin"))
        return static_cast< AwFileIOPlugin*>(const_cast< FIFF_ReaderPlugin*>(this));
    return AwFileIOPlugin::qt_metacast(_clname);
}

int FIFF_ReaderPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    0xa8, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x94, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x1d, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'F',  'i',  'l',  'e',  'I',  'O', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    0x1b, 0x09, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x11, 0x00, 'F',  'I',  'F',  'F',  '_',  'R', 
    'e',  'a',  'd',  'e',  'r',  'P',  'l',  'u', 
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
    0x1d, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'F',  'i',  'l',  'e',  'I',  'O', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    0x15, 0x09, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x9b, 0x0c, 0x00, 0x00,
    0x09, 0x00, 'c',  'l',  'a',  's',  's',  'N', 
    'a',  'm',  'e',  0x00, 0x11, 0x00, 'F',  'I', 
    'F',  'F',  '_',  'R',  'e',  'a',  'd',  'e', 
    'r',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    '1',  0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, ':',  0xe0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    '8',  0x00, 0x00, 0x00, 'T',  0x00, 0x00, 0x00,
    'x',  0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(FIFF_ReaderPlugin, FIFF_ReaderPlugin)

QT_END_MOC_NAMESPACE
