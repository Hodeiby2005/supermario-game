#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPointF>

class Player;

class Game : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Game(QWidget *parent = nullptr);

    QGraphicsScene *scene;
    Player *player;

    // HUD / score
    int score;
    int lastScoreX;
    QGraphicsTextItem *scoreText;

    // Game state
    bool isGameOver;
    bool isLevelComplete;

    void addScore(int delta);
    void updateHUD();
    void centerHorizontallyOn(qreal x);
    void showScorePopup(int value, QPointF position);

    void setupLevel1();
    void setupLevel2();
    void setupLevel3();      // ⭐ NEW — Level 3 builder

    void restartLevel();
    void gameOver();
    void levelCompleted();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void createHud();
    void disableControls();
    void goToNextLevel();    // already existed, now used for level 3 also

    int currentLevel;        // 1, 2, or 3 ⭐ now supports three levels
};

// Global pointer
extern Game *game;

#endif // GAME_H


extern Game *game;

#endif // GAME_H

