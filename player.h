// player.h
#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>

class Player : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Player(QGraphicsItem *parent = nullptr);
    void stopMotion(); // Stops all player movement immediately

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void updatePlayer();

private:
    double velocityY;
    double velocityX; // Horizontal velocity for smooth movement
    bool onGround;
    bool movingLeft;
    bool movingRight;

    // Animation variables (assuming these were in your original file)
    bool facingRight;
    bool isMoving;
    QString lastPath;

    void handleVerticalCollisions();
    void keepInBounds();
};

#endif // PLAYER_H
