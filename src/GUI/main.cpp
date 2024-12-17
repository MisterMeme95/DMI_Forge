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
#include <QGridLayout>
#include <QPainter>
#include "../../dmi.h"

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
        scrollArea->setBaseSize(200, 600);
        // Set up QListWidget as a sidebar for image files
        fileListWidget = new QListWidget;
        connect(fileListWidget, &QListWidget::itemClicked, this, &ImageViewer::loadImageFromList);

        // Set up a splitter to display sidebar and image viewer side-by-side
        QSplitter *splitter = new QSplitter;
        splitter->addWidget(fileListWidget);
        splitter->addWidget(scrollArea);
        setCentralWidget(splitter);
        splitter->setSizes({200, 600});

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
        QString folderPath = QFileDialog::getExistingDirectory(this, "Open Folder", "");
        if (!folderPath.isEmpty()) {
            loadImagesFromFolder(folderPath);
        }
//        file_clicked = QFileDialog::getOpenFileName(this, "Select File", "");
//        QPixmap pixmap(file_clicked);
//        if (!pixmap.isNull()) {
//            imageLabel->setPixmap(pixmap);
//            imageLabel->adjustSize();
//        }
    }

    void loadImageFromList(QListWidgetItem *item) {
        QString filePath = item->data(Qt::UserRole).toString();

        // Check if the file has a .dmi extension
        if (filePath.endsWith(".dmi", Qt::CaseInsensitive)) {
            qDebug() << "The file is a .dmi file.";

            // Convert file path to a C-style string
            char *cFilePath = (char *)malloc(filePath.toUtf8().size() + 1);
            strcpy(cFilePath, filePath.toUtf8().data());

            // Initialize DMI struct
            DMI new_icon;
            initialize_dmi_struct(&new_icon, cFilePath);

            // Create a large QPixmap canvas to merge all the DMI images
            int rows = (new_icon.iconStates.size + 4) / 5; // 5 images per row
            int cols = 5; // 5 columns
            int imageWidth = new_icon.icon_width;
            int imageHeight = new_icon.icon_height;

            QPixmap combinedPixmap(cols * imageWidth * 2, rows * imageHeight * 2);
            combinedPixmap.fill(Qt::transparent); // Start with transparent background
            QPainter painter(&combinedPixmap);

            // Traverse iconStates and draw images
            node *currentNode = new_icon.iconStates.head;
            int row = 0, col = 0;

            while (currentNode != nullptr) {
                auto *currentState = (icon_state *)currentNode->data;

                QImage temp_image;
                auto *pixel_data = (png_bytepp *)currentState->frame_vector[0].data;
                png_bytepp pixel_data2;
                QVector<QRgb> colorTable;

                switch (new_icon.image->color_type) {
                    case PNG_COLOR_TYPE_PALETTE: {
                        // Initialize QImage with Indexed8 format
                        temp_image = QImage(imageWidth, imageHeight, QImage::Format_Indexed8);

                        // Copy pixel data row by row
                        pixel_data2 = pixel_data[0];
                        for (int i = 0; i < new_icon.icon_height; i++) {
                            memcpy(temp_image.scanLine(i), pixel_data2[i], new_icon.icon_row_bytes);
                        }

                        // Build color table
                        for (int c = 0; c < new_icon.image->palette_num; ++c) {
                            png_color color = new_icon.image->palette[c];
                            colorTable.push_back(qRgb(color.red, color.green, color.blue));
                        }
                        temp_image.setColorTable(colorTable);

                        break;
                    }
                    default:
                        qWarning() << "Unsupported color type!";
                        break;
                }

                // Paint the individual image into the correct grid position
                QPixmap pixmap = QPixmap::fromImage(temp_image);
                QRect targetRect(col * imageWidth, row * imageHeight, imageWidth, imageHeight);
                painter.drawPixmap(targetRect, pixmap);

                QString name = QString::fromUtf8(currentState->state); // Assuming state is a C-string (char*)
                QRect textRect(col * imageWidth, (row + 1) * imageHeight - 5, imageWidth*2, 20);
                painter.drawText(textRect, Qt::AlignCenter, name);
                // Update grid position
                col += 2;
                if (col >= cols * 2) {
                    col = 0;
                    row += 2;
                }

                // Move to the next node
                currentNode = currentNode->next;
            }

            painter.end();
            free(cFilePath);

            // Set the combined image on the central imageLabel
            imageLabel->setPixmap(combinedPixmap);
            imageLabel->adjustSize();
        }

            // Handle non-DMI files
        else {
            QPixmap pixmap(filePath);
            if (!pixmap.isNull()) {
                imageLabel->setPixmap(pixmap);
                imageLabel->adjustSize();
            }
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
