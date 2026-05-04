#include "mainwindow.h"
#include "globals.h"
#include "./ui_mainwindow.h"

#include <QFileSystemModel>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    savesDirectory = QCoreApplication::applicationDirPath() + "/Saves";
    qDebug() << "mainwindow.cpp: savesDirectory: " << savesDirectory;

    // set stuff
    // setGlobals();
    loadSettings();
    setSignals();

    // set widgets
    createFileTrees();
    createProfilesAnimation();

    // update state depending on valid path
    widgetLock(checkGameDirPath(gameDirectory));

    // load the profile at the game path
    if (checkGameDirPath(gameDirectory)) {
        // get the current save file
        QString profilePath = gameDirectory;
        profilePath.chop(12);
        profilePath += "Profile.dat";

        // check if the file exists
        QFile checkFile(profilePath);
        if (!checkFile.isOpen()) {
            qDebug() << "mainwindow.cpp: coudln't open the current profile on startup";
        }
        else {
            emit profilePathUpdated(profilePath);
        }

        emit profilePathUpdated(profilePath);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// save/load
void MainWindow::saveSettings() {
    QSettings settings("JordanV", "CaveStoryProfileManager");
    settings.setValue("gameDirectory", gameDirectory);
    qDebug() << "mainwindow.cpp: Settings saved, gameDirectory =" << gameDirectory;
}

void MainWindow::loadSettings() {
    QSettings settings("JordanV", "CaveStoryProfileManager");

    if (!settings.contains("gameDirectory")) {
        qDebug() << "mainwindow.cpp: settings don't contain 'gameDirectory'";
        return;
    }

    QString tryDirectory = settings.value("gameDirectory").toString();

    // double check file directory
    if (!checkGameDirPath(tryDirectory)) return;

    // if our dir is good!
    qDebug() << "mainwindow.cpp: Settings loaded, gameDirectory =" << gameDirectory;
}

void MainWindow::setSignals() {
    // buttons
    connect(ui->launchPushButton, SIGNAL(clicked(bool)), this, SLOT(onSimpleRunButtonPressed()));
    connect(ui->runPushButton, SIGNAL(clicked(bool)), this, SLOT(onRunButtonPressed()));
    connect(ui->HelpPushButton, SIGNAL(clicked(bool)), this, SLOT(onHelpButtonPressed()));
    connect(ui->updateDirPushButton, SIGNAL(clicked(bool)), this, SLOT(onUpdateDirectoryButtonPressed()));
    connect(ui->profilesCollapseButton, SIGNAL(clicked(bool)), this, SLOT(onProfilesCollapsed()));

    // profile selections
    connect(ui->profiles, SIGNAL(saveFilePressed(QString)), this, SLOT(onProfilesSaveFilePressed(QString)));
    connect(ui->profiles, SIGNAL(saveAsButtonPressed(QString)), this, SLOT(onProfilesSaveAsButtonPressed(QString))); // the signal from the profiles signals our signal which will go to the inventory
    // connect(ui->profiles, SIGNAL(profilesCollapsed(bool)), this, SLOT(onProfilesCollapsed(bool)));

    // inventory
    connect(this, SIGNAL(profilePathUpdated(QString)), ui->inventory, SLOT(_onSelectFile(QString)));
    connect(this, SIGNAL(writeToProfile(QString)), ui->inventory, SLOT(_PushInventoryToProfile(QString)));
}

// set widgets
void MainWindow::createFileTrees() {
    ui->profiles->setSavesDirectory(savesDirectory);
}

/*
Code modified from: https://github.com/MichaelVoelkel/qt-collapsible-section/tree/master
Specifically for expanding and collapsing a widget
*/
void MainWindow::createProfilesAnimation() {
    // set bool
    profilesCollapsed = false;

    // add animations
    profilesAnimation = new QParallelAnimationGroup(this);
    profilesAnimation->addAnimation(new QPropertyAnimation(ui->profilesGrBox, "maximumWidth"));
    profilesAnimation->addAnimation(new QPropertyAnimation(ui->profilesGrBox, "minimumWidth"));
    profilesAnimation->addAnimation(new QPropertyAnimation(ui->profiles->getProfilesGroup(), "maximumWidth"));

    // constants
    int animationDuration = 500;
    int collapsedWidth = 50;
    int contentWidth = ui->profilesGrBox->width();

    qDebug() << "check widths: " << contentWidth << ", real: " << ui->profiles->width();

    // iterate through animations and set values
    for (int i = 0; i < profilesAnimation->animationCount() - 1; ++i)
    {
        QPropertyAnimation* SectionAnimation = static_cast<QPropertyAnimation *>(profilesAnimation->animationAt(i));
        SectionAnimation->setDuration(animationDuration);
        SectionAnimation->setStartValue(collapsedWidth);
        SectionAnimation->setEndValue(contentWidth);
    }

    // set the values for the content animation
    QPropertyAnimation* contentAnimation = static_cast<QPropertyAnimation *>(profilesAnimation->animationAt(profilesAnimation->animationCount() - 1));
    contentAnimation->setDuration(animationDuration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentWidth);
}
