#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "transmission.h"
#include "transmissiongraphicsview.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();
    void on_startButton_clicked();
    void updateStatus(QString status);
    void updateFrameList(QString frameInfo);
    void updateChecksum(QString checksum);
    void on_btnPause_clicked();
    void on_btnResume_clicked();

private:
    Ui::MainWindow *ui;
    Transmission *transmission;
    TransmissionGraphicsView *gfxView;
};

#endif // MAINWINDOW_H
