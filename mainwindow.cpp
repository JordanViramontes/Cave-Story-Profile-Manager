#include "mainwindow.h"
#include "globals.h"
#include "./ui_mainwindow.h"
#include "dialog.h"

#include <QFileSystemModel>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QFileDialog>
#include <QStandardPaths>
#include <qabstractitemmodel.h>
#include <QModelIndex>
#include <qfilesystemmodel.h>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    savesDirectory = QCoreApplication::applicationDirPath() + "/Saves";
    qDebug() << "mainwindow.cpp: savesDirectory: " << savesDirectory;

    // Load Settings
    {
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

    // Connections
    {
        // buttons
        connect(ui->launchPushButton, SIGNAL(clicked(bool)), this, SLOT(onSimpleRunButtonPressed()));
        connect(ui->runPushButton, SIGNAL(clicked(bool)), this, SLOT(onRunButtonPressed()));
        connect(ui->HelpPushButton, SIGNAL(clicked(bool)), this, SLOT(onHelpButtonPressed()));
        connect(ui->updateDirPushButton, SIGNAL(clicked(bool)), this, SLOT(onUpdateDirectoryButtonPressed()));

        // profile selections
        connect(ui->profiles, SIGNAL(saveFilePressed(QString)), this, SLOT(onProfilesSaveFilePressed(QString)));
        connect(ui->profiles, SIGNAL(saveAsButtonPressed(QString)), this, SLOT(onProfilesSaveAsButtonPressed(QString))); // the signal from the profiles signals our signal which will go to the inventory

        // inventory
        connect(this, SIGNAL(profilePathUpdated(QString)), ui->inventory, SLOT(_onSelectFile(QString)));
        connect(this, SIGNAL(writeToProfile(QString)), ui->inventory, SLOT(_PushInventoryToProfile(QString)));
    }

    // set widgets
    ui->profiles->setSavesDirectory(savesDirectory);

    // update state depending on valid path
    widgetLock(checkGameDirPath(gameDirectory));

    // load the profile at the game path
    if (checkGameDirPath(gameDirectory)) {
        // get the current save file
        QString profilePath = gameDirectory;
        profilePath.chop(12);
        profilePath += "Profile.dat";

        // check if the file exists
        QFile checkFile(profilePath);
        if (!checkFile.isOpen()) {
            qDebug() << "mainwindow.cpp: coudln't open the current profile on startup";
        }
        else {
            emit profilePathUpdated(profilePath);
        }

        emit profilePathUpdated(profilePath);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

//====================================
// METHODS
//====================================

// save/load
void MainWindow::saveSettings() {
    QSettings settings("JordanV", "CaveStoryProfileManager");
    settings.setValue("gameDirectory", gameDirectory);
    qDebug() << "mainwindow.cpp: Settings saved, gameDirectory =" << gameDirectory;
}

// given the path of the game directory, check if its real and then give a popup
bool MainWindow::checkGameDirPath(QString path) {
    // check if we're a doukutsu exe
    QFileInfo fileInfo(path);
    if (fileInfo.fileName() != "Doukutsu.exe") {
        qDebug() << "mainwindowmethods.cpp: The selected file is not Doukutsu.exe. It is:" << fileInfo.fileName();
        Dialog error_box(this);
        error_box.setStackedWidgetPage("directoryErrorBox");
        error_box.exec();

        return false;
    }

    // we good!
    // qDebug() << "mainwindowmethods.cpp: The selected file is a valid Doukutsu.exe.";
    gameDirectory = path;




    return true;
}


// if enable is true, enable widgets, if false, disable widgets
void MainWindow::widgetLock(bool enable) {
    // file systems
    ui->profiles->widgetLock(enable);

    // enables
    ui->runPushButton->setEnabled(enable);
    // ui->profilesGrBox->setEnabled(enable);
    ui->inventoryGrBox->setEnabled(enable);

    // ui elements
    if (enable) {
        ui->exeDirLabel->setText("exe directory: " + gameDirectory);
    }
    else {
        ui->exeDirLabel->setText("No Doukutsu.exe path set!");
    }
}

void MainWindow::disableInventory(bool enable) {
    // turn on / off inventory and apply button
    isEnabled = enable;
    ui->inventory->setEnabled(enable);
    ui->runPushButton->setEnabled(enable);
    ui->beginPromptLabel->setVisible(!enable);
}

//====================================
// SLOTS
//====================================

// just running
void MainWindow::onSimpleRunButtonPressed() {
    // launch game
    qDebug() << "mainwindowslots: launching game at: " << gameDirectory;
    if (!checkGameDirPath(gameDirectory)) {
        qDebug() << "mainwindowslots: ERROR, game directory incorrect!";
        return;
    }

    QProcess *process = new QProcess(this);
    QString gameExeFile = gameDirectory;
    process->setProgram(gameExeFile);
    process->start();
}

// when you click on the apply and run button
void MainWindow::onRunButtonPressed() {
    // request the inventory to update Profile
    // emit applyButtonPressed(gameDirectory);

    // get save file and tell the inventory to push new data
    QString profilePath = gameDirectory;
    profilePath.chop(12);
    profilePath += "Profile.dat";

    emit writeToProfile(profilePath);

    // launch game!
    onSimpleRunButtonPressed();
}

// when you click the help button
void MainWindow::onHelpButtonPressed() {
    Dialog error_box(this);
    error_box.setStackedWidgetPage("helpScreen");
    error_box.exec();
}

// when you click the update directory button
void MainWindow::onUpdateDirectoryButtonPressed() {
    // create a file select
    QString selectedFile = QFileDialog::getOpenFileName(
        nullptr,                // Parent widget (nullptr for no parent)
        "Select Doukutsu executable",          // Dialog title
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),       // Initial directory
        "Executable Files (*.exe);;All Files (*)" // File filter
        );

    qDebug() << "mainwindowslots.cpp: double check exe file: " << selectedFile;

    // double check that it is Doukutsu
    if (!checkGameDirPath(selectedFile)) {
        qDebug() << "mainwindowslots.cpp: new game dir file invalid!";
        return;
    }

    // update game state
    widgetLock(true);

    // save game
    saveSettings();
}

// signal to signal handlers
void MainWindow::onProfilesSaveFilePressed(QString profilePath) {
    emit profilePathUpdated(profilePath);
}

void MainWindow::onProfilesSaveAsButtonPressed(QString savePath) {
    emit writeToProfile(savePath);
}






















