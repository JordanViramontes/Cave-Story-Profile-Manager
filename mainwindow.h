#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include <QModelIndex>
#include <QTableWidget>
#include <QParallelAnimationGroup>

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

    // save/load
    void saveSettings();

    // enable / disable states
    void setUIfromEnabled(bool state);
    bool isEnabled = false;
    bool initialStartup = true;
    QVector<QWidget*> importantWidgets;

    // helper functions
    bool checkGameDirPath(QString path);

signals:
    void profilePathUpdated(QString profilePath);
    void applyButtonPressed(QString profilePath);
    void writeToProfile(QString profilePath);

private slots:
    // buttons
    void onSimpleRunButtonPressed();
    void onApplyAndRunButtonPressed();
    void onUpdateDirectoryButtonPressed();

    // signal to signal handlers
    void onProfilesSaveFilePressed(QString);
    void onProfilesSaveAsButtonPressed(QString);
};

#endif // MAINWINDOW_H
