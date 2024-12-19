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
#include <QMessageBox>
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
        //scrollArea->setWidget(imageLabel);
        scrollArea->setBaseSize(200, 600);
        // Set up QListWidget as a sidebar for image files
        fileListWidget = new QListWidget;
        connect(fileListWidget, &QListWidget::itemClicked, this, &ImageViewer::loadImageFromList);
//        QMessageBox::about(this, "I like eggs", "Goat");
//        auto lol = QMessageBox::question(this, "LIFE OR DEATH", "Do you like eggs?",
//                                         QMessageBox::StandardButton::YesAll, QMessageBox::NoAll);
//        // Set up a splitter to display sidebar and image viewer side-by-side

        // Create a container widget for the grid layout
        containerWidget = new QWidget;
        scrollArea->setWidget(containerWidget);    // Set the container widget as the scroll area's content
        scrollArea->setWidgetResizable(true);
        layout = new QGridLayout(containerWidget);
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
    void showContextMenu(const QPoint &pos) {
        // Close the existing menu if open
        if (currentMenu) {
            currentMenu->close();
            delete currentMenu; // Clean up the old menu
        }

        // Create a new menu
        currentMenu = new QMenu(this);

        QAction *action1 = currentMenu->addAction("Command 1");
        QAction *action2 = currentMenu->addAction("Command 2");

        connect(action1, &QAction::triggered, this, []() { qDebug() << "Command 1 executed"; });
        connect(action2, &QAction::triggered, this, []() { qDebug() << "Command 2 executed"; });

        // Show the menu at the cursor position
        currentMenu->exec(QCursor::pos());
        currentMenu = nullptr; // Reset after exec
    }
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

            //qDebug() << "The file is a .dmi file.";
            // Convert file path to a C-style string




            qDebug() << "Number of cols: " << layout->columnCount() << "\n";
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

//            QPixmap combinedPixmap(cols * imageWidth * 2, rows * imageHeight * 2);
//            combinedPixmap.fill(Qt::transparent); // Start with transparent background
//            QPainter painter(&combinedPixmap);

            // Traverse iconStates and draw images
            node *currentNode = new_icon.iconStates.head;
            int row = 0, col = 0;

            clearLayout();

            while (currentNode != nullptr) {
                auto *currentState = (icon_state *)currentNode->data;

                QImage temp_image;
                auto *pixel_data = (png_bytepp *)currentState->frame_vector[0].data;
                png_bytepp pixel_data2;
                QVector<QRgb> colorTable;

                switch (new_icon.image->color_type) {
                    case PNG_COLOR_TYPE_PALETTE: {
                        // Initialize QImage with Indexed8 format
                        if(new_icon.image->bit_depth == 4) {
                            temp_image = QImage(imageWidth, imageHeight, QImage::Format_Indexed8);
                            // Copy pixel data row by row
                            pixel_data2 = pixel_data[0];
                            for (int i = 0; i < new_icon.icon_height; i++) {
                                QVector<uint8_t> pixelRow(new_icon.icon_width);
                                for (int o = 0; o < new_icon.icon_width; o++) {
                                    // Determine whether to take the first or second half based on the index `i`
                                    int inner_index = o / 2;
                                    uint8_t pixel = pixel_data2[i][o/2]; // Access the current pixel data

                                    uint8_t maskedPixel;

                                    if (o % 2 == 0) {
                                        // Take the first half (high nibble for 4-bit data)
                                        maskedPixel = (pixel & 0xF0) >> 4;
                                    } else {
                                        // Take the second half (low nibble for 4-bit data)
                                        maskedPixel = pixel & 0x0F;
                                    }

                                    // Add the masked pixel to the row
                                    pixelRow[o] = maskedPixel;
                                }
                                memcpy(temp_image.scanLine(i), pixelRow.data(), new_icon.icon_width);
                           //     printf("I = %d\n", i);
                            }

                           // printf("Now to color table. .\n");
                            // Generate a random color table with the size of palette_num

                            // Build color table
                            for (int c = 0; c < new_icon.image->palette_num; ++c) {
                                png_color color = new_icon.image->palette[c];
                                colorTable.push_back(qRgb(color.red, color.green, color.blue));
                            }
                            temp_image.setColorTable(colorTable);

                        }


                        else {
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
                        }

                        break;
                    }
                    default:
                        qWarning() << "Unsupported color type!";
                        break;
                }

                // Convert QImage to QPixmap
                QPixmap pixmap = QPixmap::fromImage(temp_image);

                // Create a new QLabel to display the icon
                QLabel *iconLabel = new QLabel("Hello, QLabel!");

                iconLabel->setPixmap(pixmap);
                iconLabel->setFixedSize(imageWidth+1, imageHeight);
                iconLabel->setStyleSheet("border: 1px solid black;"); // Optional styling for appearance
                iconLabel->setAlignment(Qt::AlignCenter);

                iconLabel->setContextMenuPolicy(Qt::CustomContextMenu);

                // Connect the customContextMenuRequested signal
                connect(iconLabel, &QLabel::customContextMenuRequested, this, &ImageViewer::showContextMenu);

                QFont font = iconLabel->font();
                font.setPointSize(10); // Adjust the font size as needed
                iconLabel->setFont(font);
                // Add the QLabel to the layout
                layout->addWidget(iconLabel, row, col);
                containerWidget->update();

                // Update grid posxition
                col += 1;
                if (col >= cols) {
                    col = 0;
                    row += 1;
                }

                // Move to the next node
                currentNode = currentNode->next;
            }

// Commented out painting nonsense:
// painter.end();
// free(cFilePath);

// Set the combined image on the central imageLabel (if needed)
// imageLabel->setPixmap(combinedPixmap);
// imageLabel->adjustSize();


//            painter.end();
            free(cFilePath);

//            // Set the combined image on the central imageLabel
//            imageLabel->setPixmap(combinedPixmap);
//            imageLabel->adjustSize();
        }

            // Handle non-DMI files
        else {
            QPixmap pixmap(filePath);
            if (!pixmap.isNull()) {
                auto *new_image = new QLabel();
                clearLayout();
                new_image->setPixmap(pixmap);
                new_image->adjustSize();
                layout->addWidget(new_image);
            }
        }
    }


private:
    QMenu *currentMenu = nullptr;

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


    void clearLayout() {
        if (!layout) return;

        // Remove and delete all child widgets
        while (QLayoutItem *item = layout->takeAt(0)) {
            if (QWidget *widget = item->widget()) {
                widget->deleteLater(); // Schedule the widget for deletion
            }
            delete item; // Delete the layout item itself
        }
    }
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    QListWidget *fileListWidget;
    QString file_clicked;
    QGridLayout* layout;
    QWidget *containerWidget; // Added container widget for the grid layout

};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ImageViewer viewer;
    viewer.show();
    return app.exec();
}
