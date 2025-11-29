#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>

class Block : public QGraphicsPixmapItem
{
public:
    enum BlockType { Brick, Question };
    Block(BlockType type);

    BlockType blockType() const { return m_type; }
    void hit();

private:
    BlockType m_type;
    bool used = false;
};

#endif // BLOCK_H
