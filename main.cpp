#include "mainwindow.h"

#include <QApplication>
#include <QStyleFactory>

// helper functions
QPalette setLightThemePalette();

int main(int argc, char *argv[])
{
    // main application
    QApplication a(argc, argv);

    // set the visual style, get the application's palette and change it
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette p = setLightThemePalette();

    // apply the pallette
    a.setPalette(p);

    MainWindow w;
    w.show();

    return a.exec();
}

QPalette setLightThemePalette() {
    QPalette p;

    // Window background
    p.setColor(QPalette::Window, QColor(255,255,255));
    p.setColor(QPalette::WindowText, Qt::black);

    // Text editors, treeviews, tables
    p.setColor(QPalette::Base, QColor(255,255,255));
    p.setColor(QPalette::AlternateBase, QColor(245,245,245));    // <- fixes file tree alternating rows
    p.setColor(QPalette::Text, Qt::black);

    // Buttons
    p.setColor(QPalette::Button, QColor(255,255,255));
    p.setColor(QPalette::ButtonText, Qt::black);

    // Highlighting
    p.setColor(QPalette::Highlight, QColor(0,120,215));
    p.setColor(QPalette::HighlightedText, Qt::white);

    // Frames, grooves, shadows (QSlider uses these)
    p.setColor(QPalette::Dark, QColor(200,200,200));      // <- fixes dark slider groove
    p.setColor(QPalette::Mid, QColor(220,220,220));
    p.setColor(QPalette::Midlight, QColor(230,230,230));
    p.setColor(QPalette::Shadow, QColor(120,120,120));    // borders

    // Tooltips
    p.setColor(QPalette::ToolTipBase, Qt::white);
    p.setColor(QPalette::ToolTipText, Qt::black);

    // Disabled state (optional)
    p.setColor(QPalette::Disabled, QPalette::Text, QColor(120,120,120));
    p.setColor(QPalette::Disabled, QPalette::WindowText, QColor(120,120,120));
    p.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120,120,120));

    // return
    return p;
}
