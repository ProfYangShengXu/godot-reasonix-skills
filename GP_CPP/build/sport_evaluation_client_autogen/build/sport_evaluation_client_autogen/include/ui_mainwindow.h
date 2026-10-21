/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tabBasicInfo;
    QFormLayout *formLayout;
    QLabel *labelName;
    QLineEdit *editName;
    QLabel *labelAge;
    QSpinBox *spinAge;
    QLabel *labelGender;
    QComboBox *comboGender;
    QLabel *labelWeight;
    QDoubleSpinBox *spinWeight;
    QLabel *labelHeight;
    QDoubleSpinBox *spinHeight;
    QLabel *labelType;
    QComboBox *comboType;
    QWidget *tabTestData;
    QFormLayout *formLayout_2;
    QLabel *labelLungCapacity;
    QDoubleSpinBox *spinLungCapacity;
    QLabel *labelRun50m;
    QDoubleSpinBox *spinRun50m;
    QLabel *labelSitAndReach;
    QDoubleSpinBox *spinSitAndReach;
    QLabel *labelStandingJump;
    QDoubleSpinBox *spinStandingJump;
    QLabel *labelPullUps;
    QSpinBox *spinPullUps;
    QLabel *labelLongRun;
    QHBoxLayout *horizontalLayout;
    QSpinBox *spinLongRunMin;
    QSpinBox *spinLongRunSec;
    QWidget *tabOptional;
    QFormLayout *formLayout_3;
    QCheckBox *checkBodyFat;
    QDoubleSpinBox *spinBodyFat;
    QCheckBox *checkSquat;
    QDoubleSpinBox *spinSquat;
    QCheckBox *checkBenchPress;
    QDoubleSpinBox *spinBenchPress;
    QCheckBox *checkDeadlift;
    QDoubleSpinBox *spinDeadlift;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonCalculate;
    QLabel *labelResult;
    QLCDNumber *lcdGPA;
    QPushButton *btnOpenTrainWindow;
    QPushButton *btnOpenChatWindow;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabBasicInfo = new QWidget();
        tabBasicInfo->setObjectName("tabBasicInfo");
        formLayout = new QFormLayout(tabBasicInfo);
        formLayout->setObjectName("formLayout");
        labelName = new QLabel(tabBasicInfo);
        labelName->setObjectName("labelName");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, labelName);

        editName = new QLineEdit(tabBasicInfo);
        editName->setObjectName("editName");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, editName);

        labelAge = new QLabel(tabBasicInfo);
        labelAge->setObjectName("labelAge");

        formLayout->setWidget(1, QFormLayout::ItemRole::LabelRole, labelAge);

        spinAge = new QSpinBox(tabBasicInfo);
        spinAge->setObjectName("spinAge");
        spinAge->setMinimum(1);
        spinAge->setMaximum(120);

        formLayout->setWidget(1, QFormLayout::ItemRole::FieldRole, spinAge);

        labelGender = new QLabel(tabBasicInfo);
        labelGender->setObjectName("labelGender");

        formLayout->setWidget(2, QFormLayout::ItemRole::LabelRole, labelGender);

        comboGender = new QComboBox(tabBasicInfo);
        comboGender->addItem(QString());
        comboGender->addItem(QString());
        comboGender->setObjectName("comboGender");

        formLayout->setWidget(2, QFormLayout::ItemRole::FieldRole, comboGender);

        labelWeight = new QLabel(tabBasicInfo);
        labelWeight->setObjectName("labelWeight");

        formLayout->setWidget(3, QFormLayout::ItemRole::LabelRole, labelWeight);

        spinWeight = new QDoubleSpinBox(tabBasicInfo);
        spinWeight->setObjectName("spinWeight");
        spinWeight->setDecimals(1);
        spinWeight->setMaximum(300.000000000000000);

        formLayout->setWidget(3, QFormLayout::ItemRole::FieldRole, spinWeight);

        labelHeight = new QLabel(tabBasicInfo);
        labelHeight->setObjectName("labelHeight");

        formLayout->setWidget(4, QFormLayout::ItemRole::LabelRole, labelHeight);

        spinHeight = new QDoubleSpinBox(tabBasicInfo);
        spinHeight->setObjectName("spinHeight");
        spinHeight->setDecimals(1);
        spinHeight->setMaximum(300.000000000000000);

        formLayout->setWidget(4, QFormLayout::ItemRole::FieldRole, spinHeight);

        labelType = new QLabel(tabBasicInfo);
        labelType->setObjectName("labelType");

        formLayout->setWidget(5, QFormLayout::ItemRole::LabelRole, labelType);

        comboType = new QComboBox(tabBasicInfo);
        comboType->addItem(QString());
        comboType->addItem(QString());
        comboType->addItem(QString());
        comboType->addItem(QString());
        comboType->addItem(QString());
        comboType->addItem(QString());
        comboType->setObjectName("comboType");

        formLayout->setWidget(5, QFormLayout::ItemRole::FieldRole, comboType);

        tabWidget->addTab(tabBasicInfo, QString());
        tabTestData = new QWidget();
        tabTestData->setObjectName("tabTestData");
        formLayout_2 = new QFormLayout(tabTestData);
        formLayout_2->setObjectName("formLayout_2");
        labelLungCapacity = new QLabel(tabTestData);
        labelLungCapacity->setObjectName("labelLungCapacity");

        formLayout_2->setWidget(0, QFormLayout::ItemRole::LabelRole, labelLungCapacity);

        spinLungCapacity = new QDoubleSpinBox(tabTestData);
        spinLungCapacity->setObjectName("spinLungCapacity");
        spinLungCapacity->setMaximum(10000.000000000000000);

        formLayout_2->setWidget(0, QFormLayout::ItemRole::FieldRole, spinLungCapacity);

        labelRun50m = new QLabel(tabTestData);
        labelRun50m->setObjectName("labelRun50m");

        formLayout_2->setWidget(1, QFormLayout::ItemRole::LabelRole, labelRun50m);

        spinRun50m = new QDoubleSpinBox(tabTestData);
        spinRun50m->setObjectName("spinRun50m");
        spinRun50m->setDecimals(2);
        spinRun50m->setMaximum(30.000000000000000);

        formLayout_2->setWidget(1, QFormLayout::ItemRole::FieldRole, spinRun50m);

        labelSitAndReach = new QLabel(tabTestData);
        labelSitAndReach->setObjectName("labelSitAndReach");

        formLayout_2->setWidget(2, QFormLayout::ItemRole::LabelRole, labelSitAndReach);

        spinSitAndReach = new QDoubleSpinBox(tabTestData);
        spinSitAndReach->setObjectName("spinSitAndReach");
        spinSitAndReach->setMinimum(-50.000000000000000);
        spinSitAndReach->setMaximum(50.000000000000000);

        formLayout_2->setWidget(2, QFormLayout::ItemRole::FieldRole, spinSitAndReach);

        labelStandingJump = new QLabel(tabTestData);
        labelStandingJump->setObjectName("labelStandingJump");

        formLayout_2->setWidget(3, QFormLayout::ItemRole::LabelRole, labelStandingJump);

        spinStandingJump = new QDoubleSpinBox(tabTestData);
        spinStandingJump->setObjectName("spinStandingJump");
        spinStandingJump->setDecimals(2);
        spinStandingJump->setMaximum(5.000000000000000);

        formLayout_2->setWidget(3, QFormLayout::ItemRole::FieldRole, spinStandingJump);

        labelPullUps = new QLabel(tabTestData);
        labelPullUps->setObjectName("labelPullUps");

        formLayout_2->setWidget(4, QFormLayout::ItemRole::LabelRole, labelPullUps);

        spinPullUps = new QSpinBox(tabTestData);
        spinPullUps->setObjectName("spinPullUps");
        spinPullUps->setMaximum(100);

        formLayout_2->setWidget(4, QFormLayout::ItemRole::FieldRole, spinPullUps);

        labelLongRun = new QLabel(tabTestData);
        labelLongRun->setObjectName("labelLongRun");

        formLayout_2->setWidget(5, QFormLayout::ItemRole::LabelRole, labelLongRun);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        spinLongRunMin = new QSpinBox(tabTestData);
        spinLongRunMin->setObjectName("spinLongRunMin");
        spinLongRunMin->setMaximum(30);

        horizontalLayout->addWidget(spinLongRunMin);

        spinLongRunSec = new QSpinBox(tabTestData);
        spinLongRunSec->setObjectName("spinLongRunSec");
        spinLongRunSec->setMaximum(59);

        horizontalLayout->addWidget(spinLongRunSec);


        formLayout_2->setLayout(5, QFormLayout::ItemRole::FieldRole, horizontalLayout);

        tabWidget->addTab(tabTestData, QString());
        tabOptional = new QWidget();
        tabOptional->setObjectName("tabOptional");
        formLayout_3 = new QFormLayout(tabOptional);
        formLayout_3->setObjectName("formLayout_3");
        checkBodyFat = new QCheckBox(tabOptional);
        checkBodyFat->setObjectName("checkBodyFat");

        formLayout_3->setWidget(0, QFormLayout::ItemRole::LabelRole, checkBodyFat);

        spinBodyFat = new QDoubleSpinBox(tabOptional);
        spinBodyFat->setObjectName("spinBodyFat");
        spinBodyFat->setEnabled(false);
        spinBodyFat->setMaximum(100.000000000000000);

        formLayout_3->setWidget(0, QFormLayout::ItemRole::FieldRole, spinBodyFat);

        checkSquat = new QCheckBox(tabOptional);
        checkSquat->setObjectName("checkSquat");

        formLayout_3->setWidget(1, QFormLayout::ItemRole::LabelRole, checkSquat);

        spinSquat = new QDoubleSpinBox(tabOptional);
        spinSquat->setObjectName("spinSquat");
        spinSquat->setEnabled(false);
        spinSquat->setMaximum(1000.000000000000000);

        formLayout_3->setWidget(1, QFormLayout::ItemRole::FieldRole, spinSquat);

        checkBenchPress = new QCheckBox(tabOptional);
        checkBenchPress->setObjectName("checkBenchPress");

        formLayout_3->setWidget(2, QFormLayout::ItemRole::LabelRole, checkBenchPress);

        spinBenchPress = new QDoubleSpinBox(tabOptional);
        spinBenchPress->setObjectName("spinBenchPress");
        spinBenchPress->setEnabled(false);
        spinBenchPress->setMaximum(500.000000000000000);

        formLayout_3->setWidget(2, QFormLayout::ItemRole::FieldRole, spinBenchPress);

        checkDeadlift = new QCheckBox(tabOptional);
        checkDeadlift->setObjectName("checkDeadlift");

        formLayout_3->setWidget(3, QFormLayout::ItemRole::LabelRole, checkDeadlift);

        spinDeadlift = new QDoubleSpinBox(tabOptional);
        spinDeadlift->setObjectName("spinDeadlift");
        spinDeadlift->setEnabled(false);
        spinDeadlift->setMaximum(1000.000000000000000);

        formLayout_3->setWidget(3, QFormLayout::ItemRole::FieldRole, spinDeadlift);

        tabWidget->addTab(tabOptional, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        buttonCalculate = new QPushButton(centralwidget);
        buttonCalculate->setObjectName("buttonCalculate");

        horizontalLayout_2->addWidget(buttonCalculate);

        labelResult = new QLabel(centralwidget);
        labelResult->setObjectName("labelResult");

        horizontalLayout_2->addWidget(labelResult);

        lcdGPA = new QLCDNumber(centralwidget);
        lcdGPA->setObjectName("lcdGPA");

        horizontalLayout_2->addWidget(lcdGPA);


        verticalLayout->addLayout(horizontalLayout_2);

        btnOpenTrainWindow = new QPushButton(centralwidget);
        btnOpenTrainWindow->setObjectName("btnOpenTrainWindow");

        verticalLayout->addWidget(btnOpenTrainWindow);

        btnOpenChatWindow = new QPushButton(centralwidget);
        btnOpenChatWindow->setObjectName("btnOpenChatWindow");

        verticalLayout->addWidget(btnOpenChatWindow);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\277\220\345\212\250\350\257\204\344\274\260\347\263\273\347\273\237", nullptr));
        labelName->setText(QCoreApplication::translate("MainWindow", "\345\247\223\345\220\215\357\274\232", nullptr));
        labelAge->setText(QCoreApplication::translate("MainWindow", "\345\271\264\351\276\204\357\274\232", nullptr));
        labelGender->setText(QCoreApplication::translate("MainWindow", "\346\200\247\345\210\253\357\274\232", nullptr));
        comboGender->setItemText(0, QCoreApplication::translate("MainWindow", "\347\224\267", nullptr));
        comboGender->setItemText(1, QCoreApplication::translate("MainWindow", "\345\245\263", nullptr));

        labelWeight->setText(QCoreApplication::translate("MainWindow", "\344\275\223\351\207\215(kg)\357\274\232", nullptr));
        labelHeight->setText(QCoreApplication::translate("MainWindow", "\350\272\253\351\253\230(cm)\357\274\232", nullptr));
        labelType->setText(QCoreApplication::translate("MainWindow", "\347\224\250\346\210\267\347\261\273\345\236\213\357\274\232", nullptr));
        comboType->setItemText(0, QCoreApplication::translate("MainWindow", "\345\237\272\347\241\200\347\224\250\346\210\267", nullptr));
        comboType->setItemText(1, QCoreApplication::translate("MainWindow", "\344\270\276\351\207\215\350\277\220\345\212\250\345\221\230", nullptr));
        comboType->setItemText(2, QCoreApplication::translate("MainWindow", "\351\225\277\350\267\221\350\277\220\345\212\250\345\221\230", nullptr));
        comboType->setItemText(3, QCoreApplication::translate("MainWindow", "\347\237\255\350\267\221\350\277\220\345\212\250\345\221\230", nullptr));
        comboType->setItemText(4, QCoreApplication::translate("MainWindow", "\345\201\245\347\276\216\350\277\220\345\212\250\345\221\230", nullptr));
        comboType->setItemText(5, QCoreApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\347\224\250\346\210\267", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(tabBasicInfo), QCoreApplication::translate("MainWindow", "\345\237\272\346\234\254\344\277\241\346\201\257", nullptr));
        labelLungCapacity->setText(QCoreApplication::translate("MainWindow", "\350\202\272\346\264\273\351\207\217(ml)\357\274\232", nullptr));
        labelRun50m->setText(QCoreApplication::translate("MainWindow", "50\347\261\263\350\267\221(\347\247\222)\357\274\232", nullptr));
        labelSitAndReach->setText(QCoreApplication::translate("MainWindow", "\344\275\223\345\211\215\345\261\210(cm)\357\274\232", nullptr));
        labelStandingJump->setText(QCoreApplication::translate("MainWindow", "\347\253\213\345\256\232\350\267\263\350\277\234(m)\357\274\232", nullptr));
        labelPullUps->setText(QCoreApplication::translate("MainWindow", "\345\274\225\344\275\223\345\220\221\344\270\212(\346\254\241)\357\274\232", nullptr));
        labelLongRun->setText(QCoreApplication::translate("MainWindow", "\351\225\277\350\267\221\346\227\266\351\227\264\357\274\232", nullptr));
        spinLongRunMin->setSuffix(QCoreApplication::translate("MainWindow", "\345\210\206", nullptr));
        spinLongRunSec->setSuffix(QCoreApplication::translate("MainWindow", "\347\247\222", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabTestData), QCoreApplication::translate("MainWindow", "\344\275\223\346\265\213\346\225\260\346\215\256", nullptr));
        checkBodyFat->setText(QCoreApplication::translate("MainWindow", "\344\275\223\350\204\202\347\216\207\346\265\213\350\257\225", nullptr));
        spinBodyFat->setSuffix(QCoreApplication::translate("MainWindow", "%", nullptr));
        checkSquat->setText(QCoreApplication::translate("MainWindow", "\346\267\261\350\271\262\346\265\213\350\257\225", nullptr));
        spinSquat->setSuffix(QCoreApplication::translate("MainWindow", "kg", nullptr));
        checkBenchPress->setText(QCoreApplication::translate("MainWindow", "\345\215\247\346\216\250\346\265\213\350\257\225", nullptr));
        spinBenchPress->setSuffix(QCoreApplication::translate("MainWindow", "kg", nullptr));
        checkDeadlift->setText(QCoreApplication::translate("MainWindow", "\347\241\254\346\213\211\346\265\213\350\257\225", nullptr));
        spinDeadlift->setSuffix(QCoreApplication::translate("MainWindow", "kg", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabOptional), QCoreApplication::translate("MainWindow", "\345\217\257\351\200\211\351\241\271\347\233\256", nullptr));
        buttonCalculate->setText(QCoreApplication::translate("MainWindow", "\350\256\241\347\256\227\350\257\204\345\210\206", nullptr));
        labelResult->setText(QCoreApplication::translate("MainWindow", "\350\277\220\345\212\250GPA\357\274\232", nullptr));
        btnOpenTrainWindow->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\350\256\255\347\273\203\350\256\241\345\210\222\347\256\241\347\220\206", nullptr));
        btnOpenTrainWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "QPushButton {\n"
"    background-color: #4CAF50;\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #45a049;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #3d8b40;\n"
"}", nullptr));
        btnOpenChatWindow->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\350\201\212\345\244\251\345\256\244", nullptr));
        btnOpenChatWindow->setStyleSheet(QCoreApplication::translate("MainWindow", "QPushButton {\n"
"    background-color: #2196F3;\n"
"    color: white;\n"
"    font-size: 14px;\n"
"    font-weight: bold;\n"
"    padding: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #0b7dda;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #0c68b3;\n"
"}", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
