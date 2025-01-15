#ifndef WINDOW_H
#define WINDOW_H

#include "profileloader.h"
#include "qmainwindow.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "tablewidgetdragrows.h"

#include "qabstractitemmodel.h"
#include "qfilesystemmodel.h"
#include "qlineedit.h"
#include <QWidget>
#include <QFileInfo>
#include <fstream>
#include <QProcess>
#include <QSettings>
#include <QFileDialog>

class Window : public QWidget
{
    Q_OBJECT
private:
    QWidget * createFileWidget();
    QWidget * createImageLoader();
    QWidget * createScrollArea();
    QWidget * createButtonArea();
    QMenuBar * createMenuBar();

    // directories and file management
    QFileSystemModel *presetProfileModel = nullptr;
    QString exeDirectory = "";
    QString saveDirectory = "";
    QString settingsFile = "";
    void saveSettings();
    void loadSettings();
    bool isFirstOpen = true;


    // widgets
    TableWidgetDragRows * tableWidget = new TableWidgetDragRows(this);;
    QLineEdit * AmmoEdit = new QLineEdit(this);;
    QSlider *slider = nullptr;
    QLabel *validLabel = new QLabel("Directory to Doukutsu.exe Invalid!!\nPlease update the directory:", this);;
    QLabel *tempDirectoryLabel = new QLabel(".exe Directory:\n" + exeDirectory, this);
    QTabWidget *fileTabs = nullptr;
    QComboBox * currentWeapon = nullptr;

    // buttons and stuff
    QPushButton *quickApplyButton = new QPushButton(this);;

    // loading and saving
    ProfileLoader profileLoader = ProfileLoader();
    QLineEdit * saveEdit = nullptr;
    QPushButton *saveButton = nullptr;
    void saveToFile(QString filePath) {
        // update profile contents
        QVector<WeaponSlot> newWeapons = tableWidget->getWeapons();
        // qDebug() << "curr: " << currentWeapon->currentIndex() + 1;
        // qDebug() << "size: " << newWeapons.size();
        if (currentWeapon->currentIndex() + 1 > newWeapons.size()) {
            currentWeapon->setCurrentIndex(newWeapons.size()-1);
        }

        // profileLoader.printContents();
        profileLoader.setWeapons(newWeapons);
        profileLoader.setCurrentWeapon(currentWeapon->currentIndex());
        profileLoader.updateBuffer();



        QVector<char> bufferVec = profileLoader.getBuffer();
        char buffer[1540];
        for (unsigned int i = 0; i < bufferVec.size(); i++) {
            buffer[i] = bufferVec.at(i);
        }
        // qDebug() << "buffer size: " << bufferVec.size();

        // write to the profile in the directory
        std::ofstream file(filePath.toStdString(), std::ios::binary);

        if (!file.is_open()) {
            qDebug() << "Profile.dat ostream not open";
        }

        qDebug() << "writing to: " << filePath;

        file.write(buffer, sizeof(buffer));
    }

    // flag stuff
    bool isStartingUp = true;
    bool canUseProfile = true;
    bool isDirectoryOkay = false;
    void disableWidgets() {
        quickApplyButton->setEnabled(false);
        tableWidget->setEnabled(false);
        AmmoEdit->setEnabled(false);
        slider->setEnabled(false);
        saveEdit->setEnabled(false);
        saveButton->setEnabled(false);
        currentWeapon->setEnabled(false);
    }
    void enableWidgets() {
        quickApplyButton->setEnabled(true);
        tableWidget->setEnabled(true);
        AmmoEdit->setEnabled(true);
        slider->setEnabled(true);
        saveEdit->setEnabled(true);
        saveButton->setEnabled(true);
        currentWeapon->setEnabled(true);
    }


public:
    explicit Window(QWidget *parent = nullptr);

private slots:
    void updateAmmoEdit(int n) {
        AmmoEdit->setText(QString::number(n));
    }

    // when clicked get a profileloader
    void getProfileDirectory(QModelIndex index) {
        // parse file
        if (!profileLoader.parseProfile(presetProfileModel->filePath(index).toStdString())) {
            // qDebug() << "failed";
            emit updateCanUseProfile(false);
            return;
        }
        emit updateCanUseProfile(true);

        // set ammo
        int ammoMax = profileLoader.getMaxAmmo();
        slider->setValue(ammoMax);
        emit slider->sliderMoved(ammoMax);

        // set selected weapon
        int currWeaponFromProfile = profileLoader.getCurrentWeapon();
        currentWeapon->setCurrentIndex(currWeaponFromProfile);

        // set weapon table
        QVector<WeaponSlot> weapons = profileLoader.getWeapons();
        tableWidget->newWeaponTable(weapons);

        // set save labels
        QString saveStr = presetProfileModel->fileName(index);
        saveStr.erase(saveStr.end()-4, saveStr.end());
        saveEdit->setText(saveStr);
    }

    // quick apply (apply profile and launch game)
    void quickApplySlot() {
        QString profileLocation = exeDirectory;
        profileLocation.erase(profileLocation.end()-13, profileLocation.end());
        profileLocation += "/Profile.dat";
        saveToFile(profileLocation);

        // launch game
        QProcess *process = new QProcess(this);
        QString gameExeFile = exeDirectory;
        process->setProgram(gameExeFile);
        process->start();
    }

    // update UI when user clicks a profile
    void updateWidgetEnabled(bool canUpdate) {
        qDebug() << "a";
        // double check that it is Doukutsu
        QFileInfo fileInfo(exeDirectory);

        if (fileInfo.fileName() != "Doukutsu.exe" || !fileInfo.exists()) {
            isDirectoryOkay = false;
        }

        // dont need to do work if we dont have to
        if (!isDirectoryOkay) {
            if (validLabel) validLabel->setVisible(true);
            disableWidgets();
            return; // has priority
        }

        if (validLabel) validLabel->setVisible(false);

        canUseProfile = canUpdate;

        qDebug() << "canUse: " << canUseProfile;

        if (!canUseProfile) {
            disableWidgets();

            // Create an error message box
            QMessageBox * errorBox = new QMessageBox(this);
            errorBox->setIcon(QMessageBox::Warning);
            errorBox->setText("Profile selected is Invalid!");
            errorBox->setWindowTitle("Error");
            errorBox->setStandardButtons(QMessageBox::Ok);
            errorBox->exec();

        }
        else {
            qDebug() << "enabling";
            enableWidgets();
        }
    }

    // choose a new folder for the exe file
    void updateExeDirectory() {
        QString selectedFile = QFileDialog::getOpenFileName(
            nullptr,                // Parent widget (nullptr for no parent)
            "Select File",          // Dialog title
            QDir::homePath(),       // Initial directory
            "Executable Files (*.exe);;All Files (*)" // File filter
            );

        qDebug() << "double check: " << selectedFile;

        // double check that it is Doukutsu
        QFileInfo fileInfo(selectedFile);

        if (fileInfo.fileName() == "Doukutsu.exe") {
            qDebug() << "The selected file is Doukutsu.exe.";
            exeDirectory = selectedFile;
            isDirectoryOkay = true;
            emit updateCanUseProfile(true);
            saveSettings();
            tempDirectoryLabel->setText(".exe Directory:\n" + exeDirectory);
            return;
        }
        else {
            qDebug() << "The selected file is not Doukutsu.exe. It is:" << fileInfo.fileName();
            isDirectoryOkay = false;

            // Create an error message box
            QMessageBox * errorBox = new QMessageBox(this);
            errorBox->setIcon(QMessageBox::Warning);
            errorBox->setText("Directory selected is Invalid!");
            errorBox->setDetailedText("You must select the \'Doukutsu.exe\' application to validate");
            errorBox->setWindowTitle("Error");
            errorBox->setStandardButtons(QMessageBox::Ok);
            errorBox->exec();

        }


    }

    // saving a new file
    void slot_saveNewProfile() {
        QString filePath = saveDirectory + "/Custom/" + saveEdit->text() + ".dat";

        QModelIndex index = presetProfileModel->index(filePath);

        // check for existance and then do warning stuff if it exists
        if (index.isValid()) {
            QMessageBox helpBox = QMessageBox();
            QString helpStr = "This file already exists!, Do you want to Overwrite it?";
            helpBox.setWindowTitle("File Conflict");
            helpBox.setIcon(QMessageBox::Warning);
            helpBox.setStandardButtons(QMessageBox::Yes);
            helpBox.addButton(QMessageBox::Cancel);
            helpBox.setText(helpStr);
            helpBox.exec();

            QAbstractButton *clickedButton = helpBox.clickedButton();
            if (clickedButton) {
                if (clickedButton->text() == "Cancel") return;
            }
        }

        // save to file
        saveToFile(filePath);

    }

signals:
    void updateAmmoFromProfile(int n);
    void updateCanUseProfile(bool b);
    void needToUpdateDirectorySignal();
};

#endif // WINDOW_H
