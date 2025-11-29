#ifndef COIN_H
#define COIN_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Coin : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Coin(QGraphicsItem *parent = nullptr);

private slots:
    void checkCollision();
};

#endif // COIN_H
