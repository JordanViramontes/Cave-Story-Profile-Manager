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

// given a weapon table slot, find the index of the table where its living
int MainWindow::findTableWidgetIndex(const QWeaponTableSlot* weaponSlot) {
    QTableWidget* table = ui->weaponsTable;
    for (int i = 0; i < table->rowCount(); i++) {
        if (table->cellWidget(i, 0) == weaponSlot) {
            return i;
        }
    }

    return -1;
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

// given a vector of weapons in the order we want in the table, reorder the table!
void MainWindow::reorderTable(QVector<int> weapons) {
    QTableWidget *table = ui->weaponsTable;

    // insert weapons.size amount of new rows at the top (will be filled later)
    int newRowAmount = weapons.size();
    for (int i = 0; i < newRowAmount; i++) { table->insertRow(i); }

    // move the rows from where they were to where they need to be
    int rowIt = 0;
    for (auto i : weapons) {
        // qDebug() << "mainwindowmethods.cpp: reordering: " << i;

        // get row index of the widget
        QWeaponTableSlot* weapon = weaponsTableDictionary[i];
        int index = findTableWidgetIndex(weapon);

        if (index < 0) {
            qDebug() << "mainwindowmethods.cpp: ERROR, INDEX OF OLD ROW DOESNT EXIST!!";
            return;
        }

        // set the cell and then remove the old row
        table->setCellWidget(rowIt, 0, weapon);
        table->removeRow(index);

        // update ui stuff
        table->setRowHeight(rowIt, weapon->sizeHint().height());
        weapon->clearFocus();

        rowIt++;
    }

    table->viewport()->update();
    table->updateGeometry();
}





















