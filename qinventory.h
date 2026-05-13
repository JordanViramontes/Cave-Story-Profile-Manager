#ifndef QINVENTORY_H
#define QINVENTORY_H

#include "profileloader.h"
#include <QWidget>

namespace Ui {
class QInventory;
}

class QInventory : public QWidget
{
    Q_OBJECT

public:
    explicit QInventory(QWidget *parent = nullptr);
    ~QInventory();

private:
    ProfileLoader parser;

    // HP bar stuff
    QVector<QWidget*> importantWidgets;
    int hp = 0;
    int maxHp = 0;

public slots:
    void onSelectFile(QString filePath);
    void PushInventoryToProfile(QString profilePath);

private slots:
    void onUpdateSelectWeaponChoices(QVector<int> weapons);

    // health bar
    void healthChanged(int health);
    void maxHealthChanged(int maxNewHp);

private:
    Ui::QInventory *ui;
};

#endif // QINVENTORY_H
