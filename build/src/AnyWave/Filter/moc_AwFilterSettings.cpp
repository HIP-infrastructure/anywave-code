/****************************************************************************
** Meta object code from reading C++ file 'AwFilterSettings.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/AnyWave/Filter/AwFilterSettings.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwFilterSettings.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwFilterSettings_t {
    QByteArrayData data[13];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwFilterSettings_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwFilterSettings_t qt_meta_stringdata_AwFilterSettings = {
    {
QT_MOC_LITERAL(0, 0, 16), // "AwFilterSettings"
QT_MOC_LITERAL(1, 17, 14), // "filtersApplied"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 5), // "apply"
QT_MOC_LITERAL(4, 39, 4), // "show"
QT_MOC_LITERAL(5, 44, 13), // "updateFilters"
QT_MOC_LITERAL(6, 58, 9), // "closeFile"
QT_MOC_LITERAL(7, 68, 14), // "setICASettings"
QT_MOC_LITERAL(8, 83, 4), // "type"
QT_MOC_LITERAL(9, 88, 2), // "lp"
QT_MOC_LITERAL(10, 91, 2), // "hp"
QT_MOC_LITERAL(11, 94, 17), // "setSourceSettings"
QT_MOC_LITERAL(12, 112, 5) // "reset"

    },
    "AwFilterSettings\0filtersApplied\0\0apply\0"
    "show\0updateFilters\0closeFile\0"
    "setICASettings\0type\0lp\0hp\0setSourceSettings\0"
    "reset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwFilterSettings[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   55,    2, 0x0a /* Public */,
       4,    0,   56,    2, 0x0a /* Public */,
       5,    0,   57,    2, 0x0a /* Public */,
       6,    0,   58,    2, 0x0a /* Public */,
       7,    3,   59,    2, 0x0a /* Public */,
      11,    3,   66,    2, 0x0a /* Public */,
      12,    0,   73,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    8,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Float, QMetaType::Float,    8,    9,   10,
    QMetaType::Void,

       0        // eod
};

void AwFilterSettings::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwFilterSettings *_t = static_cast<AwFilterSettings *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->filtersApplied(); break;
        case 1: _t->apply(); break;
        case 2: _t->show(); break;
        case 3: _t->updateFilters(); break;
        case 4: _t->closeFile(); break;
        case 5: _t->setICASettings((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 6: _t->setSourceSettings((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 7: _t->reset(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwFilterSettings::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwFilterSettings::filtersApplied)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AwFilterSettings::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_AwFilterSettings.data,
      qt_meta_data_AwFilterSettings,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwFilterSettings::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwFilterSettings::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwFilterSettings.stringdata0))
        return static_cast<void*>(const_cast< AwFilterSettings*>(this));
    if (!strcmp(_clname, "FilterSettingsClass"))
        return static_cast< FilterSettingsClass*>(const_cast< AwFilterSettings*>(this));
    return QWidget::qt_metacast(_clname);
}

int AwFilterSettings::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AwFilterSettings::filtersApplied()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
