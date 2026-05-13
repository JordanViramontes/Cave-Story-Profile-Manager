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

private:
    Ui::ProfilesSelection *ui;
    QString savesDirectory = "";

    // animation widgets
    QParallelAnimationGroup* profilesAnimation;
    bool profilesCollapsed;

signals:
    void saveFilePressed(QString);
    void saveAsButtonPressed(QString);

private slots:
    void onPressedFile(QModelIndex);
    void onSaveAsButtonPressed();
    void onCollapseButtonPressed();
};

#endif // PROFILESSELECTION_H
