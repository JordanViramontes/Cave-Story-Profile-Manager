#include "mainwindow.h"
#include "ui_mainwindow.h"

bool MainWindow::checkGameDirPath(QString path) {
    qDebug() << "check: " << path << ";\t" << path.right(12);

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
    qDebug() << "mainwindowmethods.cpp: The selected file is a valid Doukutsu.exe.";
    gameDirectory = path;

    // update ui label
    ui->exeDirLabel->setText("exe directory: " + path);


    return true;
}
