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

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updatePlayer();

private:
    double velocityY;
    bool onGround;

    void handleVerticalCollisions();
    void keepInBounds();
};

#endif // PLAYER_H
