// main.cpp
#include <QApplication>
#include "game.h"
#include <QLoggingCategory>

// 🌟 FIX: The global 'game' object is defined here.
Game *game = nullptr;

int main(int argc, char *argv[])
{
    // Suppress the 'libpng warning: eXIf: invalid' messages
    QLoggingCategory::setFilterRules("qt.gui.imageio=false");

    QApplication a(argc, argv);

    // Now initialize the global pointer
    game = new Game();
    game->show();

    return a.exec();
}
