// pipe.h
#ifndef PIPE_H
#define PIPE_H

#include <QGraphicsPixmapItem>

class Pipe : public QGraphicsPixmapItem
{
public:
    explicit Pipe(QGraphicsItem *parent = nullptr)
        : QGraphicsPixmapItem(parent) {}
};

#endif // PIPE_H
