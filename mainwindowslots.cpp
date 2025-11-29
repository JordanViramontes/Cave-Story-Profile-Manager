#include "mainwindow.h"
#include "ui_mainwindow.h"
// #include "ui_"

#include <QFileDialog>
#include <QStandardPaths>
#include <qabstractitemmodel.h>
#include <QModelIndex>
#include <qfilesystemmodel.h>

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

void MainWindow::_onSelectFile(QModelIndex fileIndex) {
    // get the file path
    auto tab = ui->filesTabWidget->currentIndex();
    QFileSystemModel* model = nullptr;

    switch (tab) {
    case (0): model = qobject_cast<QFileSystemModel*>(ui->presetFileTree->model()); break;
    case (1): model = qobject_cast<QFileSystemModel*>(ui->customFileTree->model());
    default: break;
    }

    if (!model) {
        qDebug() << "mainwindowslots.cpp: QTreeView model invalid!";
        return;
    }

    qDebug() << "check file: " << model->filePath(fileIndex).toStdString();

    //
}
