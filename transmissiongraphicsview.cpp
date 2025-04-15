#include "TransmissionGraphicsView.h"
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QDebug>

TransmissionGraphicsView::TransmissionGraphicsView(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this))
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setMinimumSize(600, 300);
    m_scene->setSceneRect(0, 0, 800, 300);
    resetScene();
}

void TransmissionGraphicsView::resetScene()
{
    m_scene->clear();
    frameMap.clear();
    createNodes();
}

void TransmissionGraphicsView::createNodes()
{
    senderPos = QPointF(50, 100);
    receiverPos = QPointF(600, 100);

    senderNode = m_scene->addRect(0, 0, 80, 80, QPen(Qt::darkGreen), QBrush(Qt::green));
    senderNode->setPos(senderPos);
    auto senderText = m_scene->addSimpleText("Gönderici");
    senderText->setPos(senderPos.x(), senderPos.y() + 85);

    receiverNode = m_scene->addRect(0, 0, 80, 80, QPen(Qt::darkBlue), QBrush(Qt::blue));
    receiverNode->setPos(receiverPos);
    auto receiverText = m_scene->addSimpleText("Alıcı");
    receiverText->setPos(receiverPos.x(), receiverPos.y() + 85);
}

void TransmissionGraphicsView::animateFrame(int frameId, const QString &status)
{
    // ACK animasyonları ters yönde (receiver'dan sender'a) olmalı.
    bool reverse = (status == "ack" || status == "ack_lost" || status == "ack_bad");
    QColor color = colorForStatus(status);

    // Oluşturulacak frame kutusu: 40x40
    QGraphicsRectItem *rect = m_scene->addRect(0, 0, 40, 40, QPen(Qt::black), QBrush(color));
    rect->setZValue(1);

    // Ekranda, frame numarasını yazdırmak için küçük text ekleyelim.
    QGraphicsTextItem *numText = m_scene->addText(QString::number(frameId + 1));
    numText->setDefaultTextColor(Qt::black);

    if (!reverse) {
        rect->setPos(senderPos.x() + 100, senderPos.y() + 20);
        numText->setPos(rect->pos().x() + 10, rect->pos().y() + 10);
    } else {
        rect->setPos(receiverPos.x() - 40, receiverPos.y() + 20);
        numText->setPos(rect->pos().x() + 10, rect->pos().y() + 10);
    }

    frameMap[frameId] = rect;
    animateRect(frameId, rect, status, reverse);
}

void TransmissionGraphicsView::animateRect(int frameId, QGraphicsRectItem *item, const QString &status, bool reverse)
{
    QPointF start, end;
    if (!reverse) {
        start = senderPos + QPointF(100, 20);
        end = receiverPos + QPointF(0, 20);
    } else {
        start = receiverPos + QPointF(-40, 20);
        end = senderPos + QPointF(100, 20);
    }

    int duration = 1000;
    int steps = 20;
    int interval = duration / steps;
    int currentStep = 0;
    QPointF delta = (end - start) / steps;
    item->setPos(start);

    QTimer *moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, [=]() mutable {
        item->moveBy(delta.x(), delta.y());
        currentStep++;

        // Eğer frame "lost" veya "ack_lost", yarıda opaklık 0 olur.
        if ((status == "lost" || status == "ack_lost") && currentStep == steps/2) {
            item->setOpacity(0.0);
        }

        if (currentStep >= steps) {
            moveTimer->stop();
            // Eğer ACK animasyonu (ters yönde) ise, animasyonun bitişinde ackAnimationFinished sinyalini gönder.
            if (reverse) {
                emit ackAnimationFinished(frameId, status);
            }
            if (status == "corrupt") {
                QTimer *blinkTimer = new QTimer(this);
                int blinkCount = 6;
                connect(blinkTimer, &QTimer::timeout, this, [=]() mutable {
                    QColor currentColor = item->brush().color();
                    item->setBrush(currentColor == Qt::red ? Qt::white : Qt::red);
                    if (--blinkCount == 0) {
                        blinkTimer->stop();
                        m_scene->removeItem(item);
                        delete item;
                        blinkTimer->deleteLater();
                    }
                });
                blinkTimer->start(150);
            } else {
                m_scene->removeItem(item);
                delete item;
            }
            moveTimer->deleteLater();
        }
    });
    moveTimer->start(interval);
}

QColor TransmissionGraphicsView::colorForStatus(const QString &status)
{
    if (status == "corrupt" || status == "ack_bad")
        return Qt::red;
    if (status == "ack" || status == "ack_lost" )
        return Qt::cyan;
    return Qt::green; // lost veya success
}
