
#ifndef TRANSMISSION_H
#define TRANSMISSION_H
#include <QObject>
#include <QTimer>
#include <QVector>
#include "frame.h"

static constexpr quint8 SEQ_MASK  = 0x0F;  // 0000 1111
static constexpr quint8 TYPE_MASK = 0x10;  // 0001 0000 checksum icin

struct GuiEvent{ enum Kind{FrameTx,AckTx}; Kind kind; int id; QString status; };

class Transmission: public QObject{
    Q_OBJECT
public:
    explicit Transmission(const QString &file, QObject*parent=nullptr);
    void updateSimInterval(int ms);
    void start();
    void stop();               // simülasyonu anında bitir
signals:
    void guiEvent(const GuiEvent&);
    void logLine(const QString&);
    void checksumReady(const QString&);
    void ackRequest(int id, bool bad);
public slots:
    void ackAnimFinished(int id,const QString &stat);
public slots:
    void finishImmediately();          // <─ YENİ
private slots:
    void timeout();
private:
    QByteArray readFile();
    void splitFrames(const QByteArray &d);
    void sendNext();
    void sendChecksumFrame(quint8 cs);
    void sendFrame();
    void sendAck(bool bad);
    quint8  computeChecksum() const;      // 8‑bit
    bool finishing = false;
    QString frameInfoLine(int idx, const Frame &fr) const;   // açıklayıcı satır

    QString path;
    QVector<Frame> frames;
    QVector<quint16> crcs;
    int idx=0;
    bool waiting=false;
    bool paused=false;
    QTimer timer;
    int interval=1600;
    bool randomChance(int p);
};
#endif
