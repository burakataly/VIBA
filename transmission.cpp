#include "transmission.h"
#include "crc.h"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

#define HEADER 0xAA

Transmission::Transmission(QString filePath, QObject *parent)
    : QObject(parent),
    m_filePath(filePath),
    m_checksum(0),
    m_currentFrame(0),
    m_attemptCount(0),
    m_simInterval(800),
    m_paused(false)
{
    m_simulate = true;
}

void Transmission::setSimulateMode(bool simulate, int interval)
{
    m_simulate = simulate;
    m_simInterval = interval;
}

void Transmission::updateSimInterval(int interval)
{
    m_simInterval = interval;
    if (frameTimer && frameTimer->isActive()) {
        frameTimer->setInterval(m_simInterval);
    }
}

bool Transmission::randomChance(int percent)
{
    bool result = QRandomGenerator::global()->bounded(100) < percent;
    qDebug() << "[RandomChance] %" << percent << "→" << result;
    return result;
}

QByteArray Transmission::readFile()
{
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit statusUpdated("Dosya açılamadı.");
        return QByteArray();
    }
    QByteArray data = file.readAll();
    file.close();
    return data;
}

void Transmission::createFrames(const QByteArray &data)
{
    int bitsPerFrame = 100;
    QString bitString;
    for (unsigned char byte : data) {
        bitString.append(QString("%1").arg(byte, 8, 2, QLatin1Char('0')));
    }
    int remainder = bitString.length() % bitsPerFrame;
    if (remainder != 0) {
        bitString.append(QString(bitsPerFrame - remainder, '0'));
    }
    for (int i = 0; i < bitString.length(); i += bitsPerFrame) {
        QString frameBits = bitString.mid(i, bitsPerFrame);
        QByteArray frame = frameBits.toUtf8();
        m_frames.append(frame);
    }
}

void Transmission::startTransmission()
{
    QByteArray data = readFile();
    if (data.isEmpty()) {
        emit statusUpdated("Dosya okunamadı.");
        return;
    }
    createFrames(data);
    if (m_frames.isEmpty())
        return;

    m_crcCodes.clear();
    m_currentFrame = 0;
    m_attemptCount = 0;
    m_paused = false;

    emit statusUpdated("Gönderim başlatılıyor...");
    sendNextFrame();
}

void Transmission::sendNextFrame()
{
    if (m_currentFrame < m_frames.size()) {
        m_attemptCount = 0;
        attemptSendFrame();
    } else {
        sendChecksumFrame();
    }
}

void Transmission::attemptSendFrame()
{
    if (m_paused)
        return;

    m_attemptCount++;
    emit statusUpdated(QString("[Gönderici] Frame %1 gönderiliyor... (Deneme %2)")
                           .arg(m_currentFrame + 1)
                           .arg(m_attemptCount));

    bool transmissionSuccess = false;

    // Simüle: %10 olasılıkla frame kaybolur
    if (randomChance(10)) {
        emit statusUpdated("[Simülasyon] Frame KAYBOLDU!");
        emit frameAnimationStatus(m_currentFrame, "lost");
    } else {
        QByteArray frameToTransmit = m_frames[m_currentFrame];
        bool dataCorrupt = randomChance(20);
        if (dataCorrupt) {
            emit statusUpdated("[Simülasyon] Frame BOZULDU!");
            emit frameAnimationStatus(m_currentFrame, "corrupt");
            if (!frameToTransmit.isEmpty())
                frameToTransmit[0] ^= 0x01;
        } else {
            emit frameAnimationStatus(m_currentFrame, "success");
        }
        quint16 originalCRC = calculateCRC(m_frames[m_currentFrame]);
        quint16 receivedCRC = calculateCRC(frameToTransmit);
        // Eğer veri bozulduysa ve CRC uyuşmuyorsa; ACK olarak "ack_bad" simülasyonu
        if (dataCorrupt && (receivedCRC != originalCRC)) {
            // Bu durumda, ACK animasyonu view tarafından çalışsın, ancak hatalı ACK sinyali (ack_bad)
            // şimdi de gecikmeli şekilde tetiklenecek.
            QTimer::singleShot(m_simInterval, this, [this]() {
                attemptAckForFrame(true);  // true => ack_bad isteniyor
            });
            return;
        } else {
            // Veri frame başarılı; bekleyip ACK aşamasına geçelim.
            QTimer::singleShot(m_simInterval, this, [this]() {
                attemptAckForFrame(false);
            });
            return;
        }
    }
    QTimer::singleShot(m_simInterval, this, [this]() {
        attemptSendFrame();
    });
}

void Transmission::attemptAckForFrame(bool ackBad)
{
    if (randomChance(15)) {
        emit statusUpdated("[Simülasyon] ACK KAYBOLDU!");
        emit frameAnimationStatus(m_currentFrame, "ack_lost");
        QTimer::singleShot(m_simInterval, this, [this]() {
            attemptSendFrame();
        });
    } else {
        if (ackBad) {
            emit statusUpdated("[Alıcı] ACK (Hatalı) gönderiliyor.");
            emit frameAnimationStatus(m_currentFrame, "ack_bad");
        } else {
            emit statusUpdated("[Alıcı] ACK gönderiliyor.");
            emit frameAnimationStatus(m_currentFrame, "ack");
        }
        // Artık view, ack animasyonunu tamamladığında ackAnimationFinished sinyalini gönderecek.
        // Transmission, onAckAnimationFinished() slotunda bu sinyali dinleyecek.
    }
}

void Transmission::onAckAnimationFinished(int frameId, const QString &ackStatus)
{
    if (frameId != m_currentFrame)
        return;

    if (ackStatus == "ack") {
        emit statusUpdated("[Alıcı] ACK alındı. Frame doğru alındı.");
        quint16 crc = calculateCRC(m_frames[m_currentFrame]);
        m_crcCodes.append(crc);
        emit frameProcessed(QString("Frame %1: CRC = 0x%2")
                                .arg(m_currentFrame + 1)
                                .arg(crc, 4, 16, QLatin1Char('0')));
        m_currentFrame++;
        m_attemptCount = 0;
        sendNextFrame();
    } else if (ackStatus == "ack_bad") {
        emit statusUpdated("[Alıcı] ACK alındı ancak frame hatalı alındı. Tekrar gönderiliyor.");
        QTimer::singleShot(m_simInterval, this, [this]() {
            attemptSendFrame();
        });
    }
}

void Transmission::sendChecksumFrame()
{
    m_checksum = computeChecksum(m_crcCodes);
    emit checksumCalculated(QString("0x%1").arg(m_checksum, 4, 16, QLatin1Char('0')));

    QByteArray checksumFrame;
    checksumFrame.append(static_cast<char>(HEADER));
    checksumFrame.append(static_cast<char>((m_checksum >> 8) & 0xFF));
    checksumFrame.append(static_cast<char>(m_checksum & 0xFF));

    bool transmissionSuccess = true;
    if (randomChance(5)) {
        checksumFrame[2] ^= 0xFF;
        emit statusUpdated("[Simülasyon] Checksum frame BOZULDU!");
        transmissionSuccess = false;
    }

    emit statusUpdated("[Gönderici] Checksum frame gönderiliyor...");
    emit frameAnimationStatus(m_currentFrame, transmissionSuccess ? "ack" : "corrupt");

    QTimer::singleShot(m_simInterval, this, [this, transmissionSuccess]() {
        bool ackReceived = transmissionSuccess;
        if (ackReceived && randomChance(15)) {
            emit statusUpdated("[Simülasyon] Checksum ACK KAYBOLDU!");
            emit frameAnimationStatus(m_currentFrame, "ack_lost");
            ackReceived = false;
        }
        if (ackReceived) {
            emit statusUpdated("[Alıcı] Checksum ACK alındı. Checksum frame doğru alındı.");
            emit statusUpdated("Tüm frame'ler ve checksum başarıyla gönderildi.");
        } else {
            emit statusUpdated("[Simülasyon] Checksum frame için ACK alınamadı, tekrar deneniyor...");
            sendChecksumFrame();
        }
    });
}

quint16 Transmission::calculateCRC(const QByteArray &frame)
{
    return CRC::computeCRC(frame);
}

quint16 Transmission::computeChecksum(const QVector<quint16> &crcCodes)
{
    quint16 sum = 0;
    for (quint16 code : crcCodes) {
        code = ~code;
        sum += code;
        sum &= 0xFFFF;
    }
    return (~sum + 1) & 0xFFFF;
}

void Transmission::pause()
{
    m_paused = true;
    emit statusUpdated("[Kontrol] Simülasyon duraklatıldı.");
}

void Transmission::resume()
{
    if (m_paused) {
        m_paused = false;
        emit statusUpdated("[Kontrol] Simülasyon devam ediyor...");
        if (m_currentFrame < m_frames.size()) {
            QTimer::singleShot(m_simInterval, this, [this]() {
                attemptSendFrame();
            });
        }
    }
}
