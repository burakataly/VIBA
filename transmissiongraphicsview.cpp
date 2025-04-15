// transmissiongraphicsview.cpp

#include "transmissiongraphicsview.h"
#include <QGraphicsTextItem>
#include <QBrush>

TransmissionGraphicsView::TransmissionGraphicsView(QWidget *parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this)), currentX(10)
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);               // ✅ tooltip için gerekli
    viewport()->setMouseTracking(true);   // ✅ alt view'a da yay
}


void TransmissionGraphicsView::resetScene()
{
    scene->clear();
    frameItems.clear();
    currentX = 10;
}

void TransmissionGraphicsView::addFrame(int index, QString crc)
{
    QGraphicsRectItem *rect = scene->addRect(currentX, 50, 80, 40, QPen(Qt::black), QBrush(Qt::lightGray));
    rect->setAcceptHoverEvents(true); // ✅ Tooltip'ler için kritik
    QGraphicsTextItem *label = scene->addText(QString("F%1").arg(index+1));
    label->setPos(currentX + 10, 55);

    frameItems[index] = rect;
    frameCRCMap[index] = crc;
    frameStatusMap[index] = "Hazır";

    rect->setToolTip(QString("Frame #%1\nCRC: 0x%2\nDurum: %3")
                         .arg(index+1)
                         .arg(crc)
                         .arg("Hazır"));

    currentX += 100;
}



void TransmissionGraphicsView::setFrameStatus(int index, const QString &status)
{
    if (!frameItems.contains(index)) return;

    // Renk değiştir
    QBrush brush;
    if (status == "sent") brush = QBrush(Qt::green);
    else if (status == "corrupted") brush = QBrush(Qt::red);
    else if (status == "lost") brush = QBrush(Qt::gray);
    else if (status == "ack_failed") brush = QBrush(Qt::yellow);
    else brush = QBrush(Qt::blue); // ack_ok
    frameItems[index]->setBrush(brush);

    // Tooltip bilgilerini güncelle
    frameStatusMap[index] = status;
    QString tooltip = QString("Frame #%1\nCRC: 0x%2\nDurum: %3")
                          .arg(index+1)
                          .arg(frameCRCMap[index])
                          .arg(frameStatusMap[index]);
    frameItems[index]->setToolTip(tooltip);

    // Oku da renklendir (varsa)
    if (arrowItems.contains(index)) {
        QPen pen;
        if (status == "sent") pen = QPen(Qt::green, 2);
        else if (status == "corrupted") pen = QPen(Qt::red, 2, Qt::DashLine);
        else if (status == "lost") pen = QPen(Qt::gray, 2, Qt::DotLine);
        else pen = QPen(Qt::yellow, 2);
        arrowItems[index]->setVisible(true);
        arrowItems[index]->setPen(pen);



    }
}

