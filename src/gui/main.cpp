#include <QApplication>
#include "ImageViewer.h"

int main(int argc, char *argv[]) {
    qputenv("QT_ENABLE_HIGHDPI_SCALING", "0");
    QApplication app(argc, argv);
    ImageViewer viewer;

    viewer.show();
    return app.exec();
}
