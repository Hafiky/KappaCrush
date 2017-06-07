#ifndef BLOK_H
#define BLOK_H
#include <QGraphicsItem>

const int SIRKA_CTVERCE=50;

class Blok : public QGraphicsObject
{
    Q_OBJECT
public:
    Blok();
    QRectF boundingRect() const;

    QColor getBarva() const;
    void setBarva(const QColor &value);


    QPointF getPrvni_pred() const;
    void setPrvni_pred(const QPointF &value);

    QPointF getDruhy_pred() const;
    void setDruhy_pred(const QPointF &value);

protected:

    QPainterPath shape() const;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    BlockMove(QPointF prvni_pred,QPointF druhy_pred);
private:
 QPointF prvni;
 QPointF druhy;
  QColor barva;
 QPointF prvni_pred;
 QPointF druhy_pred;



};

#endif // BLOK_H
