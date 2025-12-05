#include "player.h"
#include "ground.h"
#include "block.h"
#include "enemy.h"
#include "castle.h"
#include "coin.h"
#include "pipe.h"
#include "verticalturtle.h"   // ⭐ NEW
#include "game.h"

#include <QKeyEvent>
#include <QGraphicsScene>
#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include <cmath>

extern Game *game;

Player::Player(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent),
    velocityY(0.0),
    velocityX(0.0),
    onGround(false),
    movingLeft(false),
    movingRight(false),
    facingRight(true),
    isMoving(false),
    lastPath("")
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Player::updatePlayer);
    timer->start(16);
}

void Player::stopMotion()
{
    velocityY = 0;
    velocityX = 0;
    movingLeft = false;
    movingRight = false;
    isMoving = false;
}

void Player::keyPressEvent(QKeyEvent *event)
{
    if (!game || game->isGameOver || game->isLevelComplete)
        return;

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Right) {
        movingRight = true;
    }
    else if (event->key() == Qt::Key_Left) {
        movingLeft = true;
    }
    else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up)
    {
        if (onGround) {
            velocityY = -17;
            onGround = false;
        }
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    if (!game || game->isGameOver || game->isLevelComplete)
        return;

    if (event->isAutoRepeat()) return;

    if (event->key() == Qt::Key_Right)
        movingRight = false;
    else if (event->key() == Qt::Key_Left)
        movingLeft = false;
}

void Player::updatePlayer()
{
    if (game && (game->isGameOver || game->isLevelComplete))
        return;

    const double MaxSpeed = 7.0;
    const double Acceleration = 1.2;
    const double Deceleration = 1.2;
    const double gravity = 1.1;

    if (movingRight) {
        velocityX += Acceleration;
        if (velocityX > MaxSpeed) velocityX = MaxSpeed;
        facingRight = true;
    } else if (movingLeft) {
        velocityX -= Acceleration;
        if (velocityX < -MaxSpeed) velocityX = -MaxSpeed;
        facingRight = false;
    } else {
        if (velocityX > 0) {
            velocityX -= Deceleration;
            if (velocityX < 0) velocityX = 0;
        } else if (velocityX < 0) {
            velocityX += Deceleration;
            if (velocityX > 0) velocityX = 0;
        }
    }

    if (std::abs(velocityX) < 0.5 && !movingLeft && !movingRight) {
        velocityX = 0.0;
    }

    isMoving = (velocityX != 0);

    // ---------- Horizontal movement ----------
    moveBy(velocityX, 0);

    QList<QGraphicsItem *> h_coll = collidingItems();
    for (auto item : h_coll) {
        Ground *g = dynamic_cast<Ground*>(item);
        Block *b  = dynamic_cast<Block*>(item);
        Pipe  *p  = dynamic_cast<Pipe*>(item);

        if (g || b || p) {
            moveBy(-velocityX, 0);
            velocityX = 0;
            break;
        }
    }

    // ---------- Gravity ----------
    velocityY += gravity;
    moveBy(0, velocityY);

    if (scene()) {
        const double sceneBottom = scene()->sceneRect().bottom();
        if (y() > sceneBottom + 100) {
            game->gameOver();
            return;
        }
    }

    // ============================================================
    //                COLLISION: ENEMY / TURTLE / CASTLE
    // ============================================================

    QList<QGraphicsItem *> coll = collidingItems();
    for (auto item : coll) {

        // ---------------- Goomba enemy ----------------
        Enemy *e = dynamic_cast<Enemy*>(item);
        if (e) {
            QRectF pRect = sceneBoundingRect();
            QRectF eRect = e->sceneBoundingRect();

            bool isFalling = (velocityY > 0);
            bool isStomping =
                isFalling &&
                pRect.bottom() >= eRect.top() &&
                pRect.bottom() <= eRect.top() + 30;

            if (isStomping)
            {
                e->die();
                setY(eRect.top() - boundingRect().height());
                velocityY = -12;
                onGround = false;

                if (game) {
                    game->addScore(5);
                    game->showScorePopup(5, e->pos());
                }
            } else {
                if (!e->getIsDead()) {
                    game->gameOver();
                    return;
                }
            }
        }

        // ---------------- Vertical Turtle (⭐ NEW) ----------------
        VerticalTurtle *vt = dynamic_cast<VerticalTurtle*>(item);
        if (vt)
        {
            QRectF pRect = sceneBoundingRect();
            QRectF tRect = vt->sceneBoundingRect();

            bool isFalling = (velocityY > 0);
            bool isStomping =
                isFalling &&
                pRect.bottom() >= tRect.top() &&
                pRect.bottom() <= tRect.top() + 30;

            if (isStomping && !vt->getIsDead())
            {
                vt->die();
                setY(tRect.top() - boundingRect().height());
                velocityY = -12;
                onGround = false;

                if (game) {
                    game->addScore(5);
                    game->showScorePopup(5, vt->pos());
                }
            }
            else {
                if (!vt->getIsDead()) {
                    game->gameOver();
                    return;
                }
            }
        }

        // ---------------- Castle ----------------
        if (dynamic_cast<Castle*>(item)) {
            game->levelCompleted();
            return;
        }
    }

    // ---------- Vertical collisions ----------
    handleVerticalCollisions();

    // ---------- Camera + HUD ----------
    if (game && scene()) {
        QPointF playerCenter = sceneBoundingRect().center();
        const double StableCameraY = 500.0;

        QPointF centerPoint = QPointF(std::round(playerCenter.x()),
                                      StableCameraY);

        game->centerOn(centerPoint);
        game->updateHUD();
        keepInBounds();
    }

    // ---------- Sprite / animation ----------
    QString path;
    if (isMoving) {
        if (facingRight) {
            path = ":/Graphics/supermario.png";
        } else {
            path = ":/Graphics/walking_left.png";
        }
    } else {
        path = ":/Graphics/Mario_Idle.png";
    }

    if (path != lastPath) {
        QPixmap newPix;
        newPix.load(path);
        setPixmap(newPix.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        lastPath = path;
    }
}

void Player::handleVerticalCollisions()
{
    QList<QGraphicsItem*> items = collidingItems();

    if (velocityY != 0) {
        onGround = false;
    }

    for (QGraphicsItem *item : items)
    {
        Ground *g = dynamic_cast<Ground*>(item);
        Block  *b = dynamic_cast<Block*>(item);
        Pipe   *p = dynamic_cast<Pipe*>(item);

        if (!g && !b && !p) continue;

        Block *block = dynamic_cast<Block*>(item);

        QGraphicsItem *platform = nullptr;
        if (g)      platform = g;
        else if (b) platform = b;
        else        platform = p;

        QRectF myRect   = sceneBoundingRect();
        QRectF platRect = platform->sceneBoundingRect();

        double myBottom   = myRect.bottom();
        double platTop    = platRect.top();
        double myTop      = myRect.top();
        double platBottom = platRect.bottom();

        // Landing
        if (velocityY >= 0 &&
            myBottom >= platTop &&
            myBottom <= platTop + 30)
        {
            setY(platTop - boundingRect().height());
            velocityY = 0;
            onGround  = true;
            return;
        }

        // Head bump
        if (velocityY < 0 &&
            myTop <= platBottom &&
            myTop >= platBottom - 30)
        {
            velocityY = 0;
            setY(platBottom + 1);

            if (block && block->getType() == Block::Question)
            {
                block->hit();
                if (game) {
                    game->addScore(10);
                    QPointF popupPos = QPointF(platRect.center().x(),
                                               platRect.top());
                    game->showScorePopup(10, popupPos);
                }
            }
            return;
        }
    }
}

void Player::keepInBounds()
{
    if (!scene()) return;

    if (x() < 0)
        setX(0);

    double maxX = scene()->sceneRect().right() - boundingRect().width();
    if (x() > maxX)
        setX(maxX);
}



