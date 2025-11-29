#include "block.h"
#include "coin.h"
#include "game.h"
#include <QGraphicsScene>
#include <QTimer>

extern Game *game;

Block::Block(BlockType type)
    : m_type(type), used(false)
{
}

void Block::hit()
{
    if (m_type == Question && !used) {
        Coin *c = new Coin();
        QPixmap coinPix(":/Graphics/coin.png");
        c->setPixmap(coinPix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));


        c->setPos(x(), y() - c->pixmap().height());
        scene()->addItem(c);


        QTimer *timer = new QTimer(c);
        int steps = 15;
        int stepCount = 0;
        QObject::connect(timer, &QTimer::timeout, [c, timer, steps, &stepCount]() mutable {
            if (stepCount < steps) {
                c->moveBy(0, -2);
            } else {
                timer->stop();

            }
            stepCount++;
        });
        timer->start(16);

        used = true;
        setPixmap(QPixmap(":/Graphics/brick.jpeg").scaled(40, 40));
    }
}
