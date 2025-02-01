//
// Created by jonat on 12/19/2024.
//
#pragma once
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

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        QRect rect = contentsRect();

        // 1. Draw Checkerboard Background
        for (int y = 0; y < height(); y += squareSize) {
            for (int x = 0; x < width(); x += squareSize) {
                QColor color = ((x / squareSize + y / squareSize) % 2 == 0)
                               ? QColor(220, 220, 220)  // Light gray
                               : QColor(255, 255, 255); // White
                painter.fillRect(x, y, squareSize, squareSize, color);
            }
        }

        // 2. Draw the Pixmap (if set)
        QPixmap scaledPixmap;
        int xOffset = 0, yOffset = 0;

        if (!pixmap().isNull()) {
            scaledPixmap = pixmap().scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            xOffset = (rect.width() - pixmap().width()) / 2;
            yOffset = (rect.height() - pixmap().height()) / 2;

            // Draw the pixmap centered on the checkerboard
            painter.drawPixmap(xOffset, yOffset, pixmap());
        }

        // 3. Draw Border Over the Pixmap
        if (!scaledPixmap.isNull()) {
            QPen pen(Qt::darkGray, 1);  // Black outline
            painter.setPen(pen);

            // Draw border slightly inside the pixmap area
            painter.drawRect(0, 0, this->width()-1, this->height()-1);
        }

        // Call base QLabel paint event (optional)
       // QLabel::paintEvent(event);
    }

public slots:
    void Run_Code();
};



