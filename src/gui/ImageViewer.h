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
#include <iostream>
#include <QGraphicsColorizeEffect>
#include "Docker.h"

class ImageViewer : public QMainWindow {

public:
    ImageViewer() {
        // Set up QLabel for displaying the image
        imageLabel = new QLabel;
        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(false);

        // Set up QScrollArea for handling large images
        scrollArea = new QScrollArea;
        scrollArea->setStyleSheet("background-color: #282828;");
        // scrollArea->setBackgroundRole(QPalette::Dark);
        //scrollArea->setWidget(imageLabel);
        scrollArea->setBaseSize(200, 600);
        // Set up QListWidget as a sidebar for image files
        fileListWidget = new QListWidget;
        connect(fileListWidget, &QListWidget::itemClicked, this, &ImageViewer::loadImageFromList);

        containerWidget = new QWidget;
        scrollArea->setWidget(containerWidget);    // Set the container widget as the scroll area's content
        scrollArea->setWidgetResizable(true);
        layout = new QGridLayout(containerWidget);
        layout->setHorizontalSpacing(5);
        layout->setColumnStretch(0, 0);
        layout->setRowStretch(0, 0);
        layout->setSizeConstraint(QLayout::SetFixedSize);

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

        auto icon = this->new_icon;
        const int one = 1;
        const char *list_of_icon_states[] = {"\"BlowingUp\"", "state2", "state3"}; // Example constant list of icon states


        connect(action2, &QAction::triggered, this, []() { qDebug() << "Command 2 executed"; });

        // Show the menu at the cursor position
        currentMenu->exec(QCursor::pos());
//        currentMenu = nullptr; // Reset after exec
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




            //qDebug() << "Number of cols: " << layout->columnCount() << "\n";
            char *cFilePath = (char *)malloc(filePath.toUtf8().size() + 1);
            strcpy(cFilePath, filePath.toUtf8().data());

            // Initialize DMI struct
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
                                // printf("C = %d Trans_Num =%d\n", c, new_icon.image->trans_num);
                                if((c+1) <= new_icon.image->trans_num){
                                    colorTable.push_back(qRgba(color.red, color.green, color.blue, 0));
                                }

                                else{
                                    colorTable.push_back(qRgba(color.red, color.green, color.blue, 255));
                                }
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
                                if(c == 0){
                                    colorTable.push_back(qRgba(color.red, color.green, color.blue, 0));

                                }
                                else{
                                    colorTable.push_back(qRgb(color.red, color.green, color.blue));
                                }
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

                QHBoxLayout *first_row = new QHBoxLayout();

                // Create a new QLabel to display the icon
             //   QVBoxLayout *vbox = new QVBoxLayout();
                QHBoxLayout *row_1 = new QHBoxLayout();
                QHBoxLayout *row_2 = new QHBoxLayout();
                // QPixmap checkerboard = createCheckerboard(new_icon.icon_width, new_icon.icon_height, 10);  // 20x20 cells, 10px square
                //vbox->setSizeConstraint(QLayout::SetFixedSize);  // Prevent vertical stretching
              //  vbox->setAlignment(Qt::AlignCenter);
                auto *iconLabel = new IconLabel(currentState->state);
                iconLabel->setAlignment(Qt::AlignCenter);
                iconLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

                // iconLabel->setStyleSheet("border: 1px solid black;"); // Optional styling for appearance
                //  iconLabel->setPixmap(pixmap);

                //   QGraphicsColorizeEffect *colorizeEffect = new QGraphicsColorizeEffect();
                //     colorizeEffect->setColor( (QColor(70, 130, 180)));  // Cornflower Blue

// Adjust strength to be less overpowering
                //    colorizeEffect->setStrength(0.4);  // Lower the strength (0.0 - 1.0)
                //     iconLabel->setGraphicsEffect(colorizeEffect);

                iconLabel->setFixedSize(imageWidth + 3, imageHeight + 2);

                iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                iconLabel->setPixmap(pixmap.scaled(imageWidth, imageHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));

                //    iconLabel->setFixedSize(imageWidth+3, imageHeight+2);
                iconLabel->setContextMenuPolicy(Qt::CustomContextMenu);
                iconLabel->state_pointer = currentState;
                iconLabel->icon_pointer = &new_icon;

                iconLabel->setCursor(Qt::DragLinkCursor);

                //  qDebug() << iconLabel->minimumSize() << "\n";
                IconText *stateNameLabel = new IconText(currentState->state);
                stateNameLabel->setFixedSize(168, 33);
                stateNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
                stateNameLabel->setStyleSheet(
                        "font-size: 17px; "
                        "font-family: bahnschrift; "  // Set the font face
                        "color: white; "  // Set font color
                        "background-color: #2C2C2C;"
                        "padding-left: 3px;" // Shift text 4 pixels to the right

                   //     "text-align: left; "  // Align text to the left

                    //     "border-radius: 3px; "
                 //        "border: 1px solid #43BDFC;"
                );
                QLabel *widget = new QLabel();
                widget->setFixedSize(202, 176);
                widget->setStyleSheet("background-color: #383838; border-radius: 4px; border: 1px solid #383838;");
                QHBoxLayout *widget_layout = new QHBoxLayout(widget);
                widget_layout->addWidget(iconLabel);
                stateNameLabel->setWordWrap(true);
                stateNameLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
                row_1->setAlignment(Qt::AlignLeft);
                row_1->addSpacerItem(new QSpacerItem(7, 0, QSizePolicy::Minimum, QSizePolicy::Fixed)); // 2px spacing

                QLabel *move_state = new QLabel();
                QPixmap newpixmap("movement_state.png");
                move_state->setStyleSheet("background-color: #2C2C2C");
                move_state->setPixmap(newpixmap);
                row_1->addWidget(stateNameLabel);
                row_1->addWidget(move_state);
                row_2->setContentsMargins(0, 0, 0, 0);
                row_2->addSpacerItem(new QSpacerItem(7, 0, QSizePolicy::Fixed, QSizePolicy::Fixed)); // 2px spacing
                row_2->addWidget(widget);
                //   stateNameLabel->buddy();
                //   std::cout << stateNameLabel->textInteractionFlags();
                //vbox->addWidget(iconLabel);
                //vbox->addWidget(stateNameLabel);
                //vbox->setAlignment(Qt::AlignBottom);

                // Connect the customContextMenuRequested signal
                connect(iconLabel, &QLabel::customContextMenuRequested, iconLabel, &IconLabel::showContextMenu);

                QFont font = iconLabel->font();
                font.setPointSize(5); // Adjust the font size as needed
                iconLabel->setFont(font);
                Docker *new_dock = new Docker();
                new_dock->mainlayout->setContentsMargins(0, 0, 0, 0); // Set margins: left, top, right, bottom
                new_dock->mainlayout->setAlignment(Qt::AlignTop);
                new_dock->mainlayout->addSpacerItem(new QSpacerItem(0, 7, QSizePolicy::Minimum, QSizePolicy::Fixed)); // 2px spacing

                new_dock->mainlayout->addLayout(row_1);
                new_dock->mainlayout->addLayout(row_2);

                new_dock->frame_icon = iconLabel;
                new_dock->originalPixmap = iconLabel->pixmap();
                layout->setRowStretch(row, 0);
                layout->setColumnStretch(col, 0);
                iconLabel->setMinimumSize(imageWidth + 3, imageHeight + 2);
       //         layout->addLayout(new_dock->mainlayout, row, col);
                layout->addWidget(new_dock, row, col);
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
                new_image->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

                layout->addWidget(new_image);
            }
        }
    }


private:
    QMenu *currentMenu = nullptr;

private:

    QPixmap createCheckerboard(int width, int height, int squareSize) {
        QPixmap pixmap(width, height);
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        painter.setBrush(QColor(200, 200, 200));  // Gray color for the squares
        painter.setPen(Qt::NoPen);

        for (int y = 0; y < height; y += squareSize) {
            for (int x = 0; x < width; x += squareSize) {
                if ((x / squareSize + y / squareSize) % 2 == 0) {
                    painter.drawRect(x, y, squareSize, squareSize);
                }
            }
        }
        return pixmap;
    }
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
            if (QLayout *layout_w = item->layout()) {
                layout_w->deleteLater(); // Schedule the widget for deletion
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
    DMI new_icon;


};
