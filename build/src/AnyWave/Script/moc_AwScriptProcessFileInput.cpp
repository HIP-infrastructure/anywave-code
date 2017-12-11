/****************************************************************************
** Meta object code from reading C++ file 'AwScriptProcessFileInput.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Script/AwScriptProcessFileInput.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwScriptProcessFileInput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwScriptProcessFileInput_t {
    QByteArrayData data[26];
    char stringdata0[286];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwScriptProcessFileInput_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwScriptProcessFileInput_t qt_meta_stringdata_AwScriptProcessFileInput = {
    {
QT_MOC_LITERAL(0, 0, 24), // "AwScriptProcessFileInput"
QT_MOC_LITERAL(1, 25, 16), // "addFileExtension"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 3), // "ext"
QT_MOC_LITERAL(4, 47, 10), // "setRootDir"
QT_MOC_LITERAL(5, 58, 4), // "root"
QT_MOC_LITERAL(6, 63, 8), // "addFiles"
QT_MOC_LITERAL(7, 72, 4), // "path"
QT_MOC_LITERAL(8, 77, 10), // "markerPath"
QT_MOC_LITERAL(9, 88, 11), // "montagePath"
QT_MOC_LITERAL(10, 100, 10), // "addDirPath"
QT_MOC_LITERAL(11, 111, 7), // "dirPath"
QT_MOC_LITERAL(12, 119, 17), // "readInputFromFile"
QT_MOC_LITERAL(13, 137, 10), // "setFilters"
QT_MOC_LITERAL(14, 148, 11), // "channelType"
QT_MOC_LITERAL(15, 160, 2), // "lp"
QT_MOC_LITERAL(16, 163, 2), // "hp"
QT_MOC_LITERAL(17, 166, 10), // "downSample"
QT_MOC_LITERAL(18, 177, 12), // "samplingRate"
QT_MOC_LITERAL(19, 190, 20), // "setGlobalMontageFile"
QT_MOC_LITERAL(20, 211, 13), // "setMarkerFile"
QT_MOC_LITERAL(21, 225, 10), // "markerFile"
QT_MOC_LITERAL(22, 236, 5), // "clear"
QT_MOC_LITERAL(23, 242, 13), // "useMarkerFile"
QT_MOC_LITERAL(24, 256, 14), // "useMontageFile"
QT_MOC_LITERAL(25, 271, 14) // "fileExtensions"

    },
    "AwScriptProcessFileInput\0addFileExtension\0"
    "\0ext\0setRootDir\0root\0addFiles\0path\0"
    "markerPath\0montagePath\0addDirPath\0"
    "dirPath\0readInputFromFile\0setFilters\0"
    "channelType\0lp\0hp\0downSample\0samplingRate\0"
    "setGlobalMontageFile\0setMarkerFile\0"
    "markerFile\0clear\0useMarkerFile\0"
    "useMontageFile\0fileExtensions"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwScriptProcessFileInput[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       3,  118, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x0a /* Public */,
       4,    1,   77,    2, 0x0a /* Public */,
       6,    3,   80,    2, 0x0a /* Public */,
       6,    2,   87,    2, 0x2a /* Public | MethodCloned */,
       6,    1,   92,    2, 0x2a /* Public | MethodCloned */,
      10,    1,   95,    2, 0x0a /* Public */,
      12,    1,   98,    2, 0x0a /* Public */,
      13,    3,  101,    2, 0x0a /* Public */,
      17,    1,  108,    2, 0x0a /* Public */,
      19,    1,  111,    2, 0x0a /* Public */,
      20,    1,  114,    2, 0x0a /* Public */,
      22,    0,  117,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    7,    8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::Float, QMetaType::Float,   14,   15,   16,
    QMetaType::Void, QMetaType::Float,   18,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,   21,
    QMetaType::Void,

 // properties: name, type, flags
      23, QMetaType::Bool, 0x00095003,
      24, QMetaType::Bool, 0x00095003,
      25, QMetaType::QStringList, 0x00095001,

       0        // eod
};

void AwScriptProcessFileInput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwScriptProcessFileInput *_t = static_cast<AwScriptProcessFileInput *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->addFileExtension((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setRootDir((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->addFiles((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->addFiles((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->addFiles((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->addDirPath((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->readInputFromFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->setFilters((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 8: _t->downSample((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->setGlobalMontageFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->setMarkerFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->clear(); break;
        default: ;
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        AwScriptProcessFileInput *_t = static_cast<AwScriptProcessFileInput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = _t->useMarkerFile(); break;
        case 1: *reinterpret_cast< bool*>(_v) = _t->useMontageFile(); break;
        case 2: *reinterpret_cast< QStringList*>(_v) = _t->fileExtensions(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        AwScriptProcessFileInput *_t = static_cast<AwScriptProcessFileInput *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setMarkerFile(*reinterpret_cast< bool*>(_v)); break;
        case 1: _t->setMontageFile(*reinterpret_cast< bool*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject AwScriptProcessFileInput::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_AwScriptProcessFileInput.data,
      qt_meta_data_AwScriptProcessFileInput,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwScriptProcessFileInput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwScriptProcessFileInput::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwScriptProcessFileInput.stringdata0))
        return static_cast<void*>(const_cast< AwScriptProcessFileInput*>(this));
    return QObject::qt_metacast(_clname);
}

int AwScriptProcessFileInput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
