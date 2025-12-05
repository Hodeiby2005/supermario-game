// game.cpp
#include "game.h"
#include "player.h"
#include "ground.h"
#include "enemy.h"
#include "coin.h"
#include "block.h"
#include "castle.h"
#include "flag.h"
#include "pipe.h"
#include "verticalturtle.h"

#include <QKeyEvent>
#include <QDebug>
#include <QBrush>
#include <QPixmap>
#include <QFont>
#include <QTimer>
#include <cmath>

// Use the global game pointer defined in main.cpp
extern Game *game;

Game::Game(QWidget *parent)
    : QGraphicsView(parent),
    scene(new QGraphicsScene(this)),
    player(nullptr),
    score(0),
    lastScoreX(0),
    scoreText(nullptr),
    isGameOver(false),
    isLevelComplete(false),
    currentLevel(1)        // ⭐ Start at level 1
{
    // View size on screen
    setFixedSize(800, 450);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Default level 1 width
    scene->setSceneRect(0, 0, 8000, 450);
    setScene(scene);

    // Background for Level 1
    QPixmap bg(":/Graphics/background.png");
    if (bg.isNull()) {
        qDebug() << "BACKGROUND FAILED TO LOAD, using fallback";
        bg = QPixmap(800, 450);
        bg.fill(Qt::black);
    }
    setBackgroundBrush(QBrush(bg));

    createHud();
    setupLevel1();
}

void Game::centerHorizontallyOn(qreal x)
{
    if (isGameOver || isLevelComplete) return;

    QPointF currentCenter = mapToScene(width() / 2, height() / 2);
    qreal targetX = x;
    qreal targetY = currentCenter.y();
    centerOn(targetX, targetY);
}

void Game::createHud()
{
    score = 0;
    lastScoreX = 0;

    scoreText = new QGraphicsTextItem(QString("Score: %1").arg(score));
    QFont font("Times", 18);
    scoreText->setFont(font);
    scoreText->setDefaultTextColor(Qt::white);
    scoreText->setZValue(1000);
    scoreText->setPos(10, 10);
    scene->addItem(scoreText);
}

void Game::addScore(int delta)
{
    if (delta <= 0) return;
    score += delta;
    if (scoreText) {
        scoreText->setPlainText(QString("Score: %1").arg(score));
    }
}

void Game::updateHUD()
{
    if (!scoreText) return;
    QPointF topLeft = mapToScene(0, 0);
    scoreText->setPos(topLeft.x() + 10, topLeft.y() + 10);
}

void Game::showScorePopup(int value, QPointF position)
{
    QGraphicsTextItem *popup = new QGraphicsTextItem(QString("+%1").arg(value));
    popup->setDefaultTextColor(Qt::white);
    popup->setFont(QFont("Arial", 14, QFont::Bold));
    popup->setPos(position.x() + 5, position.y() - 20);
    popup->setZValue(50);
    scene->addItem(popup);

    QTimer::singleShot(500, popup, [popup]() {
        if (popup->scene()) {
            popup->scene()->removeItem(popup);
        }
        delete popup;
    });
}

// =====================================================
// ================ LEVEL 1 (UNCHANGED) ================
// =====================================================

void Game::setupLevel1()
{
    isGameOver = false;
    isLevelComplete = false;

    // ----- Ground tiles -----
    QPixmap rawGround(":/Graphics/ground.png");
    if (rawGround.isNull()) {
        qDebug() << "GROUND FAILED TO LOAD, using fallback";
        rawGround = QPixmap(256, 64);
        rawGround.fill(Qt::darkYellow);
    }

    QPixmap groundTile = rawGround.scaled(256, 64,
                                          Qt::IgnoreAspectRatio,
                                          Qt::SmoothTransformation);

    const int sceneHeight = static_cast<int>(scene->sceneRect().height());
    const int groundY     = sceneHeight - groundTile.height();
    const int tileWidth   = groundTile.width();
    const int numTiles    =
        static_cast<int>(scene->sceneRect().width()) / tileWidth + 3;

    // Two gaps (holes) in the ground
    int gap1Start = 10;
    int gap1End   = 12; // Gap starts at 10 * 256, ends at 13 * 256
    int gap2Start = 22;
    int gap2End   = 23; // Gap starts at 22 * 256, ends at 24 * 256

    for (int i = 0; i < numTiles; ++i) {
        if ((i >= gap1Start && i <= gap1End) ||
            (i >= gap2Start && i <= gap2End)) {
            continue; // skip tiles -> create gaps
        }

        Ground *g = new Ground();
        g->setPixmap(groundTile);
        g->setPos(i * tileWidth, groundY);
        g->setZValue(1);
        scene->addItem(g);
    }

    // ----- Player -----
    player = new Player();

    QPixmap mario(":/Graphics/Mario_Idle.png");
    if (mario.isNull()) {
        qDebug() << "MARIO IDLE FAILED TO LOAD, using fallback";
        mario = QPixmap(64, 64);
        mario.fill(Qt::red);
    }
    mario = mario.scaled(64, 64, Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);
    player->setPixmap(mario);

    player->setPos(80, groundY - player->boundingRect().height());
    scene->addItem(player);
    player->setZValue(10);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    // ----- Brick platforms & question blocks -----
    QPixmap brickPix(":/Graphics/brick.jpeg");
    if (brickPix.isNull()) {
        qDebug() << "BRICK FAILED TO LOAD";
        brickPix = QPixmap(150, 40);
        brickPix.fill(Qt::darkRed);
    }

    QPixmap questionPix(":/Graphics/mystryblock.png");
    if (questionPix.isNull()) {
        qDebug() << "QUESTION BLOCK FAILED TO LOAD";
        questionPix = QPixmap(40, 40);
        questionPix.fill(Qt::yellow);
    }

    int brickY1 = groundY - 120; // Lower platform height
    int brickY2 = groundY - 180; // Higher platform height

    const int platformWidth = 150;
    const int platformHeight = 40;
    const int platformSpacing = 50; // Gap between platforms

    // Existing platforms
    Block *p1 = new Block(Block::Brick);
    p1->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    p1->setPos(700, brickY1);
    scene->addItem(p1);

    Block *p2 = new Block(Block::Brick);
    p2->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    p2->setPos(1300, brickY2+100);
    scene->addItem(p2);

    Block *p3 = new Block(Block::Brick);
    p3->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    p3->setPos(2000, brickY1);
    scene->addItem(p3);

    Block *p4 = new Block(Block::Brick);
    p4->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    p4->setPos(2800, brickY2);
    scene->addItem(p4);

    // Question blocks
    Block *q1 = new Block(Block::Question);
    q1->setPixmap(questionPix.scaled(40, 40));
    q1->setPos(1000, brickY1 - 60);
    scene->addItem(q1);

    Block *q2 = new Block(Block::Question);
    q2->setPixmap(questionPix.scaled(40, 40));
    q2->setPos(2600, brickY2 - 60);
    scene->addItem(q2);

    // --- Mystery Box Support Platforms ---
    const int supportBrickY = brickY2 + 75;
    const int gap = 50;

    Block *supportBrick = new Block(Block::Brick);
    supportBrick->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    supportBrick->setPos(2550, supportBrickY);
    scene->addItem(supportBrick);

    Block *newRightBrick = new Block(Block::Brick);
    newRightBrick->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    newRightBrick->setPos(2550 + platformWidth + gap + 300, supportBrickY);
    scene->addItem(newRightBrick);

    // NEW CODE: 2 BLOCKS AFTER FIRST GAP
    int afterGap1X = (gap1End + 1) * tileWidth + 50;

    Block *a1 = new Block(Block::Brick);
    a1->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    a1->setPos(afterGap1X + 150 + 20, brickY2 + 40);
    scene->addItem(a1);

    Block *a2 = new Block(Block::Brick);
    a2->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    a2->setPos(afterGap1X + platformWidth + platformSpacing + 1000, brickY1);
    scene->addItem(a2);

    // NEW CODE: 2 BLOCKS AFTER SECOND GAP
    int afterGap2X = (gap2End + 1) * tileWidth + 50;

    Block *b1 = new Block(Block::Brick);
    b1->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    b1->setPos(afterGap2X + 400, brickY1);
    scene->addItem(b1);

    Block *b2 = new Block(Block::Brick);
    b2->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    b2->setPos(afterGap2X + platformWidth + platformSpacing + 500, brickY2 - 50);
    scene->addItem(b2);

    // EXISTING END-OF-LEVEL BRICKS
    Block *p5 = new Block(Block::Brick);
    p5->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    p5->setPos(5700, brickY1);
    scene->addItem(p5);

    Block *p6 = new Block(Block::Brick);
    p6->setPixmap(brickPix.scaled(platformWidth, platformHeight));
    p6->setPos(6100, brickY1+100);
    scene->addItem(p6);

    // ----- Enemies (5 total) -----
    QPixmap enemyPix(":/Graphics/enemy.png");
    if (enemyPix.isNull()) {
        qDebug() << "ENEMY FAILED TO LOAD";
        enemyPix = QPixmap(40, 40);
        enemyPix.fill(Qt::black);
    }

    auto spawnEnemy = [&](int x, int leftBound, int rightBound) {
        Enemy *e = new Enemy(leftBound, rightBound);
        e->setPixmap(enemyPix.scaled(40, 40,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
        e->setPos(x, groundY - e->pixmap().height());
        scene->addItem(e);
    };

    spawnEnemy(900, 850, 1050);
    spawnEnemy(1700, 1650, 1900);
    spawnEnemy(2400, 2350, 2600);
    spawnEnemy(3600, 3550, 3800);
    spawnEnemy(5200, 5150, 5350);

    // ----- Coins -----
    QPixmap coinPix(":/Graphics/coin.png");
    if (coinPix.isNull()) {
        qDebug() << "COIN FAILED TO LOAD";
        coinPix = QPixmap(32, 32);
        coinPix.fill(Qt::yellow);
    }

    auto spawnCoin = [&](int x, int y) {
        Coin *c = new Coin();
        c->setPixmap(coinPix.scaled(32, 32,
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
        c->setPos(x, y);
        scene->addItem(c);
    };

    // row of coins over first gap
    spawnCoin(gap1Start * tileWidth + 50, groundY - 150);
    spawnCoin(gap1Start * tileWidth + 100, groundY - 170);
    spawnCoin(gap1Start * tileWidth + 150, groundY - 150);

    // coins above platforms
    spawnCoin(720, brickY1 - 60);
    spawnCoin(1320, brickY2 - 60);
    spawnCoin(2020, brickY1 - 60);
    spawnCoin(2820, brickY2 - 60);

    // some coins on the way
    spawnCoin(4000, groundY - 120);
    spawnCoin(4500, groundY - 120);
    spawnCoin(5000, groundY - 120);

    // End position for the castle and flag
    const int endX = 7500;

    // ----- Castle -----
    QPixmap castlePix(":/Graphics/castle.png");
    if (castlePix.isNull()) {
        qDebug() << "CASTLE FAILED TO LOAD";
        castlePix = QPixmap(160, 160);
        castlePix.fill(Qt::gray);
    }

    Castle *castle = new Castle();
    castle->setPixmap(castlePix.scaled(400, 400,
                                       Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation));
    castle->setPos(endX, groundY - castle->pixmap().height() + groundTile.height() - 10);
    castle->setZValue(5);
    scene->addItem(castle);

    // 🚩 Flag beside the Castle
    const int flagHeight = 370;
    Flag *f = new Flag(endX - 50, groundY - flagHeight + 35);
    f->setZValue(6);
    scene->addItem(f);
}

// =====================================================
// =================== LEVEL 2 (NEW) ===================
// =====================================================

void Game::setupLevel2()
{
    isGameOver = false;
    isLevelComplete = false;

    // Make level 2 a bit longer than level 1
    scene->setSceneRect(0, 0, 10000, 450);

    // ----- Ground tiles using floor2.png -----
    QPixmap rawGround(":/Graphics/floor2.png");
    if (rawGround.isNull()) {
        qDebug() << "FLOOR2 FAILED TO LOAD, using fallback";
        rawGround = QPixmap(256, 64);
        rawGround.fill(Qt::darkCyan);
    }

    QPixmap groundTile = rawGround.scaled(256, 64,
                                          Qt::IgnoreAspectRatio,
                                          Qt::SmoothTransformation);

    const int sceneHeight = static_cast<int>(scene->sceneRect().height());
    const int groundY     = sceneHeight - groundTile.height();
    const int tileWidth   = groundTile.width();
    const int numTiles    =
        static_cast<int>(scene->sceneRect().width()) / tileWidth + 3;

    // MORE GAPS in level 2
    struct Gap { int start; int end; };
    QVector<Gap> gaps = {
        {8,  9},
        {16, 18},
        {25, 27},
        {33, 35}
    };

    auto isInGap = [&](int tileIndex) {
        for (const Gap &g : gaps) {
            if (tileIndex >= g.start && tileIndex <= g.end)
                return true;
        }
        return false;
    };

    for (int i = 0; i < numTiles; ++i) {
        if (isInGap(i)) continue;

        Ground *g = new Ground();
        g->setPixmap(groundTile);
        g->setPos(i * tileWidth, groundY);
        g->setZValue(1);
        scene->addItem(g);
    }

    // ----- Player (same Mario) -----
    player = new Player();

    QPixmap mario(":/Graphics/Mario_Idle.png");
    if (mario.isNull()) {
        qDebug() << "MARIO IDLE FAILED TO LOAD, using fallback";
        mario = QPixmap(64, 64);
        mario.fill(Qt::red);
    }
    mario = mario.scaled(64, 64, Qt::IgnoreAspectRatio,
                         Qt::SmoothTransformation);
    player->setPixmap(mario);

    player->setPos(80, groundY - player->boundingRect().height());
    scene->addItem(player);
    player->setZValue(10);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    // ----- New Brick & Question block graphics -----
    QPixmap brickPix(":/Graphics/brick2.png");
    if (brickPix.isNull()) {
        qDebug() << "BRICK2 FAILED TO LOAD";
        brickPix = QPixmap(150, 40);
        brickPix.fill(Qt::gray);
    }

    QPixmap questionPix(":/Graphics/mysterybox2.png");
    if (questionPix.isNull()) {
        qDebug() << "MYSTERYBOX2 FAILED TO LOAD";
        questionPix = QPixmap(40, 40);
        questionPix.fill(Qt::yellow);
    }

    int brickY1 = groundY - 120;
    int brickY2 = groundY - 200;
    int brickY3 = groundY - 260;

    const int platformWidth  = 150;
    const int platformHeight = 40;

    // Platforms of different heights
    auto spawnBrick = [&](int x, int y) {
        Block *b = new Block(Block::Brick);
        b->setPixmap(brickPix.scaled(platformWidth, platformHeight));
        b->setPos(x, y);
        scene->addItem(b);
    };

    auto spawnQuestion = [&](int x, int y) {
        Block *q = new Block(Block::Question);
        q->setPixmap(questionPix.scaled(40, 40));
        q->setPos(x, y);
        scene->addItem(q);
    };

    // Some brick patterns:
    spawnBrick(600,  brickY1);
    spawnBrick(900,  brickY2);
    spawnBrick(1300, brickY1);
    spawnBrick(1650-85, brickY3+50);
    spawnBrick(2100-30, brickY2);
    spawnBrick(2600+150, brickY1);
    spawnBrick(3100-40, brickY2+90-40);
    spawnBrick(3600-80+900, brickY3+100);
    spawnBrick(6500,300);
    spawnBrick(8600,300);
    spawnBrick(8830,220);

    // Question blocks near / above bricks
    spawnQuestion(650,  brickY1 - 160);
    spawnQuestion(950,  brickY2 - 160);
    spawnQuestion(1350, brickY1 - 160);
    spawnQuestion(2150, brickY2 - 160);
    spawnQuestion(3150, brickY2 - 160);
    spawnQuestion(3650-200, brickY3 - 80);

    // ----- Pipes (NEW CLASS) of different heights -----
    QPixmap pipePix(":/Graphics/pipe2.png");
    if (pipePix.isNull()) {
        qDebug() << "PIPE2 FAILED TO LOAD";
        pipePix = QPixmap(64, 128);
        pipePix.fill(Qt::green);
    }

    auto spawnPipe = [&](int x, int height) {
        Pipe *p = new Pipe();
        p->setPixmap(pipePix.scaled(80, height,
                                    Qt::IgnoreAspectRatio,
                                    Qt::SmoothTransformation));
        p->setPos(x, groundY - height);
        p->setZValue(2);
        scene->addItem(p);
    };

    // Different pipe heights
    spawnPipe(2000-40, 100);
    spawnPipe(2300, 140);
    spawnPipe(2600-50, 180);
    spawnPipe(3200+60, 120);
    spawnPipe(4200-30,60);
    spawnPipe(6800, 100);

    // ----- Enemies (using enemy2, with double-consecutive pairs) -----
    QPixmap enemyPix(":/Graphics/enemy2.png");
    if (enemyPix.isNull()) {
        qDebug() << "ENEMY2 FAILED TO LOAD";
        enemyPix = QPixmap(40, 40);
        enemyPix.fill(Qt::black);
    }

    auto spawnEnemy = [&](int x, int leftBound, int rightBound) {
        Enemy *e = new Enemy(leftBound, rightBound);
        e->setPixmap(enemyPix.scaled(40, 40,
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation));
        e->setPos(x, groundY - e->pixmap().height());
        scene->addItem(e);
    };

    // ⭐ Double consecutive enemies
    auto spawnEnemyPair = [&](int leftBound, int rightBound) {
        Enemy *e1 = new Enemy(leftBound, rightBound);
        Enemy *e2 = new Enemy(leftBound, rightBound);

        QPixmap scaled = enemyPix.scaled(40, 40,
                                         Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation);
        e1->setPixmap(scaled);
        e2->setPixmap(scaled);

        e1->setPos(leftBound + 20, groundY - scaled.height());
        e2->setPos(leftBound + 70, groundY - scaled.height());

        scene->addItem(e1);
        scene->addItem(e2);
    };

    // Single enemies
    spawnEnemy(1100, 1050, 1250);
    spawnEnemy(1850, 1800, 2050);
    spawnEnemy(3500, 3450, 3650);
    spawnEnemy(4800, 4750, 4950);

    // Double pairs placed after gaps / pipes
    spawnEnemyPair(2600, 2850);
    spawnEnemyPair(3400, 3650);
    spawnEnemyPair(5600, 5850);
    spawnEnemyPair(7000, 7250);

    // ----- Coins with coin2.png -----
    QPixmap coinPix(":/Graphics/coin2.png");
    if (coinPix.isNull()) {
        qDebug() << "COIN2 FAILED TO LOAD";
        coinPix = QPixmap(32, 32);
        coinPix.fill(Qt::yellow);
    }

    auto spawnCoin = [&](int x, int y) {
        Coin *c = new Coin();
        c->setPixmap(coinPix.scaled(32, 32,
                                    Qt::KeepAspectRatio,
                                    Qt::SmoothTransformation));
        c->setPos(x, y);
        scene->addItem(c);
    };

    // Coins over first two gaps
    spawnCoin(8  * tileWidth + 40, groundY - 150-70);
    spawnCoin(8  * tileWidth + 80, groundY - 170-70);
    spawnCoin(9  * tileWidth + 40, groundY - 150-70);

    spawnCoin(16 * tileWidth + 40, groundY - 150);
    spawnCoin(17 * tileWidth + 80, groundY - 170);
    spawnCoin(18 * tileWidth + 40, groundY - 150);

    // Coins above bricks / pipes
    spawnCoin(600,  brickY1 - 60);
    spawnCoin(900,  brickY2 - 60);
    spawnCoin(1300, brickY1 - 60);
    spawnCoin(2100, brickY2 - 60);
    spawnCoin(2600, brickY1 - 80);
    spawnCoin(3200, brickY2 - 80);
    spawnCoin(4200, groundY - 200);
    spawnCoin(5200, groundY - 230);

    // Extra line of coins near the end
    spawnCoin(8000, groundY - 150);
    spawnCoin(8100, groundY - 150);
    spawnCoin(8200, groundY - 150);
    spawnCoin(8300, groundY - 150);

    // ----- Castle2 and Flag at the end -----
    const int endX = 9400;

    QPixmap castlePix(":/Graphics/castle2.png");
    if (castlePix.isNull()) {
        qDebug() << "CASTLE2 FAILED TO LOAD";
        castlePix = QPixmap(160, 160);
        castlePix.fill(Qt::gray);
    }

    Castle *castle = new Castle();
    castle->setPixmap(castlePix.scaled(200, 200,
                                       Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation));
    castle->setPos(endX, groundY - castle->pixmap().height() + groundTile.height()-63);
    castle->setZValue(5);
    scene->addItem(castle);

    const int flagHeight = 370;
    Flag *f = new Flag(endX - 60, groundY - flagHeight + 35);
    f->setZValue(6);
    scene->addItem(f);
}
void Game::setupLevel3()
{
    isGameOver = false;
    isLevelComplete = false;

    // World size
    scene->setSceneRect(0, 0, 10000, 450);

    // ----- Ground -----
    QPixmap rawGround(":/Graphics/ground3.png.");
    QPixmap groundTile = rawGround.scaled(256, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    const int sceneHeight = scene->sceneRect().height();
    const int groundY     = sceneHeight - groundTile.height();
    const int tileWidth   = groundTile.width();
    const int numTiles    = 10000 / tileWidth + 3;

    // Gaps for level 3
    struct Gap { int start; int end; };
    QVector<Gap> gaps = {
        {8, 10},
        {19, 21},
        {28, 29},
        {35, 36}
    };

    auto isGap = [&](int i){
        for (auto &g : gaps) if (i >= g.start && i <= g.end) return true;
        return false;
    };

    for (int i = 0; i < numTiles; i++) {
        if (isGap(i)) continue;

        Ground *g = new Ground();
        g->setPixmap(groundTile);
        g->setPos(i * tileWidth, groundY);
        scene->addItem(g);
    }

    // ----- Player -----
    player = new Player();
    QPixmap mario(":/Graphics/Mario_Idle.png");
    mario = mario.scaled(64, 64);
    player->setPixmap(mario);
    player->setPos(80, groundY - 64);
    player->setZValue(10);
    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
    scene->addItem(player);

    // ----- Brick -----
    QPixmap brickPix(":/Graphics/brick3.png.");

    auto spawnBrick = [&](int x, int y){
        Block *b = new Block(Block::Brick);
        b->setPixmap(brickPix.scaled(150, 40));
        b->setPos(x, y);
        scene->addItem(b);
    };

    spawnBrick(600,  groundY - 150+20);
    spawnBrick(2250-100,  groundY - 200+120-20+40);
    spawnBrick(1500-60-30+100, groundY+100 - 180+30+50-20-100);
    spawnBrick(2500-60-70, groundY - 210+30-30+70);
    spawnBrick(3800, groundY - 180+60);
     spawnBrick(6570, groundY - 165+40);
    spawnBrick(7300, groundY - 200);
    spawnBrick(7550, groundY - 165);
    spawnBrick(7800-30, groundY - 230);
    spawnBrick(8200, groundY - 150);
    spawnBrick(9250, groundY - 150+50);
    // ----- Mystery Boxes (7) -----
    QPixmap qPix(":/Graphics/question.png.");

    auto spawnQ = [&](int x, int y){
        Block *q = new Block(Block::Question);
        q->setPixmap(qPix.scaled(40, 40));
        q->setPos(x, y);
        scene->addItem(q);
    };

    spawnQ(700-50+100,  groundY - 240+30-30-40);
    spawnQ(1400+80+600-250-30-100, groundY - 260+20-60-100+100);
    spawnQ(2000-250+50+250+100, groundY - 240+60-100+100-30);
    spawnQ(3000, groundY - 260+150-80);
    spawnQ(3500, groundY - 240+30);
    spawnQ(5000, groundY - 260+150-120-200);
    spawnQ(8200-200, groundY - 240-300);

    // ----- Pipes (4–5) -----
    QPixmap pipePix(":/Graphics/pipeGold.png.");

    auto spawnPipe = [&](int x, int h){
        Pipe *p = new Pipe();
        p->setPixmap(pipePix.scaled(80, h));
        p->setPos(x, groundY - h);
        scene->addItem(p);
    };

    spawnPipe(4700+150, 140);
    spawnPipe(5000+50, 160);
    spawnPipe(5500-150, 120);
    spawnPipe(9000, 180-80);
    spawnPipe(7200, 140);

    // ----- Vertical Turtles (4) -----
    QPixmap vtPix(":/Graphics/koopa.png.");

    auto spawnVT = [&](int x, int top, int bottom){
        VerticalTurtle *t = new VerticalTurtle(top, bottom);
        t->setPixmap(vtPix.scaled(40, 50));
        t->setPos(x, top);
        scene->addItem(t);
    };

    spawnVT(1200+600,  groundY - 200, groundY - 50+100);
    spawnVT(2150+250+200-120-140-20,  groundY - 250-30, groundY - 80+50);
    spawnVT(4500,  groundY - 220, groundY - 60);
    spawnVT(7000,  groundY - 240, groundY - 70);

    // ----- Double Enemy Pairs (4) -----
    QPixmap enPix(":/Graphics/goomba.png");

    auto spawnPair = [&](int L, int R){
        Enemy *e1 = new Enemy(L, R);
        Enemy *e2 = new Enemy(L, R);

        QPixmap p = enPix.scaled(40, 40);

        e1->setPixmap(p);
        e2->setPixmap(p);

        e1->setPos(L + 30, groundY - 40);
        e2->setPos(L + 90, groundY - 40);

        scene->addItem(e1);
        scene->addItem(e2);
    };

    spawnPair(850, 1300);
    spawnPair(2800, 3000);
    spawnPair(3100, 3400);
    spawnPair(4000, 4700);
    spawnPair(5200+300+300, 6100);
    spawnPair(8600, 8900);

    // ----- Castle + Flag -----
    QPixmap castlePix(":/Graphics/castle3.png.");

    Castle *c = new Castle();
    c->setPixmap(castlePix.scaled(350, 350));
    c->setPos(9600, groundY - 350);
    scene->addItem(c);

    Flag *f = new Flag(9500, groundY - 370+60);
    scene->addItem(f);
}


// =====================================================

void Game::disableControls()
{
    if (player)
        player->setEnabled(false);
}

void Game::gameOver()
{
    if (isGameOver || isLevelComplete) return;
    isGameOver = true;
    disableControls();

    if (player) {
        player->stopMotion();
        player->clearFocus();
    }

    QGraphicsTextItem *msg = new QGraphicsTextItem("GAME OVER");
    msg->setDefaultTextColor(Qt::red);
    QFont f("Times", 32, QFont::Bold);
    msg->setFont(f);
    msg->setZValue(2000);
    QPointF center = mapToScene(width() / 2, height() / 2);
    msg->setPos(center.x() - msg->boundingRect().width() / 2, center.y() - 50);
    scene->addItem(msg);

    QGraphicsTextItem *retry = new QGraphicsTextItem("Press R to Restart");
    retry->setDefaultTextColor(Qt::white);
    retry->setFont(QFont("Arial", 16));
    retry->setZValue(2001);
    retry->setPos(center.x() - retry->boundingRect().width() / 2, center.y() + 10);
    scene->addItem(retry);
}

void Game::levelCompleted()
{
    if (isLevelComplete || isGameOver) return;
    isLevelComplete = true;

    if (player) {
        player->stopMotion();
        player->clearFocus();
    }

    QPointF center = mapToScene(width() / 2, height() / 2);

    // ================================
    // LEVEL 1 COMPLETE → GO TO LEVEL 2
    // ================================
    if (currentLevel == 1) {

        QGraphicsTextItem *msg = new QGraphicsTextItem("LEVEL 1 COMPLETE!");
        msg->setDefaultTextColor(Qt::yellow);
        msg->setFont(QFont("Times", 28, QFont::Bold));
        msg->setZValue(2000);
        msg->setPos(center.x() - msg->boundingRect().width()/2, center.y() - 50);
        scene->addItem(msg);

        QGraphicsTextItem *info = new QGraphicsTextItem("Get ready for LEVEL 2...");
        info->setDefaultTextColor(Qt::white);
        info->setFont(QFont("Arial", 16));
        info->setZValue(2001);
        info->setPos(center.x() - info->boundingRect().width()/2, center.y() + 10);
        scene->addItem(info);

        QTimer::singleShot(2000, this, [this]() {
            goToNextLevel();
        });
    }

    // ================================
    // LEVEL 2 COMPLETE → GO TO LEVEL 3
    // ================================
    else if (currentLevel == 2) {

        QGraphicsTextItem *msg = new QGraphicsTextItem("LEVEL 2 COMPLETE!");
        msg->setDefaultTextColor(Qt::yellow);
        msg->setFont(QFont("Times", 28, QFont::Bold));
        msg->setZValue(2000);
        msg->setPos(center.x() - msg->boundingRect().width()/2, center.y() - 50);
        scene->addItem(msg);

        QGraphicsTextItem *info = new QGraphicsTextItem("Get ready for LEVEL 3...");
        info->setDefaultTextColor(Qt::white);
        info->setFont(QFont("Arial", 16));
        info->setZValue(2001);
        info->setPos(center.x() - info->boundingRect().width()/2, center.y() + 10);
        scene->addItem(info);

        QTimer::singleShot(2000, this, [this]() {
            goToNextLevel();
        });
    }

    // ================================
    // LEVEL 3 COMPLETE → FINAL MESSAGE
    // ================================
    else if (currentLevel == 3) {

        QGraphicsTextItem *msg = new QGraphicsTextItem("LEVEL 3 COMPLETE!");
        msg->setDefaultTextColor(Qt::green);
        msg->setFont(QFont("Times", 30, QFont::Bold));
        msg->setZValue(2000);
        msg->setPos(center.x() - msg->boundingRect().width()/2, center.y() - 50);
        scene->addItem(msg);

        QGraphicsTextItem *fin = new QGraphicsTextItem("YOU WIN! Press R to Restart");
        fin->setDefaultTextColor(Qt::white);
        fin->setFont(QFont("Arial", 18));
        fin->setZValue(2001);
        fin->setPos(center.x() - fin->boundingRect().width()/2, center.y() + 10);
        scene->addItem(fin);
    }
}

void Game::goToNextLevel()
{
    if (currentLevel == 1) {
        // go to level 2
        scene->clear();
        scoreText = nullptr;
        score = 0;
        lastScoreX = 0;
        isGameOver = false;
        isLevelComplete = false;

        QPixmap bg2(":/Graphics/Background2.png");
        if (bg2.isNull()) {
            bg2 = QPixmap(800, 450);
            bg2.fill(Qt::black);
        }
        setBackgroundBrush(QBrush(bg2));

        createHud();
        currentLevel = 2;
        setupLevel2();
    }
    else if (currentLevel == 2) {
        // go to level 3
        scene->clear();
        scoreText = nullptr;
        score = 0;
        lastScoreX = 0;
        isGameOver = false;
        isLevelComplete = false;

        QPixmap bg3(":/Graphics/background3.png");
        if (bg3.isNull()) {
            bg3 = QPixmap(800, 450);
            bg3.fill(Qt::black);
        }
        setBackgroundBrush(QBrush(bg3));

        createHud();
        currentLevel = 3;
        setupLevel3();
    }
}


void Game::restartLevel()
{
    scene->clear();
    scoreText   = nullptr;
    score       = 0;
    lastScoreX  = 0;
    isGameOver  = false;
    isLevelComplete = false;

    // Reset background depending on level
    if (currentLevel == 1) {
        QPixmap bg(":/Graphics/background.png");
        if (bg.isNull()) {
            qDebug() << "BACKGROUND FAILED TO LOAD, using fallback";
            bg = QPixmap(800, 450);
            bg.fill(Qt::black);
        }
        setBackgroundBrush(QBrush(bg));
    }
    else if (currentLevel == 2) {
        QPixmap bg2(":/Graphics/Background2.png");
        if (bg2.isNull()) {
            qDebug() << "BACKGROUND2 FAILED TO LOAD, using fallback";
            bg2 = QPixmap(800, 450);
            bg2.fill(Qt::black);
        }
        setBackgroundBrush(QBrush(bg2));
    }
    else if (currentLevel == 3) {
        QPixmap bg3(":/Graphics/background3.png");   // <- same as Level 1 / Level 3
        if (bg3.isNull()) {
            qDebug() << "BACKGROUND3 FAILED TO LOAD, using fallback";
            bg3 = QPixmap(800, 450);
            bg3.fill(Qt::black);
        }
        setBackgroundBrush(QBrush(bg3));
    }

    createHud();

    if (currentLevel == 1) {
        setupLevel1();
    } else if (currentLevel == 2) {
        setupLevel2();
    } else if (currentLevel == 3) {
        setupLevel3();
    }
}



void Game::keyPressEvent(QKeyEvent *event)
{
    if ((isGameOver || isLevelComplete)) {
        if (event->key() == Qt::Key_R) {
            restartLevel();
        }
        return;
    }

    QGraphicsView::keyPressEvent(event);
}
