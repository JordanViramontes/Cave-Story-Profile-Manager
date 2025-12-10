#ifndef QWEAPONORDERTABLE_H
#define QWEAPONORDERTABLE_H

#include <QWidget>

namespace Ui {
class QWeaponOrderTable;
}

class QWeaponOrderTable : public QWidget
{
    Q_OBJECT

public:
    explicit QWeaponOrderTable(QWidget *parent = nullptr);
    ~QWeaponOrderTable();

    // set
    void setAllSlots(QVector<int> weapons);

private:
    // variables
    Ui::QWeaponOrderTable *ui;
    QHash<int, QString> weaponImageDictionary = {
        {-1, ":/weaponImages/images/empty.png"},
        {0x01, ":/weaponImages/images/snake.png"},
        {0x02, ":/weaponImages/images/polarStar.png"},
        {0x03, ":/weaponImages/images/fireball.png"},
        {0x04, ":/weaponImages/images/machineGun.png"},
        {0x05, ":/weaponImages/images/missile.png"},
        {0x07, ":/weaponImages/images/bubble.png"},
        {0x09, ":/weaponImages/images/blade.png"},
        {0x0A, ":/weaponImages/images/missileSuper.png"},
        {0x0C, ":/weaponImages/images/nemesis.png"},
        {0x0D, ":/weaponImages/images/spur.png"},
    };
    QString colorFrameSelected = "#6495ED";


    // set
    void setSlot(int slot, int weapon);

public slots:
    void weaponUiChanged(QVector<int> enabledWeapons);
    void setHighlightedSlot(int slot);

protected:
};

#endif // QWEAPONORDERTABLE_H
