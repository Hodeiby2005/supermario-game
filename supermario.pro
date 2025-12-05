[cite_start]QT       += core gui animation [cite: 1]

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    block.cpp \
    coin.cpp \
    enemy.cpp \
    flag.cpp \
    ground.cpp \
    main.cpp \
    game.cpp \
    pipe.cpp \
    player.cpp \
    verticalturtle.cpp

HEADERS += \
    block.h \
    castle.h \
    coin.h \
    enemy.h \
    flag.h \
    game.h \
    ground.h \
    pipe.h \
    player.h \
    verticalturtle.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Graphics/Graphics.qrc

DISTFILES += \
    Graphics/BACK.jpg \
    Graphics/Background2.png \
    Graphics/Mario_Idle.png \
    Graphics/background.png \
    Graphics/brick.jpeg \
    Graphics/brick2.png \
    Graphics/castle.png \
    Graphics/castle2.png \
    Graphics/castle3.png \
    Graphics/coin-2.png \
    Graphics/coin.png \
    Graphics/enemy.png \
    Graphics/enemy2.png \
    Graphics/flag.png \
    Graphics/floor2.png \
    Graphics/ground.png \
    Graphics/mysterbox2.png \
    Graphics/mystryblock.png \
    Graphics/pipe-med.png \
    Graphics/pipe-med2.png \
    Graphics/piranha-0.png \
    Graphics/small_brick.png \
    Graphics/squashed_enemy.png \
    Graphics/supermario.png \
    Graphics/turtle-turtleback-0.png \
    Graphics/turtle-walk-0.png \
    Graphics/walking_left.png
