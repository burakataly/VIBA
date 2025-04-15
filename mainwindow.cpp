#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transmission.h"
#include "TransmissionGraphicsView.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , transmission(nullptr)
    , gfxView(nullptr)
{
    ui->setupUi(this);

    // Başlangıç hızı gösterimi
    ui->lblSpeedValue->setText(QString("%1 ms").arg(ui->sliderSpeed->value()));
    connect(ui->sliderSpeed, &QSlider::valueChanged, this, [=](int val) {
        ui->lblSpeedValue->setText(QString("%1 ms").arg(val));
        if (transmission)
            transmission->updateSimInterval(val);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete transmission;
}

void MainWindow::on_btnSelectFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Veri Dosyası Seç", "", "Tüm Dosyalar (*.*)");
    if (!fileName.isEmpty())
        ui->fileLineEdit->setText(fileName);
}

void MainWindow::on_btnStart_clicked()
{
    // Önce önceki Transmission nesnesini temizle
    if (transmission) {
        delete transmission;
        transmission = nullptr;
    }

    // Grafik sahnesi hazırlanıyor
    if (!gfxView) {
        gfxView = new TransmissionGraphicsView(this);
        if (ui->graphicsLayout) {
            ui->graphicsLayout->addWidget(gfxView);
        }
    } else {
        gfxView->resetScene();
    }

    // Dosya kontrolü
    QString fileName = ui->fileLineEdit->text();
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen bir dosya seçiniz.");
        return;
    }

    // Yeni Transmission nesnesi oluşturuluyor
    transmission = new Transmission(fileName, this);
    transmission->setSimulateMode(true, ui->sliderSpeed->value());

    // SIGNAL-SLOT BAĞLANTILARI
    connect(transmission, &Transmission::statusUpdated,
            this, &MainWindow::updateStatus);
    connect(transmission, &Transmission::frameProcessed,
            this, &MainWindow::updateFrameList);
    connect(transmission, &Transmission::checksumCalculated,
            this, &MainWindow::updateChecksum);
    connect(transmission, &Transmission::frameAnimationStatus,
            gfxView, &TransmissionGraphicsView::animateFrame);
    // ACK animasyonu bittikten sonra Transmission, onAckAnimationFinished() slotunu tetikler.
    connect(gfxView, &TransmissionGraphicsView::ackAnimationFinished,
            transmission, &Transmission::onAckAnimationFinished);

    transmission->startTransmission();
}

void MainWindow::updateStatus(const QString &msg)
{
    ui->txtLog->append(msg);
}

void MainWindow::updateFrameList(const QString &info)
{
    ui->listCRC->addItem(info);
}

void MainWindow::updateChecksum(const QString &checksum)
{
    ui->lblChecksum->setText(checksum);
}

void MainWindow::on_btnPause_clicked()
{
    if (transmission)
        transmission->pause();
}

void MainWindow::on_btnResume_clicked()
{
    if (transmission)
        transmission->resume();
}
