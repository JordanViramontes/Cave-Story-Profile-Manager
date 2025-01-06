#include "window.h"
#include "tablewidgetdragrows.h"

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
#include <QDrag>
#include <QMimeData>
#include <QTableView>
#include <QAbstractTableModel>
#include <QHeaderView>

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
    centerColumn->addWidget(scrollArea, 1);
    centerColumn->setStretch(0, 1);
    centerColumn->setStretch(1, 4);


    // Right Column
    QWidget *rightWidget = new QWidget(this);
    QVBoxLayout *rightColumn = new QVBoxLayout(rightWidget);
    rightColumn->addWidget(imageLoader, 0, Qt::AlignHCenter);
    rightColumn->setAlignment(Qt::AlignTop);
    rightColumn->addWidget(tempButton, 0);

    // main grid
    QGridLayout *mainGrid = new QGridLayout(this);
    mainGrid->addWidget(leftWidget, 0, 0);
    mainGrid->addWidget(centerWidget, 0, 1);
    mainGrid->addWidget(rightWidget, 0, 2);

    mainGrid->setColumnStretch(0, 4);
    mainGrid->setColumnStretch(1, 12);
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

    label->setPixmap(pixmap.scaled(50,50,Qt::KeepAspectRatio));
    label->setScaledContents(true);

    return label;
}

QWidget * Window::createScrollArea() {
    // create Weapons Table
    TableWidgetDragRows * tableWidget = new TableWidgetDragRows(this);

    // text box
    QString weaponsNotes = "Only the first 5 weapons in the table will be \nadded to the Profile's Inventory";
    QLabel *detailsLabel = new QLabel(weaponsNotes, this);
    detailsLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QVBoxLayout *boxlayout = new QVBoxLayout(this);
    boxlayout->addWidget(detailsLabel);
    QGroupBox *box = new QGroupBox(tr("Notes:"), this);
    box->setLayout(boxlayout);

    // create scroll layout
    QVBoxLayout *scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(tableWidget);
    scrollLayout->addWidget(box);

    // scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    scrollArea->setLayout(scrollLayout);

    // QWidget *scrollArea = new QWidget(this);
    // scrollArea->setLayout(scrollLayout);

    return scrollArea;
}


