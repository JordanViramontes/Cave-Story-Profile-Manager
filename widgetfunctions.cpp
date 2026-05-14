#include "widgetfunctions.h"
#include "dialog.h"

// opening up dialog box given which page
int runDialogBox(QWidget* parent, const QString& page) {
    Dialog error_box(parent);
    error_box.setStackedWidgetPage(page);

    return error_box.exec();
}

// widget locks
void widgetLock(const bool enable, const QVector<QWidget*>& widgets) {
    for (auto i : widgets) {
        i->setEnabled(enable);
    }
}



