#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <QObject>
#include <QVector>
#include <QTimer>

// HEADER sabiti tanımlanıyor (checksum frame tanımlayıcı)
#define HEADER 0xAA

class Transmission : public QObject
{
    Q_OBJECT

public:
    explicit Transmission(QString filePath, QObject *parent = nullptr);

    // GUI'den gelen ayarlar
    void setSimulateMode(bool simulate, int interval);
    void updateSimInterval(int interval);

    // Aktarım işlemini başlatan fonksiyon
    void startTransmission();
    void pause();        // Simülasyonu durdur
    void resume();       // Devam ettir

signals:
    // GUI güncellemeleri için sinyaller
    void statusUpdated(QString message);             // Log mesajı
    void frameProcessed(QString frameInfo);          // CRC gösterimi
    void checksumCalculated(QString hexChecksum);    // GUI'de gösterilecek

private:
    QString m_filePath;                  // Dosya yolu
    QVector<QByteArray> m_frames;        // Bit dizilerinden oluşan frame'ler
    QVector<quint16> m_crcCodes;         // Her frame için CRC kodları
    quint16 m_checksum;                  // Checksum sonucu

    int m_currentFrame;                  // Sıradaki frame indeksi
    int m_attemptCount;                  // Her frame için deneme sayısı
    bool m_simulate = true;                     // Simülasyon aktif mi?
    int m_simInterval;                   // Simülasyon aralığı (ms)
    QTimer *frameTimer = nullptr;        // Simülasyon zamanlayıcısı
    bool m_paused = false;


    // Yardımcı metodlar
    bool randomChance(int percent);                          // Yüzdelik ihtimalli olay
    QByteArray readFile();                                   // Dosya okuma
    void createFrames(const QByteArray &data);               // Veriyi frame'lere bölme
    void sendNextFrame();                                    // Bir sonraki frame'e geç
    void attemptSendFrame();                                 // Frame gönderimini dene
    void sendChecksumFrame();                                // Checksum frame gönder
    quint16 calculateCRC(const QByteArray &frame);           // CRC hesapla
    quint16 computeChecksum(const QVector<quint16> &codes);  // Checksum hesapla
};

#endif // TRANSMISSION_H
