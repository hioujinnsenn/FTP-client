/****************************************************************************
** Meta object code from reading C++ file 'upload_qThread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../ftpsock/upload_qThread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'upload_qThread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_uploadThread_t {
    QByteArrayData data[18];
    char stringdata0[189];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_uploadThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_uploadThread_t qt_meta_stringdata_uploadThread = {
    {
QT_MOC_LITERAL(0, 0, 12), // "uploadThread"
QT_MOC_LITERAL(1, 13, 12), // "sendProgress"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "progress"
QT_MOC_LITERAL(4, 36, 2), // "id"
QT_MOC_LITERAL(5, 39, 9), // "finishOne"
QT_MOC_LITERAL(6, 49, 6), // "nextId"
QT_MOC_LITERAL(7, 56, 18), // "send_Dir_filecount"
QT_MOC_LITERAL(8, 75, 9), // "filecount"
QT_MOC_LITERAL(9, 85, 7), // "setStop"
QT_MOC_LITERAL(10, 93, 15), // "receive_filemsg"
QT_MOC_LITERAL(11, 109, 7), // "FileMsg"
QT_MOC_LITERAL(12, 117, 3), // "msg"
QT_MOC_LITERAL(13, 121, 19), // "receive_remote_path"
QT_MOC_LITERAL(14, 141, 6), // "string"
QT_MOC_LITERAL(15, 148, 4), // "path"
QT_MOC_LITERAL(16, 153, 18), // "receive_local_path"
QT_MOC_LITERAL(17, 172, 16) // "receive_pause_id"

    },
    "uploadThread\0sendProgress\0\0progress\0"
    "id\0finishOne\0nextId\0send_Dir_filecount\0"
    "filecount\0setStop\0receive_filemsg\0"
    "FileMsg\0msg\0receive_remote_path\0string\0"
    "path\0receive_local_path\0receive_pause_id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_uploadThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       5,    2,   59,    2, 0x06 /* Public */,
       7,    2,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   69,    2, 0x0a /* Public */,
      10,    1,   70,    2, 0x0a /* Public */,
      13,    1,   73,    2, 0x0a /* Public */,
      16,    1,   76,    2, 0x0a /* Public */,
      17,    1,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void uploadThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<uploadThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->finishOne((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->send_Dir_filecount((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->setStop(); break;
        case 4: _t->receive_filemsg((*reinterpret_cast< FileMsg(*)>(_a[1]))); break;
        case 5: _t->receive_remote_path((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 6: _t->receive_local_path((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 7: _t->receive_pause_id((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (uploadThread::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&uploadThread::sendProgress)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (uploadThread::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&uploadThread::finishOne)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (uploadThread::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&uploadThread::send_Dir_filecount)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject uploadThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_uploadThread.data,
    qt_meta_data_uploadThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *uploadThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *uploadThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_uploadThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int uploadThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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
void uploadThread::sendProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void uploadThread::finishOne(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void uploadThread::send_Dir_filecount(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
