#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>

class Block : public QGraphicsPixmapItem
{
public:
    enum Type { Brick, Question };

    explicit Block(Type t, QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent), type(t) {}

    Type getType() const { return type; }

private:
    Type type;
};

#endif // BLOCK_H
