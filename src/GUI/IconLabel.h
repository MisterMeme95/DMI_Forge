//
// Created by jonat on 12/19/2024.
//
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#ifndef DMI_H
#include "../DMI/dmi.h"
#endif
class IconLabel : public QLabel {
public:
    // Constructor
    IconLabel(const QString &text = QString(), QWidget *parent = nullptr);
    void showContextMenu(const QPoint &pos);

    icon_state *state_pointer;
    DMI* icon_pointer;
    QMenu *currentMenu = nullptr;


    QPixmap backgroundPixmap;
    int squareSize = 10;  // Checkerboard square size (in pixels)

    // Draw the checkerboard behind the pixmap
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);

        // Draw Checkerboard Background
        for (int y = 0; y < height(); y += squareSize) {
            for (int x = 0; x < width(); x += squareSize) {
                QColor color = ((x / squareSize + y / squareSize) % 2 == 0)
                               ? QColor(220, 220, 220)  // Light gray
                               : QColor(255, 255, 255); // White
                painter.fillRect(x, y, squareSize, squareSize, color);
            }
        }

        // Draw the pixmap (if set) with black outline
        if (!pixmap().isNull()) {

            // Draw black border around the pixmap
            painter.setPen(QPen(Qt::black, 1));  // Black outline with 1px thickness
            painter.drawRect(1, 1, width()-1, height()-1);
        }

        // Call base class paintEvent to ensure QLabel text/behavior is respected
        QLabel::paintEvent(event);
    }
public slots:
    void Run_Code();
};

