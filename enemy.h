#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Enemy(double leftBound, double rightBound, QGraphicsItem *parent = nullptr);

private slots:
    void moveEnemy();

private:
    double leftLimit;
    double rightLimit;
    double speed;
};

#endif // ENEMY_H
