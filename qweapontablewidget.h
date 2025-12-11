#ifndef QWEAPONTABLEWIDGET_H
#define QWEAPONTABLEWIDGET_H

#include <QTableWidget>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <qapplication.h>

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
    QVector<QWeaponTableSlot*> getValidEnabledWeaponPointers();
    QString getWeaponIcon();

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
    int dropIndicatorGap = -1;
    QString disabledColor = "darkGray";
    QString enabledColor = "lightblue";
    QString enabledButLeftBehindColor = "lightGray";
    QPoint pressedEventPos;
    int draggingFromRow = -1;

    // methods
    int findTableWidgetIndex(const QWeaponTableSlot* weaponSlot);
    void reorderTable(QVector<int> weapons);
    void resetTable() { reorderTable({1, 2, 3, 4, 5, 7, 9, 10, 12, 13}); };
    void paintEnabledRows();
    int getGapRow(const QPoint &pos, int tol);

signals:
    void weaponTableChanged(QVector<int> enabledWeapons);

public slots:
    void paintTable(QWeaponTableSlot* weapon, int enabled);

protected:
    void startDrag(Qt::DropActions supportedActions) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dragLeaveEvent(QDragLeaveEvent * event) override;
    void dropEvent(QDropEvent* event) override;
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
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
        // if (event->type() == QEvent::MouseButtonDblClick) {
        //     event->ignore();
        //     return true;
        // }

        // when releasing
        if (event->type() == QEvent::MouseButtonRelease) {
            // Clear selection and then remove the focus
            table->clearSelection();
            table->setCurrentIndex(QModelIndex());
            return true;
        }

        // call the OG function to do the rest of the events
        return QObject::eventFilter(obj, event);
    }
};

#endif // QWEAPONTABLEWIDGET_H
