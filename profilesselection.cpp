#include "profilesselection.h"
#include "ui_profilesselection.h"
#include "dialog.h"

#include <QFileSystemModel>

ProfilesSelection::ProfilesSelection(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfilesSelection)
{
    ui->setupUi(this);

    // connections
    connect(ui->presetFileTree, SIGNAL(pressed(QModelIndex)), this, SLOT(onPressedFile(QModelIndex)));
    connect(ui->customFileTree, SIGNAL(pressed(QModelIndex)), this, SLOT(onPressedFile(QModelIndex)));
    connect(ui->saveAsPushButton, SIGNAL(clicked(bool)), this, SLOT(onSaveAsButtonPressed()));
    connect(ui->collapseButton, SIGNAL(clicked(bool)), this, SLOT(onCollapsedButtonPressed()));

    // etc
    ui->presetFileTree->setFocusPolicy(Qt::NoFocus);
    ui->customFileTree->setFocusPolicy(Qt::NoFocus);

    // variables
    collapsed = false; // should start fully opened
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

void ProfilesSelection::widgetLock(bool enable) {
    ui->presetFileTree->setMouseTracking(enable);
    ui->customFileTree->setMouseTracking(enable);
}

void ProfilesSelection::onCollapsedButtonPressed() {
    qDebug() << "profileselection.cpp: Collapsing profiles!";

    // check if we're expanding or collapsing
    if (!collapsed) { // we're collapsing
        ui->collapseButton->setText(">\n>\n>");
        collapsed = true;

        // update profilesGroup
        ui->profilesGroup->setMaximumWidth(0);
        ui->profilesGroup->setVisible(false);
    }
    else { // we're expanding
        ui->collapseButton->setText("<\n<\n<");
        collapsed = false;

        // update profilesGroup
        ui->profilesGroup->setMaximumWidth(16777215);
        ui->profilesGroup->setVisible(true);
    }

    // update widgets
    ui->profilesGroup->adjustSize();

    // emit to mainwindow that we're changing sizes
    emit profilesCollapsed(collapsed);
}
