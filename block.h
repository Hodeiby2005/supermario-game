// block.h
#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>
#include <QObject>

class Block : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    // BlockType must be public to be accessed by Player
    enum BlockType { Brick, Question, Used };

    explicit Block(BlockType t, QGraphicsItem *parent = nullptr);

    // Public getters/setters needed by Player
    BlockType getType() const;
    void hit();

private:
    BlockType type;

    //... other private members ...
};

#endif // BLOCK_H

