#ifndef QINVENTORY_H
#define QINVENTORY_H

#include "qweapontableslot.h"
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
    // general stuff
    char map = 0;               // 008
    char song = 0;              // 00C
    char horizPos[4] = {0};     // 010-013
    char vertPos[4] = {0};      // 014-017
    char facingDir = 0;         // 018
    char whimsicalSt = 0;       // 01E (0-3 stars)
    char currWeapon = 0;        // 024 (0-04 AKA slots 1-5)
    char equipIt[2];            // 02C-02D (low byte first)
    char time[3] = {0};         // 034-036

    // HP bar stuff
    QVector<QWidget*> importantWidgets;
    int hp = 0;
    int maxHp = 0;

    // parsing
    QByteArray buffer;

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
