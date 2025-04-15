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
    QVBoxLayout *mainLayout;
    QHBoxLayout *fileLayout;
    QLineEdit *fileLineEdit;
    QPushButton *browseButton;
    QPushButton *startButton;
    QHBoxLayout *simSettingsLayout;
    QLabel *lblSimDesc;
    QSlider *sliderSpeed;
    QLabel *lblSpeedValue;
    QHBoxLayout *controlButtonsLayout;
    QPushButton *btnPause;
    QPushButton *btnResume;
    QHBoxLayout *mainDisplayLayout;
    QWidget *graphicsContainer;
    QVBoxLayout *graphicsLayout;
    QVBoxLayout *rightSideLayout;
    QListWidget *frameListWidget;
    QTextEdit *statusTextEdit;
    QLabel *checksumLabel;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1000, 700);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        mainLayout = new QVBoxLayout(centralwidget);
        mainLayout->setObjectName("mainLayout");
        fileLayout = new QHBoxLayout();
        fileLayout->setObjectName("fileLayout");
        fileLineEdit = new QLineEdit(centralwidget);
        fileLineEdit->setObjectName("fileLineEdit");

        fileLayout->addWidget(fileLineEdit);

        browseButton = new QPushButton(centralwidget);
        browseButton->setObjectName("browseButton");

        fileLayout->addWidget(browseButton);

        startButton = new QPushButton(centralwidget);
        startButton->setObjectName("startButton");

        fileLayout->addWidget(startButton);


        mainLayout->addLayout(fileLayout);

        simSettingsLayout = new QHBoxLayout();
        simSettingsLayout->setObjectName("simSettingsLayout");
        lblSimDesc = new QLabel(centralwidget);
        lblSimDesc->setObjectName("lblSimDesc");

        simSettingsLayout->addWidget(lblSimDesc);

        sliderSpeed = new QSlider(centralwidget);
        sliderSpeed->setObjectName("sliderSpeed");
        sliderSpeed->setMinimum(1);
        sliderSpeed->setMaximum(2000);
        sliderSpeed->setValue(800);
        sliderSpeed->setOrientation(Qt::Horizontal);
        sliderSpeed->setTickInterval(100);
        sliderSpeed->setTickPosition(QSlider::TicksBelow);

        simSettingsLayout->addWidget(sliderSpeed);

        lblSpeedValue = new QLabel(centralwidget);
        lblSpeedValue->setObjectName("lblSpeedValue");

        simSettingsLayout->addWidget(lblSpeedValue);


        mainLayout->addLayout(simSettingsLayout);

        controlButtonsLayout = new QHBoxLayout();
        controlButtonsLayout->setObjectName("controlButtonsLayout");
        btnPause = new QPushButton(centralwidget);
        btnPause->setObjectName("btnPause");

        controlButtonsLayout->addWidget(btnPause);

        btnResume = new QPushButton(centralwidget);
        btnResume->setObjectName("btnResume");

        controlButtonsLayout->addWidget(btnResume);


        mainLayout->addLayout(controlButtonsLayout);

        mainDisplayLayout = new QHBoxLayout();
        mainDisplayLayout->setObjectName("mainDisplayLayout");
        graphicsContainer = new QWidget(centralwidget);
        graphicsContainer->setObjectName("graphicsContainer");
        graphicsContainer->setMinimumSize(QSize(500, 400));
        graphicsLayout = new QVBoxLayout(graphicsContainer);
        graphicsLayout->setObjectName("graphicsLayout");
        graphicsLayout->setContentsMargins(0, 0, 0, 0);

        mainDisplayLayout->addWidget(graphicsContainer);

        rightSideLayout = new QVBoxLayout();
        rightSideLayout->setObjectName("rightSideLayout");
        frameListWidget = new QListWidget(centralwidget);
        frameListWidget->setObjectName("frameListWidget");

        rightSideLayout->addWidget(frameListWidget);

        statusTextEdit = new QTextEdit(centralwidget);
        statusTextEdit->setObjectName("statusTextEdit");
        statusTextEdit->setReadOnly(true);

        rightSideLayout->addWidget(statusTextEdit);


        mainDisplayLayout->addLayout(rightSideLayout);


        mainLayout->addLayout(mainDisplayLayout);

        checksumLabel = new QLabel(centralwidget);
        checksumLabel->setObjectName("checksumLabel");
        checksumLabel->setAlignment(Qt::AlignCenter);
        checksumLabel->setMinimumHeight(25);

        mainLayout->addWidget(checksumLabel);

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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "VIBA \303\207er\303\247eve Sim\303\274lat\303\266r\303\274", nullptr));
        browseButton->setText(QCoreApplication::translate("MainWindow", "Browse", nullptr));
        startButton->setText(QCoreApplication::translate("MainWindow", "Start Transmission", nullptr));
        lblSimDesc->setText(QCoreApplication::translate("MainWindow", "\342\232\231 Sim\303\274lasyon H\304\261z\304\261 (ms)", nullptr));
        lblSpeedValue->setText(QCoreApplication::translate("MainWindow", "800 ms", nullptr));
        btnPause->setText(QCoreApplication::translate("MainWindow", "\342\217\270 Pause", nullptr));
        btnResume->setText(QCoreApplication::translate("MainWindow", "\342\226\266 Resume", nullptr));
        checksumLabel->setText(QCoreApplication::translate("MainWindow", "Checksum: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
