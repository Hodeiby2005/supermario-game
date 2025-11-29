#include <QApplication>
#include "game.h"

Game *game = nullptr;   // global pointer

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    game = new Game();
    game->show();

    return a.exec();
}
