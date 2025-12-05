// flag.cpp
#include "flag.h"

#include <QPixmap>
#include <QDebug>

Flag::Flag(qreal x, qreal y, QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent)
{
    // Load the flag image (flag.png)
    QPixmap flagPix(":/Graphics/flag.png");

    if (flagPix.isNull()) {
        qDebug() << "ERROR: Failed to load flag.png sprite!";
        // Fallback: Use a simple colored square if the image is missing
        QPixmap fallback(64, 64);
        fallback.fill(Qt::white);
        setPixmap(fallback);
    } else {
        // Set the pixmap and scale it to a standard size (e.g., 64x64)
        setPixmap(flagPix.scaled(200, 370, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }

    // Set the position of the flag
    setPos(x, y);
}
