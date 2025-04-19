
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transmission.h"
#include "transmissiongraphicsview.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget*parent):QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

}
MainWindow::~MainWindow(){ delete tx; delete ui;}

void MainWindow::on_btnSelectFile_clicked(){
    QString f=QFileDialog::getOpenFileName(this,"Dosya Seç","","Tüm Dosyalar (*.*)");
    if(!f.isEmpty()) ui->fileLineEdit->setText(f);
}
void MainWindow::on_btnStart_clicked(){
    if(tx){ delete tx; tx=nullptr;}
    if(!view){ view=new TransmissionGraphicsView(this); ui->graphicsLayout->addWidget(view);} else view->reset();
    QString f=ui->fileLineEdit->text(); if(f.isEmpty()){ QMessageBox::warning(this,"Uyarı","Dosya seçin"); return;}
    tx=new Transmission(f,this);
    connect(tx,&Transmission::guiEvent,view,&TransmissionGraphicsView::enqueueEvent);
    connect(view,&TransmissionGraphicsView::ackAnimationFinished,tx,&Transmission::ackAnimFinished);
    connect(tx,&Transmission::logLine,this,&MainWindow::logLine);
    connect(tx,&Transmission::checksumReady,this,&MainWindow::checksum);

    connect(ui->btnStop, &QPushButton::clicked, this, [=]{
        if (tx) tx->finishImmediately();
        });

    tx->start();
}

void MainWindow::logLine(const QString&l){ ui->txtLog->append(l);}

void MainWindow::checksum(const QString&c){ ui->lblChecksum->setText("Checksum: "+c);}
