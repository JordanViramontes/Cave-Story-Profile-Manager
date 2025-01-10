#include "tablewidgetdragrows.h"
#include "qscrollbar.h"

#include <QHeaderView>

TableWidgetDragRows::TableWidgetDragRows(QWidget *parent)
    : QTableWidget{parent}
{
    setDragEnabled(true);
    setAcceptDrops(true);
    viewport()->setAcceptDrops(true);
    setDragDropOverwriteMode(false);
    setDropIndicatorShown(true);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setDragDropMode(QAbstractItemView::InternalMove);
    horizontalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFocusPolicy(Qt::NoFocus);

    setRowCount(weaponTotal);
    setColumnCount(1);

    int w; bool m; QString n; int l; int x; QVector<int> xN; TableWidgetDragRows * pT = this; QWidget *p = this;

    // fill default table
    for (int i = 0; i < weaponTotal; i++) {
        // initialize variables
        switch(i) {
            case (0): w=0x1; m=0; n="SN"; l=1; x=0; xN={30, 40, 16, 0}; break; // snake
            case (1): w=0x2; m=0; n="PS"; l=1; x=0; xN={10, 20, 10, 0}; break; // PS
            case (2): w=0x3; m=0; n="FB"; l=1; x=0; xN={10, 20, 20, 0}; break; // FB
            case (3): w=0x4; m=0; n="MG"; l=1; x=0; xN={30, 40, 10, 0}; break; // MG
            case (4): w=0x5; m=1; n="ML"; l=1; x=0; xN={10, 20, 10, 0}; break; // ML
            case (5): w=0x7; m=0; n="BB"; l=1; x=0; xN={10, 20, 5, 0}; break; // BB
            case (6): w=0x9; m=0; n="BL"; l=1; x=0; xN={30, 60, 0, 0}; break; // BL
            case (7): w=0xA; m=1; n="SM"; l=1; x=0; xN={30, 60, 10, 0}; break; // SM
            case (8): w=0xC; m=0; n="NS"; l=1; x=0; xN={1, 1, 1, 0}; break; // NS
            case (9): w=0xD; m=0; n="SP"; l=1; x=0; xN={40, 60, 200, 0}; break; // SP
            default: w=-1; m=0; n="xx"; l=1; x=0; xN={0, 0, 0, 0}; break; // default
        }

        // create widget
        WeaponWidget *widget = new WeaponWidget(w, m, n, l, x, xN, pT, p);

        // UI
        setCellWidget(i, 0, widget);
        setRowHeight(i, rowHeight);

        // connection for updating the painter for each row dynamically
        connect(widget, &WeaponWidget::isEnabledChanged, this, [this, i]() {
            updatePainter(i, 0); // Call your custom update logic
        });

        connect(this, SIGNAL(ammoChanged(int)), widget, SLOT(maxAmmoSlot(int)));
    }

    // delegate
    WeaponsDelegate * delegate = new WeaponsDelegate(5, weaponTotal, this);
    setItemDelegate(delegate);

    // WeaponWidget *widget = qobject_cast<WeaponWidget*>(cellWidget(0, 0));
    // widget->setEnabled(!widget->getIsEnabled());
}

void TableWidgetDragRows::newWeaponTable(QVector<WeaponSlot> &weapons) {
    QVector<int> w; // vector to know the order of parsed weapons
    QVector<int> weaponPlace;
    QVector<QWidget*> firstHalf(5, nullptr);
    QVector<QWidget*> secondHalf;
    QVector<int> oldRowsFirst(5, -1);
    QVector<int> oldRowsSecond;
    int amountActive = 0;

    // add weapons queue
    for (unsigned int i = 0; i < weapons.size(); i++) {
        // qDebug() << "check weapons: " << static_cast<int>(weapons[i].type);
        w.push_back(static_cast<int>(weapons[i].type));
        weaponPlace.push_back(i);
    }


    // prepare order of swaps and set widget amounts
    for (int i = 0; i < rowCount(); i++) { // i->cells, j->weapons
        // cast the cell widget and check
        WeaponWidget *weaponWidget = qobject_cast<WeaponWidget*>(cellWidget(i, 0));
        if (!weaponWidget) continue;

        // reset weapon values
        weaponWidget->setLvl(1);
        weaponWidget->setXp(0);
        // weaponWidget->setEnabled(false);
        if (weaponWidget->getIsMissile())
            weaponWidget->setAmmo(0);

        // add to vector
        bool isInWeapons = false;
        for (int j = 0; j < w.size(); j++) { // hate this is nested but w can only be 5 large at most
            if (weaponWidget->getWeaponType() == w.at(j)) {
                firstHalf[j] = cellWidget(i, 0); // make sure to push a QWIDGET
                isInWeapons = true;

                // get weapon values and set them
                int lvl = static_cast<int>(weapons[weaponPlace[j]].level);
                int xp = static_cast<int>(weapons[weaponPlace[j]].energy);
                int getNeeded = weaponWidget->getxpNeeded().at(2);

                amountActive++; // counter for later

                if (lvl == 3 && xp == getNeeded) {
                    // qDebug() << "a";
                    weaponWidget->setLvl(4);
                }
                else {
                    weaponWidget->setLvl(static_cast<int>(weapons[weaponPlace[j]].level));
                    weaponWidget->setXp(static_cast<int>(weapons[weaponPlace[j]].energy));
                }

                if (weaponWidget->getIsMissile())
                    weaponWidget->setAmmo(static_cast<int>(weapons[weaponPlace[j]].currentAmmo));

                // set up oldrows
                oldRowsFirst[j] = i;

                break;
            }
        }
        if (!isInWeapons) {
            secondHalf.push_back(cellWidget(i, 0));
            // set up oldrows
            oldRowsSecond.push_back(i);
        } // make sure to push a QWIDGET
    }

    // make 10 new rows to swap to
    for (int i = 0; i < weaponTotal; i++) { insertRow(0); }

    int newRow = 0;

    // first half
    for (unsigned int i = 0; i < oldRowsFirst.size(); i++) {
        // if empty
        if (oldRowsFirst[i] == -1) continue;

        // offset row and move widget
        int oldRow = oldRowsFirst[i] + 10;
        setRowHeight(newRow, rowHeight);

        // get widget and set new values
        QWidget *widget = cellWidget(oldRow, 0);

        if (widget) {
            setCellWidget(newRow, 0, widget);
        }

        // incriment
        newRow++;
    }

    // second half
    for (unsigned int i = 0; i < oldRowsSecond.size(); i++) {

        // offset row and move widget
        int oldRow = oldRowsSecond[i] + 10;
        setRowHeight(newRow, rowHeight);

        QWidget *widget = cellWidget(oldRow, 0);
        if (widget) {
            setCellWidget(newRow, 0, widget);
        }

        // incriment
        newRow++;
    }

    // reset to normal row count
    setRowCount(weaponTotal);

    // reset scroll
    verticalScrollBar()->setValue(0);

    // reset paint
    // qDebug() << "size: " << amountActive;
    for (int i = 0; i < rowCount(); i++) {
        WeaponWidget *widget = qobject_cast<WeaponWidget*>(cellWidget(i, 0));

        if (i < amountActive) widget->setEnabled(true);
        else widget->setEnabled(false);

        // widget->updatePaint();
    }
}

QVector<WeaponSlot> TableWidgetDragRows::getWeapons() const {
    QVector<WeaponSlot> weapons;

    /*
    char type = 0;
    char level = 0;
    char energy = 0;
    char maxAmmo = 0;
    char currentAmmo = 0;
     */

    // you only check the first 5 rows
    for (int i = 0; i < 5; i++) {
        WeaponWidget *widget = qobject_cast<WeaponWidget*>(cellWidget(i, 0));
        if (!widget->getIsEnabled()) {
            qDebug() << "nope: " << i;
            continue; // check its enabled and inside rows
        }

        WeaponSlot currentWeapon;

        currentWeapon.type = static_cast<char>(widget->getWeaponType());
        currentWeapon.level = static_cast<char>(widget->getlvl());
        currentWeapon.energy = static_cast<char>(widget->getXp());
        currentWeapon.maxAmmo = static_cast<char>(widget->getAmmoMax());
        currentWeapon.currentAmmo = static_cast<char>(widget->getAmmo());

        // check max
        if (currentWeapon.level >= 4) {
            currentWeapon.level = 3;
            QVector<int> xpNeeded = widget->getxpNeeded();
            currentWeapon.energy = xpNeeded.at(2);
        }

        qDebug() << currentWeapon.type+0 << ", "
                 << currentWeapon.level+0 << ", "
                 << currentWeapon.energy+0 << ", "
                 << currentWeapon.maxAmmo+0 << ", "
                 << currentWeapon.currentAmmo+0 << ", ";


        weapons.push_back(currentWeapon);
    }
    return weapons;
}



