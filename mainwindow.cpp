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

    setCentralWidget(window);
    setFixedSize(854, 480);
    setWindowTitle("8 Slider Puzzle Solver GUI");
}

MainWindow::~MainWindow()
{
    delete ui;
}
