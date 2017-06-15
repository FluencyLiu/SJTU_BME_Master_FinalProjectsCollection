/****************************************************************************
** Meta object code from reading C++ file 'tabboard.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../medImgViewer-src/tabboard.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tabboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_tabBoard_t {
    QByteArrayData data[6];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_tabBoard_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_tabBoard_t qt_meta_stringdata_tabBoard = {
    {
QT_MOC_LITERAL(0, 0, 8), // "tabBoard"
QT_MOC_LITERAL(1, 9, 16), // "setLineEditValue"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 5), // "value"
QT_MOC_LITERAL(4, 33, 13), // "setSiderValue"
QT_MOC_LITERAL(5, 47, 4) // "text"

    },
    "tabBoard\0setLineEditValue\0\0value\0"
    "setSiderValue\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_tabBoard[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void tabBoard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        tabBoard *_t = static_cast<tabBoard *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setLineEditValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setSiderValue((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject tabBoard::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_tabBoard.data,
      qt_meta_data_tabBoard,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *tabBoard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *tabBoard::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_tabBoard.stringdata0))
        return static_cast<void*>(const_cast< tabBoard*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int tabBoard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
