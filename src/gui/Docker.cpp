//
// Created by jonat on 12/29/2024.
//
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QLineEdit>
#include <QPixmap>
#include <QIntValidator>
#include <QSpinBox>
#include "Docker.h"
#include "IconLabel.h"

// Docker Constructor
Docker::Docker() : QWidget() {
    // Main horizontal layout
    this->setObjectName("dockerFrame");
    this->setStyleSheet(
            "#dockerFrame { border: 1px solid #444444; border-radius: 3px; background-color: #2C2C2C; }"
    );
    mainlayout = new QVBoxLayout(this);  // Attach layout to Docker widget
    this->setFixedSize(216, 322);

//    // Vertical layout for slider and controls
//    auto *slider_layout = new QVBoxLayout();
//
//    auto spin_box = new QSpinBox();
//    spin_box->setMaximum(500);
//    spin_box->setMinimum(20);
//    // Create Input Box (at the top)
//    QLineEdit *inputBox = new QLineEdit();
//    inputBox->setAlignment(Qt::AlignCenter);
//    inputBox->setFixedWidth(40);  // Width to fit 3 characters
//    inputBox->setMaxLength(3);  // Max 3 digits (100-200)
//    inputBox->setValidator(new QIntValidator(100, 200, this));  // Only allow 100-200
//
//    // Plus Button (above slider)
//    QPushButton *plusButton = new QPushButton("+");
//    plusButton->setFixedSize(30, 30);

    // Slider (Vertical Orientation)
//    slider = new QSlider(Qt::Vertical);
//    slider->setRange(20, 500);  // Slider range from 100 to 200
//    slider->setValue(100);  // Start at 100


//    slider->setMaximumSize(20, 100);
//// Create a horizontal layout to contain the slider
//    QHBoxLayout *sliderWrapper = new QHBoxLayout();
//    //sliderWrapper->setAlignment(Qt::AlignRight);  // Align slider to the right
//    sliderWrapper->addWidget(slider);
//
//// Adjust spin box minimum size
//    spin_box->setMinimumSize(50, 20);
//    spin_box->setSuffix("%");
//
//    spin_box->setStyleSheet(
//            "QSpinBox {"
//            "    font-size: 12px;"  // Slightly larger font
//            "    border: 1px solid #a0a0a0;"  // Explicit border to avoid focus underline
//            "    border-radius: 4px;"
//           // "    padding: 2px 4px;"
//            "    width: 60px;"  // Adjust width for 4 digits
//            "}"
//    );

//    slider->setStyleSheet(
//
//            "QSlider::handle:vertical {"
//            "    background: #0078D7;"  // Blue handle
//            "    width: 6px;"  // Square handle size
//            "    height: 3px;"
//            "    margin: -6px 0;"  // Center handle over groove
//            "    border: 1px solid #005bb5;"  // Blue outline
//            "}"
//    );
//    spin_box->setButtonSymbols(QAbstractSpinBox::NoButtons);
//    spin_box->setAlignment(Qt::AlignRight);
//// Add spin box and slider (wrapped) to the vertical layout
//    slider_layout->addWidget(spin_box);
//    slider_layout->addLayout(sliderWrapper);  // Add the slider via wrapper
//    slider_layout->setAlignment(Qt::AlignCenter);

// Add the vertical layout to the main layout
//    mainlayout->addLayout(slider_layout);
//    connect(slider, &QSlider::valueChanged, spin_box, &QSpinBox::setValue);
//    connect(spin_box, &QSpinBox::valueChanged, slider, &QSlider::setValue);
//    connect(slider, &QSlider::valueChanged, this, [this](int value){
//        this->resizeIcon(value);
//    });
    // Add frame_icon to the main layout (to the right of the slider)
//    mainlayout->addWidget(frame_icon);

    // Set the layout to Docker widget (to make it visible)
    this->setLayout(mainlayout);
}



// Resize Icon Based on Slider Value
void Docker::resizeIcon(int value) {
    if (!originalPixmap.isNull()) {
        int size = value;  // Direct mapping from slider (100 to 200)
        double scaleFactor = value / 100.0;
        bool isInteger = (value % 100 == 0);

        if(isInteger){
            frame_icon->setPixmap(originalPixmap.scaled(originalPixmap.size() * scaleFactor, Qt::KeepAspectRatio, Qt::FastTransformation));
        }
        else{
            frame_icon->setPixmap(originalPixmap.scaled(originalPixmap.size() * scaleFactor, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}