#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "profileloader.h"
// #include "ui_"

#include <QFileDialog>
#include <QStandardPaths>
#include <qabstractitemmodel.h>
#include <QModelIndex>
#include <qfilesystemmodel.h>
#include <QProcess>

// when you click on the run button
void MainWindow::_onRunButton() {

    // write to the save file
    QString profilePath = gameDirectory;
    profilePath.chop(12);
    profilePath += "Profile.dat";

    if (!parser.writeToFile(profilePath)) {
        qDebug() << "mainwindowslots.cpp: Writing to file DID NOT complete";
        return;
    }
    // qDebug() << "mainwindowslots.cpp: Writing to file completed with no error";

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

// when you click the update directory button
void MainWindow::_onUpdateDirectoryButton() {
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

// when you click on a file in the file tree
void MainWindow::_onSelectFile(QModelIndex fileIndex) {
    // get the file path
    auto tab = ui->filesTabWidget->currentIndex();
    QFileSystemModel* model = nullptr;

    // get the model depending on which tab we're on
    switch (tab) {
        case (0): model = qobject_cast<QFileSystemModel*>(ui->presetFileTree->model()); break;
        case (1): model = qobject_cast<QFileSystemModel*>(ui->customFileTree->model()); break;
        default: break;
    }
    if (!model) {
        qDebug() << "mainwindowslots.cpp: QTreeView model invalid!";
        return;
    }

    // parse the file now that we have the filepath
    QString filePath = model->filePath(fileIndex);
    // qDebug() << "check file: " << filePath;

    if (!parser.parseProfile(filePath)) {
        qDebug() << "mainwindowslots.cpp: Parsing DID NOT complete";
        return;
    }
    // qDebug() << "mainwindowslots.cpp: Parsing completed with no error";

    parser.printSaveData();
    // parser.printBuffer();

    // update widgets
    ui->saveAsEdit->setText(QFileInfo(filePath).fileName());

    // update the weapons table via parser information
    ui->weaponsTable->setWeaponsFromParser(parser.getWeapons());
}










