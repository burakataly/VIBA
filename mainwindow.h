
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
class Transmission;
class TransmissionGraphicsView;
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_btnSelectFile_clicked();
    void on_btnStart_clicked();

    void logLine(const QString &);

    void checksum(const QString &);

private:
    Ui::MainWindow *ui;
    Transmission *tx = nullptr;
    TransmissionGraphicsView *view = nullptr;
};
#endif
