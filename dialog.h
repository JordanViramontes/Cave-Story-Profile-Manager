#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QHash>

struct PageInfo {
    int index;
    QString header;
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    // methods
    void setStackedWidgetPage(QString pageName);

private:
    Ui::Dialog *ui;

    QHash<QString, PageInfo> stackedWidgetPageDictionary = {
        {"directoryErrorBox", {0, "Error!"}},
        {"helpScreen", {1, "Help!"}},
    };

};

#endif // DIALOG_H
