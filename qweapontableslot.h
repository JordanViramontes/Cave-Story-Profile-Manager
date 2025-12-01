#ifndef QWEAPONTABLESLOT_H
#define QWEAPONTABLESLOT_H

#include <QWidget>

namespace Ui {
class QWeaponTableSlot;
}

class QWeaponTableSlot : public QWidget
{
    Q_OBJECT

public:
    explicit QWeaponTableSlot(QWidget *parent = nullptr);
    ~QWeaponTableSlot();

private:
    Ui::QWeaponTableSlot *ui;
};

#endif // QWEAPONTABLESLOT_H
