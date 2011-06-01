/****************************************************************************
** Meta object code from reading C++ file 'mapzoom.h'
**
** Created: Mon Jan 17 16:00:07 2011
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mapzoom.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mapzoom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MapZoom[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      22,    8,    8,    8, 0x08,
      33,    8,    8,    8, 0x08,
      51,    8,    8,    8, 0x08,
      70,    8,    8,    8, 0x08,
      81,    8,    8,    8, 0x08,
      95,    8,    8,    8, 0x08,
     112,    8,    8,    8, 0x08,
     130,    8,    8,    8, 0x08,
     148,    8,    8,    8, 0x08,
     161,    8,    8,    8, 0x08,
     179,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MapZoom[] = {
    "MapZoom\0\0runGebabel()\0aboutOsm()\0"
    "zoom_plus_click()\0zoom_minus_click()\0"
    "set_home()\0draw_tracks()\0draw_waypoints()\0"
    "actual_position()\0control_devices()\0"
    "readStdout()\0connectToDevice()\0"
    "infoAboutDevice()\0"
};

const QMetaObject MapZoom::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MapZoom,
      qt_meta_data_MapZoom, 0 }
};

const QMetaObject *MapZoom::metaObject() const
{
    return &staticMetaObject;
}

void *MapZoom::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MapZoom))
        return static_cast<void*>(const_cast< MapZoom*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MapZoom::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: runGebabel(); break;
        case 1: aboutOsm(); break;
        case 2: zoom_plus_click(); break;
        case 3: zoom_minus_click(); break;
        case 4: set_home(); break;
        case 5: draw_tracks(); break;
        case 6: draw_waypoints(); break;
        case 7: actual_position(); break;
        case 8: control_devices(); break;
        case 9: readStdout(); break;
        case 10: connectToDevice(); break;
        case 11: infoAboutDevice(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
