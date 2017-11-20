/****************************************************************************
** Meta object code from reading C++ file 'qgleswidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qgleswidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgleswidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QGLESWIDGET_t {
    QByteArrayData data[14];
    char stringdata0[113];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QGLESWIDGET_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QGLESWIDGET_t qt_meta_stringdata_QGLESWIDGET = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QGLESWIDGET"
QT_MOC_LITERAL(1, 12, 12), // "sendKeyEvent"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "KEYMODE"
QT_MOC_LITERAL(4, 34, 3), // "key"
QT_MOC_LITERAL(5, 38, 14), // "sendMouseEvent"
QT_MOC_LITERAL(6, 53, 9), // "MOUSEMODE"
QT_MOC_LITERAL(7, 63, 5), // "mouse"
QT_MOC_LITERAL(8, 69, 10), // "CELL::int2"
QT_MOC_LITERAL(9, 80, 6), // "pstart"
QT_MOC_LITERAL(10, 87, 4), // "pend"
QT_MOC_LITERAL(11, 92, 14), // "sendWheelEvent"
QT_MOC_LITERAL(12, 107, 1), // "p"
QT_MOC_LITERAL(13, 109, 3) // "pos"

    },
    "QGLESWIDGET\0sendKeyEvent\0\0KEYMODE\0key\0"
    "sendMouseEvent\0MOUSEMODE\0mouse\0"
    "CELL::int2\0pstart\0pend\0sendWheelEvent\0"
    "p\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QGLESWIDGET[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       5,    3,   32,    2, 0x06 /* Public */,
      11,    3,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8, 0x80000000 | 8,    7,    9,   10,
    QMetaType::Void, 0x80000000 | 6, QMetaType::Int, 0x80000000 | 8,    7,   12,   13,

       0        // eod
};

void QGLESWIDGET::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QGLESWIDGET *_t = static_cast<QGLESWIDGET *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendKeyEvent((*reinterpret_cast< KEYMODE(*)>(_a[1]))); break;
        case 1: _t->sendMouseEvent((*reinterpret_cast< MOUSEMODE(*)>(_a[1])),(*reinterpret_cast< CELL::int2(*)>(_a[2])),(*reinterpret_cast< CELL::int2(*)>(_a[3]))); break;
        case 2: _t->sendWheelEvent((*reinterpret_cast< MOUSEMODE(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< CELL::int2(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QGLESWIDGET::*_t)(KEYMODE );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QGLESWIDGET::sendKeyEvent)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QGLESWIDGET::*_t)(MOUSEMODE , CELL::int2 , CELL::int2 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QGLESWIDGET::sendMouseEvent)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QGLESWIDGET::*_t)(MOUSEMODE , int , CELL::int2 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QGLESWIDGET::sendWheelEvent)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject QGLESWIDGET::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QGLESWIDGET.data,
      qt_meta_data_QGLESWIDGET,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QGLESWIDGET::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QGLESWIDGET::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QGLESWIDGET.stringdata0))
        return static_cast<void*>(const_cast< QGLESWIDGET*>(this));
    return QWidget::qt_metacast(_clname);
}

int QGLESWIDGET::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QGLESWIDGET::sendKeyEvent(KEYMODE _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QGLESWIDGET::sendMouseEvent(MOUSEMODE _t1, CELL::int2 _t2, CELL::int2 _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QGLESWIDGET::sendWheelEvent(MOUSEMODE _t1, int _t2, CELL::int2 _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
