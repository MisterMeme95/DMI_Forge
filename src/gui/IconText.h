//
// Created by jonat on 12/27/2024.
//
#include <QLabel>
#ifndef DMI_STRUCT_C_ICONTEXT_H
#define DMI_STRUCT_C_ICONTEXT_H

#endif //DMI_STRUCT_C_ICONTEXT_H

class IconText : public QLabel {
public:
    IconText(const QString &text = QString(), QWidget *parent = nullptr) : QLabel(text, parent){

    }

//    //   IconLabel(const QString &text = QString(), QWidget *parent = nullptr);
//    void keyPressEvent(QKeyEvent *ev) override{
//        printf("Lol. . \n");
//
//        QLabel::keyPressEvent(ev);
//    }
};