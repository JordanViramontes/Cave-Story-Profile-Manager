#ifndef PROFILESSELECTION_H
#define PROFILESSELECTION_H

#include <QWidget>

namespace Ui {
class ProfilesSelection;
}

class ProfilesSelection : public QWidget
{
    Q_OBJECT

public:
    explicit ProfilesSelection(QWidget *parent = nullptr);
    ~ProfilesSelection();

    // lock
    void widgetLock(bool enable);

    // set
    void setSavesDirectory(QString);

private:
    Ui::ProfilesSelection *ui;
    QString savesDirectory = "";

signals:
    // void saveAsButtonPressed();
    void saveFilePressed(QString);
    void saveAsButtonPressed(QString);

private slots:
    void onPressedFile(QModelIndex);
    void onSaveAsButtonPressed();
};

#endif // PROFILESSELECTION_H
