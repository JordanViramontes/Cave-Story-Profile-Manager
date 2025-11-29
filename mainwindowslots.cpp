#include "mainwindow.h"
#include "globals.h"
#include "dialog.h"
#include "ui_mainwindow.h"
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
    QFileInfo fileInfo(selectedFile);
    if (fileInfo.fileName() != "Doukutsu.exe") {
        qDebug() << "mainwindowslots.cpp: The selected file is not Doukutsu.exe. It is:" << fileInfo.fileName();
        auto error_box = new Dialog(this);
        error_box->setStackedWidgetPage("directoryErrorBox");
        error_box->show();
        return;
    }

    // we good!
    qDebug() << "mainwindowslots.cpp: The selected file is a valid Doukutsu.exe.";
    GAMEDIRECTORY = selectedFile;

    // update label
    ui->exeDirLabel->setText(".exe directory: " + GAMEDIRECTORY);


}


