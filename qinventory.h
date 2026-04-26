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
    int hp = 0;
    int maxHp = 0;

    // locks used to avoid repeating functions
    void lockSignals();
    void unlockSignals();

public slots:
    void _onSelectFile(QString filePath);
    void _PushInventoryToProfile(QString profilePath);

private slots:
    void _onUpdateSelectWeaponChoices(QVector<int> weapons);

    // health bar
    void healthChanged(int health);
    void maxHealthChanged(int maxNewHp);

private:
    Ui::QInventory *ui;
};

#endif // QINVENTORY_H
