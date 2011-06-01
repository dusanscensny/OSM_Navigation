/****************************************************************************
** Meta object code from reading C++ file 'slippymap.h'
**
** Created: Mon Jan 17 15:59:58 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "slippymap.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'slippymap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SlippyMap[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      16,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   31,   10,   10, 0x08,
      71,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SlippyMap[] = {
    "SlippyMap\0\0rect\0updated(QRect)\0reply\0"
    "handleNetworkData(QNetworkReply*)\0"
    "download()\0"
};

const QMetaObject SlippyMap::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SlippyMap,
      qt_meta_data_SlippyMap, 0 }
};

const QMetaObject *SlippyMap::metaObject() const
{
    return &staticMetaObject;
}

void *SlippyMap::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SlippyMap))
        return static_cast<void*>(const_cast< SlippyMap*>(this));
    return QObject::qt_metacast(_clname);
}

int SlippyMap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updated((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 1: handleNetworkData((*reinterpret_cast< QNetworkReply*(*)>(_a[1]))); break;
        case 2: download(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void SlippyMap::updated(const QRect & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
