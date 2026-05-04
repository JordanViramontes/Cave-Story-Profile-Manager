#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "profileloader.h"
#include "dialog.h"
// #include "qweapontableslot.h"
// #include "ui_"

#include <QFileDialog>
#include <QStandardPaths>
#include <qabstractitemmodel.h>
#include <QModelIndex>
#include <qfilesystemmodel.h>
#include <QProcess>

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

// // save as button uses this
// void MainWindow::onApplyButtonPressed() {
//     return;
//     // get save file
//     QString profilePath = gameDirectory;
//     profilePath.chop(12);
//     profilePath += "Profile.dat";

//     emit writeToProfile(profilePath);
// }

// signal to signal handlers
void MainWindow::onProfilesSaveFilePressed(QString profilePath) {
    emit profilePathUpdated(profilePath);
}

void MainWindow::onProfilesSaveAsButtonPressed(QString savePath) {
    emit writeToProfile(savePath);
}
