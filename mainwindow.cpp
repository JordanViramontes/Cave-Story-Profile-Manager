#include "mainwindow.h"
#include "globals.h"
#include "./ui_mainwindow.h"
#include "qweapontableslot.h"

#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    savesDirectory = QCoreApplication::applicationDirPath() + "/saves";

    // set stuff
    // setGlobals();
    loadSettings();
    setSignals();

    // set widgets
    createFileTrees();
    createWeaponTable();


    // update state depending on valid path
    // gameDirectory = "";
    widgetLock(checkGameDirPath(gameDirectory));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// save/load
void MainWindow::saveSettings() {
    QSettings settings("JordanV", "CaveStoryProfileManager");
    settings.setValue("gameDirectory", gameDirectory);
    qDebug() << "mainwindow.cpp: Settings saved, gameDirectory =" << gameDirectory;
}

void MainWindow::loadSettings() {
    QSettings settings("JordanV", "CaveStoryProfileManager");

    if (!settings.contains("gameDirectory")) {
        qDebug() << "mainwindow.cpp: settings don't contain 'gameDirectory'";
        return;
    }

    QString tryDirectory = settings.value("gameDirectory").toString();

    // double check file directory
    if (!checkGameDirPath(tryDirectory)) return;

    // if our dir is good!
    qDebug() << "mainwindow.cpp: Settings loaded, gameDirectory =" << gameDirectory;
}

void MainWindow::setSignals() {
    // buttons
    connect(ui->runPushButton, SIGNAL(clicked(bool)), this, SLOT(_onRunButton()));
    connect(ui->updateDirPushButton, SIGNAL(clicked(bool)), this, SLOT(_onUpdateDirectoryButton()));

    // file
    connect(ui->presetFileTree, SIGNAL(clicked(QModelIndex)), this, SLOT(_onSelectFile(QModelIndex)));
    connect(ui->customFileTree, SIGNAL(clicked(QModelIndex)), this, SLOT(_onSelectFile(QModelIndex)));
}

// set widgets
void MainWindow::createFileTrees() {
    // initialize file system
    QFileSystemModel* presetProfileModel = new QFileSystemModel(this);
    presetProfileModel->setRootPath(QDir::rootPath()); // default path

    // update to correct saves file if it exists
    if (QDir(savesDirectory).exists()) presetProfileModel->setRootPath(savesDirectory);
    else qDebug() << "mainwindow.cpp: ERROR, path to saves folder incorrect, setting to root";

    // set preset and column tabs
    for (auto i : {ui->presetFileTree, ui->customFileTree}) {
        i->setModel(presetProfileModel);
        i->hideColumn(1);
        i->hideColumn(2);
        i->hideColumn(3);
        i->setHeaderHidden(true);
        if (i == ui->presetFileTree)
            i->setRootIndex(presetProfileModel->index(savesDirectory + "\\Presets"));
        else
            i->setRootIndex(presetProfileModel->index(savesDirectory + "\\Custom"));
    }
}

void MainWindow::createWeaponTable() {
    // set table settings
    QTableWidget* table = ui->weaponsTable;
    table->setRowCount(totalWeapons);
    // table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // table->setFocusPolicy(Qt::NoFocus);
    // table->setSelectionMode(QAbstractItemView::NoSelection);

    // set header settings
    QHeaderView* horizHeader = table->horizontalHeader();
    QHeaderView* vertHeader = table->verticalHeader();
    horizHeader->setSectionResizeMode(QHeaderView::Fixed);
    vertHeader->setSectionResizeMode(QHeaderView::Fixed);
    horizHeader->setSectionResizeMode(QHeaderView::Stretch);
    // QHeaderView* vertHeader = table->verticalHeader();
    // vertHeader->setSectionResizeMode(QHeaderView::Stretch);

    // set weaponslots
    for (int row = 0; row < totalWeapons; row++) {
        bool hasAmmo = false;
        QString text = "weapon";

        switch (row) {
            case (0): hasAmmo = false; text = "PS"; break;
            case (1): hasAmmo = false; text = "FB"; break;
            case (2): hasAmmo = true; text = "MG"; break;
            case (3): hasAmmo = true; text = "ML"; break;
            case (4): hasAmmo = true; text = "BB"; break;
            case (5): hasAmmo = false; text = "BL"; break;
            case (6): hasAmmo = true; text = "SM"; break;
            case (7): hasAmmo = false; text = "SN"; break;
            case (8): hasAmmo = false; text = "NS"; break;
            case (9): hasAmmo = false; text = "SP"; break;

        default:
            break;
        }

        // set widget to cell
        QWeaponTableSlot* defaultWeapon = new QWeaponTableSlot(hasAmmo, text, this);
        table->setCellWidget(row, 0, defaultWeapon);

        // set the fixed row height
        table->setRowHeight(row, defaultWeapon->height());
    }







}






