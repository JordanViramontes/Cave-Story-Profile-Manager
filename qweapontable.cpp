#include "qweapontable.h"
#include <qheaderview.h>
#include <qdebug.h>

QWeaponTable::QWeaponTable(QWidget *parent)
    : QTableWidget(parent)
{
    setRowCount(totalWeapons);
    setColumnCount(1);

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
        case (0x01): hasAmmo = false; text = "SN"; weaponLvls = {29, 39, 16, 16}; break;
        case (0x02): hasAmmo = false; text = "PS"; weaponLvls = {9, 19, 10, 10}; break;
        case (0x03): hasAmmo = false; text = "FB"; weaponLvls = {9, 19, 20, 20}; break;
        case (0x04): hasAmmo = true; text = "MG"; weaponLvls = {29, 39, 10, 10}; ammo = 100; break;
        case (0x05): hasAmmo = true; text = "ML"; weaponLvls = {9, 19, 10, 10};  break;
        case (0x07): hasAmmo = true; text = "BB"; weaponLvls = {9, 19, 5, 5}; ammo = 100; break;
        case (0x09): hasAmmo = false; text = "BL"; weaponLvls = {29, 59, 0, 0}; break;
        case (0x0A): hasAmmo = true; text = "SM"; weaponLvls = {29, 59, 10, 10};  break;
        case (0x0C): hasAmmo = false; text = "NS"; weaponLvls = {0, 0, 1, 1}; break;
        case (0x0D): hasAmmo = false; text = "SP"; weaponLvls = {39, 59, 200, 200}; break;
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
    }

    enabledWeaponsCount = 0;
}

//================================
// GET
//================================

QVector<int> QWeaponTable::getValidEnabledWidgets() {
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

QVector<QWeaponTableSlot*> QWeaponTable::getValidEnabledWeaponPointers() {
    QVector<QWeaponTableSlot*> enabledWeaponsVector;

    for (int i = 0; i < rowCount(); i++) {
        QWeaponTableSlot* weapon = qobject_cast<QWeaponTableSlot*>(cellWidget(i, 0));
        if (!weapon) return {}; // fail safe
        if (!weapon->getEnableChecked()) continue;

        enabledWeaponsVector.push_back(weapon);
        if (enabledWeaponsVector.size() >= 5 || enabledWeaponsVector.size() == enabledWeaponsCount)
            return enabledWeaponsVector;
    }

    return enabledWeaponsVector;
}

// get every weapon in the table
QVector<QWidget*> QWeaponTable::getAllWeaponSlots() {
    QVector<QWidget*> widgets;
    for (int i = 0; i < rowCount(); i++) {
        QWidget* weapon = cellWidget(i, 0);
        QWeaponTableSlot * weaponTest = qobject_cast<QWeaponTableSlot*>(cellWidget(i, 0));
        qDebug() << "check: " << weaponTest->getWeaponType();
        widgets.push_back(weapon);
    }

    return widgets;
}

//================================
// SET
//================================

// reset the whole weapons table
void QWeaponTable::resetAllWeapons() {
    for (auto i : weaponsTableDictionary.keys()) {
        QWeaponTableSlot* currentWeapon = weaponsTableDictionary[i];
        currentWeapon->resetData();
    }

    // reorder table
    reorderTable({1, 2, 3, 4, 5, 7, 9, 10, 12, 13});
}

// update the data of 1 weapon from qinventory
void QWeaponTable::setWeaponFromParser(int type, bool iniEnabled, int iniLvl, int iniEnergy, int iniMaxAmmo, int iniCurrentAmmo) {
    QWeaponTableSlot* currentWeapon = weaponsTableDictionary[type];
    currentWeapon->setData(iniEnabled, iniLvl, iniEnergy, iniMaxAmmo, iniCurrentAmmo);
}

// given a vector of weapons in the order we want in the table, reorder the table!
void QWeaponTable::reorderTable(QVector<int> weapons) {
    // insert weapons.size amount of new rows at the top (will be filled later)
    int newRowAmount = weapons.size();
    for (int i = 0; i < newRowAmount; i++) { insertRow(i); }

    // move the rows from where they were to where they need to be
    int rowIt = 0;
    for (auto i : weapons) {
        // qDebug() << "mainwindowmethods.cpp: reordering: " << i;

        // get row index of the widget
        QWeaponTableSlot* weapon = weaponsTableDictionary[i];
        int index = -1;
        for (int i = 0; i < rowCount(); i++) {
            if (cellWidget(i, 0) == weapon) {
                index = i;
            }
        }

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

// paint all of the rows in the table
void QWeaponTable::paintEnabledRows() {
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

//================================
// SLOTS
//================================

// connected with the enable check signal from widget in order to repaint specific row
void QWeaponTable::paintTable(QWeaponTableSlot* weapon, int enabledChanged) {
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

//================================
// USER INPUT OVERRIDES
//================================

// start drag event, used for setting the drag preview thing
void QWeaponTable::startDrag(Qt::DropActions supportedActions) {
    // check we're actually dragging something
    QModelIndexList indexes = selectedIndexes();
    if (indexes.isEmpty()) return;

    // get the row
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = model()->mimeData(indexes);
    drag->setMimeData(mimeData);

    // update vars
    draggingFromRow = currentRow();

    // customize pixmap
    QWeaponTableSlot* weapon = qobject_cast<QWeaponTableSlot*>(cellWidget(indexes.first().row(), 0));
    QPixmap p(weapon->getWeaponIconPath());
    int pixmapScale = 2;
    int newW = p.width() * pixmapScale;
    int newH = p.height() * pixmapScale;
    p = p.scaled(newW, newH, Qt::KeepAspectRatio);
    drag->setPixmap(p);

    // sets the spot where the cursor holds onto the pixmap
    drag->setHotSpot(QPoint(p.width()/-1.5, p.width()/10));

    drag->exec(supportedActions);
}

// drag event, used for checking if our drop point is valid
void QWeaponTable::dragMoveEvent(QDragMoveEvent* event) {
    // qDebug() << "drag event";

    // get the item at mouse cursor
    QPoint pos = event->position().toPoint();

    // getGapRow detects if we are in between rows
    int gap = [this, pos]() -> int
    {
        int tol = 6; // offset for detecting valid drop points
        int rows = rowCount();
        int y = pos.y();

        int firstTop = rowViewportPosition(0);
        if (y < firstTop + tol)
            return 0;

        for (int r = 0; r < rows - 1; r++) {
            int bottom = rowViewportPosition(r) + rowHeight(r);
            int nextTop = rowViewportPosition(r + 1);

            if (y > bottom - tol && y < nextTop + tol)
                return r + 1;
        }

        int lastBottom = rowViewportPosition(rows - 1) + rowHeight(rows - 1);
        if (y > lastBottom - tol)
            return rows;

        return -1;
    }();

    // check if we arent INN in a cell, and that we aren't trying to drop between ourselves
    if (gap < 0 || draggingFromRow == gap || draggingFromRow + 1 == gap) {
        dropIndicatorGap = -1;
        viewport()->update();
        event->ignore();
        return;
    }
    else {
        // check that we are on the grab handle in the object


        dropIndicatorGap = gap;
        viewport()->update();
        event->accept();
    }


    // if we're valid continue the event!
    QTableWidget::dragMoveEvent(event);
}

// if we're dragging but we havent left the og cell
void QWeaponTable::dragLeaveEvent(QDragLeaveEvent * event) {
    // call OG!
    QTableWidget::dragLeaveEvent(event);
}

// drop event, used for clearing selections and painting rows
void QWeaponTable::dropEvent(QDropEvent* event) {
    // cursor reset
    QApplication::restoreOverrideCursor();

    // the OG function!
    QTableWidget::dropEvent(event);

    // reset painting stuff
    dropIndicatorGap = -1;
    viewport()->update();
    draggingFromRow = -1;

    // paint the correct colors on the table and emit our signal to the weapon preview widget
    paintEnabledRows();
    emit weaponTableChanged(getValidEnabledWidgets());

    // deselect the row and cell
    clearSelection();
    setCurrentIndex(QModelIndex());
}

// used for painting the drop indicator line
void QWeaponTable::paintEvent(QPaintEvent *event) {
    // call OG event!
    QTableWidget::paintEvent(event);

    // if we have an unset drop indicator
    if (dropIndicatorGap < 0) return;

    QPainter painter(viewport());
    int topRowOffset = 2;

    // set our pen to draw with!
    QPen pen(Qt::black);
    pen.setWidth(3);
    painter.setPen(pen);

    // we want y to be drawn usually on the top of the row unless its at the last row
    int y;
    if (dropIndicatorGap == rowCount()) {
        QRect last = visualRect(model()->index(rowCount()-1, 0));
        y = last.bottom();
    }
    else if (dropIndicatorGap == 0) {
        QRect firstRect = visualRect(model()->index(0, 0));
        y = firstRect.top() - topRowOffset;
    }
    else {
        QRect r = visualRect(model()->index(dropIndicatorGap, 0));
        y = r.top();
    }

    // draw our line!
    painter.drawLine(0, y, viewport()->width(), y);
}

// manual start drags!
void QWeaponTable::mousePressEvent(QMouseEvent* event) {
    pressedEventPos = event->pos(); // set the position of the mouse press
    QTableWidget::mousePressEvent(event);
}

void QWeaponTable::mouseMoveEvent(QMouseEvent* event) {
    // check left button press
    if (!(event->buttons() & Qt::LeftButton)) return;

    // check we have dragged enough with the cursor to start event
    if ((event->pos() - pressedEventPos).manhattanLength() < QApplication::startDragDistance())
        return;

    // get the index of the pressed event in order to get our weapon
    QModelIndex index = indexAt(pressedEventPos);
    if (!index.isValid()) return;

    QWeaponTableSlot* weapon = qobject_cast<QWeaponTableSlot*>(cellWidget(index.row(), 0));
    if (!weapon) return;

    // check if we're on the grab handle
    if (!weapon->isValidGrabPos(pressedEventPos)) return;

    // finall start the drag!
    startDrag(Qt::MoveAction);
}

// when releasing the mouse
void QWeaponTable::mouseReleaseEvent(QMouseEvent *event) {
    QTableWidget::mouseReleaseEvent(event);

    // Clear selection and remove focus
    clearSelection();
    setCurrentIndex(QModelIndex());
}

//================================
// DEBUG PRINTS
//================================

void QWeaponTable::printWeaponsTableDictionary() {
    qDebug() << "qweapontablewidget.h: print all weapon table dicitonary entries: ";
    for (auto i : weaponsTableDictionary.keys()) {
        QWeaponTableSlot* weapon = weaponsTableDictionary[i];
        qDebug() << i << ":" << weapon->getWeaponType();
    }
}
void QWeaponTable::printWeaponsTable() {
    qDebug() << "qweapontablewidget.h: print all weapon table row entries: ";
    for (int i = 0; i < rowCount(); i++) {
        if (cellWidget(i, 0) == nullptr) continue;

        QWeaponTableSlot* weapon = qobject_cast<QWeaponTableSlot*>(cellWidget(i,0));
        qDebug() << "row: " << i << " has weapon: " << weapon->getWeaponType();
    }
}





























