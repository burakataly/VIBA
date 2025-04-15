#ifndef TRANSMISSIONGRAPHICSVIEW_H
#define TRANSMISSIONGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMap>
#include <QGraphicsTextItem>

class TransmissionGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TransmissionGraphicsView(QWidget *parent = nullptr);
    void resetScene();
    void animateFrame(int frameId, const QString &status);

signals:
    // ACK animasyonu tamamlandığında view, ACK'nin durumunu bildirir.
    // ackStatus değerleri: "ack", "ack_lost", "ack_bad"
    void ackAnimationFinished(int frameId, const QString &ackStatus);

private:
    QGraphicsScene *m_scene;
    QGraphicsItem *senderNode;
    QGraphicsItem *receiverNode;
    QMap<int, QGraphicsRectItem*> frameMap;

    QPointF senderPos;
    QPointF receiverPos;

    void createNodes();
    void animateRect(int frameId, QGraphicsRectItem *item, const QString &status, bool reverse = false);
    QColor colorForStatus(const QString &status);
};

#endif // TRANSMISSIONGRAPHICSVIEW_H
