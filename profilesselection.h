#ifndef PROFILESSELECTION_H
#define PROFILESSELECTION_H

#include <QWidget>
#include <QParallelAnimationGroup>

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

    // animation widgets
    QParallelAnimationGroup* profilesAnimation;
    bool profilesCollapsed;

signals:
    // void saveAsButtonPressed();
    void saveFilePressed(QString);
    void saveAsButtonPressed(QString);

private slots:
    void onPressedFile(QModelIndex);
    void onSaveAsButtonPressed();
    void onCollapseButtonPressed();
    void animationFinished();
};

#endif // PROFILESSELECTION_H
