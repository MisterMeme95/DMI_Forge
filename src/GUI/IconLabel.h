//
// Created by jonat on 12/19/2024.
//
#include <QLabel>
#ifndef DMI_H
#include "../../dmi.h"
#endif
class IconLabel : public QLabel {
public:
    // Constructor
    IconLabel(const QString &text = QString(), QWidget *parent = nullptr);
    void showContextMenu(const QPoint &pos);

    icon_state *state_pointer;
    DMI* icon_pointer;
    QMenu *currentMenu = nullptr;
public slots:
    void Run_Code();
};

