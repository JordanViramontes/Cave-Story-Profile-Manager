#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // main layout
    Window *window = new Window;
    window->setGeometry(0, 0, 854, 480);

    setCentralWidget(window);
    // setFixedSize(854, 480);
    setWindowTitle("Cave Story Profile Manager");
}

MainWindow::~MainWindow()
{
    delete ui;
}
