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
    // window->setGeometry(0, 0, 1280, 720);

    // menu bar
    // QMenuBar * menuBar = createMenuBar();
    // QMenu * fileMenu = new QMenu("&file", this);

    // QMenuBar * menuBar = new QMenuBar(this);
    // menuBar->addMenu(fileMenu);
    // menuBar->setStyleSheet(
    //     "QMenuBar { background-color: #333; color: white; border: 1px solid #444; }"
    //     "QMenuBar::menu { border: 1px solid #444; background-color: #333; }"
    // );

    // setMenuBar(menuBar);

    setCentralWidget(window);
    // setFixedSize(854, 480);
    // setGeometry(1280, 720);
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
