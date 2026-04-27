#ifndef QWEAPONTABLESLOT_H
#define QWEAPONTABLESLOT_H

#include <QWidget>
#include <QEvent>
#include <QWheelEvent>
#include <QSlider>
#include <QVector>
#include <qcombobox.h>

namespace Ui {
class QWeaponTableSlot;
}

class QWeaponTableSlot : public QWidget
{
    Q_OBJECT

public:
    explicit QWeaponTableSlot(QWidget *parent = nullptr);
    explicit QWeaponTableSlot(int intType, int intMaxAmmo, bool hasAmmo, QString text, QVector<int> iniWeaponLvls, QWidget *parent = nullptr);
    ~QWeaponTableSlot();

    // get
    bool getEnableChecked() { return enabledState; }
    int getWeaponType() { return type; }
    int getWeaponLevel() {
        if (lvl > 2) lvl = 2;
        return lvl;
    }
    int getWeaponEnergy() { return xp; }
    int getWeaponMaxAmmo() { return maxAmmo; }
    int getWeaponAmmo() { return ammo; }
    bool isValidGrabPos(QPoint pos);
    Ui::QWeaponTableSlot* getUi() const { return ui; }

    QString getWeaponIconPath() { return weaponLabelIconPath; };

    // set
    void setData(bool iniEnabled, int iniLvl, int iniEnergy, int iniMaxAmmo, int iniCurrentAmmo);
    void resetData() { setData(false, 0, 0, initialMaxAmmo, 0); }
    void setBackgroundColor(QString color);

    // locks used to avoid repeating functions
    void lockSignals();
    void unlockSignals();

private:
    Ui::QWeaponTableSlot *ui;

    // vars
    QVector<int> weaponLvls;
    QString weaponLabelIconPath = "";
    bool enabledState = false;
    int type = 0;
    int lvl = 0;
    int xp = 0;
    int maxXp = 1;
    int ammo = 0;
    int maxAmmo = 0;
    int initialMaxAmmo = 0;

    // debug
    bool testLvlChangeXpRatio = true;

signals:
    bool enabledChanged(QWeaponTableSlot* myself, int enableChanged);

private slots:
    void enableChanged(bool enable);
    void xpChanged(int newXp);
    void ammoChanged(int newAmmo);
    void ammoMaxChanged(int newAmmoMax);
    void lvlChanged(int lvl);
};

// event filters
class QWeaponLvlComboBoxEventFilters : public QObject
{
public:
    explicit QWeaponLvlComboBoxEventFilters(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (auto combo = qobject_cast<QComboBox*>(obj)) {

            // if we are scrolling from MAX->3, jump to 2
            if (event->type() == QEvent::Wheel) {
                // get the scroll wheel and check the direction its going
                QWheelEvent* wheel = static_cast<QWheelEvent*>(event);
                int delta = wheel->angleDelta().y();

                // if we're at max AND we're scrolling up
                if (combo->currentIndex() == 3 & delta > 0) {
                    combo->setCurrentIndex(1);
                    return true;
                }
                return QObject::eventFilter(obj, event);
            }
        }

        // call the OG function to do the rest of the events
        return QObject::eventFilter(obj, event);
    }
};





#endif // QWEAPONTABLESLOT_H
