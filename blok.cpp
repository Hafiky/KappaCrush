#include "blok.h"
#include <QPainter>
#include <QPicture>
#include <QPixmap>
#include <QImage>
#include <qDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>




Blok::Blok()
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

QRectF Blok::boundingRect() const
{

    return QRectF(0,0,SIRKA_CTVERCE,SIRKA_CTVERCE);
}

QColor Blok::getBarva() const
{
    return barva;
}

void Blok::setBarva(const QColor &value)
{
    barva = value;
}

QPainterPath Blok::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Blok::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    int posy = event->scenePos().y();
    int posx = event->scenePos().x();

    posy = posy-posy%SIRKA_CTVERCE;
    posx = posx-posx%SIRKA_CTVERCE;

    prvni.setX(posx);
    prvni.setY(posy);
    prvni_pred.setX(posx);
    prvni_pred.setY(posy);
}

void Blok::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // qDebug()<<event->pos();
    QPoint pos(25,25);
    setPos(event->scenePos()-pos);
    this->setZValue(2);

}

void Blok::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    bool ano=true;

    int posy = event->scenePos().y();
    int posx = event->scenePos().x();

    posy = posy-posy%SIRKA_CTVERCE;
    posx = posx-posx%SIRKA_CTVERCE;

    druhy.setX(posx);
    druhy.setY(posy);
    druhy_pred.setX(posx);
    druhy_pred.setY(posy);

    this->setZValue(1);    
    if (druhy.x()+50 < prvni.x() || druhy.x()-50 > prvni.x() ||
            druhy.y()+50 < prvni.y() || druhy.y()-50 > prvni.y()) {
        qDebug()<<"víc než 1";
        ano=true;

    }else{
        qDebug()<<"mín než 1";
        ano=false;
    }


    if (prvni.x() > druhy.x()) {
        druhy.setX(prvni.x()-SIRKA_CTVERCE);

    }
    if (prvni.y() > druhy.y()) {

        druhy.setY(prvni.y()-SIRKA_CTVERCE);
    }
    if (prvni.x() < druhy.x()) {
        druhy.setX(prvni.x()+SIRKA_CTVERCE);

    }
    if (prvni.y() < druhy.y()) {

        druhy.setY(prvni.y()+SIRKA_CTVERCE);
    }


    if (ano) {
        setPos(prvni);
    }else{
       setPos(druhy);
    }



    QList<QGraphicsItem *> list =  this->collidingItems(Qt::IntersectsItemBoundingRect);

    foreach(QGraphicsItem * i , list)
    {
        Blok * item= qgraphicsitem_cast<Blok *>(i);
        // qDebug()<<"pos";
        if (item)
        {
            if (ano) {
                item->setPos(druhy);
            }else{
            item->setPos(prvni);
            }

            //qDebug()<<"macka"<<item->barva.name();
        }
    }
    if (ano) {

    }else{
    emit BlockMove(prvni_pred,druhy_pred);
    }

}

void Blok::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QBrush(this->barva));
    painter->drawPath(shape());





}

QPointF Blok::getDruhy_pred() const
{
    return druhy_pred;
}

void Blok::setDruhy_pred(const QPointF &value)
{
    druhy_pred = value;
}

QPointF Blok::getPrvni_pred() const
{
    return prvni_pred;
}

void Blok::setPrvni_pred(const QPointF &value)
{
    prvni_pred = value;
}

