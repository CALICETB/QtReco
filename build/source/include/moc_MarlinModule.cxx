/****************************************************************************
** Meta object code from reading C++ file 'MarlinModule.h'
**
** Created: Sat May 19 14:40:30 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../source/include/MarlinModule.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MarlinModule.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MarlinModule[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   14,   13,   13, 0x05,
      48,   13,   13,   13, 0x05,
      58,   13,   13,   13, 0x05,
      68,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      91,   79,   13,   13, 0x0a,
     105,   13,   13,   13, 0x0a,
     114,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MarlinModule[] = {
    "MarlinModule\0\0type,message\0"
    "log(QString,QString)\0started()\0stopped()\0"
    "finished()\0exit_status\0Finished(int)\0"
    "StdOut()\0clean()\0"
};

const QMetaObject MarlinModule::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MarlinModule,
      qt_meta_data_MarlinModule, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MarlinModule::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MarlinModule::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MarlinModule::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MarlinModule))
        return static_cast<void*>(const_cast< MarlinModule*>(this));
    return QObject::qt_metacast(_clname);
}

int MarlinModule::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: log((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: started(); break;
        case 2: stopped(); break;
        case 3: finished(); break;
        case 4: Finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: StdOut(); break;
        case 6: clean(); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MarlinModule::log(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MarlinModule::started()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void MarlinModule::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void MarlinModule::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
