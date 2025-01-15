#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "window.h"
#include "global.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // global variables
    QDir dir(QCoreApplication::applicationDirPath());
    dir.cd("images");
    IMAGEPATH = dir.path();

    ui->setupUi(this);

    // main layout
    Window *window = new Window(this);

    setCentralWidget(window);
    resize(854, 480);
    setWindowTitle("Cave Story Profile Manager");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMenuBar * MainWindow::createMenuBar() {
    QMenuBar *menu = new QMenuBar(this);
    // QMenu *fileMenu = menu->addMenu("&File");
    // QMenu *editMenu = menu->addMenu("&Edit");
    // QMenu *helpMenu = menu->addMenu("&Help");

    // QAction *fileAction = new QAction("&File", this);
    // QAction *editAction = new QAction("&Edit", this);
    // QAction *helpAction = new QAction("&Help", this);

    // // Add actions to toolbar
    // QToolBar * toolBar = addToolBar("Header Ribbon");
    // toolBar->addAction(fileAction);
    // toolBar->addAction(editAction);
    // toolBar->addAction(helpAction);

    // QAction *fileAction = new QAction("&File", this);
    // QAction *editAction = new QAction("&Help", this);
    // QAction *helpAction = new QAction("&Credits", this);

    // QToolBar * toolBar = new QToolBar(this);
    // toolBar->setWindowTitle("test");
    // toolBar->addAction(fileAction);
    // toolBar->addAction(editAction);
    // toolBar->addAction(helpAction);


    return menu;
}
