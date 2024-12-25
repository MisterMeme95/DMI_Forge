#include "IconLabel.h"
#include <QMenu>
// Constructor implementation
IconLabel::IconLabel(const QString &text, QWidget *parent) : QLabel(text, parent), state_pointer(nullptr) {

    // Additional initialization for IconLabel, if necessary
}

void IconLabel::Run_Code(){
    create_png_from_icon_state(icon_pointer, "temporary.png", state_pointer);
}
void IconLabel::showContextMenu(const QPoint &pos) {
    if (currentMenu) {
        currentMenu->close();
        delete currentMenu;
    }
    currentMenu = new QMenu(this);
    QAction *action1 = currentMenu->addAction("Export to Sheet");
    connect(action1, &QAction::triggered, this, &IconLabel::Run_Code);
    currentMenu->exec(QCursor::pos());
    currentMenu = nullptr; // Reset after exec
}

