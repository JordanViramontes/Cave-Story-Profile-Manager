#ifndef QWEAPONTABLE_H
#define QWEAPONTABLE_H

#include <QTableWidget>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <qapplication.h>

#include "qweapontableslot.h"


class QWeaponTable : public QTableWidget
{
    Q_OBJECT
public:
    QWeaponTable(QWidget *parent = nullptr);

    // get
    QVector<int> getValidEnabledWidgets();
    QVector<QWeaponTableSlot*> getValidEnabledWeaponPointers();

    // set
    void resetAllWeapons();
    void setWeaponFromParser(int type, bool iniEnabled, int iniLvl, int iniEnergy, int iniMaxAmmo, int iniCurrentAmmo);
    void reorderTable(QVector<int> weapons);

    // debug
    void printWeaponsTableDictionary();
    void printWeaponsTable();

private:
    // variables
    QHash<int, QWeaponTableSlot*> weaponsTableDictionary;
    QVector<QWeaponTableSlot*> enabledWeapons;
    int totalWeapons = 10;
    int enabledWeaponsCount = 0;
    int dropIndicatorGap = -1;
    QString disabledColor = "darkGray";
    QString enabledColor = "lightblue";
    QString enabledButLeftBehindColor = "lightGray";
    QPoint pressedEventPos;
    int draggingFromRow = -1;

    // methods
    void paintEnabledRows();

signals:
    void weaponTableChanged(QVector<int> enabledWeapons);

public slots:
    void paintTable(QWeaponTableSlot* weapon, int enabled);

protected:
    // all of these are for custom drag and drop events
    void startDrag(Qt::DropActions supportedActions) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent * event) override;
    void dropEvent(QDropEvent* event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // QWEAPONTABLE_H
