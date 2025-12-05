// castle.h
#ifndef CASTLE_H
#define CASTLE_H

#include <QGraphicsPixmapItem>

class Castle : public QGraphicsPixmapItem
{
public:
    explicit Castle(QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent) {}
};

#endif // CASTLE_H

