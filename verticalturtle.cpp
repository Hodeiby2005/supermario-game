#include "verticalturtle.h"
#include <QTimer>
#include <QPixmap>
#include <QDebug>

VerticalTurtle::VerticalTurtle(int topLimit, int bottomLimit, QGraphicsItem *parent)
    : QObject(),
    QGraphicsPixmapItem(parent),
    topY(topLimit),
    bottomY(bottomLimit),
    goingDown(true),
    isDead(false)
{
    QPixmap pix(":/Graphics/turtle.png");
    if (pix.isNull()) {
        qDebug() << "turtle.png FAILED TO LOAD!";
        pix = QPixmap(40,40);
        pix.fill(Qt::green);
    }

    setPixmap(pix.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &VerticalTurtle::moveVertical);
    timer->start(20);
}

void VerticalTurtle::moveVertical()
{
    if (isDead) return;   // ❗ DEAD = no movement

    if (goingDown)
        setY(y() + 2);
    else
        setY(y() - 2);

    if (y() >= bottomY) goingDown = false;
    if (y() <= topY)    goingDown = true;
}

void VerticalTurtle::die()
{
    if (isDead) return;
    isDead = true;

    // ⭐ Change sprite to turtle shell
    QPixmap shell(":/Graphics/turtleShell.png.");
    if (shell.isNull()) {
        qDebug() << "turtleshell.png FAILED TO LOAD!";
        shell = QPixmap(40,40);
        shell.fill(Qt::red);
    }

    setPixmap(shell.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // ⭐ Slight downward adjustment so shell sits properly
    setY(y() + 5);
}
