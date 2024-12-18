#include "window.h"

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLineEdit>
#include <QSizePolicy>
#include <QFrame>
#include <QScrollArea>
#include <QFileSystemModel>
#include <QTreeView>
#include <QMenuBar>
#include <QLabel>
#include <QPixmap>

Window::Window(QWidget *parent)
    : QWidget{parent}
{

    // constructors
    QWidget *fileWidget = createFileWidget();
    fileWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QWidget *imageLoader = createImageLoader();
    imageLoader->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QWidget *scrollArea = createScrollArea();
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QPushButton *tempButton = new QPushButton(this);
    tempButton->setText("testButton");

    // Left Column
    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftColumn = new QVBoxLayout(leftWidget);
    leftColumn->addWidget(fileWidget, 0);

    // Center Column
    QWidget *centerWidget = new QWidget(this);
    QVBoxLayout *centerColumn = new QVBoxLayout(centerWidget);
    centerColumn->addWidget(imageLoader, 0, Qt::AlignHCenter);
    centerColumn->addWidget(scrollArea, 1);
    centerColumn->setStretch(0, 1);
    centerColumn->setStretch(1, 4);


    // Right Column
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightColumn = new QVBoxLayout(rightWidget);
    rightColumn->setAlignment(Qt::AlignTop);
    rightColumn->addWidget(tempButton, 0);

    // main grid
    QGridLayout *mainGrid = new QGridLayout(this);
    mainGrid->addWidget(leftWidget, 0, 0);
    mainGrid->addWidget(centerWidget, 0, 1);
    mainGrid->addWidget(rightWidget, 0, 2);

    mainGrid->setColumnStretch(0, 2);
    mainGrid->setColumnStretch(1, 4);
    mainGrid->setColumnStretch(2, 1);

    setLayout(mainGrid);
}

QWidget * Window::createFileWidget() {
    // file system
    QFileSystemModel *fileSystem = new QFileSystemModel(this);
    fileSystem->setRootPath(QDir::rootPath());


    // tree for file system
    QTreeView *view = new QTreeView;
    view->setModel(fileSystem);
    view->hideColumn(1);
    view->hideColumn(2);
    view->hideColumn(3);
    view->setRootIndex(fileSystem->index("C:/Users/jorda/OneDrive/Desktop/cs_practice/saves"));

    return view;
}

QWidget * Window::createImageLoader() {
    QLabel * label = new QLabel(this);

    // Load the image into a QPixmap object
    QPixmap pixmap("C:/Users/jorda/OneDrive/Documents/1-Projects/CSProfileManager/testImage.png");
    // pixmap.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    label->setPixmap(pixmap.scaled(160,120,Qt::KeepAspectRatio));
    label->setScaledContents(true);

    return label;
}

QWidget * Window::createScrollArea() {
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    return scrollArea;
}





