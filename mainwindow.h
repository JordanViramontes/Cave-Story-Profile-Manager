#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "profileloader.h"

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
    // ProfileLoader parser;

    // save/load
    void saveSettings();
    void loadSettings();

    // enable / disable states
    void widgetLock(bool);
    void disableInventory(bool);
    bool isEnabled = false;

    // constructors
    void setSignals();

    // set up widgets
    void createFileTrees();

    // helper functions
    bool checkGameDirPath(QString path);

signals:
    void profilePathUpdated(QString profilePath);
    void applyButtonPressed(QString profilePath);
    void writeToProfile(QString profilePath);

private slots:
    // buttons
    void onSimpleRunButtonPressed();
    void onRunButtonPressed();
    void onHelpButtonPressed();
    void onUpdateDirectoryButtonPressed();

    // signal to signal handlers
    void onProfilesSaveFilePressed(QString);
    void onProfilesSaveAsButtonPressed(QString);

    // from widgets
    void onProfilesCollapsed(bool collapsed);
};

#endif // MAINWINDOW_H
