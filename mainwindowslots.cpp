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
void MainWindow::_onSimpleRunButton() {
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
void MainWindow::_onRunButton() {
    // request the inventory to update Profile
    emit applyButtonPressed(gameDirectory);

    // launch game!
    _onSimpleRunButton();
}

// when you click the help button
void MainWindow::_onHelpButton() {
    Dialog error_box(this);
    error_box.setStackedWidgetPage("helpScreen");
    error_box.exec();
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
    // enable stuff
    if (!isEnabled) disableInventory(true);

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

    // update widgets
    ui->saveAsEdit->setText(QFileInfo(filePath).fileName().chopped(4));

    // signal to the inventory our new file!
    emit profilePathUpdated(filePath);
}

void MainWindow::_onSaveAsNewFile() {
    // get new file name
    QString newFileName = ui->saveAsEdit->toPlainText();

    // custom save file path
    QString newFilePath = savesDirectory + "\\Custom\\" + newFileName + ".dat";

    // check that our customs folder exists
    QDir customsDir(savesDirectory + "\\Custom\\");
    if (!customsDir.exists()) {
        qDebug() << "mainwindowslots.cpp: custom directory wasn't created, making a new one";
        customsDir.mkpath(".");
    }

    // check that file already exists
    if (QFile::exists(newFilePath)) {
        Dialog error_box(this);
        error_box.setStackedWidgetPage("fileAlreadyExistsBox");
        int result = error_box.exec();

        // in Dialog.cpp, clicking overwrite outputs rejected, so if we don't click, overwrite
        if (result != QDialog::Rejected) {
            return;
        }
    }

    // actually create the new file
    emit writeToProfile(newFilePath);
}

// save as button
void MainWindow::_onSaveAsButtonPressed() {
    // get save file
    QString profilePath = gameDirectory;
    profilePath.chop(12);
    profilePath += "Profile.dat";

    emit writeToProfile(profilePath);
}




