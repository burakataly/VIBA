#include <QGraphicsObject>

class AnimatedFrameItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    AnimatedFrameItem(qreal w, qreal h, const QColor &color, QGraphicsItem *parent = nullptr)
    {
        rect = QRectF(0, 0, w, h);
        brush = QBrush(color);
    }

    QRectF boundingRect() const override { return rect; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override {
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        painter->drawRect(rect);
    }

    void setBrush(const QBrush &b) { brush = b; update(); }

private:
    QRectF rect;
    QBrush brush;
};
