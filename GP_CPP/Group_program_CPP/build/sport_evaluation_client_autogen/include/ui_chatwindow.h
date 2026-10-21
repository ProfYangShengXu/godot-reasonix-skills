/********************************************************************************
** Form generated from reading UI file 'chatwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWINDOW_H
#define UI_CHATWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineUsername;
    QLabel *label_2;
    QLineEdit *lineHost;
    QLabel *label_3;
    QSpinBox *spinPort;
    QPushButton *btnConnect;
    QTextEdit *textChat;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *lineMessage;
    QPushButton *btnSend;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ChatWindow)
    {
        if (ChatWindow->objectName().isEmpty())
            ChatWindow->setObjectName("ChatWindow");
        ChatWindow->resize(600, 500);
        centralwidget = new QWidget(ChatWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName("horizontalLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        lineUsername = new QLineEdit(groupBox);
        lineUsername->setObjectName("lineUsername");

        horizontalLayout->addWidget(lineUsername);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        lineHost = new QLineEdit(groupBox);
        lineHost->setObjectName("lineHost");

        horizontalLayout->addWidget(lineHost);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);

        spinPort = new QSpinBox(groupBox);
        spinPort->setObjectName("spinPort");
        spinPort->setMinimum(1024);
        spinPort->setMaximum(65535);
        spinPort->setValue(12346);

        horizontalLayout->addWidget(spinPort);

        btnConnect = new QPushButton(groupBox);
        btnConnect->setObjectName("btnConnect");

        horizontalLayout->addWidget(btnConnect);


        verticalLayout->addWidget(groupBox);

        textChat = new QTextEdit(centralwidget);
        textChat->setObjectName("textChat");
        textChat->setReadOnly(true);

        verticalLayout->addWidget(textChat);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        lineMessage = new QLineEdit(centralwidget);
        lineMessage->setObjectName("lineMessage");

        horizontalLayout_2->addWidget(lineMessage);

        btnSend = new QPushButton(centralwidget);
        btnSend->setObjectName("btnSend");

        horizontalLayout_2->addWidget(btnSend);


        verticalLayout->addLayout(horizontalLayout_2);

        ChatWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ChatWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 600, 21));
        ChatWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(ChatWindow);
        statusbar->setObjectName("statusbar");
        ChatWindow->setStatusBar(statusbar);

        retranslateUi(ChatWindow);

        QMetaObject::connectSlotsByName(ChatWindow);
    } // setupUi

    void retranslateUi(QMainWindow *ChatWindow)
    {
        ChatWindow->setWindowTitle(QCoreApplication::translate("ChatWindow", "\350\201\212\345\244\251\345\256\244", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ChatWindow", "\350\277\236\346\216\245\350\256\276\347\275\256", nullptr));
        label->setText(QCoreApplication::translate("ChatWindow", "\347\224\250\346\210\267\345\220\215:", nullptr));
        lineUsername->setPlaceholderText(QCoreApplication::translate("ChatWindow", "\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        label_2->setText(QCoreApplication::translate("ChatWindow", "\346\234\215\345\212\241\345\231\250:", nullptr));
        lineHost->setText(QCoreApplication::translate("ChatWindow", "127.0.0.1", nullptr));
        label_3->setText(QCoreApplication::translate("ChatWindow", "\347\253\257\345\217\243:", nullptr));
        btnConnect->setText(QCoreApplication::translate("ChatWindow", "\350\277\236\346\216\245", nullptr));
        lineMessage->setPlaceholderText(QCoreApplication::translate("ChatWindow", "\350\276\223\345\205\245\346\266\210\346\201\257...", nullptr));
        btnSend->setText(QCoreApplication::translate("ChatWindow", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatWindow: public Ui_ChatWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWINDOW_H
