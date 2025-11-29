#include "ground.h"

Ground::Ground(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
}

QRectF Ground::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}
