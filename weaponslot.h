#ifndef WEAPONSLOT_H
#define WEAPONSLOT_H

#include <QWidget>

namespace Ui {
class WeaponSlot;
}

class WeaponSlot : public QWidget
{
    Q_OBJECT

public:
    explicit WeaponSlot(QWidget *parent = nullptr);
    ~WeaponSlot();

private:
    Ui::WeaponSlot *ui;
};

#endif // WEAPONSLOT_H
