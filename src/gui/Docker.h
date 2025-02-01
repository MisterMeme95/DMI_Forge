//
// Created by jonat on 12/29/2024.
//
#include <QWidget>
#include <QSlider>
#include <QLabel>
#ifndef DMI_STRUCT_C_DOCKER_H
#define DMI_STRUCT_C_DOCKER_H
#include "IconLabel.h"
#include "IconText.h"


class Docker : public QWidget {


public:
    Docker();
    void Add_Icon();
    void resizeIcon(int value);
    QVBoxLayout* mainlayout;
    QVBoxLayout* new_layout;
    QSlider* slider;
    QLabel* frame_icon;
    QPixmap originalPixmap;

};

#endif //DMI_STRUCT_C_DOCKER_H
