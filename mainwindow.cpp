#include "mainwindow.h"
#include "globals.h"
#include "./ui_mainwindow.h"

#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set stuff
    setGlobals();
    setSignals();

    // set widgets
    setFileTrees();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// set stuff
void MainWindow::setGlobals() {
    SAVESDIRECTORY = "C:\\Users\\jorda\\Documents\\2-Programs\\CaveStoryProfileManager2\\build\\Desktop_Qt_6_10_1_MinGW_64_bit-Debug\\saves";
}

void MainWindow::setSignals() {
    // buttons
    connect(ui->runPushButton, SIGNAL(clicked(bool)), this, SLOT(_onRunButton()));
    connect(ui->updateDirPushButton, SIGNAL(clicked(bool)), this, SLOT(_onUpdateDirectoryButton()));
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
            i->setRootIndex(presetProfileModel->index(SAVESDIRECTORY + "\\Presets"));
        else
            i->setRootIndex(presetProfileModel->index(SAVESDIRECTORY + "\\Custom"));
    }
}








