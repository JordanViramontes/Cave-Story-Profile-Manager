#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"
#include "profileloader.h"

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include <QModelIndex>

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

    // constructors
    // void setGlobals();
    void setSignals();

    // set up widgets
    void setFileTrees();

    // helper functions
    bool checkGameDirPath(QString path);

private slots:
    // buttons
    void _onRunButton();
    void _onUpdateDirectoryButton();
    void _onSelectFile(QModelIndex);
};

#endif // MAINWINDOW_H
