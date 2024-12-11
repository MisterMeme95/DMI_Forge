#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QMenuBar>
#include <QFileDialog>
#include <QPixmap>
#include <QScrollArea>
#include <QListWidget>
#include <QSplitter>
#include <QDir>
#include <QFileInfoList>

class ImageViewer : public QMainWindow {

public:
    ImageViewer() {
        // Set up QLabel for displaying the image
        imageLabel = new QLabel;
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);

        // Set up QScrollArea for handling large images
        scrollArea = new QScrollArea;
        scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setWidget(imageLabel);

        // Set up QListWidget as a sidebar for image files
        fileListWidget = new QListWidget;
        connect(fileListWidget, &QListWidget::itemClicked, this, &ImageViewer::loadImageFromList);

        // Set up a splitter to display sidebar and image viewer side-by-side
        QSplitter *splitter = new QSplitter;
        splitter->addWidget(fileListWidget);
        splitter->addWidget(scrollArea);
        setCentralWidget(splitter);

        // Set up the Open action
        QAction *openFolderAction = new QAction("&Select File...", this);
        connect(openFolderAction, &QAction::triggered, this, &ImageViewer::openFolder);

        // Set up the menu bar
        QMenu *fileMenu = menuBar()->addMenu("&File");
        fileMenu->addAction(openFolderAction);

        // Set up main window
        setWindowTitle("Image Viewer with Sidebar");
        resize(1000, 600);
    }

private slots:
    void openFolder() {
        //QString folderPath = QFileDialog::getExistingDirectory(this, "Open Folder", "");
        file_clicked = QFileDialog::getOpenFileName(this, "Select File", "");
        QPixmap pixmap(file_clicked);
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap);
            imageLabel->adjustSize();
        }
    }

    void loadImageFromList(QListWidgetItem *item) {
        QString filePath = item->data(Qt::UserRole).toString();
        QPixmap pixmap(file_clicked);
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap);
            imageLabel->adjustSize();
        }
    }

private:
    void loadImagesFromFolder(const QString &folderPath) {
        fileListWidget->clear(); // Clear any existing items
        QDir directory(R"(C:\Users\jonat\GitHub\DMI_Forge\cmake-build-debug\Old Kaimon OW)");
        QStringList filters;
        filters << "*.png" << "*.jpg" << "*.gif" << "*.dmi";
        QFileInfoList fileList = directory.entryInfoList(filters, QDir::Files);

        for (const QFileInfo &fileInfo : fileList) {
            QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName(), fileListWidget);
            item->setData(Qt::UserRole, fileInfo.absoluteFilePath()); // Store file path for loading
        }
    }

    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QListWidget *fileListWidget;
    QString file_clicked;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageViewer viewer;
    viewer.show();
    return app.exec();
}
