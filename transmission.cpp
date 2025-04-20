
#include "transmission.h"
#include <QFile>
#include <QRandomGenerator>
#include "crc.h"
bool Transmission::randomChance(int p){return QRandomGenerator::global()->bounded(100)<p;}

Transmission::Transmission(const QString&f,QObject*parent):QObject(parent),finishing(false),path(f){
    timer.setSingleShot(true); timer.setInterval(3*interval);
    connect(&timer,&QTimer::timeout,this,&Transmission::timeout);
}
void Transmission::updateSimInterval(int ms){ interval=ms; timer.setInterval(3*ms);}
void Transmission::start(){ idx=0; waiting=false; paused=false; crcs.clear(); frames.clear(); splitFrames(readFile()); sendNext();}
void Transmission::stop(){ timer.stop(); waiting=false; emit logLine("[Kontrol] Simülasyon sonlandırıldı."); }

QByteArray Transmission::readFile(){ QFile f(path); if(!f.open(QIODevice::ReadOnly)){emit logLine("Dosya okunamadı");return{};} auto d=f.readAll();f.close();return d;}

static QByteArray bits(const QByteArray &data){ QByteArray b; b.reserve(data.size()*8); for(auto ch:data) b.append(QString("%1").arg(uchar(ch),8,2,QLatin1Char('0')).toUtf8()); return b;}

void Transmission::splitFrames(const QByteArray &d){
    QByteArray bit=bits(d); const int per=100; int pad=per-(bit.size()%per); if(pad!=per) bit.append(QByteArray(pad,'0'));
    quint8 seq=0;
    for(int i=0;i<bit.size();i+=per){
        QByteArray seg=bit.mid(i,per); QByteArray payload(13,0);
        for(int j=0;j<per;++j) if(seg[j]=='1') payload[j/8]|=(1<<(7-j%8));
        Frame fr;
        fr.ctrl=seq;
        fr.data=payload; QByteArray hdr; hdr.append(char(Frame::FLAG)).append(char(fr.ctrl)).append(fr.data); fr.crc=CRC::calc(hdr);
        frames.append(fr); seq=(seq+1)&0x0F;
    }
}

void Transmission::sendChecksumFrame(quint8 cs)
{
    // — frame objesini hazırla —
    Frame ck;
    ck.ctrl = 0x10;            // üst 4 bit = seq, bit4=1 (checksum)
    ck.data.fill(0, 13);
    ck.data[0] = char(cs);

    // %5 yanlış hesap simülasyonu —
    bool wrongCalc = randomChance(5);
    if (wrongCalc) {
        ck.data[0] ^= 0x01;
        emit logLine("Checksum [YANLIŞ] hesaplandı");
    }
    // CRC tekrar hesapla (data[0] değişmiş olabilir) —
    QByteArray hdr0;
    hdr0.append(char(Frame::FLAG))
        .append(char(ck.ctrl))
        .append(ck.data);
    ck.crc = CRC::calc(hdr0);

    QByteArray raw = ck.toByteArray();

    // 2) kanal simülasyonu: %10 kayıp, %20 bozulma
    bool lost    = randomChance(10);
    bool corrupt = randomChance(20);

    // 3) log & GUI transmit
    emit logLine(QString("▶ Checksum frame gönderiliyor DATA=0x%2  CRC=0x%3  %4")

                     .arg(quint8(ck.data[0]),2,16,QLatin1Char('0'))
                     .arg(ck.crc,4,16,QLatin1Char('0'))
                     .arg(wrongCalc ? "(Gonderilen CheckSum [Yanlış])" : ""));
    if (lost) {
        emit guiEvent({GuiEvent::FrameTx, idx, "lost"});
        emit logLine("   ↳ Checksum frame kayboldu");
        waiting = true;  timer.start();
        return;
    }
    if (corrupt) {
        raw[2] ^= 0xFF;
        emit guiEvent({GuiEvent::FrameTx, idx, "corrupt"});
        emit logLine("   ↳ Checksum frame [BOZULDU]");
    } else {
        emit guiEvent({GuiEvent::FrameTx, idx, "success"});
        emit logLine("   ↳ Checksum frame [iletildi]");
    }

    // 4) ACK bekleme
    waiting = true;
    timer.start();

    // 5) alıcı simülasyonu ve ACK gönderme
    QTimer::singleShot(interval, this, [=]() {
        if (!waiting) return;

        // raw’dan Frame’a dönüştür
        Frame recv = Frame::fromByteArray(raw);
        quint8 ctrl = recv.ctrl;
        bool crcOk = false;
        {
            QByteArray hdr1;
            hdr1.append(char(Frame::FLAG))
                .append(char(ctrl))
                .append(recv.data);
            crcOk = (CRC::calc(hdr1) == recv.crc);
        }

        quint8 checksumData = static_cast<quint8>(recv.data.at(0));
        quint8 receiverCalculated = computeChecksum();
        bool checksumOk = (receiverCalculated == checksumData);


        // %15 ACK kayıp simülasyonu
        if (randomChance(15)) {
            emit guiEvent({GuiEvent::AckTx, idx, "ack_lost"});
            emit logLine("   ↳ ACK (checksum) kayboldu, timeout bekleniyor");
            return;
        }
        // normal ACK / NAK

        QString status = (crcOk && checksumOk) ? "ack" : "ack_bad";

        emit guiEvent({GuiEvent::AckTx, idx, status});
        emit logLine(
            QString("   ↳ ACK (checksum) alındı: %1  (CS_OK=%2)")
                .arg(status)
                .arg(checksumOk));
    });

}


void Transmission::sendNext(){
    if(idx<frames.size()) sendFrame();
    else {
        quint8 cs = computeChecksum();
        sendChecksumFrame(cs);
    }
}
void Transmission::sendFrame(){
    Frame fr=frames[idx];
    QByteArray raw=fr.toByteArray();
    bool lost=randomChance(10);
    bool corrupt=randomChance(20);
    emit logLine(frameInfoLine(idx, fr));
    if(lost){
        emit guiEvent({GuiEvent::FrameTx,idx,"lost"});
        emit logLine("        ↳ Frame iletilemedi (kayboldu)");
        waiting=true;
        timer.start();
        return;
    }
    if(corrupt){
        raw[2]^=0xFF;
        emit guiEvent({GuiEvent::FrameTx,idx,"corrupt"});
        emit logLine("        ↳ Veri [BOZULDU]");
    }
    else{
        emit guiEvent({GuiEvent::FrameTx,idx,"success"});
        emit logLine(QString("Frame %1 gönderildi\n"
            "  ▶ Data  : %2\n").arg(idx+1).arg(QString(fr.data.toHex(' '))));
    }
    waiting=true;
    timer.start();
    QTimer::singleShot(interval, this, [=](){
        if (finishing) return;
        // raw’dan Frame’a dönüştür
        Frame recv = Frame::fromByteArray(raw);
        quint8 ctrl = recv.ctrl;
        bool isChecksum = (ctrl & TYPE_MASK) != 0;
        quint8 seq     = ctrl & SEQ_MASK;

        // header’ı yeniden oluştur
        QByteArray hdr;
        hdr.append(char(Frame::FLAG))
            .append(char(ctrl))
            .append(recv.data);

        bool crcOk = (CRC::calc(hdr) == recv.crc);

        if (isChecksum) {
            // --- Checksum frame işle ---
            if (crcOk) {
                quint8 receivedCs = quint8(recv.data[0]);
                emit logLine(QString("▶ Gelen checksum frame. SEQ=%1  CS=0x%2  CRC_OK")
                                 .arg(seq)
                                 .arg(receivedCs, 2, 16, QLatin1Char('0')));
                emit checksumReady(
                    QString("0x%1").arg(receivedCs, 2, 16, QLatin1Char('0')));
            } else {
                emit logLine("▶ Gelen checksum frame, CRC HATASI");
            }
            waiting = false;
        } else {
            // --- Data frame işle ---
            if (crcOk) {
                emit logLine(
                    QString("▶ Frame %1 başarıyla alındı (CRC OK)").arg(idx+1));
            } else {
                emit logLine(
                    QString("▶ Frame %1 hatalı (CRC HATA)").arg(idx+1));
            }
            // ACK/NAK gönder
            sendAck(!crcOk);
        }
    });
}

void Transmission::sendAck(bool bad){
    if(!waiting) return;
    if(randomChance(15)){ emit guiEvent({GuiEvent::AckTx,idx,"ack_lost"});
    emit logLine("ACK kayıp - timeout bekleniyor"); return;}
    emit guiEvent({GuiEvent::AckTx,idx,bad?"ack_bad":"ack"});
}

void Transmission::timeout()
{
    if (!waiting) return;
    emit logLine("Timeout [RESEND]");
    waiting = false;
    if (idx < frames.size()) {
        // hâlâ data frameler var → data’yı yeniden yolla
        sendFrame();
    } else {
        // checksum aşamasındayız → checksum’u yeniden yolla
        quint8 cs = computeChecksum();
        sendChecksumFrame(cs);
    }
}

void Transmission::ackAnimFinished(int id, const QString &stat) {
    if (id != idx || !waiting) return;
    timer.stop();
    waiting = false;

    if (id < frames.size()) {
        // — data frame ACK handling —
        if (stat == "ack") {
            quint16 crc = frames[idx].crc;
            crcs.append(crc);
            emit logLine(QString("ACK [ALINDI] ► Frame %1").arg(idx+1));
            ++idx;
            sendNext();
        } else {
            emit logLine("ACK_BAD:CRC [UYUŞMADI]");
            sendFrame();
        }
    } else {
        // — checksum frame ACK handling —
        if (stat == "ack") {
            finishing = false;
            emit logLine("▶ Checksum frame başarılı şekilde onaylandı. Simülasyon tamam.");
            // UI'da checksum göster
            quint8 finalCs = computeChecksum();
            emit checksumReady(
                QString("0x%1").arg(finalCs, 2, 16, QLatin1Char('0')));
        } else {
            emit logLine("▶ Checksum frame UYUŞMADI (NACK). Yeniden gönderiliyor...");
            sendChecksumFrame(computeChecksum());
        }
    }
}


quint8 Transmission::computeChecksum() const {
    quint16 sum = 0;
    for (quint16 crc : crcs) {
        quint8 high = (crc >> 8) & 0xFF; // üst 8 bit
        quint8 low = crc & 0xFF;         // alt 8 bit

        sum += high;
        while (sum > 0xFF)
            sum = (sum & 0xFF) + (sum >> 8);  // wrap-around
        sum += low;
        while (sum > 0xFF)
            sum = (sum & 0xFF) + (sum >> 8);
    }
    return quint8((~sum + 1) & 0xFF);  // twos complement
}


void Transmission::finishImmediately()
{
    finishing = true;

    timer.stop();
    waiting = false;

    // 3) Yolda kalmış data frameleri hemen işaretle (log’la, crc’ye ekle)
    for (; idx < frames.size(); ++idx) {
        const Frame &fr = frames[idx];
        crcs.append(fr.crc);
        emit logLine(frameInfoLine(idx, fr));
        emit logLine(QString("Frame %1 [FORCED SEND]\n  ▶ Data  : %2\n")
                         .arg(idx+1)
                         .arg(QString(fr.data.toHex(' '))));
    }

    quint8 cs = computeChecksum();
    emit logLine(QString("▼ TOPLAM %1 FRAME, FORCED CHECKSUM 0x%2")
                     .arg(frames.size())
                     .arg(cs, 2, 16, QLatin1Char('0')));

    sendChecksumFrame(cs);
}


QString Transmission::frameInfoLine(int i, const Frame &fr) const
{
    return QString("Frame %1 │ Header: [FLAG 0x%2  CTRL 0x%3] │ "
                          "CRC: 0x%4")
                  .arg(i + 1)
        .arg(Frame::FLAG, 2, 16, QLatin1Char('0'))
        .arg(fr.ctrl,     2, 16, QLatin1Char('0'))
        .arg(fr.crc,      4, 16, QLatin1Char('0'));
}

