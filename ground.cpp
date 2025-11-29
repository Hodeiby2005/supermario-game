#include "ground.h"

Ground::Ground(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    // REQUIRED: use the real pixmap collision
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}

