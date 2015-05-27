/****************************************************************************
** Meta object code from reading C++ file 'menu.h'
**
<<<<<<< HEAD
** Created: Wed May 27 10:57:46 2015
=======
** Created: Wed May 27 13:31:59 2015
>>>>>>> 9acc3238afe5aefa437a2908be74afa291926fea
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pro/menu.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'menu.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QMainMenu[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      27,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QMainMenu[] = {
    "QMainMenu\0\0OnShowPressed()\0Histogram()\0"
};

void QMainMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QMainMenu *_t = static_cast<QMainMenu *>(_o);
        switch (_id) {
        case 0: _t->OnShowPressed(); break;
        case 1: _t->Histogram(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QMainMenu::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QMainMenu::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QMainMenu,
      qt_meta_data_QMainMenu, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QMainMenu::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QMainMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QMainMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QMainMenu))
        return static_cast<void*>(const_cast< QMainMenu*>(this));
    return QWidget::qt_metacast(_clname);
}

int QMainMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
