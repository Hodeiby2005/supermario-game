#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTimer>

class Player;

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);

    QGraphicsScene *scene;
    Player *player;

    int score;
    QGraphicsTextItem *scoreText;

    void addScore(int delta);
    void setupLevel1();
    void gameOver();
    void levelCompleted();

private:
    void createHud();
};

extern Game *game;

#endif // GAME_H
