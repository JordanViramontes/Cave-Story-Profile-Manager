#ifndef QWEAPONTABLESLOT_H
#define QWEAPONTABLESLOT_H

#include <QWidget>
#include <QEvent>
#include <QWheelEvent>
#include <QSlider>
#include <QVector>

namespace Ui {
class QWeaponTableSlot;
}

class QWeaponTableSlot : public QWidget
{
    Q_OBJECT

public:
    explicit QWeaponTableSlot(QWidget *parent = nullptr);
    explicit QWeaponTableSlot(bool hasAmmo, QString text, QWidget *parent = nullptr);
    ~QWeaponTableSlot();

private:
    Ui::QWeaponTableSlot *ui;

    // vars
    QVector<int> weaponLvls;
    int lvl = 0;
    int xp = 0;
    int maxXp = 1;
    int ammo = 0;
    int maxAmmo = 0;

private slots:
    void xpChanged(int newXp);
    void ammoChanged(int newAmmo);
    void ammoMaxChanged(int newAmmoMax);
    void lvlChanged(int lvl);
};

// event filters
class QWeaponSlotEventFilters : public QObject
{
public:
    explicit QWeaponSlotEventFilters(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        // scrolling with the mouse!
        if (event->type() == QEvent::Wheel) {
            event->ignore();
            return true;
        }

        // call the OG function to do the rest of the events
        return QObject::eventFilter(obj, event);
    }
};





#endif // QWEAPONTABLESLOT_H
