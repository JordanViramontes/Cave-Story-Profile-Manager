#include "mainwindow.h"
#include "globals.h"
#include "./ui_mainwindow.h"
#include "widgetfunctions.h"

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

        // test
        tryDirectory = "";

        // double check file directory
        isEnabled = checkGameDirPath(tryDirectory);

        if (isEnabled) { qDebug() << "mainwindow.cpp: Settings loaded, gameDirectory =" << gameDirectory; }
        else           { qDebug() << "mainwindow.cpp: Settings NOT loaded"; }
    }

    // Connections
    {
        // buttons
        connect(ui->launchPushButton, SIGNAL(clicked(bool)), this, SLOT(onSimpleRunButtonPressed()));
        connect(ui->runPushButton, SIGNAL(clicked(bool)), this, SLOT(onApplyAndRunButtonPressed()));
        connect(ui->HelpPushButton, &QPushButton::clicked, this, [this]() { runDialogBox(this, "helpScreen"); });
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
    importantWidgets = {
        ui->runPushButton,
        ui->launchPushButton,
        ui->inventoryGrBox,
        ui->profiles,
        ui->inventory,
    };

    // update state depending on valid path
    setUIfromEnabled(isEnabled);

    // load the profile at the game path
    if (isEnabled) {
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
    }

    // update that we're done with this!
    initialStartup = false;
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
        if (!initialStartup) runDialogBox(this, "directoryErrorBox");

        return false;
    }

    // we good!
    // qDebug() << "mainwindowmethods.cpp: The selected file is a valid Doukutsu.exe.";
    gameDirectory = path;

    return true;
}

void MainWindow::setUIfromEnabled(bool state) {
    // update states
    isEnabled = state;
    widgetLock(isEnabled, importantWidgets);

    // upate ui
    if (isEnabled) {
        ui->exeDirLabel->setText("exe directory: " + gameDirectory);
    }
    else {
        ui->exeDirLabel->setText("No Doukutsu.exe path set!");
    }
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
void MainWindow::onApplyAndRunButtonPressed() {
    // get save file and tell the inventory to push new data
    QString profilePath = gameDirectory;
    profilePath.chop(12);
    profilePath += "Profile.dat";

    emit writeToProfile(profilePath);

    // launch game!
    onSimpleRunButtonPressed();
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
    setUIfromEnabled(true);

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






















