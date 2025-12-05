#ifndef VERTICALTURTLE_H
#define VERTICALTURTLE_H

#include <QGraphicsPixmapItem>
#include <QObject>

class VerticalTurtle : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit VerticalTurtle(int topLimit, int bottomLimit, QGraphicsItem *parent = nullptr);

    void die();              // ⭐ new: switch to shell
    bool getIsDead() const { return isDead; }

protected slots:
    void moveVertical();

private:
    int topY;
    int bottomY;
    bool goingDown;
    bool isDead;             // ⭐ needed for death state
};

#endif // VERTICALTURTLE_H

