// block.cpp
#include "block.h"
#include "game.h"
#include "coin.h" // Required for spawning coins

#include <QPixmap>
#include <QGraphicsScene>
#include <QDebug>

// Use the global game pointer defined in main.cpp
extern Game *game;

Block::Block(BlockType t, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), type(t)
{
    // The initial pixmap setup is usually handled in game.cpp or setupLevel1()
}

Block::BlockType Block::getType() const
{
    return type;
}

void Block::hit()
{
    // Only Question blocks (Mystery Boxes) should react to a hit
    if (type == Question) {
        // 1. Change internal type and update sprite to a "used" block
        type = Used;

        // --- Resource Path for the "Used" Block Sprite ---
        QPixmap usedPix(":/Graphics/small_brick.png");

        if (usedPix.isNull()) {
            qDebug() << "USED BLOCK SPRITE FAILED TO LOAD! Using fallback.";
            // Fallback: create a dark gray rectangle if sprite is missing
            QPixmap fallback(sceneBoundingRect().width(), sceneBoundingRect().height());
            fallback.fill(Qt::darkGray);
            setPixmap(fallback);
        } else {
            // Scale and set the new sprite
            setPixmap(usedPix.scaled(sceneBoundingRect().width(),
                                     sceneBoundingRect().height(),
                                     Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation));
        }

        // 2. Spawn a coin/item
        if (game && scene()) {
            game->addScore(1); // Optional: add 1 point for the coin spawn itself

            // Find a position slightly above the block to spawn the item
            QPointF spawnPos = scenePos();
            spawnPos.setY(spawnPos.y() - 40);

            // Create and add the coin to the scene
            Coin *c = new Coin();
            c->setPos(spawnPos);
            scene()->addItem(c);
        }
    }
}
