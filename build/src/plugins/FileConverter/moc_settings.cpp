/****************************************************************************
** Meta object code from reading C++ file 'settings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/plugins/FileConverter/settings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_settings_t {
    QByteArrayData data[8];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_settings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_settings_t qt_meta_stringdata_settings = {
    {
QT_MOC_LITERAL(0, 0, 8), // "settings"
QT_MOC_LITERAL(1, 9, 6), // "accept"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 4), // "exec"
QT_MOC_LITERAL(4, 22, 19), // "changeCurrentWriter"
QT_MOC_LITERAL(5, 42, 5), // "index"
QT_MOC_LITERAL(6, 48, 8), // "addFiles"
QT_MOC_LITERAL(7, 57, 15) // "changeOutputDir"

    },
    "settings\0accept\0\0exec\0changeCurrentWriter\0"
    "index\0addFiles\0changeOutputDir"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_settings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    1,   41,    2, 0x09 /* Protected */,
       6,    0,   44,    2, 0x09 /* Protected */,
       7,    0,   45,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Int,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void settings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        settings *_t = static_cast<settings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->accept(); break;
        case 1: { int _r = _t->exec();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: _t->changeCurrentWriter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->addFiles(); break;
        case 4: _t->changeOutputDir(); break;
        default: ;
        }
    }
}

const QMetaObject settings::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_settings.data,
      qt_meta_data_settings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *settings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *settings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_settings.stringdata0))
        return static_cast<void*>(const_cast< settings*>(this));
    return QDialog::qt_metacast(_clname);
}

int settings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
