
#include "transmission.h"
#include <QFile>
#include <QRandomGenerator>
#include "crc.h"
bool Transmission::randomChance(int p){return QRandomGenerator::global()->bounded(100)<p;}

Transmission::Transmission(const QString&f,QObject*parent):QObject(parent),path(f){
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
        Frame fr; fr.ctrl=seq; fr.data=payload; QByteArray hdr; hdr.append(char(Frame::FLAG)).append(char(fr.ctrl)).append(fr.data); fr.crc=CRC::calc(hdr);
        frames.append(fr); seq=(seq+1)&0x0F;
    }
}

void Transmission::sendChecksumFrame(quint8 cs)
{
    /* -------- Frame oluştur -------- */
    Frame ck;
    ck.ctrl = 0x10;             // TYPE‑bit = 1  (özel header)
    ck.data.fill(0, 13);
    ck.data[0] = char(cs);      // checksum kodu DATA[0]

    /* CRC‑16 ( Trailer ) */
       QByteArray hdr;
    hdr.append(char(Frame::FLAG))
        .append(char(ck.ctrl))
        .append(ck.data);
    ck.crc = CRC::calc(hdr);

    /* -------- %5 bozulma -------- */
    bool corrupt = randomChance(5);
    if (corrupt) {
        ck.data[0] ^= 0x01;     // en düşük biti çevir
        emit logLine("★Checksum frame [BOZULDU]");
    }

    /* -------- Log -------- */
    emit logLine(QString("Checksum frame gönderildi   "
                         "DATA:0x%1  CRC:0x%2  %3")
                     .arg(quint8(ck.data[0]),2,16,QLatin1Char('0'))
                     .arg(ck.crc,4,16,QLatin1Char('0'))
                     .arg(corrupt?"(hatalı)":""));

    /* -------- GUI & sinyaller -------- */
    emit guiEvent({GuiEvent::FrameTx, idx, corrupt ? "corrupt" : "success"});
    emit checksumReady(QString("0x%1").arg(quint8(ck.data[0]),2,16,QLatin1Char('0')));

    /* Bitir modunda ACK beklemiyoruz, waiting=false kalır */
}


void Transmission::sendNext(){
    if(idx<frames.size()) sendFrame();
    else {
        quint8 cs = computeChecksum();
        sendChecksumFrame(cs);
    }
}
void Transmission::sendFrame(){
    Frame fr=frames[idx]; QByteArray raw=fr.toByteArray();
    bool lost=randomChance(10); bool corrupt=randomChance(20);
    emit logLine(frameInfoLine(idx, fr));
    if(lost){
        emit guiEvent({GuiEvent::FrameTx,idx,"lost"});
        emit logLine("        ↳ Frame iletilemedi (kayboldu)");
        waiting=true; timer.start(); return;
    }
    if(corrupt){
        raw[2]^=0xFF;
        emit guiEvent({GuiEvent::FrameTx,idx,"corrupt"});
        emit logLine("        ↳ Veri [BOZULDU]");
    }
    else{
        emit guiEvent({GuiEvent::FrameTx,idx,"success"});
        emit logLine(QString("Frame %1 gönderildi\n"
            "  ▶ Data  : %2\n").arg(idx+1).arg(QString(fr.data.toHex(' '))).arg(fr.crc,4,16,QLatin1Char('0')));
    }
    waiting=true; timer.start();
    Frame recv=Frame::fromByteArray(raw); QByteArray hdr; hdr.append(char(Frame::FLAG)).append(char(recv.ctrl)).append(recv.data);
    bool crcOk=CRC::calc(hdr)==recv.crc;
    QTimer::singleShot(interval,this,[=]{ sendAck(!crcOk);});
}
void Transmission::sendAck(bool bad){
    if(!waiting) return;
    if(randomChance(15)){ emit guiEvent({GuiEvent::AckTx,idx,"ack_lost"});
    emit logLine("ACK kayıp - timeout bekleniyor"); return;}
    emit guiEvent({GuiEvent::AckTx,idx,bad?"ack_bad":"ack"});
}
void Transmission::timeout(){ if(waiting){ emit logLine("Timeout [RESEND]"); waiting=false; sendFrame();}}
void Transmission::ackAnimFinished(int id,const QString &stat){
    if(id!=idx||!waiting) return; timer.stop(); waiting=false;
    if(stat=="ack"){
        quint16 crc = frames[idx].crc;
        crcs.append(crc);
        emit logLine(QString("ACK [ALINDI] ► Frame %1").arg(idx+1));
        ++idx; sendNext();}
    else{ emit logLine("ACK_BAD:CRC [UYUŞMADI]"); sendFrame();}
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
    timer.stop();
    waiting = false;
    for (; idx < frames.size(); ++idx) {            // kalan tüm frame'ler
        const Frame &fr = frames[idx];
        crcs.append(fr.crc);
        emit logLine(frameInfoLine(idx, fr));       // ayrıntılı satır
        emit logLine(QString("Frame %1 gönderildi\n"
                             "  ▶ Data  : %2\n").arg(idx+1).arg(QString(fr.data.toHex(' '))).arg(fr.crc,4,16,QLatin1Char('0')));
    }


    quint8 cs = computeChecksum();
    emit logLine(QString("▼ TOPLAM  %1  FRAME").arg(frames.size()));
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

