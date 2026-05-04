#include "profilesselection.h"
#include "ui_profilesselection.h"
#include "dialog.h"

#include <QFileSystemModel>
#include <QPropertyAnimation>

ProfilesSelection::ProfilesSelection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfilesSelection)
{
    ui->setupUi(this);

    // connections
    connect(ui->presetFileTree, SIGNAL(pressed(QModelIndex)), this, SLOT(onPressedFile(QModelIndex)));
    connect(ui->customFileTree, SIGNAL(pressed(QModelIndex)), this, SLOT(onPressedFile(QModelIndex)));
    connect(ui->saveAsPushButton, SIGNAL(clicked(bool)), this, SLOT(onSaveAsButtonPressed()));
    connect(ui->collapseButton, SIGNAL(clicked(bool)), this, SLOT(onCollapseButtonPressed()));

    // etc
    ui->presetFileTree->setFocusPolicy(Qt::NoFocus);
    ui->customFileTree->setFocusPolicy(Qt::NoFocus);

    // variables
    profilesCollapsed = false;

    /*
    The following code modified from: https://github.com/MichaelVoelkel/qt-collapsible-section/tree/master
    Specifically for expanding and collapsing a widget
    */

    // add animations, all of these will run together in parallel
    profilesAnimation = new QParallelAnimationGroup(this);
    profilesAnimation->addAnimation(new QPropertyAnimation(this, "maximumWidth"));
    profilesAnimation->addAnimation(new QPropertyAnimation(this, "minimumWidth"));
    auto subAnimation = new QPropertyAnimation(ui->profilesContent, "maximumWidth");
    profilesAnimation->addAnimation(subAnimation);

    // constants
    int animationDuration = 300;
    int mainCollapsedWidth = 45;
    int mainContentWidth = width();
    ui->profilesContent->adjustSize(); // set size before getting width so we get the right constant
    int subWidth = ui->profilesContent->width();

    // set up all animations except for the sub animation, assuming its going from collapsed->expanded
    for (int i = 0; i < profilesAnimation->animationCount() - 1; ++i)
    {
        QPropertyAnimation* SectionAnimation = static_cast<QPropertyAnimation *>(profilesAnimation->animationAt(i));
        SectionAnimation->setDuration(animationDuration);
        SectionAnimation->setStartValue(mainCollapsedWidth);
        SectionAnimation->setEndValue(mainContentWidth);
    }

    // set the values for the subanimation
    subAnimation->setDuration(animationDuration);
    subAnimation->setStartValue(0);
    subAnimation->setEndValue(subWidth);

    // when an animation finishes, hide the sub content
    connect(profilesAnimation, SIGNAL(finished()), this, SLOT(animationFinished()));
}

ProfilesSelection::~ProfilesSelection()
{
    delete ui;
}

void ProfilesSelection::setSavesDirectory(QString newSavesDirectory) {
    // create file trees
    QFileSystemModel* presetProfileModel = new QFileSystemModel(this);
    presetProfileModel->setRootPath(QDir::rootPath()); // default path

    // update to correct saves file if it exists
    if (QDir(newSavesDirectory).exists()) {
        savesDirectory = newSavesDirectory;
        presetProfileModel->setRootPath(savesDirectory);
    }
    else qDebug() << "mainwindow.cpp: ERROR, path to saves folder incorrect, setting to root";

    // set preset and column tabs
    for (auto i : {ui->presetFileTree, ui->customFileTree}) {
        i->setModel(presetProfileModel);
        i->hideColumn(1);
        i->hideColumn(2);
        i->hideColumn(3);
        i->setHeaderHidden(true);
        if (i == ui->presetFileTree)
            i->setRootIndex(presetProfileModel->index(savesDirectory + "\\Presets"));
        else
            i->setRootIndex(presetProfileModel->index(savesDirectory + "\\Custom"));
    }
}

void ProfilesSelection::onPressedFile(QModelIndex fileIndex) {
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

    // get the file path
    QString filePath = model->filePath(fileIndex);

    // update widgets
    ui->saveAsEdit->setText(QFileInfo(filePath).fileName().chopped(4));

    // emit signal
    emit saveFilePressed(filePath);
}

void ProfilesSelection::onSaveAsButtonPressed() {
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

    // emit that we should make a new profile
    emit saveAsButtonPressed(newFilePath);
}

void ProfilesSelection::onCollapseButtonPressed() {
    // make sub content visible if needed
    if (profilesCollapsed) ui->profilesContent->setVisible(true);

    // run animation
    profilesAnimation->setDirection(profilesCollapsed ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    profilesAnimation->start();

    // change button stuff
    ui->collapseButton->setText(profilesCollapsed ? "<\n<\n<" : ">\n>\n>");
    ui->collapseButton->setToolTip(profilesCollapsed ? "Collapse Profiles Section" : "Expand Profiles Section");

    // update the collapsed bool
    profilesCollapsed = !profilesCollapsed;
}

void ProfilesSelection::widgetLock(bool enable) {
    ui->presetFileTree->setMouseTracking(enable);
    ui->customFileTree->setMouseTracking(enable);
}

// slots
void ProfilesSelection::animationFinished() {
    // if we're fully collapsed, make the sub contents invisible so that the button is fully centered
    if (profilesCollapsed) {
        ui->profilesContent->setVisible(false);
    }

    //
    qDebug() << "profilesselection.cpp: check widths after animation: " << width() << ", sub: " << ui->profilesContent->width() << ", button: " << ui->collapseButton->width();
}


