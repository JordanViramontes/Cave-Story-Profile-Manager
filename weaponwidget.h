#ifndef WEAPONWIDGET_H
#define WEAPONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QSlider>

class WeaponWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WeaponWidget(QWidget *parent = nullptr);
    explicit WeaponWidget(QString n, int l, int x, QVector<int> xN, QWidget *parent = nullptr);

    // get
    QString getName() const { return name; }
    int getlvl() const { return lvl; }
    int getXp() const { return xp; }
    QVector<int> getxpNeeded() const { return xpNeeded; }

    // set
    void setName(QString n) { name = n; }
    void setLvl(int l) { lvl = l; }
    void setXp(int x) { xp = x; }
    void setXpNeeded(QVector<int> xN) { xpNeeded = xN; }

private:
    bool isEnabled = false;
    QString name = "temp";
    int lvl = 1;
    int xp = 0;
    QVector<int> xpNeeded = {-1, -1, -1};

    // constructors
    QWidget * createIcon();
    QWidget * createLevel();
    QSlider * createSlider();
    QWidget * createCheck();
    QLabel * createXpText();

};

Q_DECLARE_METATYPE(WeaponWidget*)

#endif // WEAPONWIDGET_H
