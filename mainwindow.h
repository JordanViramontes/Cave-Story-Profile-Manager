#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "profileloader.h"
#include "qweaponordertable.h"

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include <QModelIndex>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString gameDirectory;
    QString savesDirectory;
    ProfileLoader parser;

    // save/load
    void saveSettings();
    void loadSettings();

    // enable / disable states
    void widgetLock(bool);

    // constructors
    void setSignals();

    // set up widgets
    void createFileTrees();

    // helper functions
    bool checkGameDirPath(QString path);

private slots:
    // buttons
    void _onSimpleRunButton();
    void _onRunButton();
    void _onHelpButton();
    void _onUpdateDirectoryButton();
    void _onSelectFile(QModelIndex);
    void _onUpdateSelectWeaponChoices(QVector<int> weapons);
};

#endif // MAINWINDOW_H
