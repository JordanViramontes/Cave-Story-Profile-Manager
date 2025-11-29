#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    // set up stuff
    void setGlobals();
    void setSignals();

    // set up widgets
    void setFileTrees();

private slots:
    // buttons
    void _onRunButton();
    void _onUpdateDirectoryButton();
};

#endif // MAINWINDOW_H
