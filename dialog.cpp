#include "dialog.h"
#include "ui_dialog.h"
#include "qweapontableslot.h"

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

    // get the page number
    if (!stackedWidgetPageDictionary.contains(pageName)) {
        qDebug() << "dialog.cpp: given page \'" << pageName << "\' doesn't exist!";
    }

    // set the page index
    int pageNumber = stackedWidgetPageDictionary[pageName].index;
    widget->setCurrentIndex(pageNumber);

    // set the window header
    setWindowTitle(stackedWidgetPageDictionary[pageName].header);

    // set all other pages to ignore size policy
    for (int i = 0; i < widget->count(); i++) {
        if (i == pageNumber) {
            widget->widget(i)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            continue;
        }
        widget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    }



    resize(minimumSizeHint());
}
