#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Transmission;
class TransmissionGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSelectFile_clicked();
    void on_btnStart_clicked();
    void on_btnPause_clicked();
    void on_btnResume_clicked();

    void updateStatus(const QString &msg);
    void updateFrameList(const QString &info);
    void updateChecksum(const QString &checksum);

private:
    Ui::MainWindow *ui;
    Transmission *transmission;
    TransmissionGraphicsView *gfxView;
};

#endif // MAINWINDOW_H
