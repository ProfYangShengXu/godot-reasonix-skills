/********************************************************************************
** Form generated from reading UI file 'trainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRAINWINDOW_H
#define UI_TRAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TrainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupCalendar;
    QVBoxLayout *verticalLayout;
    QCalendarWidget *calendarWidget;
    QLabel *label;
    QListWidget *listDateTrains;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *btnAddTrainToDate;
    QPushButton *btnDeleteTrainFromDate;
    QGroupBox *groupTrainPlans;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listTrainPlans;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnNewTrainPlan;
    QPushButton *btnDeleteTrainPlan;
    QPushButton *btnSaveTrainPlan;
    QLabel *label_2;
    QLineEdit *lineTrainName;
    QLabel *label_3;
    QListWidget *listActions;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *btnAddAction;
    QPushButton *btnDeleteAction;
    QPushButton *btnModifyAction;
    QGroupBox *groupSets;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QListWidget *listSets;
    QLabel *label_5;
    QFormLayout *formLayout;
    QLabel *label_6;
    QSpinBox *spinWeight;
    QLabel *label_7;
    QSpinBox *spinReps;
    QLabel *label_8;
    QSpinBox *spinGroups;
    QLabel *label_9;
    QSpinBox *spinRest;
    QLabel *label_10;
    QComboBox *comboEval;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *btnAddSet;
    QPushButton *btnDeleteSet;
    QPushButton *btnModifySet;
    QLabel *label_11;
    QListWidget *listTutorials;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *btnAddTutorial;
    QPushButton *btnDeleteTutorial;
    QPushButton *btnCopyTutorial;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *TrainWindow)
    {
        if (TrainWindow->objectName().isEmpty())
            TrainWindow->setObjectName("TrainWindow");
        TrainWindow->resize(1200, 800);
        centralwidget = new QWidget(TrainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName("horizontalLayout");
        groupCalendar = new QGroupBox(centralwidget);
        groupCalendar->setObjectName("groupCalendar");
        verticalLayout = new QVBoxLayout(groupCalendar);
        verticalLayout->setObjectName("verticalLayout");
        calendarWidget = new QCalendarWidget(groupCalendar);
        calendarWidget->setObjectName("calendarWidget");

        verticalLayout->addWidget(calendarWidget);

        label = new QLabel(groupCalendar);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        listDateTrains = new QListWidget(groupCalendar);
        listDateTrains->setObjectName("listDateTrains");

        verticalLayout->addWidget(listDateTrains);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        btnAddTrainToDate = new QPushButton(groupCalendar);
        btnAddTrainToDate->setObjectName("btnAddTrainToDate");

        horizontalLayout_2->addWidget(btnAddTrainToDate);

        btnDeleteTrainFromDate = new QPushButton(groupCalendar);
        btnDeleteTrainFromDate->setObjectName("btnDeleteTrainFromDate");

        horizontalLayout_2->addWidget(btnDeleteTrainFromDate);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout->addWidget(groupCalendar);

        groupTrainPlans = new QGroupBox(centralwidget);
        groupTrainPlans->setObjectName("groupTrainPlans");
        verticalLayout_2 = new QVBoxLayout(groupTrainPlans);
        verticalLayout_2->setObjectName("verticalLayout_2");
        listTrainPlans = new QListWidget(groupTrainPlans);
        listTrainPlans->setObjectName("listTrainPlans");

        verticalLayout_2->addWidget(listTrainPlans);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        btnNewTrainPlan = new QPushButton(groupTrainPlans);
        btnNewTrainPlan->setObjectName("btnNewTrainPlan");

        horizontalLayout_3->addWidget(btnNewTrainPlan);

        btnDeleteTrainPlan = new QPushButton(groupTrainPlans);
        btnDeleteTrainPlan->setObjectName("btnDeleteTrainPlan");

        horizontalLayout_3->addWidget(btnDeleteTrainPlan);

        btnSaveTrainPlan = new QPushButton(groupTrainPlans);
        btnSaveTrainPlan->setObjectName("btnSaveTrainPlan");

        horizontalLayout_3->addWidget(btnSaveTrainPlan);


        verticalLayout_2->addLayout(horizontalLayout_3);

        label_2 = new QLabel(groupTrainPlans);
        label_2->setObjectName("label_2");

        verticalLayout_2->addWidget(label_2);

        lineTrainName = new QLineEdit(groupTrainPlans);
        lineTrainName->setObjectName("lineTrainName");

        verticalLayout_2->addWidget(lineTrainName);

        label_3 = new QLabel(groupTrainPlans);
        label_3->setObjectName("label_3");

        verticalLayout_2->addWidget(label_3);

        listActions = new QListWidget(groupTrainPlans);
        listActions->setObjectName("listActions");

        verticalLayout_2->addWidget(listActions);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        btnAddAction = new QPushButton(groupTrainPlans);
        btnAddAction->setObjectName("btnAddAction");

        horizontalLayout_4->addWidget(btnAddAction);

        btnDeleteAction = new QPushButton(groupTrainPlans);
        btnDeleteAction->setObjectName("btnDeleteAction");

        horizontalLayout_4->addWidget(btnDeleteAction);

        btnModifyAction = new QPushButton(groupTrainPlans);
        btnModifyAction->setObjectName("btnModifyAction");

        horizontalLayout_4->addWidget(btnModifyAction);


        verticalLayout_2->addLayout(horizontalLayout_4);


        horizontalLayout->addWidget(groupTrainPlans);

        groupSets = new QGroupBox(centralwidget);
        groupSets->setObjectName("groupSets");
        verticalLayout_3 = new QVBoxLayout(groupSets);
        verticalLayout_3->setObjectName("verticalLayout_3");
        label_4 = new QLabel(groupSets);
        label_4->setObjectName("label_4");

        verticalLayout_3->addWidget(label_4);

        listSets = new QListWidget(groupSets);
        listSets->setObjectName("listSets");

        verticalLayout_3->addWidget(listSets);

        label_5 = new QLabel(groupSets);
        label_5->setObjectName("label_5");

        verticalLayout_3->addWidget(label_5);

        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        label_6 = new QLabel(groupSets);
        label_6->setObjectName("label_6");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, label_6);

        spinWeight = new QSpinBox(groupSets);
        spinWeight->setObjectName("spinWeight");
        spinWeight->setMaximum(500);
        spinWeight->setValue(60);

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, spinWeight);

        label_7 = new QLabel(groupSets);
        label_7->setObjectName("label_7");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, label_7);

        spinReps = new QSpinBox(groupSets);
        spinReps->setObjectName("spinReps");
        spinReps->setMinimum(1);
        spinReps->setMaximum(100);
        spinReps->setValue(10);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, spinReps);

        label_8 = new QLabel(groupSets);
        label_8->setObjectName("label_8");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, label_8);

        spinGroups = new QSpinBox(groupSets);
        spinGroups->setObjectName("spinGroups");
        spinGroups->setMinimum(1);
        spinGroups->setMaximum(20);
        spinGroups->setValue(3);

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, spinGroups);

        label_9 = new QLabel(groupSets);
        label_9->setObjectName("label_9");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, label_9);

        spinRest = new QSpinBox(groupSets);
        spinRest->setObjectName("spinRest");
        spinRest->setMaximum(600);
        spinRest->setValue(90);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, spinRest);

        label_10 = new QLabel(groupSets);
        label_10->setObjectName("label_10");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, label_10);

        comboEval = new QComboBox(groupSets);
        comboEval->addItem(QString());
        comboEval->addItem(QString());
        comboEval->addItem(QString());
        comboEval->setObjectName("comboEval");

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, comboEval);


        verticalLayout_3->addLayout(formLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        btnAddSet = new QPushButton(groupSets);
        btnAddSet->setObjectName("btnAddSet");

        horizontalLayout_5->addWidget(btnAddSet);

        btnDeleteSet = new QPushButton(groupSets);
        btnDeleteSet->setObjectName("btnDeleteSet");

        horizontalLayout_5->addWidget(btnDeleteSet);

        btnModifySet = new QPushButton(groupSets);
        btnModifySet->setObjectName("btnModifySet");

        horizontalLayout_5->addWidget(btnModifySet);


        verticalLayout_3->addLayout(horizontalLayout_5);

        label_11 = new QLabel(groupSets);
        label_11->setObjectName("label_11");

        verticalLayout_3->addWidget(label_11);

        listTutorials = new QListWidget(groupSets);
        listTutorials->setObjectName("listTutorials");

        verticalLayout_3->addWidget(listTutorials);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        btnAddTutorial = new QPushButton(groupSets);
        btnAddTutorial->setObjectName("btnAddTutorial");

        horizontalLayout_6->addWidget(btnAddTutorial);

        btnDeleteTutorial = new QPushButton(groupSets);
        btnDeleteTutorial->setObjectName("btnDeleteTutorial");

        horizontalLayout_6->addWidget(btnDeleteTutorial);

        btnCopyTutorial = new QPushButton(groupSets);
        btnCopyTutorial->setObjectName("btnCopyTutorial");

        horizontalLayout_6->addWidget(btnCopyTutorial);


        verticalLayout_3->addLayout(horizontalLayout_6);


        horizontalLayout->addWidget(groupSets);

        TrainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(TrainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1200, 21));
        TrainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(TrainWindow);
        statusbar->setObjectName("statusbar");
        TrainWindow->setStatusBar(statusbar);

        retranslateUi(TrainWindow);

        QMetaObject::connectSlotsByName(TrainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *TrainWindow)
    {
        TrainWindow->setWindowTitle(QCoreApplication::translate("TrainWindow", "\350\256\255\347\273\203\350\256\241\345\210\222\347\256\241\347\220\206\347\263\273\347\273\237", nullptr));
        groupCalendar->setTitle(QCoreApplication::translate("TrainWindow", "\346\227\245\345\216\206\350\247\206\345\233\276", nullptr));
        label->setText(QCoreApplication::translate("TrainWindow", "\350\257\245\346\227\245\346\234\237\347\232\204\350\256\255\347\273\203\350\256\241\345\210\222\357\274\232", nullptr));
        btnAddTrainToDate->setText(QCoreApplication::translate("TrainWindow", "\346\267\273\345\212\240\350\256\255\347\273\203\350\256\241\345\210\222", nullptr));
        btnDeleteTrainFromDate->setText(QCoreApplication::translate("TrainWindow", "\345\210\240\351\231\244\350\256\255\347\273\203\350\256\241\345\210\222", nullptr));
        groupTrainPlans->setTitle(QCoreApplication::translate("TrainWindow", "\350\256\255\347\273\203\350\256\241\345\210\222\345\272\223", nullptr));
        btnNewTrainPlan->setText(QCoreApplication::translate("TrainWindow", "\346\226\260\345\273\272\350\256\241\345\210\222", nullptr));
        btnDeleteTrainPlan->setText(QCoreApplication::translate("TrainWindow", "\345\210\240\351\231\244\350\256\241\345\210\222", nullptr));
        btnSaveTrainPlan->setText(QCoreApplication::translate("TrainWindow", "\344\277\235\345\255\230\350\256\241\345\210\222", nullptr));
        label_2->setText(QCoreApplication::translate("TrainWindow", "\350\256\241\345\210\222\345\220\215\347\247\260\357\274\232", nullptr));
        label_3->setText(QCoreApplication::translate("TrainWindow", "\345\212\250\344\275\234\345\210\227\350\241\250\357\274\232", nullptr));
        btnAddAction->setText(QCoreApplication::translate("TrainWindow", "\346\267\273\345\212\240\345\212\250\344\275\234", nullptr));
        btnDeleteAction->setText(QCoreApplication::translate("TrainWindow", "\345\210\240\351\231\244\345\212\250\344\275\234", nullptr));
        btnModifyAction->setText(QCoreApplication::translate("TrainWindow", "\344\277\256\346\224\271\345\212\250\344\275\234", nullptr));
        groupSets->setTitle(QCoreApplication::translate("TrainWindow", "\350\256\255\347\273\203\347\273\204\350\256\276\347\275\256", nullptr));
        label_4->setText(QCoreApplication::translate("TrainWindow", "\350\256\255\347\273\203\347\273\204\345\210\227\350\241\250\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("TrainWindow", "\346\267\273\345\212\240\346\226\260\350\256\255\347\273\203\347\273\204\357\274\232", nullptr));
        label_6->setText(QCoreApplication::translate("TrainWindow", "\351\207\215\351\207\217(kg):", nullptr));
        label_7->setText(QCoreApplication::translate("TrainWindow", "\346\254\241\346\225\260:", nullptr));
        label_8->setText(QCoreApplication::translate("TrainWindow", "\347\273\204\346\225\260:", nullptr));
        label_9->setText(QCoreApplication::translate("TrainWindow", "\344\274\221\346\201\257(\347\247\222):", nullptr));
        label_10->setText(QCoreApplication::translate("TrainWindow", "\351\232\276\345\272\246\350\257\204\344\273\267:", nullptr));
        comboEval->setItemText(0, QCoreApplication::translate("TrainWindow", "\350\275\273\346\235\276", nullptr));
        comboEval->setItemText(1, QCoreApplication::translate("TrainWindow", "\344\270\255\347\255\211", nullptr));
        comboEval->setItemText(2, QCoreApplication::translate("TrainWindow", "\345\233\260\351\232\276", nullptr));

        btnAddSet->setText(QCoreApplication::translate("TrainWindow", "\346\267\273\345\212\240\350\256\255\347\273\203\347\273\204", nullptr));
        btnDeleteSet->setText(QCoreApplication::translate("TrainWindow", "\345\210\240\351\231\244\350\256\255\347\273\203\347\273\204", nullptr));
        btnModifySet->setText(QCoreApplication::translate("TrainWindow", "\344\277\256\346\224\271\350\256\255\347\273\203\347\273\204", nullptr));
        label_11->setText(QCoreApplication::translate("TrainWindow", "\346\225\231\345\255\246\350\247\206\351\242\221\351\223\276\346\216\245\357\274\232", nullptr));
        btnAddTutorial->setText(QCoreApplication::translate("TrainWindow", "\346\267\273\345\212\240\351\223\276\346\216\245", nullptr));
        btnDeleteTutorial->setText(QCoreApplication::translate("TrainWindow", "\345\210\240\351\231\244\351\223\276\346\216\245", nullptr));
        btnCopyTutorial->setText(QCoreApplication::translate("TrainWindow", "\345\244\215\345\210\266\351\223\276\346\216\245", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrainWindow: public Ui_TrainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRAINWINDOW_H
