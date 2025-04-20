
#include "transmissiongraphicsview.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

TransmissionGraphicsView::TransmissionGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    scene->setSceneRect(0, 0, 900, 350);
    setMinimumSize(700, 350);
    addNodes();
}
void TransmissionGraphicsView::addNodes()
{
    scene->addRect(sPos.x(), sPos.y(), 100, 100, QPen(Qt::darkGreen, 2), QBrush("#4caf50"));
    scene->addText("Gönderici")->setPos(sPos.x() - 5, sPos.y() + 105);
    scene->addRect(rPos.x(), rPos.y(), 100, 100, QPen(Qt::darkBlue, 2), QBrush("#2196f3"));
    scene->addText("Alıcı")->setPos(rPos.x() + 20, rPos.y() + 105);
}
void TransmissionGraphicsView::reset()
{
    for (auto a : findChildren<QVariantAnimation *>())
    {
        a->stop();
        a->deleteLater();
    }
    q.clear();
    scene->clear();
    addNodes();
}
void TransmissionGraphicsView::enqueueEvent(const GuiEvent &e)
{
    q.enqueue({e});
    if (q.size() == 1)
        start();
}
void TransmissionGraphicsView::start()
{
    if (!q.isEmpty())
        play(q.head().e);
}
QColor TransmissionGraphicsView::color(const QString &s) const
{
    if (s == "ack" || s == "ack_lost")
        return "#00bcd4";
    if (s == "corrupt" || s == "ack_bad")
        return "#f44336";
    return "#8bc34a";
}
void TransmissionGraphicsView::play(const GuiEvent &ev)
{
    const bool isAck = (ev.kind == GuiEvent::AckTx);
    const QColor base = color(ev.status);

    // Grup
    auto grp = new QGraphicsItemGroup;
    scene->addItem(grp);
    grp->setZValue(2);

    // Sabitler
    constexpr int rectW = 100;               // node dikdörtgen genişliği
    constexpr int rectH = 100;               // node dikdörtgen yüksekliği
    constexpr int spacing = 20;              // node ile shape arası boşluk
    constexpr int shapeH = 20;               // tüm şekillerin yüksekliği
    constexpr int dataWidth = 32 + 200 + 32; // veri frame toplam genişliği

    // Şekli oluştur
    if (!isAck)
    {
        // —— VERİ FRAME’i ——
        auto hdr = new QGraphicsRectItem(0, 0, 32, shapeH);
        auto dat = new QGraphicsRectItem(32, 0, 200, shapeH);
        auto trl = new QGraphicsRectItem(232, 0, 32, shapeH);
        hdr->setPen(QPen(Qt::black));
        hdr->setBrush(base.lighter(130));
        dat->setPen(QPen(Qt::black));
        dat->setBrush(base);
        trl->setPen(QPen(Qt::black));
        trl->setBrush(base.lighter(130));
        grp->addToGroup(hdr);
        grp->addToGroup(dat);
        grp->addToGroup(trl);
    }
    else
    {
        // —— ACK FRAME’i —— (sola bakan üçgen)
        QPolygonF tri;
        tri << QPointF(0, shapeH / 2) // uç
            << QPointF(26, 0)         // taban köşe 1
            << QPointF(26, shapeH);   // taban köşe 2
        auto ackShape = new QGraphicsPolygonItem(tri);
        ackShape->setBrush(base);
        ackShape->setPen(QPen(Qt::black, 1, Qt::DashLine));
        grp->addToGroup(ackShape);
    }

    // Numara etiketi
    auto num = new QGraphicsTextItem(QString::number(ev.id + 1));
    num->setDefaultTextColor(Qt::black);
    num->setPos(isAck ? -35 : (rectW + spacing + 32), -15);
    grp->addToGroup(num);

    // Dikey hizalama: node ortası
    const int yOff = (rectH - shapeH) / 2;

    // Başlangıç ve bitiş noktaları
    const QPointF st = isAck
                           ? /*ACK*/ rPos + QPointF(0, yOff)
                           : /*DATA*/ sPos + QPointF(rectW + spacing, yOff);
    const QPointF ed = isAck
                           ? /*ACK*/ sPos + QPointF(rectW, yOff)
                           : /*DATA*/ rPos + QPointF(-dataWidth, yOff);

    // Grupun pozisyonunu set et ve animasyonu başlat
    grp->setPos(st);
    auto anim = new QVariantAnimation(this);
    anim->setDuration(animMs);
    anim->setStartValue(st);
    anim->setEndValue(ed);

    connect(anim, &QVariantAnimation::valueChanged, this,
            [=](const QVariant &v)
            {
                grp->setPos(v.toPointF());
                if ((ev.status == "lost" || ev.status == "ack_lost") &&
                    anim->currentTime() >= animMs / 2)
                    grp->setOpacity(0.0);
            });

    connect(anim, &QVariantAnimation::finished, this, [=]
            {
                if (isAck && (ev.status == "ack" || ev.status == "ack_bad"))
                    emit ackAnimationFinished(ev.id, ev.status);
                scene->removeItem(grp);
                delete grp;
                q.dequeue();
                start();
                anim->deleteLater(); });

    anim->start();
}
