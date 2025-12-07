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

private:
    Ui::QWeaponOrderTable *ui;

protected:
};

#endif // QWEAPONORDERTABLE_H
