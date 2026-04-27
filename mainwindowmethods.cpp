#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"

#include <QTableWidget>

// given the path of the game directory, check if its real and then give a popup
bool MainWindow::checkGameDirPath(QString path) {
    // check if we're a doukutsu exe
    QFileInfo fileInfo(path);
    if (fileInfo.fileName() != "Doukutsu.exe") {
        qDebug() << "mainwindowmethods.cpp: The selected file is not Doukutsu.exe. It is:" << fileInfo.fileName();
        auto error_box = new Dialog(this);
        error_box->setStackedWidgetPage("directoryErrorBox");
        error_box->show();

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
    ui->presetFileTree->setMouseTracking(enable);
    ui->customFileTree->setMouseTracking(enable);
    ui->presetFileTree->setFocusPolicy(Qt::NoFocus);

    // enables
    ui->runPushButton->setEnabled(enable);
    ui->profilesGrBox->setEnabled(enable);
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






















