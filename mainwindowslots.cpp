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
    parser.printBuffer();

    // update widgets
    ui->saveAsEdit->setText(QFileInfo(filePath).fileName());

    // get the enabled weapons!
    QVector<int> enabledWeapons;
    for (auto i : parser.getWeapons()) {
        if (i.type == 0x00) continue;
        // add to known weapons
        enabledWeapons.push_back((int)i.type);
    }

    // update the weapons table via parser information
    ui->weaponsTable->setWeaponsFromParser(parser.getWeapons(), enabledWeapons);

    // update weapons order table
    ui->weaponOrderTable->setAllSlots(enabledWeapons);

    // update selected weapon
    _onUpdateSelectWeaponChoices(ui->weaponsTable->getValidEnabledWidgets());
    ui->selectedWeaponCombo->setCurrentIndex(parser.getCurrentWeapon());
}

// updating the select weapon stuff
void MainWindow::_onUpdateSelectWeaponChoices(QVector<int> weapons) {
    // dictionary for later
    QHash<int, QString> weaponTextDictionary = {
        {0x01, "Snake"},
        {0x02, "Polar Star"},
        {0x03, "Fireball"},
        {0x04, "Machine Gun"},
        {0x05, "Missile Launcher"},
        {0x07, "Bubbler"},
        {0x09, "Blade"},
        {0x0A, "Super Missile Launcher"},
        {0x0C, "Nemesis"},
        {0x0D, "Spur"},
    };

    // get the combo box
    QComboBox * combo = ui->selectedWeaponCombo;

    // get the current selection
    QString prevSelection = "";
    if (combo->count() > 0) prevSelection = combo->currentText();

    // resize combo box and add contents
    combo->clear();
    for (auto i : weapons) {
        combo->addItem(weaponTextDictionary[i]);
    }

    // if prev selection is still here keep it!
    for (int i = 0; i < combo->count(); i++) {
        if (combo->itemText(i) == prevSelection) {
            combo->setCurrentIndex(i);
            break;
        }
    }

    // change the color of the selected thing!
    ui->weaponOrderTable->setHighlightedSlot(combo->currentIndex());
}








