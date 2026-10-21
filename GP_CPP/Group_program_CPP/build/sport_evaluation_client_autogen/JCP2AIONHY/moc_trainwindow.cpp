/****************************************************************************
** Meta object code from reading C++ file 'trainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/ui/trainwindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN11TrainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto TrainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN11TrainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TrainWindow",
        "on_calendarWidget_clicked",
        "",
        "date",
        "on_btnAddTrainToDate_clicked",
        "on_btnDeleteTrainFromDate_clicked",
        "on_btnNewTrainPlan_clicked",
        "on_btnDeleteTrainPlan_clicked",
        "on_btnSaveTrainPlan_clicked",
        "on_listTrainPlans_itemClicked",
        "QListWidgetItem*",
        "item",
        "on_btnAddAction_clicked",
        "on_btnDeleteAction_clicked",
        "on_btnModifyAction_clicked",
        "on_listActions_itemClicked",
        "on_btnAddSet_clicked",
        "on_btnDeleteSet_clicked",
        "on_btnModifySet_clicked",
        "on_listSets_itemClicked",
        "on_btnAddTutorial_clicked",
        "on_btnDeleteTutorial_clicked",
        "on_btnCopyTutorial_clicked",
        "onTrainDataSaved",
        "success",
        "message",
        "onTrainDataLoaded",
        "train*",
        "trainPlan"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_calendarWidget_clicked'
        QtMocHelpers::SlotData<void(const QDate &)>(1, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QDate, 3 },
        }}),
        // Slot 'on_btnAddTrainToDate_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnDeleteTrainFromDate_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnNewTrainPlan_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnDeleteTrainPlan_clicked'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnSaveTrainPlan_clicked'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_listTrainPlans_itemClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'on_btnAddAction_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnDeleteAction_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnModifyAction_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_listActions_itemClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'on_btnAddSet_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnDeleteSet_clicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnModifySet_clicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_listSets_itemClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 },
        }}),
        // Slot 'on_btnAddTutorial_clicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnDeleteTutorial_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_btnCopyTutorial_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onTrainDataSaved'
        QtMocHelpers::SlotData<void(bool, const QString &)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 24 }, { QMetaType::QString, 25 },
        }}),
        // Slot 'onTrainDataLoaded'
        QtMocHelpers::SlotData<void(bool, train *)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 24 }, { 0x80000000 | 27, 28 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TrainWindow, qt_meta_tag_ZN11TrainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TrainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TrainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TrainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN11TrainWindowE_t>.metaTypes,
    nullptr
} };

void TrainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TrainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_calendarWidget_clicked((*reinterpret_cast<std::add_pointer_t<QDate>>(_a[1]))); break;
        case 1: _t->on_btnAddTrainToDate_clicked(); break;
        case 2: _t->on_btnDeleteTrainFromDate_clicked(); break;
        case 3: _t->on_btnNewTrainPlan_clicked(); break;
        case 4: _t->on_btnDeleteTrainPlan_clicked(); break;
        case 5: _t->on_btnSaveTrainPlan_clicked(); break;
        case 6: _t->on_listTrainPlans_itemClicked((*reinterpret_cast<std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 7: _t->on_btnAddAction_clicked(); break;
        case 8: _t->on_btnDeleteAction_clicked(); break;
        case 9: _t->on_btnModifyAction_clicked(); break;
        case 10: _t->on_listActions_itemClicked((*reinterpret_cast<std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 11: _t->on_btnAddSet_clicked(); break;
        case 12: _t->on_btnDeleteSet_clicked(); break;
        case 13: _t->on_btnModifySet_clicked(); break;
        case 14: _t->on_listSets_itemClicked((*reinterpret_cast<std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 15: _t->on_btnAddTutorial_clicked(); break;
        case 16: _t->on_btnDeleteTutorial_clicked(); break;
        case 17: _t->on_btnCopyTutorial_clicked(); break;
        case 18: _t->onTrainDataSaved((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 19: _t->onTrainDataLoaded((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<train*>>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObject *TrainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN11TrainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int TrainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
