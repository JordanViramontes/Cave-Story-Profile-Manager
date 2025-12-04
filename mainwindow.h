#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qweapontableslot.h"
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

    // given the name of a weapon, get the table entry (ex: "PS")
    QHash<int, QWeaponTableSlot*> weaponsTableDictionary;

    // game information
    int totalWeapons = 10;

    // save/load
    void saveSettings();
    void loadSettings();

    // enable / disable states
    void widgetLock(bool);

    // constructors
    void setSignals();

    // set up widgets
    void createFileTrees();
    void createWeaponTable();

    // helper functions
    bool checkGameDirPath(QString path);
    int findTableWidgetIndex(const QWeaponTableSlot* weaponSlot);
    void reorderTable(QVector<int> weapons);
    void resetTable() { reorderTable({1, 2, 3, 4, 5, 7, 9, 10, 12, 13}); };

private slots:
    // buttons
    void _onRunButton();
    void _onUpdateDirectoryButton();
    void _onSelectFile(QModelIndex);
};

#endif // MAINWINDOW_H
