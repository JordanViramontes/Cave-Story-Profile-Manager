#ifndef QWEAPONTABLEWIDGET_H
#define QWEAPONTABLEWIDGET_H

#include <QTableWidget>

#include "qweapontableslot.h"
#include "profileloader.h"


class QWeaponTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    QWeaponTableWidget(QWidget *parent = nullptr);

    // get
    const QHash<int, QWeaponTableSlot*> &getWeaponsTableDictionary() { return weaponsTableDictionary; }

    // set
    void setWeaponsFromParser(const QVector<WeaponDataSlot> parserWeapons);

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
    int totalWeapons = 10;

    // methods
    int findTableWidgetIndex(const QWeaponTableSlot* weaponSlot);
    void reorderTable(QVector<int> weapons);
    void resetTable() { reorderTable({1, 2, 3, 4, 5, 7, 9, 10, 12, 13}); };

protected:
    void dropEvent(QDropEvent* event) override {
        // the OG function!
        QTableWidget::dropEvent(event);

        // deselect everything
        clearSelection();
        setCurrentIndex(QModelIndex());

        printWeaponsTable();
    }
};

// event filters
class QTableWidgetEventFilters : public QObject
{
public:
    explicit QTableWidgetEventFilters(QObject *parent = nullptr) : QObject(parent) {}

protected:
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
                return true;
            }
        }

        // call the OG function to do the rest of the events
        return QObject::eventFilter(obj, event);

    }
};

#endif // QWEAPONTABLEWIDGET_H
