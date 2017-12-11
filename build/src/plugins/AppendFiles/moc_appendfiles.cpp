/****************************************************************************
** Meta object code from reading C++ file 'appendfiles.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/AppendFiles/appendfiles.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'appendfiles.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AppendFiles_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AppendFiles_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AppendFiles_t qt_meta_stringdata_AppendFiles = {
    {
QT_MOC_LITERAL(0, 0, 11) // "AppendFiles"

    },
    "AppendFiles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AppendFiles[] = {

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

void AppendFiles::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AppendFiles::staticMetaObject = {
    { &AwProcess::staticMetaObject, qt_meta_stringdata_AppendFiles.data,
      qt_meta_data_AppendFiles,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AppendFiles::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppendFiles::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AppendFiles.stringdata0))
        return static_cast<void*>(const_cast< AppendFiles*>(this));
    return AwProcess::qt_metacast(_clname);
}

int AppendFiles::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwProcess::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_AppendFilesPlugin_t {
    QByteArrayData data[1];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AppendFilesPlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AppendFilesPlugin_t qt_meta_stringdata_AppendFilesPlugin = {
    {
QT_MOC_LITERAL(0, 0, 17) // "AppendFilesPlugin"

    },
    "AppendFilesPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AppendFilesPlugin[] = {

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

void AppendFilesPlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject AppendFilesPlugin::staticMetaObject = {
    { &AwProcessPlugin::staticMetaObject, qt_meta_stringdata_AppendFilesPlugin.data,
      qt_meta_data_AppendFilesPlugin,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AppendFilesPlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AppendFilesPlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AppendFilesPlugin.stringdata0))
        return static_cast<void*>(const_cast< AppendFilesPlugin*>(this));
    if (!strcmp(_clname, "AnyWave.ProcessPluginInterface"))
        return static_cast< AwProcessPlugin*>(const_cast< AppendFilesPlugin*>(this));
    return AwProcessPlugin::qt_metacast(_clname);
}

int AppendFilesPlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    0x11, 0x00, 'A',  'p',  'p',  'e',  'n',  'd', 
    'F',  'i',  'l',  'e',  's',  'P',  'l',  'u', 
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
    'a',  'm',  'e',  0x00, 0x11, 0x00, 'A',  'p', 
    'p',  'e',  'n',  'd',  'F',  'i',  'l',  'e', 
    's',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    '1',  0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, ':',  0xe0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    '8',  0x00, 0x00, 0x00, 'T',  0x00, 0x00, 0x00,
    'x',  0x00, 0x00, 0x00, 0x84, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(AppendFilesPlugin, AppendFilesPlugin)

QT_END_MOC_NAMESPACE
