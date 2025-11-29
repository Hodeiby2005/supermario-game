// game.cpp
#include "game.h"
#include "player.h"
#include "ground.h"
#include "enemy.h"
#include "coin.h"
#include "block.h"
#include "castle.h"

#include <QScrollBar>

//Game *game = nullptr;   // defined here

Game::Game(QWidget *parent)
    : QGraphicsView(parent),
    scene(new QGraphicsScene(this)),
    player(nullptr),
    score(0),
    scoreText(nullptr)
{
    // View size on the screen
    setFixedSize(800, 450);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Scene size (long level)
    scene->setSceneRect(0, 0, 3000, 450);
    this->setScene(scene);

    // Background image
    setBackgroundBrush(QBrush(QPixmap(":/Graphics/background.png")));

    createHud();
    setupLevel1();
}

void Game::createHud()
{
    score = 0;
    scoreText = new QGraphicsTextItem(QString("Score: %1").arg(score));
    QFont font("Times", 16);
    scoreText->setFont(font);
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setZValue(1000);
    scoreText->setPos(10, 10);
    scene->addItem(scoreText);
}

void Game::addScore(int delta)
{
    score += delta;
    if (scoreText)
        scoreText->setPlainText(QString("Score: %1").arg(score));
}

void Game::setupLevel1()
{
    // ----- Create ground tiles -----
    const int sceneHeight = static_cast<int>(scene->sceneRect().height());

    // load raw image and scale it to a reasonable tile size
    QPixmap rawGround(":/Graphics/ground.jpeg");
    QPixmap groundTile = rawGround.scaled(
        256, 80,                      // tile width, tile height
        Qt::IgnoreAspectRatio,
        Qt::SmoothTransformation);

    const int groundHeight = groundTile.height();    // 80
    const int groundY      = sceneHeight - groundHeight;

    int tileWidth = groundTile.width();
    int numTiles  = scene->sceneRect().width() / tileWidth + 3;

    int gapStart = 12;
    int gapEnd   = 14;

    for (int i = 0; i < numTiles; ++i) {
        if (i >= gapStart && i <= gapEnd) continue;  // leave a hole

        Ground *g = new Ground();
        g->setPixmap(groundTile);
        g->setPos(i * tileWidth, groundY);
        g->setZValue(1);
        scene->addItem(g);
    }

    // ----- Player -----
    player = new Player();

    QPixmap mario(":/Graphics/supermario.png");
    mario = mario.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    player->setPixmap(mario);

    // put Mario standing on the ground
    player->setPos(50, groundY - player->pixmap().height());

    scene->addItem(player);
    player->setZValue(10);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    // ----- Simple brick platform -----
    QPixmap brickPix(":/Graphics/brick.jpeg");
    int brickY = groundY - 100;

    Block *platform1 = new Block(Block::Brick);
    platform1->setPixmap(brickPix.scaled(150, 40));
    platform1->setPos(600, brickY);
    scene->addItem(platform1);

    Block *platform2 = new Block(Block::Brick);
    platform2->setPixmap(brickPix.scaled(150, 40));
    platform2->setPos(1000, brickY - 40);
    scene->addItem(platform2);

    // ----- One question block above player path -----
    QPixmap mysteryPix(":/Graphics/mystryblock.png");
    Block *question = new Block(Block::Question);
    question->setPixmap(mysteryPix.scaled(40, 40));
    question->setPos(1200, brickY - 80);
    scene->addItem(question);

    // ----- Enemies -----
    QPixmap enemyPix(":/Graphics/enemy.png");

    Enemy *e1 = new Enemy(500, 800);
    e1->setPixmap(enemyPix.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    e1->setPos(550, groundY - e1->pixmap().height());
    scene->addItem(e1);

    Enemy *e2 = new Enemy(1900, 2200);
    e2->setPixmap(enemyPix.scaled(40, 40));
    e2->setPos(1950, groundY - e2->pixmap().height());
    scene->addItem(e2);

    // ----- Coins -----
    QPixmap coinPix(":/Graphics/coin.png");

    for (int i = 0; i < 3; ++i) {
        Coin *c = new Coin();
        c->setPixmap(coinPix.scaled(32, 32, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        c->setPos(1400 + i * 50, groundY - 150 - i * 15);
        scene->addItem(c);
    }

    Coin *c2 = new Coin();
    c2->setPixmap(coinPix.scaled(32, 32));
    c2->setPos(1200, brickY - 130);
    scene->addItem(c2);

    // ----- Castle (end of level) -----
    QPixmap castlePix(":/Graphics/castle.png");
    Castle *castle = new Castle();
    castle->setPixmap(castlePix.scaled(160, 160, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    castle->setPos(2600, groundY - castle->pixmap().height());
    castle->setZValue(5);
    scene->addItem(castle);
}
void Game::gameOver()
{
    QGraphicsTextItem *msg = new QGraphicsTextItem("GAME OVER");
    msg->setDefaultTextColor(Qt::red);
    QFont f("Times", 32, QFont::Bold);
    msg->setFont(f);
    msg->setZValue(2000);
    msg->setPos(mapToScene(250, 150));
    scene->addItem(msg);
}

void Game::levelCompleted()
{
    QGraphicsTextItem *msg = new QGraphicsTextItem("LEVEL 1 COMPLETE!");
    msg->setDefaultTextColor(Qt::yellow);
    QFont f("Times", 28, QFont::Bold);
    msg->setFont(f);
    msg->setZValue(2000);
    msg->setPos(mapToScene(220, 150));
    scene->addItem(msg);
}
