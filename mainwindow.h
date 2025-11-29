#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dialog.h"

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>

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
    // QSettings settings;?
    QString gameDirectory;
    QString savesDirectory;

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
};

#endif // MAINWINDOW_H
