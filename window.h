#ifndef WINDOW_H
#define WINDOW_H

#include "profileloader.h"
#include "qmainwindow.h"
#include "qmessagebox.h"
#include "qpushbutton.h"
#include "qtooltip.h"
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
    QFileSystemModel *fileSystem;
    QString exeDirectory = "";
    QString saveDirectory = "";
    QString settingsFile = "";
    void saveSettings();
    void loadSettings();
    bool isFirstOpen = true;


    // widgets
    TableWidgetDragRows * tableWidget;
    QLineEdit * AmmoEdit;
    QSlider *slider;
    QLabel *validLabel;
    QLabel *tempDirectoryLabel;

    // buttons and stuff
    QPushButton *quickApplyButton;

    // loading
    ProfileLoader profileLoader = ProfileLoader();
    QSettings *appSettings;

    // flag stuff
    bool isStartingUp = true;
    bool canUseProfile = true;
    bool isDirectoryOkay = false;
    void disableWidgets() {
        quickApplyButton->setEnabled(false);
        tableWidget->setEnabled(false);
        AmmoEdit->setEnabled(false);
        slider->setEnabled(false);
    }
    void enableWidgets() {
        quickApplyButton->setEnabled(true);
        tableWidget->setEnabled(true);
        AmmoEdit->setEnabled(true);
        slider->setEnabled(true);
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
        if (!profileLoader.parseProfile(fileSystem->filePath(index).toStdString())) {
            // qDebug() << "failed";
            emit updateCanUseProfile(false);
            return;
        }
        emit updateCanUseProfile(true);

        // set ammo
        int ammoMax = profileLoader.getMaxAmmo();
        slider->setValue(ammoMax);
        emit slider->sliderMoved(ammoMax);

        // set weapon table
        QVector<WeaponSlot> weapons = profileLoader.getWeapons();
        tableWidget->newWeaponTable(weapons);
    }

    // quick apply (apply profile and launch game)
    void quickApplySlot() {
        // update profile contents
        QVector<WeaponSlot> newWeapons = tableWidget->getWeapons();

        // profileLoader.printContents();
        profileLoader.setWeapons(newWeapons);
        profileLoader.updateBuffer();



        QVector<char> bufferVec = profileLoader.getBuffer();
        char buffer[1540];
        for (unsigned int i = 0; i < bufferVec.size(); i++) {
            buffer[i] = bufferVec.at(i);
        }
        qDebug() << "buffer size: " << bufferVec.size();

        // write to the profile in the directory
        int end = exeDirectory.size()-13;
        std::string profileLocation = exeDirectory.toStdString().erase(end, 13) + "/Profile.dat";
        // qDebug() << "LOCATION: " << profileLocation;
        std::ofstream file(profileLocation, std::ios::binary);

        if (!file.is_open()) {
            qDebug() << "Profile.dat ostream not open";
        }

        qDebug() << "writing to: " << profileLocation;

        file.write(buffer, sizeof(buffer));

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

signals:
    void updateAmmoFromProfile(int n);
    void updateCanUseProfile(bool b);
    void needToUpdateDirectorySignal();
};

#endif // WINDOW_H
