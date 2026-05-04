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
    bool collapsed;

signals:
    // void saveAsButtonPressed();
    void saveFilePressed(QString);
    void saveAsButtonPressed(QString);
    void profilesCollapsed(bool collapsed);

private slots:
    void onPressedFile(QModelIndex);
    void onSaveAsButtonPressed();
    void onCollapsedButtonPressed();
};

#endif // PROFILESSELECTION_H
