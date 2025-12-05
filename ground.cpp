// ground.cpp
#include "ground.h"

Ground::Ground(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
{
    // Use the pixmap's rect for collisions
    setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
}



