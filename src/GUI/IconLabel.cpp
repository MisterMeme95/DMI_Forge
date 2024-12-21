#include "IconLabel.h"
#include <QMenu>
// Constructor implementation
IconLabel::IconLabel(const QString &text, QWidget *parent) : QLabel(text, parent), state_pointer(nullptr) {

    // Additional initialization for IconLabel, if necessary
}

void IconLabel::Run_Code(){
//    DMI *dmi;
//    char** fake_list = (char**)malloc(sizeof(char*));
//    int lol = 1;
//    fake_list[0] = state_pointer->state;
//    export_as_sheet(icon_pointer, fake_list, lol);


    create_png_from_icon_state(icon_pointer, "temporary.png", state_pointer);
}
void IconLabel::showContextMenu(const QPoint &pos) {
    if (currentMenu) {
        currentMenu->close();
        delete currentMenu; // Clean up the old menu
    }

    currentMenu = new QMenu(this);

    QAction *action1 = currentMenu->addAction("Export to Sheet");

    connect(action1, &QAction::triggered, this, &IconLabel::Run_Code);
    currentMenu->exec(QCursor::pos());

    currentMenu = nullptr; // Reset after exec
}

