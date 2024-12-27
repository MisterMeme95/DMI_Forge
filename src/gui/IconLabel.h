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
        QRect rect = contentsRect();

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
            QPixmap scaledPixmap = pixmap().scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            int xOffset = (rect.width() - scaledPixmap.width()) / 2;
            int yOffset = (rect.height() - scaledPixmap.height()) / 2;
            // Draw black border around the pixmap
            painter.setPen(QPen(Qt::black, 1));  // Black outline with 1px thickness
            painter.drawRect(xOffset, yOffset, width()-1, height()-1);
        }
//        void paintEvent(QPaintEvent *event) override {
//                QPainter painter(this);
//                QRect rect = contentsRect();
//
//                // 1. Draw Checkerboard Background
//                for (int y = 0; y < height(); y += squareSize) {
//                    for (int x = 0; x < width(); x += squareSize) {
//                        QColor color = ((x / squareSize + y / squareSize) % 2 == 0)
//                                       ? QColor(220, 220, 220)  // Light gray
//                                       : QColor(255, 255, 255); // White
//                        painter.fillRect(x, y, squareSize, squareSize, color);
//                    }
//                }
//
//                // 2. Draw the pixmap centered
//                if (!originalPixmap.isNull()) {
//                    QPixmap scaledPixmap = originalPixmap.scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//                    int xOffset = (rect.width() - scaledPixmap.width()) / 2;
//                    int yOffset = (rect.height() - scaledPixmap.height()) / 2;
//
//                    // Draw pixmap manually
//                    painter.drawPixmap(xOffset, yOffset, scaledPixmap);
//
//                    // 3. Apply highlight OVER the pixmap if selected
//                    if (isHighlighted) {
//                        QColor overlayColor(0, 120, 255, 100);  // Semi-transparent blue
//                        painter.fillRect(xOffset, yOffset, scaledPixmap.width(), scaledPixmap.height(), overlayColor);
//
//                        // 4. Draw dashed border around the pixmap
//                        QPen pen(QColor(0, 0, 0), 1, Qt::DashLine);  // Black dashed border
//                        painter.setPen(pen);
//                        painter.drawRect(xOffset, yOffset, scaledPixmap.width() - 1, scaledPixmap.height() - 1);
//                    }
//                }
//
//                // 5. Draw QLabel text (if necessary)
//                QLabel::paintEvent(event);
//        }

        // Call base class paintEvent to ensure QLabel text/behavior is respected
        QLabel::paintEvent(event);
    }
public slots:
    void Run_Code();
};



