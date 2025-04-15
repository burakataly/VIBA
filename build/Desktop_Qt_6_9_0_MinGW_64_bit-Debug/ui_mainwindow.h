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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
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
    QPushButton *btnStart;
    QHBoxLayout *controlLayout;
    QPushButton *btnPause;
    QPushButton *btnResume;
    QSlider *sliderSpeed;
    QLabel *lblSpeedValue;
    QHBoxLayout *graphicsLayout;
    QHBoxLayout *outputLayout;
    QListWidget *listCRC;
    QLabel *lblChecksum;
    QTextEdit *txtLog;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(900, 600);
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

        btnStart = new QPushButton(centralwidget);
        btnStart->setObjectName("btnStart");

        verticalLayout->addWidget(btnStart);

        controlLayout = new QHBoxLayout();
        controlLayout->setObjectName("controlLayout");
        btnPause = new QPushButton(centralwidget);
        btnPause->setObjectName("btnPause");

        controlLayout->addWidget(btnPause);

        btnResume = new QPushButton(centralwidget);
        btnResume->setObjectName("btnResume");

        controlLayout->addWidget(btnResume);

        sliderSpeed = new QSlider(centralwidget);
        sliderSpeed->setObjectName("sliderSpeed");
        sliderSpeed->setMinimum(100);
        sliderSpeed->setMaximum(2000);
        sliderSpeed->setValue(800);
        sliderSpeed->setOrientation(Qt::Horizontal);

        controlLayout->addWidget(sliderSpeed);

        lblSpeedValue = new QLabel(centralwidget);
        lblSpeedValue->setObjectName("lblSpeedValue");

        controlLayout->addWidget(lblSpeedValue);


        verticalLayout->addLayout(controlLayout);

        graphicsLayout = new QHBoxLayout();
        graphicsLayout->setObjectName("graphicsLayout");

        verticalLayout->addLayout(graphicsLayout);

        outputLayout = new QHBoxLayout();
        outputLayout->setObjectName("outputLayout");
        listCRC = new QListWidget(centralwidget);
        listCRC->setObjectName("listCRC");

        outputLayout->addWidget(listCRC);

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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "VIBA Sim\303\274lasyon", nullptr));
        btnSelectFile->setText(QCoreApplication::translate("MainWindow", "Dosya Se\303\247", nullptr));
        btnStart->setText(QCoreApplication::translate("MainWindow", "Ba\305\237lat", nullptr));
        btnPause->setText(QCoreApplication::translate("MainWindow", "Duraklat", nullptr));
        btnResume->setText(QCoreApplication::translate("MainWindow", "Devam Et", nullptr));
        lblSpeedValue->setText(QCoreApplication::translate("MainWindow", "800 ms", nullptr));
        lblChecksum->setText(QCoreApplication::translate("MainWindow", "Checksum: -", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
