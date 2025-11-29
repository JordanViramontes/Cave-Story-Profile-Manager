#include "dialog.h"
#include "ui_dialog.h"

#include <QStackedWidget>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setStackedWidgetPage(QString pageName) {
    QStackedWidget* widget = ui->stackedWidget;

    if (!stackedWidgetPageDictionary.contains(pageName)) {
        qDebug() << "dialog.cpp: given page \'" << pageName << "\' doesn't exist!";
    }

    int pageNumber = stackedWidgetPageDictionary[pageName];

    widget->setCurrentIndex(pageNumber);
}
