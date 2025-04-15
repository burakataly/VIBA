#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , transmission(nullptr)
{
    ui->setupUi(this);
    gfxView = new TransmissionGraphicsView(this);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->graphicsContainer->layout());
    if (layout) {
        layout->addWidget(gfxView);
    }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{
    // .dat uzantılı veri dosyasını seçmek için
    QString fileName = QFileDialog::getOpenFileName(this, "Dosya Seç", "", "Data Files (*.dat)");
    if(!fileName.isEmpty()) {
        ui->fileLineEdit->setText(fileName);
    }
}

void MainWindow::on_startButton_clicked()
{
    gfxView = new TransmissionGraphicsView(this);
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->graphicsLayout);
    if (layout) layout->addWidget(gfxView);
    gfxView->resetScene();

    QString fileName = ui->fileLineEdit->text();
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen bir dosya seçiniz.");
        return;
    }

    if (transmission)
        delete transmission;

    transmission = new Transmission(fileName, this);  // ✅ transmission ARTIK VAR

    int interval = ui->sliderSpeed->value();

    // Sinyal bağlantıları
    connect(transmission, &Transmission::statusUpdated, this, &MainWindow::updateStatus);
    if (!transmission) return;

    connect(transmission, &Transmission::frameProcessed, this, &MainWindow::updateFrameList);
    if (!transmission) return;

    connect(transmission, &Transmission::checksumCalculated, this, &MainWindow::updateChecksum);
    if (!transmission) return;

    connect(ui->sliderSpeed, &QSlider::valueChanged, this, [=](int val) {
        ui->lblSpeedValue->setText(QString("%1 ms").arg(val));
        if (transmission) {
            transmission->updateSimInterval(val);
        }
    });

    if (!transmission) return;


    transmission->startTransmission();
}


void MainWindow::updateStatus(QString status)
{
    ui->statusTextEdit->append(status);
    if (status.contains("Frame")) {
        int idx = status.section(' ', 1, 1).toInt() - 1;
        if (status.contains("kayboldu"))
            gfxView->setFrameStatus(idx, "lost");
        else if (status.contains("bozuldu"))
            gfxView->setFrameStatus(idx, "corrupted");
        else if (status.contains("ACK alınamadı"))
            gfxView->setFrameStatus(idx, "ack_failed");
        else if (status.contains("başarıyla"))
            gfxView->setFrameStatus(idx, "sent");
        else if (status.contains("ACK geldi"))
            gfxView->setFrameStatus(idx, "ack_ok");
    }
}


void MainWindow::updateFrameList(QString frameInfo)
{
    ui->frameListWidget->addItem(frameInfo);
    QString crc = frameInfo.section("=", -1).trimmed();
    gfxView->addFrame(ui->frameListWidget->count() - 1, crc);
}


void MainWindow::updateChecksum(QString checksum)
{
    ui->checksumLabel->setText("Checksum: " + checksum);
}

void MainWindow::on_btnPause_clicked() {
    if (transmission) transmission->pause();
}

void MainWindow::on_btnResume_clicked() {
    if (transmission) transmission->resume();
}
