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
    for (int type = 0x00, tableRowCount = 0; type <= 0x0D; type++) {
        bool hasAmmo = false;
        QString text = "weapon";
        int ammo = 0;
        QVector<int> weaponLvls = {1, 2, 3, 3};

        // ensure that only valid weapon type numbers make it through
        switch (type) {
            case (0x01): hasAmmo = false; text = "SN"; weaponLvls = {30, 40, 16, 16}; break;
            case (0x02): hasAmmo = false; text = "PS"; weaponLvls = {10, 20, 10, 10}; break;
            case (0x03): hasAmmo = false; text = "FB"; weaponLvls = {10, 20, 20, 20}; break;
            case (0x04): hasAmmo = true; text = "MG"; weaponLvls = {30, 40, 10, 10}; ammo = 100; break;
            case (0x05): hasAmmo = true; text = "ML"; weaponLvls = {10, 20, 10, 10};  break;
            case (0x07): hasAmmo = true; text = "BB"; weaponLvls = {10, 20, 5, 5}; ammo = 100; break;
            case (0x09): hasAmmo = false; text = "BL"; weaponLvls = {30, 60, 0, 0}; break;
            case (0x0A): hasAmmo = true; text = "SM"; weaponLvls = {30, 60, 10, 10};  break;
            case (0x0C): hasAmmo = false; text = "NS"; weaponLvls = {1, 1, 1, 1}; break;
            case (0x0D): hasAmmo = false; text = "SP"; weaponLvls = {40, 60, 200, 200}; break;
            default: continue; break;
        }

        // set widget to cell
        QWeaponTableSlot* defaultWeapon = new QWeaponTableSlot(type, ammo, hasAmmo, text, weaponLvls, this);
        table->setCellWidget(tableRowCount, 0, defaultWeapon);

        // add weapon to weaponsdictionary
        weaponsTableDictionary[type] = defaultWeapon;

        // set the fixed row height
        table->setRowHeight(tableRowCount, defaultWeapon->height());

        // update table row iterator
        tableRowCount++;
    }

    qDebug() << "mainwindow.cpp: testing all weapon table dicitonary entries: ";
    for (auto i : weaponsTableDictionary.keys()) {
        QWeaponTableSlot* weapon = weaponsTableDictionary[i];
        qDebug() << i << ":" << weapon->getWeaponType();
    }

}






