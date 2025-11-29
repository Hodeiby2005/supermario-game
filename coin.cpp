#include "coin.h"
#include "player.h"
#include "game.h"

extern Game *game;

Coin::Coin(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Coin::checkCollision);
    timer->start(50);
}

void Coin::checkCollision()
{
    QList<QGraphicsItem*> items = collidingItems();
    for (QGraphicsItem *item : items) {
        Player *p = dynamic_cast<Player*>(item);
        if (p) {
            if (game) game->addScore(1);
            if (scene()) scene()->removeItem(this);
            deleteLater();
            return;
        }
    }
}
