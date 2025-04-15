#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <QObject>
#include <QVector>
#include <QByteArray>
#include <QTimer>

class Transmission : public QObject
{
    Q_OBJECT

public:
    explicit Transmission(QString filePath, QObject *parent = nullptr);

    void setSimulateMode(bool simulate, int interval);
    void updateSimInterval(int interval);
    void startTransmission();
    void pause();
    void resume();

signals:
    void statusUpdated(const QString &status);
    void frameProcessed(const QString &info);
    void checksumCalculated(const QString &checksum);
    // Data ve checksum frame animasyonları için view'e gönderilecek sinyal.
    // Status değerleri: "success", "corrupt", "lost", "ack", "ack_lost", "ack_bad"
    void frameAnimationStatus(int frameId, const QString &status);

public slots:
    // ACK animasyonu tamamlandığında view, ackAnimationFinished sinyali ile gönderir.
    // ackStatus "ack" ise, ACK başarılı; "ack_bad" ise, frame hatalı alınmış.
    void onAckAnimationFinished(int frameId, const QString &ackStatus);

private:
    QString m_filePath;
    QVector<QByteArray> m_frames;
    QVector<quint16> m_crcCodes;
    quint16 m_checksum;

    int m_currentFrame;
    int m_attemptCount;
    int m_simInterval;
    bool m_simulate = true;  // hep simülasyon modunda
    bool m_paused;

    QTimer *frameTimer = nullptr;

    QByteArray readFile();
    void createFrames(const QByteArray &data);
    quint16 calculateCRC(const QByteArray &frame);
    quint16 computeChecksum(const QVector<quint16> &crcCodes);

    void sendNextFrame();
    void attemptSendFrame();
    void attemptAckForFrame(bool ackBad = false); // ackBad true: hatalı ACK
    void sendChecksumFrame();

    bool randomChance(int percent);
};

#endif // TRANSMISSION_H
