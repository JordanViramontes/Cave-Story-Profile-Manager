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
#include <QFile>
#include <QMessageBox>

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
    mainGrid->setColumnStretch(2, 2);

    setLayout(mainGrid);
}

QWidget * Window::createFileWidget() {
    // file system
    fileSystem = new QFileSystemModel(this);
    fileSystem->setRootPath(QDir::rootPath());

    // tree for file system
    QTreeView *view = new QTreeView;
    view->setModel(fileSystem);
    view->hideColumn(1);
    view->hideColumn(2);
    view->hideColumn(3);
    view->setRootIndex(fileSystem->index(saveDirectory));

    // get the file loaded
    // connect(fileSystem, SIGNAL(directoryLoaded(QString)), this, SLOT(getProfileDirectory(QString)));
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(getProfileDirectory(QModelIndex)));

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
    tableWidget = new TableWidgetDragRows(this);

    // text box
    // QString weaponsNotes = "You can Drag and Drop rows in the table to change weapon order";
    // weaponsNotes        += "\nWeapons will be added to the inventory (and appear blue) if they are:";
    // weaponsNotes        += "\n\t- Checked (unchecked items will appear gray), and";
    // weaponsNotes        += "\n\t- Within the first 5 slots of the table";

    // QLabel *detailsLabel = new QLabel(weaponsNotes, this);
    // detailsLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    // detailsLabel->setWordWrap(true);


    // max ammo
    QLabel *maxAmmoLabel = new QLabel("Max Missile Ammo: ", this);
    AmmoEdit = new QLineEdit(this);
    AmmoEdit->setAlignment(Qt::AlignCenter);
    AmmoEdit->setFixedWidth(40);
    AmmoEdit->setMaxLength(3);
    AmmoEdit->setPlaceholderText("xx");
    AmmoEdit->setText("0");
    AmmoEdit->setToolTip("Set Max Missile Ammo");

    // slider
    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 99);
    slider->setValue(0);
    slider->setToolTip("Adjust Max Ammo");
    // slider->setGeometry(10,140, 115, 30);

    // update counter
    connect(slider, SIGNAL(sliderMoved(int)), tableWidget, SLOT(updateMaxAmmo(int)));

    // change text
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(updateAmmoEdit(int)));


    // validator
    QIntValidator *intValidator = new QIntValidator(0, 99, this);  // Valid floating-point numbers from 0.0 to 1000.0 with 2 decimal places
    intValidator->setLocale(QLocale::C);
    AmmoEdit->setValidator(intValidator);

    // Connect textChanged to handle user input
    connect(AmmoEdit, SIGNAL(textChanged(QString)), tableWidget, SLOT(updateMaxAmmo(QString)));

    QSpacerItem *spacer = new QSpacerItem(20, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *ammoLayout = new QHBoxLayout(this);
    ammoLayout->addWidget(maxAmmoLabel);
    ammoLayout->addWidget(AmmoEdit);
    ammoLayout->addWidget(slider);
    // ammoLayout->addItem(spacer);

    QWidget *ammoWidget = new QWidget(this);
    ammoWidget->setLayout(ammoLayout);

    // create scroll layout
    QVBoxLayout *scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(tableWidget, QSizePolicy::MinimumExpanding);
    scrollLayout->addWidget(ammoWidget);

    QGroupBox *box = new QGroupBox(tr("Inventory::"), this);
    box->setLayout(scrollLayout);

    // connection for profile stuff
    connect(this, SIGNAL(updateAmmoFromProfile(int)), this, SLOT(updateAmmoEdit(int)));


    return box;
}

