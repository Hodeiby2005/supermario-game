#include "player.h"
#include "ground.h"
#include "block.h"
#include "enemy.h"
#include "castle.h"
#include "coin.h"
#include "game.h"

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QList>

extern Game *game;

Player::Player(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent),
    velocityY(0.0),
    onGround(false)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Player::updatePlayer);
    timer->start(16); // ~60 FPS
}

void Player::keyPressEvent(QKeyEvent *event)
{
    const int moveStep = 10;

    if (event->key() == Qt::Key_Right) {
        moveBy(moveStep, 0);
    } else if (event->key() == Qt::Key_Left) {
        moveBy(-moveStep, 0);
    } else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) {
        if (onGround) {
            velocityY = -14.0; // jump
            onGround = false;
        }
    }

    keepInBounds();
}

void Player::updatePlayer()
{
    // Apply gravity
    const double gravity = 0.7;
    velocityY += gravity;
    moveBy(0, velocityY);

    onGround = false;
    handleVerticalCollisions();
    keepInBounds();

    // Center camera on player
    if (game) {
        game->centerOn(this);
    }

    // Fell in a hole?
    if (scene() && y() > scene()->height() + 100) {
        if (game) game->gameOver();
    }

    // Check collisions with enemies, coins, castle inside player's loop
    QList<QGraphicsItem *> colliding = collidingItems();
    for (QGraphicsItem *item : colliding) {
        if (dynamic_cast<Enemy*>(item)) {
            if (game) game->gameOver();
        } else if (dynamic_cast<Castle*>(item)) {
            if (game) game->levelCompleted();
        }
        // Coin collision is handled in Coin itself
    }
}

void Player::handleVerticalCollisions()
{
    QList<QGraphicsItem*> items = collidingItems();
    for (QGraphicsItem *item : items) {

        Ground *g = dynamic_cast<Ground*>(item);
        Block  *b = dynamic_cast<Block*>(item);
        if (!g && !b) continue;

        QGraphicsItem *platform = (g ? static_cast<QGraphicsItem*>(g)
                                     : static_cast<QGraphicsItem*>(b));

        // Player's bottom Y
        qreal playerBottom = y() + pixmap().height();

        // Platform's top Y
        qreal platformTop = platform->y();

        // If falling and landing on top of ground/platform
        if (velocityY >= 0 && playerBottom >= platformTop && playerBottom <= platformTop + 20)
        {
            setY(platformTop - pixmap().height());  // snap on top
            velocityY = 0;
            onGround = true;
        }
    }
}

void Player::keepInBounds()
{
    if (!scene()) return;

    if (x() < 0)
        setX(0);

    double maxX = scene()->sceneRect().right() - boundingRect().width();
    if (x() > maxX)
        setX(maxX);
}
