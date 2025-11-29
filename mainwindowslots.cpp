#include "mainwindow.h"
// #include "ui_"

#include <QFileDialog>
#include <QStandardPaths>

// slots
void MainWindow::_onRunButton() {
    qDebug() << "TODO: LAUNCH GAME";
}

void MainWindow::_onUpdateDirectoryButton() {
    QString selectedFile = QFileDialog::getOpenFileName(
        nullptr,                // Parent widget (nullptr for no parent)
        "Select Doukutsu executable",          // Dialog title
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),       // Initial directory
        "Executable Files (*.exe);;All Files (*)" // File filter
        );

    qDebug() << "mainwindowslots.cpp: double check exe file: " << selectedFile;

    // double check that it is Doukutsu
    checkGameDirPath(selectedFile);

    // save game
    saveSettings();
}


