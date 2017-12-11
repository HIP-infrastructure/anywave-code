/****************************************************************************
** Meta object code from reading C++ file 'meg4dreader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/4DNI_Reader/meg4dreader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meg4dreader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NI4DFileReader_t {
    QByteArrayData data[10];
    char stringdata0[114];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NI4DFileReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NI4DFileReader_t qt_meta_stringdata_NI4DFileReader = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NI4DFileReader"
QT_MOC_LITERAL(1, 15, 20), // "triggerValuesWritten"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 6), // "status"
QT_MOC_LITERAL(4, 44, 6), // "number"
QT_MOC_LITERAL(5, 51, 19), // "writeTriggerChannel"
QT_MOC_LITERAL(6, 71, 4), // "name"
QT_MOC_LITERAL(7, 76, 12), // "AwMarkerList"
QT_MOC_LITERAL(8, 89, 4), // "list"
QT_MOC_LITERAL(9, 94, 19) // "clearTriggerChannel"

    },
    "NI4DFileReader\0triggerValuesWritten\0"
    "\0status\0number\0writeTriggerChannel\0"
    "name\0AwMarkerList\0list\0clearTriggerChannel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NI4DFileReader[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   34,    2, 0x0a /* Public */,
       9,    1,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Int, QMetaType::QString, 0x80000000 | 7,    6,    8,
    QMetaType::Int, QMetaType::QString,    6,

       0        // eod
};

void NI4DFileReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NI4DFileReader *_t = static_cast<NI4DFileReader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->triggerValuesWritten((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: { int _r = _t->writeTriggerChannel((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const AwMarkerList(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->clearTriggerChannel((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMarkerList >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NI4DFileReader::*_t)(bool , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NI4DFileReader::triggerValuesWritten)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject NI4DFileReader::staticMetaObject = {
    { &AwFileIO::staticMetaObject, qt_meta_stringdata_NI4DFileReader.data,
      qt_meta_data_NI4DFileReader,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NI4DFileReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NI4DFileReader::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NI4DFileReader.stringdata0))
        return static_cast<void*>(const_cast< NI4DFileReader*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterface"))
        return static_cast< AwFileIO*>(const_cast< NI4DFileReader*>(this));
    return AwFileIO::qt_metacast(_clname);
}

int NI4DFileReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AwFileIO::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void NI4DFileReader::triggerValuesWritten(bool _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_NI4DReader_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NI4DReader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NI4DReader_t qt_meta_stringdata_NI4DReader = {
    {
QT_MOC_LITERAL(0, 0, 10) // "NI4DReader"

    },
    "NI4DReader"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NI4DReader[] = {

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

void NI4DReader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject NI4DReader::staticMetaObject = {
    { &AwFileIOPlugin::staticMetaObject, qt_meta_stringdata_NI4DReader.data,
      qt_meta_data_NI4DReader,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NI4DReader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NI4DReader::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NI4DReader.stringdata0))
        return static_cast<void*>(const_cast< NI4DReader*>(this));
    if (!strcmp(_clname, "AnyWave.FileIOInterfacePlugin"))
        return static_cast< AwFileIOPlugin*>(const_cast< NI4DReader*>(this));
    return AwFileIOPlugin::qt_metacast(_clname);
}

int NI4DReader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    0xa0, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x8c, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x1d, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'F',  'i',  'l',  'e',  'I',  'O', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    0x1b, 0x09, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x0a, 0x00, 'N',  'I',  '4',  'D',  'R',  'e', 
    'a',  'd',  'e',  'r',  ':',  0xe0, 0xa0, 0x00,
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
    0x1d, 0x00, 'A',  'n',  'y',  'W',  'a',  'v', 
    'e',  '.',  'F',  'i',  'l',  'e',  'I',  'O', 
    'I',  'n',  't',  'e',  'r',  'f',  'a',  'c', 
    'e',  'P',  'l',  'u',  'g',  'i',  'n',  0x00,
    0x15, 0x09, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x9b, 0x0c, 0x00, 0x00,
    0x09, 0x00, 'c',  'l',  'a',  's',  's',  'N', 
    'a',  'm',  'e',  0x00, 0x0a, 0x00, 'N',  'I', 
    '4',  'D',  'R',  'e',  'a',  'd',  'e',  'r', 
    '1',  0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, ':',  0xe0, 0xa0, 0x00,
    0x07, 0x00, 'v',  'e',  'r',  's',  'i',  'o', 
    'n',  0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    '8',  0x00, 0x00, 0x00, 'T',  0x00, 0x00, 0x00,
    'p',  0x00, 0x00, 0x00, '|',  0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(NI4DReader, NI4DReader)

QT_END_MOC_NAMESPACE
