// transmissiongraphicsview.h

#ifndef TRANSMISSIONGRAPHICSVIEW_H
#define TRANSMISSIONGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QMap>

class TransmissionGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TransmissionGraphicsView(QWidget *parent = nullptr);
    void resetScene();
    void addFrame(int index, QString crc);
    void setFrameStatus(int index, const QString &status);  // "sent", "corrupted", "lost", "ack_failed", "ack_ok"

private:
    QMap<int, QGraphicsLineItem*> arrowItems;
    QMap<int, QString> frameCRCMap;
    QMap<int, QString> frameStatusMap;

    QGraphicsScene *scene;
    QMap<int, QGraphicsRectItem*> frameItems;
    int currentX;
};

#endif // TRANSMISSIONGRAPHICSVIEW_H
