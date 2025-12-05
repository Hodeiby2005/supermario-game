// flag.h
#ifndef FLAG_H
#define FLAG_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Flag : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // The constructor takes the starting position x and y, and an optional parent.
    explicit Flag(qreal x, qreal y, QGraphicsItem *parent = nullptr);
};

#endif // FLAG_H
