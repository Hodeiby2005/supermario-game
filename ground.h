#ifndef GROUND_H
#define GROUND_H

#include <QGraphicsPixmapItem>

class Ground : public QGraphicsPixmapItem
{
public:
    Ground(QGraphicsItem *parent = nullptr);
    QRectF boundingRect() const override;
};

#endif // GROUND_H
