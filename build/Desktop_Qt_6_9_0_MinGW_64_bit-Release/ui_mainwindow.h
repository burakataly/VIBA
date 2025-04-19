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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBoxFrames;
    QVBoxLayout *verticalLayoutFrames;
    QTableWidget *tableWidgetFrames;
    QGroupBox *groupBoxSimulation;
    QVBoxLayout *verticalLayoutSimulation;
    QTextEdit *textEditLog;
    QPushButton *btnStartTransmission;
    QPushButton *btnOpenFile;
    QMenuBar *menubar;
    QMenu *menuFile;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(586, 358);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        groupBoxFrames = new QGroupBox(centralwidget);
        groupBoxFrames->setObjectName("groupBoxFrames");
        verticalLayoutFrames = new QVBoxLayout(groupBoxFrames);
        verticalLayoutFrames->setObjectName("verticalLayoutFrames");
        tableWidgetFrames = new QTableWidget(groupBoxFrames);
        if (tableWidgetFrames->columnCount() < 3)
            tableWidgetFrames->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidgetFrames->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidgetFrames->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidgetFrames->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidgetFrames->setObjectName("tableWidgetFrames");

        verticalLayoutFrames->addWidget(tableWidgetFrames);


        horizontalLayout->addWidget(groupBoxFrames);

        groupBoxSimulation = new QGroupBox(centralwidget);
        groupBoxSimulation->setObjectName("groupBoxSimulation");
        verticalLayoutSimulation = new QVBoxLayout(groupBoxSimulation);
        verticalLayoutSimulation->setObjectName("verticalLayoutSimulation");
        textEditLog = new QTextEdit(groupBoxSimulation);
        textEditLog->setObjectName("textEditLog");
        textEditLog->setReadOnly(true);

        verticalLayoutSimulation->addWidget(textEditLog);

        btnStartTransmission = new QPushButton(groupBoxSimulation);
        btnStartTransmission->setObjectName("btnStartTransmission");

        verticalLayoutSimulation->addWidget(btnStartTransmission);


        horizontalLayout->addWidget(groupBoxSimulation);


        verticalLayout->addLayout(horizontalLayout);

        btnOpenFile = new QPushButton(centralwidget);
        btnOpenFile->setObjectName("btnOpenFile");

        verticalLayout->addWidget(btnOpenFile);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 586, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionExit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Data Link Layer Simulation", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        groupBoxFrames->setTitle(QCoreApplication::translate("MainWindow", "Frame List", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidgetFrames->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Frame No", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidgetFrames->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "CRC Code", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidgetFrames->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        groupBoxSimulation->setTitle(QCoreApplication::translate("MainWindow", "Transmission Simulation", nullptr));
        btnStartTransmission->setText(QCoreApplication::translate("MainWindow", "Start Transmission", nullptr));
        btnOpenFile->setText(QCoreApplication::translate("MainWindow", "Open .dat File", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
