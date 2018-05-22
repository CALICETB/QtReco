/****************************************************************************
** Meta object code from reading C++ file 'DMAHCALMainWindow.h'
**
** Created: Sat May 19 14:40:30 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../source/include/DMAHCALMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DMAHCALMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_DMAHCALMainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      32,   19,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      53,   18,   18,   18, 0x0a,
      65,   18,   18,   18, 0x0a,
      85,   81,   18,   18, 0x0a,
     109,   18,   18,   18, 0x0a,
     129,   18,   18,   18, 0x0a,
     141,   18,   18,   18, 0x0a,
     152,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_DMAHCALMainWindow[] = {
    "DMAHCALMainWindow\0\0type,message\0"
    "log(QString,QString)\0UpdateGUI()\0"
    "UpdateButtons()\0map\0UpdateGUI(ParameterMap)\0"
    "Open_EventDisplay()\0Configure()\0"
    "Find_Run()\0Quit()\0"
};

const QMetaObject DMAHCALMainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DMAHCALMainWindow,
      qt_meta_data_DMAHCALMainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &DMAHCALMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *DMAHCALMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *DMAHCALMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DMAHCALMainWindow))
        return static_cast<void*>(const_cast< DMAHCALMainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DMAHCALMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: log((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: UpdateGUI(); break;
        case 2: UpdateButtons(); break;
        case 3: UpdateGUI((*reinterpret_cast< ParameterMap(*)>(_a[1]))); break;
        case 4: Open_EventDisplay(); break;
        case 5: Configure(); break;
        case 6: Find_Run(); break;
        case 7: Quit(); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void DMAHCALMainWindow::log(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
