#include "qweapontablewidget.h"
#include <qheaderview.h>

QWeaponTableWidget::QWeaponTableWidget(QWidget *parent)
    : QTableWidget(parent)
{

    setRowCount(totalWeapons);
    setColumnCount(1);

    // set event filters
    QTableWidgetEventFilters* tableWidgetFilters = new QTableWidgetEventFilters(this);
    viewport()->installEventFilter(tableWidgetFilters);

    // set header settings
    QHeaderView* horizHeader = horizontalHeader();
    QHeaderView* vertHeader = verticalHeader();
    vertHeader->setSectionResizeMode(QHeaderView::Fixed);
    horizHeader->setSectionResizeMode(QHeaderView::Stretch);

    // set weaponslots
    for (int type = 0x00, tableRowCount = 0; type <= 0x0D; type++) {
        bool hasAmmo = false;
        QString text = "weapon";
        int ammo = 0;
        QVector<int> weaponLvls = {1, 2, 3, 3};

        // ensure that only valid weapon type numbers make it through
        switch (type) {
        case (0x01): hasAmmo = false; text = "SN"; weaponLvls = {30, 40, 16, 16}; break;
        case (0x02): hasAmmo = false; text = "PS"; weaponLvls = {10, 20, 10, 10}; break;
        case (0x03): hasAmmo = false; text = "FB"; weaponLvls = {10, 20, 20, 20}; break;
        case (0x04): hasAmmo = true; text = "MG"; weaponLvls = {30, 40, 10, 10}; ammo = 100; break;
        case (0x05): hasAmmo = true; text = "ML"; weaponLvls = {10, 20, 10, 10};  break;
        case (0x07): hasAmmo = true; text = "BB"; weaponLvls = {10, 20, 5, 5}; ammo = 100; break;
        case (0x09): hasAmmo = false; text = "BL"; weaponLvls = {30, 60, 0, 0}; break;
        case (0x0A): hasAmmo = true; text = "SM"; weaponLvls = {30, 60, 10, 10};  break;
        case (0x0C): hasAmmo = false; text = "NS"; weaponLvls = {1, 1, 1, 1}; break;
        case (0x0D): hasAmmo = false; text = "SP"; weaponLvls = {40, 60, 200, 200}; break;
        default: continue; break;
        }

        // set widget to cell
        QWeaponTableSlot* defaultWeapon = new QWeaponTableSlot(type, ammo, hasAmmo, text, weaponLvls, this);
        setCellWidget(tableRowCount, 0, defaultWeapon);

        // add weapon to weaponsdictionary
        weaponsTableDictionary[type] = defaultWeapon;

        // connections
        connect(defaultWeapon, SIGNAL(enabledChanged(QWeaponTableSlot*,int)), this, SLOT(paintTable(QWeaponTableSlot*,int)));

        // set the fixed row height
        setRowHeight(tableRowCount, defaultWeapon->height());

        // paint the row disabled
        paintTable(defaultWeapon, 0);

        // update table row iterator
        tableRowCount++;
    }

    // paint all weapons
    for (int i = 0; i < rowCount(); i++) {
        QWeaponTableSlot * weapon = qobject_cast<QWeaponTableSlot*>(cellWidget(i, 0));
        qDebug() << "checking: " << weapon->getEnableChecked();
    }

    enabledWeaponsCount = 0;
}

// given a vector of weapons in the order we want in the table, reorder the table!
void QWeaponTableWidget::reorderTable(QVector<int> weapons) {
    // insert weapons.size amount of new rows at the top (will be filled later)
    int newRowAmount = weapons.size();
    for (int i = 0; i < newRowAmount; i++) { insertRow(i); }

    // move the rows from where they were to where they need to be
    int rowIt = 0;
    for (auto i : weapons) {
        // qDebug() << "mainwindowmethods.cpp: reordering: " << i;

        // get row index of the widget
        QWeaponTableSlot* weapon = weaponsTableDictionary[i];
        int index = findTableWidgetIndex(weapon);

        if (index < 0) {
            qDebug() << "mainwindowmethods.cpp: ERROR, INDEX OF OLD ROW DOESNT EXIST!!";
            return;
        }

        // set the cell and then remove the old row
        setCellWidget(rowIt, 0, weapon);
        removeRow(index);

        // update ui stuff
        setRowHeight(rowIt, weapon->sizeHint().height());
        weapon->clearFocus();

        rowIt++;
    }

    viewport()->update();
    updateGeometry();
}

// given a weapon table slot, find the index of the table where its living
int QWeaponTableWidget::findTableWidgetIndex(const QWeaponTableSlot* weaponSlot) {
    for (int i = 0; i < rowCount(); i++) {
        if (cellWidget(i, 0) == weaponSlot) {
            return i;
        }
    }

    return -1;
}

// given a vector of weapons from the parser, update the table
void QWeaponTableWidget::setWeaponsFromParser(const QVector<WeaponDataSlot> parserWeapons, QVector<int> enabledWeapons) {
    // lock or else when we set the enable widget to off it'll signal that its been changed twice
    lockWidgetSignals();

    // reset table before reordering stuff for paint logic
    resetTable();

    // we want to turn everything thats false false and then true for paint order logic

    // go through all table slots EXCLUDING the known weapons and reset them
    for (auto i : weaponsTableDictionary.keys()) {
        if (enabledWeapons.contains(i)) continue;

        QWeaponTableSlot* currentWeapon = weaponsTableDictionary[i];
        currentWeapon->resetData();
    }

    // go through parser weapons and set data!
    for (auto i : parserWeapons) {
        if (i.type == 0x00) continue;
        // qDebug() << (int)i.type << ", " << (int)i.level << ", " << (int)i.energy << ", " << (int)i.maxAmmo << ", " << (int)i.currentAmmo;

        // get the weapon table slot pointer
        QWeaponTableSlot* currentWeapon = weaponsTableDictionary[(int)i.type];
        currentWeapon->setData(true, (int)i.level - 1, (int)i.energy, (int)i.maxAmmo, (int)i.currentAmmo);
    }

    reorderTable(enabledWeapons);
    unlockWidgetSignals();
}

// get
QVector<int> QWeaponTableWidget::getValidEnabledWidgets() {
    QVector<int> enabledWeaponsVector;
    for (int i = 0; i < rowCount(); i++) {
        QWeaponTableSlot* weapon = qobject_cast<QWeaponTableSlot*>(cellWidget(i, 0));
        if (!weapon) return {}; // fail safe
        if (!weapon->getEnableChecked()) continue;

        enabledWeaponsVector.push_back(weapon->getWeaponType());
        if (enabledWeaponsVector.size() >= 5 || enabledWeaponsVector.size() == enabledWeaponsCount)
            return enabledWeaponsVector;
    }

    return enabledWeaponsVector;
}



// slots
// connected with the enable check signal from widget in order to repaint specific row
void QWeaponTableWidget::paintTable(QWeaponTableSlot* weapon, int enabledChanged) {
    // update enabledChanged
    enabledWeaponsCount += enabledChanged;

    // emit signal for the above weapon ui!
    emit weaponTableChanged(getValidEnabledWidgets());

    // qDebug() << "qweapontablewidget.cpp: painting widget: " << weapon->getWeaponType() << ", " << enabledChanged << ", total count: " << enabledWeaponsCount;

    // since weapon from parser function does false weapons before true, we will always paint correctly

    // CONDITION: if weaponcount > 5, we can have a situation where we need to check the table and repaint the enableds!
    // if we are above 5, or we are AT 5, but that was the result of disabling one
    if (enabledWeaponsCount > 5 || (enabledWeaponsCount == 5 && enabledChanged < 0)) {
        paintEnabledRows();

        // if we're enabled, we can just leave since we've already included ourselves
        if (weapon->getEnableChecked()) return;
    }

    // finally paint ourselves! O(1)
    QString backgroundColor = disabledColor;

    // check if we're enabled
    if (weapon->getEnableChecked()) {
        // if we have open slots
        if (enabledWeaponsCount <= 5) {
            backgroundColor = enabledColor;
        }

        // if we dont!
        else {
            backgroundColor = enabledButLeftBehindColor;
        }
    }

    // set the color
    weapon->setBackgroundColor(backgroundColor);
}

void QWeaponTableWidget::paintEnabledRows() {
    int checkWeaponCount = 0;
    for (int i = 0; i < rowCount(); i++) {
        QWeaponTableSlot * enabledWeapon = qobject_cast<QWeaponTableSlot*>(cellWidget(i, 0));
        if (!enabledWeapon->getEnableChecked()) continue;

        if (checkWeaponCount < 5) {
            enabledWeapon->setBackgroundColor(enabledColor);
            checkWeaponCount++;
        }
        else {
            enabledWeapon->setBackgroundColor(enabledButLeftBehindColor);
        }
    }
}







