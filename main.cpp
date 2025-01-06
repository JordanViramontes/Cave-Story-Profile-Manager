/*
Cave Story Profile Manager and Editor
by Jordan Viramontes

credits:
https://doc.qt.io/qt-6/qtquickcontrols-filesystemexplorer-example.html
https://stackoverflow.com/questions/56819085/qt-how-to-implement-simple-internal-dragdrop-for-reordering-items-in-qlistview



*/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
