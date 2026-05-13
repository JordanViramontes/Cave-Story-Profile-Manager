#include "widgetfunctions.h"
#include "dialog.h"

// opening up dialog box given which page
int runDialogBox(QWidget* parent, const QString& page) {
    qDebug() << "running this code!";

    Dialog error_box(parent);
    error_box.setStackedWidgetPage(page);

    return error_box.exec();
}