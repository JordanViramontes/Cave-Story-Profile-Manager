#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QHash>

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

    // hash table that maps page strings to the stacked widget page count
    QHash<QString, int> stackedWidgetPageDictionary = {
        {"directoryErrorBox", 0},
    };
};

#endif // DIALOG_H
