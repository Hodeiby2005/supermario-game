#include "enemy.h"

Enemy::Enemy(double leftBound, double rightBound, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent),
    leftLimit(leftBound),
    rightLimit(rightBound),
    speed(2.0)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Enemy::moveEnemy);
    timer->start(16);
}

void Enemy::moveEnemy()
{
    moveBy(speed, 0);

    if (x() < leftLimit || x() > rightLimit) {
        speed = -speed; // turn around
    }
}
