// enemy.cpp
#include "enemy.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QPixmap>
#include <QDebug>

Enemy::Enemy(double leftBound, double rightBound, QGraphicsItem *parent)
    : QObject(),
    QGraphicsPixmapItem(parent),
    leftLimit(leftBound),
    rightLimit(rightBound),
    speed(2.5),
    isDead(false)
{
    // Timer for movement
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::moveEnemy);
    timer->start(16); // ~60 FPS
}

void Enemy::die()
{
    if (isDead) return;

    isDead = true;

    // Stop movement
    if (QTimer *timer = findChild<QTimer*>()) {
        timer->stop();
    }

    // Squash the enemy sprite visually
    QPixmap currentPix = pixmap();
    if (!currentPix.isNull()) {
        // Squash the current sprite to half height
        setPixmap(currentPix.scaled(currentPix.width(), currentPix.height() / 2, Qt::IgnoreAspectRatio));
    }

    // Adjust position down by half its new height to sit correctly on the ground (squashed)
    setPos(x(), y() + boundingRect().height());

    // Schedule safe removal and deletion after 250ms for the visual effect to last a moment.
    QTimer::singleShot(250, this, [this]() {
        if (scene()) {
            scene()->removeItem(this);
        }
        delete this;
    });
}


void Enemy::moveEnemy()
{
    if (isDead) return; // Prevent dead enemies from moving

    // Move horizontally
    moveBy(speed, 0);

    // Reverse direction when reaching patrol bounds
    if (x() < leftLimit || (x() + boundingRect().width()) > rightLimit) {
        speed = -speed;
    }
}


