/****************************************************************************
** Meta object code from reading C++ file 'viewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../medImgViewer-src/viewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'viewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_viewer_t {
    QByteArrayData data[30];
    char stringdata0[323];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_viewer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_viewer_t qt_meta_stringdata_viewer = {
    {
QT_MOC_LITERAL(0, 0, 6), // "viewer"
QT_MOC_LITERAL(1, 7, 9), // "imgChange"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 7), // "QImage*"
QT_MOC_LITERAL(4, 26, 6), // "newImg"
QT_MOC_LITERAL(5, 33, 10), // "reloadInfo"
QT_MOC_LITERAL(6, 44, 8), // "maskInfo"
QT_MOC_LITERAL(7, 53, 18), // "ImageType::Pointer"
QT_MOC_LITERAL(8, 72, 10), // "itkMaskImg"
QT_MOC_LITERAL(9, 83, 25), // "ImageTypeProcess::Pointer"
QT_MOC_LITERAL(10, 109, 17), // "itkMaskImgProcess"
QT_MOC_LITERAL(11, 127, 7), // "itkInfo"
QT_MOC_LITERAL(12, 135, 6), // "itkImg"
QT_MOC_LITERAL(13, 142, 9), // "reloadImg"
QT_MOC_LITERAL(14, 152, 10), // "sendReload"
QT_MOC_LITERAL(15, 163, 12), // "thresholding"
QT_MOC_LITERAL(16, 176, 14), // "thresholdValue"
QT_MOC_LITERAL(17, 191, 10), // "edgeDetect"
QT_MOC_LITERAL(18, 202, 8), // "denosing"
QT_MOC_LITERAL(19, 211, 5), // "erode"
QT_MOC_LITERAL(20, 217, 6), // "dilate"
QT_MOC_LITERAL(21, 224, 7), // "openOpr"
QT_MOC_LITERAL(22, 232, 8), // "closeOpr"
QT_MOC_LITERAL(23, 241, 19), // "binaryDistanceTrans"
QT_MOC_LITERAL(24, 261, 11), // "skeletonize"
QT_MOC_LITERAL(25, 273, 11), // "skRestorate"
QT_MOC_LITERAL(26, 285, 9), // "morphEdge"
QT_MOC_LITERAL(27, 295, 8), // "sendMask"
QT_MOC_LITERAL(28, 304, 7), // "sendItk"
QT_MOC_LITERAL(29, 312, 10) // "maskDilate"

    },
    "viewer\0imgChange\0\0QImage*\0newImg\0"
    "reloadInfo\0maskInfo\0ImageType::Pointer\0"
    "itkMaskImg\0ImageTypeProcess::Pointer\0"
    "itkMaskImgProcess\0itkInfo\0itkImg\0"
    "reloadImg\0sendReload\0thresholding\0"
    "thresholdValue\0edgeDetect\0denosing\0"
    "erode\0dilate\0openOpr\0closeOpr\0"
    "binaryDistanceTrans\0skeletonize\0"
    "skRestorate\0morphEdge\0sendMask\0sendItk\0"
    "maskDilate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_viewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  119,    2, 0x06 /* Public */,
       5,    1,  122,    2, 0x06 /* Public */,
       6,    2,  125,    2, 0x06 /* Public */,
      11,    1,  130,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    1,  133,    2, 0x0a /* Public */,
      14,    0,  136,    2, 0x0a /* Public */,
      15,    0,  137,    2, 0x0a /* Public */,
      15,    1,  138,    2, 0x0a /* Public */,
      17,    0,  141,    2, 0x0a /* Public */,
      18,    0,  142,    2, 0x0a /* Public */,
      19,    0,  143,    2, 0x0a /* Public */,
      20,    0,  144,    2, 0x0a /* Public */,
      21,    0,  145,    2, 0x0a /* Public */,
      22,    0,  146,    2, 0x0a /* Public */,
      23,    0,  147,    2, 0x0a /* Public */,
      24,    0,  148,    2, 0x0a /* Public */,
      25,    0,  149,    2, 0x0a /* Public */,
      26,    0,  150,    2, 0x0a /* Public */,
      27,    0,  151,    2, 0x0a /* Public */,
      28,    0,  152,    2, 0x0a /* Public */,
      29,    2,  153,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 9,    8,   10,
    QMetaType::Void, 0x80000000 | 7,   12,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 9,    8,   10,

       0        // eod
};

void viewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        viewer *_t = static_cast<viewer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imgChange((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 1: _t->reloadInfo((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 2: _t->maskInfo((*reinterpret_cast< ImageType::Pointer(*)>(_a[1])),(*reinterpret_cast< ImageTypeProcess::Pointer(*)>(_a[2]))); break;
        case 3: _t->itkInfo((*reinterpret_cast< ImageType::Pointer(*)>(_a[1]))); break;
        case 4: _t->reloadImg((*reinterpret_cast< QImage*(*)>(_a[1]))); break;
        case 5: _t->sendReload(); break;
        case 6: _t->thresholding(); break;
        case 7: _t->thresholding((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->edgeDetect(); break;
        case 9: _t->denosing(); break;
        case 10: _t->erode(); break;
        case 11: _t->dilate(); break;
        case 12: _t->openOpr(); break;
        case 13: _t->closeOpr(); break;
        case 14: _t->binaryDistanceTrans(); break;
        case 15: _t->skeletonize(); break;
        case 16: _t->skRestorate(); break;
        case 17: _t->morphEdge(); break;
        case 18: _t->sendMask(); break;
        case 19: _t->sendItk(); break;
        case 20: _t->maskDilate((*reinterpret_cast< ImageType::Pointer(*)>(_a[1])),(*reinterpret_cast< ImageTypeProcess::Pointer(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (viewer::*_t)(QImage * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&viewer::imgChange)) {
                *result = 0;
            }
        }
        {
            typedef void (viewer::*_t)(QImage * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&viewer::reloadInfo)) {
                *result = 1;
            }
        }
        {
            typedef void (viewer::*_t)(ImageType::Pointer , ImageTypeProcess::Pointer );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&viewer::maskInfo)) {
                *result = 2;
            }
        }
        {
            typedef void (viewer::*_t)(ImageType::Pointer );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&viewer::itkInfo)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject viewer::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_viewer.data,
      qt_meta_data_viewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *viewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *viewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_viewer.stringdata0))
        return static_cast<void*>(const_cast< viewer*>(this));
    return QLabel::qt_metacast(_clname);
}

int viewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void viewer::imgChange(QImage * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void viewer::reloadInfo(QImage * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void viewer::maskInfo(ImageType::Pointer _t1, ImageTypeProcess::Pointer _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void viewer::itkInfo(ImageType::Pointer _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
