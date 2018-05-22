/****************************************************************************
** Meta object code from reading C++ file 'ApplicationModuleApi.h'
**
** Created: Sat May 19 14:40:30 2018
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../source/include/ApplicationModuleApi.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ApplicationModuleApi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ApplicationModuleApi[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      35,   22,   21,   21, 0x05,
      60,   56,   21,   21, 0x05,
      81,   21,   21,   21, 0x05,
      91,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     101,   21,   21,   21, 0x0a,
     109,   21,   21,   21, 0x0a,
     116,   21,   21,   21, 0x0a,
     123,   21,   21,   21, 0x0a,
     135,   21,   21,   21, 0x0a,
     146,   21,   21,   21, 0x0a,
     159,   21,   21,   21, 0x0a,
     169,   22,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ApplicationModuleApi[] = {
    "ApplicationModuleApi\0\0type,message\0"
    "log(QString,QString)\0map\0update(ParameterMap)\0"
    "started()\0stopped()\0Start()\0Stop()\0"
    "Quit()\0Configure()\0DoUpdate()\0"
    "DoAnalysis()\0OpenDEH()\0"
    "RedirectMessage(QString,QString)\0"
};

const QMetaObject ApplicationModuleApi::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ApplicationModuleApi,
      qt_meta_data_ApplicationModuleApi, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ApplicationModuleApi::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ApplicationModuleApi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ApplicationModuleApi::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ApplicationModuleApi))
        return static_cast<void*>(const_cast< ApplicationModuleApi*>(this));
    return QThread::qt_metacast(_clname);
}

int ApplicationModuleApi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: log((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: update((*reinterpret_cast< ParameterMap(*)>(_a[1]))); break;
        case 2: started(); break;
        case 3: stopped(); break;
        case 4: Start(); break;
        case 5: Stop(); break;
        case 6: Quit(); break;
        case 7: Configure(); break;
        case 8: DoUpdate(); break;
        case 9: DoAnalysis(); break;
        case 10: OpenDEH(); break;
        case 11: RedirectMessage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ApplicationModuleApi::log(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ApplicationModuleApi::update(ParameterMap _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ApplicationModuleApi::started()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ApplicationModuleApi::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
