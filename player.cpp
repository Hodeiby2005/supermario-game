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
    timer->start(16);

    idlePixmap = QPixmap(":/Graphics/Mario Idle.png").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    walkLeftPixmap = QPixmap(":/Graphics/walking left.png").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    walkRightPixmap = walkLeftPixmap.transformed(QTransform().scale(-1, 1));
    setPixmap(idlePixmap);

}

void Player::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        movingRight = true;
    }
    else if (event->key() == Qt::Key_Left) {
        movingLeft = true;
    }
    else if ((event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) && onGround) {
        velocityY = -14.0;
        onGround = false;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right) {
        movingRight = false;
    }
    else if (event->key() == Qt::Key_Left) {
        movingLeft = false;
    }
}


void Player::updatePlayer()
{
    const int moveStep = 4;

    if (movingRight) {
        moveBy(moveStep, 0);
        if (onGround) setPixmap(walkRightPixmap);
    }
    else if (movingLeft) {
        moveBy(-moveStep, 0);
        if (onGround) setPixmap(walkLeftPixmap);
    }
    else {
        if (onGround) setPixmap(idlePixmap);
    }


    const double gravity = 0.7;
    velocityY += gravity;
    moveBy(0, velocityY);

    onGround = false;
    handleVerticalCollisions();
    keepInBounds();


    if (!onGround) {
        setPixmap(idlePixmap);
    }


    if (game) {
        game->centerOn(this);
    }


    if (scene() && y() > scene()->height() + 100) {
        if (game) game->gameOver();
    }


    QList<QGraphicsItem *> colliding = collidingItems();
    for (QGraphicsItem *item : colliding) {
        if (dynamic_cast<Enemy*>(item)) {
            if (game) game->gameOver();
        } else if (dynamic_cast<Castle*>(item)) {
            if (game) game->levelCompleted();
        }

    }
}


void Player::handleVerticalCollisions()
{
    QList<QGraphicsItem*> items = collidingItems();
    for (QGraphicsItem *item : items) {

        Ground *g = dynamic_cast<Ground*>(item);
        Block  *b = dynamic_cast<Block*>(item);
        if (!g && !b) continue;

        qreal playerTop = y();
        qreal playerBottom = y() + pixmap().height();
        qreal platformTop = item->y();
        qreal platformBottom = item->y() + item->boundingRect().height();

        if (velocityY >= 0) {

            if (playerBottom >= platformTop && playerBottom <= platformTop + 20)
            {
                setY(platformTop - pixmap().height());
                velocityY = 0;
                onGround = true;
            }
        } else if (velocityY < 0) {

            if (playerTop <= platformBottom && playerTop >= platformTop - 5)
            {
                setY(platformBottom);
                velocityY = 0;


                if (b) {
                    b->hit();
                }
            }
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


