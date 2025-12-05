// coin.cpp
#include "coin.h"
#include "player.h"
#include "game.h"

#include <QGraphicsScene>
#include <QList>

extern Game *game;

Coin::Coin(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent),
    baseY(0.0),
    direction(1)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Coin::tick);
    timer->start(50);
}

void Coin::tick()
{
    if (game && (game->isGameOver || game->isLevelComplete))
        return;

    // store baseY after first tick
    if (baseY == 0.0) {
        baseY = y();
    }

    // simple bobbing animation
    double offset = 3.0;
    moveBy(0, direction * 0.7);
    if (y() > baseY + offset)
        direction = -1;
    else if (y() < baseY - offset)
        direction = 1;

    // collision with player
    QList<QGraphicsItem*> items = collidingItems();
    for (QGraphicsItem *item : items) {
        Player *p = dynamic_cast<Player*>(item);
        if (p) {
            const int scoreValue = 1; // 🌟 FIX: Score 1 for coin
            if (game) {
                game->addScore(scoreValue);
                game->showScorePopup(scoreValue, pos());
            }

            // Remove coin from the scene
            if (scene())
                scene()->removeItem(this);

            delete this; // Delete the coin object
            return;
        }
    }
}
