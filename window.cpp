#include "window.h"
#include "global.h"

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
#include <QToolBar>
#include <QStandardItemModel>
#include <QTabWidget>


Window::Window(QWidget *parent)
    : QWidget{parent}
{
    // settings
    loadSettings();

    qDebug() << "TEST IMAGE: " << IMAGEPATH;


    // constructors
    QWidget *fileWidget = createFileWidget();
    fileWidget->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    QWidget *imageLoader = createImageLoader();
    imageLoader->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    QWidget *scrollArea = createScrollArea();
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidget *buttonArea = createButtonArea();
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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
    rightColumn->addWidget(buttonArea, 0);

    // main grid
    QGridLayout *mainGrid = new QGridLayout(this);
    mainGrid->addWidget(leftWidget, 1, 0);
    mainGrid->addWidget(centerWidget, 1, 1);
    mainGrid->addWidget(rightWidget, 1, 2);

    mainGrid->setColumnStretch(0, 4);
    mainGrid->setColumnStretch(1, 12);
    mainGrid->setColumnStretch(2, 2);

    setLayout(mainGrid);

    // connections
    connect (this, SIGNAL(updateCanUseProfile(bool)), this, SLOT(updateWidgetEnabled(bool)));

    // remove flag
    isStartingUp = false;
    disableWidgets();

    // test
    // exeDirectory = "C:/Users/jorda/OneDrive/Documents/1-Projects/CSProfileManager/doukutsu";
    // saveDirectory = "C:/Users/jorda/OneDrive/Documents/1-Projects/CSProfileManager/doukutsu/saves";
    // saveSettings();
}

QWidget * Window::createFileWidget() {
    // file system
    presetProfileModel = new QFileSystemModel(this);
    presetProfileModel->setRootPath(QDir::rootPath());

    // tree for file system
    QTreeView *presetView = new QTreeView;
    presetView->setModel(presetProfileModel);
    presetView->hideColumn(1);
    presetView->hideColumn(2);
    presetView->hideColumn(3);
    presetView->header()->setVisible(false);
    presetView->setRootIndex(presetProfileModel->index(saveDirectory + "/Preset"));
    presetView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QTreeView *customView = new QTreeView;
    customView->setModel(presetProfileModel);
    customView->hideColumn(1);
    customView->hideColumn(2);
    customView->hideColumn(3);
    customView->header()->setVisible(false);
    customView->setRootIndex(presetProfileModel->index(saveDirectory + "/Custom"));
    customView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // connections
    connect(presetView, &QTreeView::clicked, this, &Window::getProfileDirectory);
    connect(customView, &QTreeView::clicked, this, &Window::getProfileDirectory);

    // tabs
    fileTabs = new QTabWidget(this);
    fileTabs->addTab(presetView, tr("Presets"));
    fileTabs->addTab(customView, tr("Custom"));


    // save labels
    QLabel * saveLabel = new QLabel("Save As:", this);
    saveLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    saveEdit = new QLineEdit(this);
    saveEdit->setAlignment(Qt::AlignLeft);
    saveEdit->setPlaceholderText("Profile");
    saveEdit->setText("Profile");
    saveEdit->setToolTip("New Profile Name");
    saveEdit->setEnabled(false);
    saveEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

    saveButton = new QPushButton("Save New Profile", this);
    saveButton->setDisabled(false);
    saveButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QLabel * saveDatLabel = new QLabel(".dat", this);
    saveDatLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);



    // layout and widget
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(fileTabs, 0, 0, 1, -1);
    layout->addWidget(saveLabel, 1, 0);
    layout->addWidget(saveEdit, 1, 1);
    layout->addWidget(saveDatLabel, 1, 2);
    layout->addWidget(saveButton, 2, 0, 1, -1);

    QGroupBox *box = new QGroupBox(tr("Profiles:"), this);
    box->setLayout(layout);




    connect(saveButton, &QPushButton::clicked, this, &Window::slot_saveNewProfile);

    return box;
}

QWidget * Window::createImageLoader() {
    QLabel * label = new QLabel(this);

    // Load the image into a QPixmap object
    // QPixmap pixmap("C:/Users/jorda/OneDrive/Documents/1-Projects/CSProfileManager/testImage.png");
    // pixmap.scaled(320, 240, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // label->setPixmap(pixmap.scaled(50,50,Qt::KeepAspectRatio));
    // label->setScaledContents(true);

    return label;
}

QWidget * Window::createScrollArea() {
    // max ammo
    QLabel *maxAmmoLabel = new QLabel("Max Ammo: ", this);
    // AmmoEdit = new QLineEdit(this);
    AmmoEdit->setAlignment(Qt::AlignCenter);
    AmmoEdit->setFixedWidth(40);
    AmmoEdit->setMaxLength(3);
    AmmoEdit->setPlaceholderText("xx");
    AmmoEdit->setText("0");
    AmmoEdit->setToolTip("Set Max Missile Ammo");

    // slider
    slider = new WeaponSlider(Qt::Horizontal, this);
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

    // current weapon
    QLabel * currentWeaponLabel = new QLabel("Selected Weapon:", this);
    currentWeapon = new WeaponComboBox(this);
    for (unsigned int i = 1; i <= 5; i++) { currentWeapon->addItem(QString::number(i)); }
    currentWeapon->setToolTip("If the currently selected weapon is higher\n"
                              "than the amount of weapons enabled, it will\n"
                              "choose the last enabled weapon");

    QHBoxLayout *ammoLayout = new QHBoxLayout(this);
    ammoLayout->addWidget(maxAmmoLabel);
    ammoLayout->addWidget(AmmoEdit);
    ammoLayout->addWidget(slider);
    ammoLayout->addWidget(currentWeaponLabel);
    ammoLayout->addWidget(currentWeapon);
    // ammoLayout->addItem(spacer);

    QWidget *ammoWidget = new QWidget(this);
    ammoWidget->setLayout(ammoLayout);

    // create scroll layout
    QVBoxLayout *scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(tableWidget, QSizePolicy::MinimumExpanding);
    scrollLayout->addWidget(ammoWidget);

    QGroupBox *box = new QGroupBox(tr("Inventory:"), this);
    box->setLayout(scrollLayout);

    // connection for profile stuff
    connect(this, SIGNAL(updateAmmoFromProfile(int)), this, SLOT(updateAmmoEdit(int)));



    return box;
}

QWidget * Window::createButtonArea() {
    // quick launch
    // quickApplyButton = new QPushButton(this);
    quickApplyButton->setText("Quick Apply");

    connect(quickApplyButton, SIGNAL(clicked(bool)), this, SLOT(quickApplySlot()));

    // valid Executable
    // validLabel = new QLabel("Directory to Doukutsu.exe Invalid!!\nPlease update the directory:", this);
    validLabel->setWordWrap(true);
    // validLabel->setVisible(false);
    QPushButton *validButton = new QPushButton("Update\nDoukutsu\nDirectory", this);
    // tempDirectoryLabel = new QLabel(".exe Directory:\n" + exeDirectory, this);
    tempDirectoryLabel->setWordWrap(true);
    QVBoxLayout *validLayout = new QVBoxLayout(this);
    validLayout->addWidget(validLabel, Qt::AlignCenter);
    validLayout->addWidget(validButton, Qt::AlignCenter);
    validLayout->addWidget(tempDirectoryLabel);
    QWidget * validWidget = new QWidget(this);
    validWidget->setLayout(validLayout);

    // spacer
    QSpacerItem *spacer = new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

    // credits
    QString creditsStr = "By: Jordan Viramontes\n@jordantimes on Discord for questions\n\n";
    QLabel *credits = new QLabel(creditsStr, this);
    credits->setWordWrap(true);
    QPushButton * helpButton = new QPushButton("Help", this);

    connect(helpButton, &QPushButton::clicked, this, [] {
        QString helpStr = "Saves:\nThe folder of Profile.dat save files should be included with the Profile Manager program and located in the same folder as the program. Leave it as is.";
        helpStr += "\n\nQuick Apply:\nPressing this button applies your changes to the local Profile.dat of the linked Doukutsu.exe and then launches the game. If the game is already launched, simply press ESC (in game), press F2 (the restart option), then load.";
        helpStr += "\n\nWeapons:\nIn order for a weapon to be included in the save it must be BOTH: \n\t1) Enabled (the check box must be checked), and\n\t2) Within the first 5 rows\nYou know that the weapon is enabled when it's row is BLUE.\nIf you want to change the currently selected weapon, use the \"Selected Weapon\" Drop down. If the currently selected weapon is higher than the amount of weapons enabled, it will select the last enabled weapon.";
        helpStr += "\n\nSaving:\nYou are able to save new profiles based on any profile. They will be saved into the \"Custom\" tab with whatever name you enter in the \"Save As\" box.\nYou may save new profiles based on preset profiles as well as custom profiles.";
        QMessageBox helpBox = QMessageBox();
        // helpBox.setIcon(QMessageBox::Warning);
        helpBox.setWindowTitle("Help");
        helpBox.setStandardButtons(QMessageBox::Ok);
        helpBox.setText(helpStr);
        helpBox.exec();
    });


    // connections
    connect(validButton, SIGNAL(clicked(bool)), this, SLOT(updateExeDirectory()));

    connect(this, SIGNAL(needToUpdateDirectorySignal()), this, SLOT(updateExeDirectory()));

    // layout and widget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(quickApplyButton);
    layout->addWidget(validWidget);
    layout->addItem(spacer);
    layout->addWidget(credits);
    layout->addWidget(helpButton);
    layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);


    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);

    return widget;
}

void Window::saveSettings() {
    // Save settings using QSettings
    QSettings settings("joran", "CaveStoryProfileManager"); // Replace with your org/app names
    settings.setValue("exeDirectory", exeDirectory);
    settings.setValue("saveDirectory", saveDirectory);
    qDebug() << "Settings saved: exeDirectory =" << exeDirectory << ", saveDirectory =" << saveDirectory;
}

void Window::loadSettings() {
    // Load settings using QSettings
    QSettings settings("joran", "CaveStoryProfileManager"); // Replace with your org/app names

    // Check if keys exist before loading
    if (settings.contains("exeDirectory")) {
        exeDirectory = settings.value("exeDirectory").toString();
        QFileInfo fileInfo(exeDirectory);

        // check that our exeDirectory is okay
        if (fileInfo.fileName() == "Doukutsu.exe") {
            isDirectoryOkay = true;
            validLabel->setVisible(false);
            if (tempDirectoryLabel) tempDirectoryLabel->setText(".exe Directory:\n" + exeDirectory);
        }
        else {
            // qDebug() << "ASDASDASD";
            isDirectoryOkay = false;
            validLabel->setVisible(true);
            disableWidgets();
        }
        qDebug() << "is: " << isDirectoryOkay;
    } else {
        exeDirectory = ""; // Default value
        isDirectoryOkay = false;
    }

    saveDirectory = QCoreApplication::applicationDirPath(); // Default value

    QDir dir(QCoreApplication::applicationDirPath());
    // dir.cdUp();
    dir.cd("saves");

    saveDirectory = dir.path(); // default value

    qDebug() << "Settings loaded: exeDirectory =" << exeDirectory;
    qDebug() << "saveDirectory =" << saveDirectory;
}





















