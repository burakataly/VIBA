/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *fileLayout;
    QPushButton *btnSelectFile;
    QLineEdit *fileLineEdit;
    QHBoxLayout *btnLayout;
    QPushButton *btnStart;
    QPushButton *btnStop;
    QHBoxLayout *graphicsLayout;
    QHBoxLayout *outputLayout;
    QLabel *lblChecksum;
    QTextEdit *txtLog;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        fileLayout = new QHBoxLayout();
        fileLayout->setObjectName("fileLayout");
        btnSelectFile = new QPushButton(centralwidget);
        btnSelectFile->setObjectName("btnSelectFile");

        fileLayout->addWidget(btnSelectFile);

        fileLineEdit = new QLineEdit(centralwidget);
        fileLineEdit->setObjectName("fileLineEdit");

        fileLayout->addWidget(fileLineEdit);


        verticalLayout->addLayout(fileLayout);

        btnLayout = new QHBoxLayout();
        btnLayout->setObjectName("btnLayout");
        btnStart = new QPushButton(centralwidget);
        btnStart->setObjectName("btnStart");

        btnLayout->addWidget(btnStart);

        btnStop = new QPushButton(centralwidget);
        btnStop->setObjectName("btnStop");

        btnLayout->addWidget(btnStop);


        verticalLayout->addLayout(btnLayout);

        graphicsLayout = new QHBoxLayout();
        graphicsLayout->setObjectName("graphicsLayout");

        verticalLayout->addLayout(graphicsLayout);

        outputLayout = new QHBoxLayout();
        outputLayout->setObjectName("outputLayout");
        lblChecksum = new QLabel(centralwidget);
        lblChecksum->setObjectName("lblChecksum");

        outputLayout->addWidget(lblChecksum);


        verticalLayout->addLayout(outputLayout);

        txtLog = new QTextEdit(centralwidget);
        txtLog->setObjectName("txtLog");
        txtLog->setReadOnly(true);

        verticalLayout->addWidget(txtLog);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "VIBA Sim", nullptr));
        btnSelectFile->setText(QCoreApplication::translate("MainWindow", "Dosya", nullptr));
        btnStart->setText(QCoreApplication::translate("MainWindow", "Ba\305\237lat", nullptr));
        btnStop->setText(QCoreApplication::translate("MainWindow", "Bitir", nullptr));
        lblChecksum->setText(QCoreApplication::translate("MainWindow", "Checksum: -", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
