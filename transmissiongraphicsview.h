
#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include <QQueue>
#include <QVariantAnimation>
#include "transmission.h"

class TransmissionGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit TransmissionGraphicsView(QWidget *parent = nullptr);
    void setAnimDuration(int ms) { animMs = qMax(200, ms); }
    void reset();
public slots:
    void enqueueEvent(const GuiEvent &e);
signals:
    void ackAnimationFinished(int id, const QString &stat);

private:
    struct P
    {
        GuiEvent e;
    };
    QQueue<P> q;
    int animMs = 1600;
    QGraphicsScene *scene = nullptr;
    QPointF sPos{70, 150}, rPos{730, 150};
    void start();
    void play(const GuiEvent &);
    QColor color(const QString &) const;
    void addNodes();
};
#endif
