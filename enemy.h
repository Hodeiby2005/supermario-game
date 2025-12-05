// enemy.h
#ifndef ENEMY_H
#define ENEMY_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Enemy : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Enemy(double leftBound,
                   double rightBound,
                   QGraphicsItem *parent = nullptr);

    void die(); // Handles stomp death
    bool getIsDead() const { return isDead; } // Getter for death status

private slots:
    void moveEnemy();

private:
    double leftLimit;
    double rightLimit;
    double speed;
    bool isDead = false; // State flag for death
};

#endif // ENEMY_H
