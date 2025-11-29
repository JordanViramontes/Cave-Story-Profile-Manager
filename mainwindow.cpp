#include "mainwindow.h"
#include "globals.h"
#include "./ui_mainwindow.h"

#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    savesDirectory = "C:\\Users\\jorda\\Documents\\2-Programs\\CaveStoryProfileManager2\\build\\Desktop_Qt_6_10_1_MinGW_64_bit-Debug\\saves";

    // set stuff
    // setGlobals();
    loadSettings();
    setSignals();

    // set widgets
    setFileTrees();
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
void MainWindow::setFileTrees() {
    // initialize file system
    QFileSystemModel* presetProfileModel = new QFileSystemModel(this);
    presetProfileModel->setRootPath(QDir::rootPath());

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








