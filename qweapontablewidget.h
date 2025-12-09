#ifndef QWEAPONTABLEWIDGET_H
#define QWEAPONTABLEWIDGET_H

#include <QTableWidget>

#include "qweaponordertable.h"
#include "qweapontableslot.h"
#include "profileloader.h"


class QWeaponTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    QWeaponTableWidget(QWidget *parent = nullptr);

    // get
    const QHash<int, QWeaponTableSlot*> &getWeaponsTableDictionary() { return weaponsTableDictionary; }
    QVector<int> getValidEnabledWidgets();

    // set
    void setWeaponsFromParser(const QVector<WeaponDataSlot> parserWeapons, QVector<int> enabledWeapons);
    void lockWidgetSignals() {
        for (int i = 0; i < rowCount(); i++) {
            qobject_cast<QWeaponTableSlot*>(cellWidget(i, 0))->lockSignals();
        }
    }
    void unlockWidgetSignals() {
        for (int i = 0; i < rowCount(); i++) {
            qobject_cast<QWeaponTableSlot*>(cellWidget(i, 0))->unlockSignals();
        }
    }

    // debug
    void printWeaponsTableDictionary() {
        qDebug() << "qweapontablewidget.h: print all weapon table dicitonary entries: ";
        for (auto i : weaponsTableDictionary.keys()) {
            QWeaponTableSlot* weapon = weaponsTableDictionary[i];
            qDebug() << i << ":" << weapon->getWeaponType();
        }
    }
    void printWeaponsTable() {
        qDebug() << "qweapontablewidget.h: print all weapon table row entries: ";
        for (int i = 0; i < rowCount(); i++) {
            if (cellWidget(i, 0) == nullptr) continue;

            QWeaponTableSlot* weapon = qobject_cast<QWeaponTableSlot*>(cellWidget(i,0));
            qDebug() << "row: " << i << " has weapon: " << weapon->getWeaponType();
        }
    }

private:
    // variables
    QHash<int, QWeaponTableSlot*> weaponsTableDictionary;
    QHash<int, bool> weaponsEnabledDictionary = {
        {0x01, false},
        {0x02, false},
        {0x03, false},
        {0x04, false},
        {0x05, false},
        {0x07, false},
        {0x09, false},
        {0x0A, false},
        {0x0C, false},
        {0x0D, false},
    };
    QVector<QWeaponTableSlot*> enabledWeapons;
    int totalWeapons = 10;
    int enabledWeaponsCount = 0;
    QString disabledColor = "darkGray";
    QString enabledColor = "lightblue";
    QString enabledButLeftBehindColor = "lightGray";

    // methods
    int findTableWidgetIndex(const QWeaponTableSlot* weaponSlot);
    void reorderTable(QVector<int> weapons);
    void resetTable() { reorderTable({1, 2, 3, 4, 5, 7, 9, 10, 12, 13}); };
    void paintEnabledRows();
    int getGapRow(const QPoint &pos, int tol = 6) {
        int rows = rowCount();
        int y = pos.y();

        // Before the first row
        int firstTop = rowViewportPosition(0);
        if (y < firstTop + tol)
            return 0;

        // Check internal gaps
        for (int r = 0; r < rows - 1; r++) {
            int bottom = rowViewportPosition(r) + rowHeight(r);
            int nextTop = rowViewportPosition(r + 1);

            if (y > bottom - tol && y < nextTop + tol)
                return r + 1;
        }

        // After last row
        int lastBottom = rowViewportPosition(rows - 1) + rowHeight(rows - 1);
        if (y > lastBottom - tol)
            return rows;

        // Cursor is *inside* a row
        return -1;
    }

signals:
    void weaponTableChanged(QVector<int> enabledWeapons);

public slots:
    void paintTable(QWeaponTableSlot* weapon, int enabled);

protected:
    // drag event
    void dragMoveEvent(QDragMoveEvent* event) override {
        // qDebug() << "drag event";

        // get the item at mouse cursor
        QPoint pos = event->position().toPoint();

        // getGapRow detects if we are in between rows
        if (getGapRow(pos) < 0) {
            event->ignore();
            return;
        }

        // if we're valid continue the event!
        QTableWidget::dragMoveEvent(event);
    }

    void dropEvent(QDropEvent* event) override {
        // the OG function!
        QTableWidget::dropEvent(event);

        // deselect everything
        clearSelection();
        setCurrentIndex(QModelIndex());

        // printWeaponsTable();
        paintEnabledRows();
        emit weaponTableChanged(getValidEnabledWidgets());
    }
};

// event filters
class QTableWidgetEventFilters : public QObject
{
public:
    explicit QTableWidgetEventFilters(QObject *parent = nullptr) : QObject(parent) {}

protected:
    // events
    bool eventFilter(QObject *obj, QEvent *event) override {
        QTableWidget *table = qobject_cast<QTableWidget*>(obj->parent());

        // disable double clicking
        if (event->type() == QEvent::MouseButtonDblClick) {
            return true;
        }

        // when releasing
        if (event->type() == QEvent::MouseButtonRelease) {
            // Clear selection and then remove the focus
            table->clearSelection();
            table->setCurrentIndex(QModelIndex());
            return true;
        }

        // when releasing drop but not dropped!
        if (event->type() == QEvent::DragLeave) {
            // we have no mouse button pressed = we must have dropped it on the same tab
            if (QGuiApplication::mouseButtons() == Qt::NoButton) {
                table->clearSelection();
                table->setCurrentIndex(QModelIndex());
                event->ignore();
                return true;
            }
        }

        // call the OG function to do the rest of the events
        return QObject::eventFilter(obj, event);
    }
};

#endif // QWEAPONTABLEWIDGET_H
