#ifndef QWEAPONTABLEWIDGET_H
#define QWEAPONTABLEWIDGET_H

#include <QTableWidget>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <qapplication.h>

#include "qweapontableslot.h"


class QWeaponTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    QWeaponTableWidget(QWidget *parent = nullptr);

    // get
    QVector<int> getValidEnabledWidgets();
    QVector<QWeaponTableSlot*> getValidEnabledWeaponPointers();
    QString getWeaponIcon();

    // set
    void resetAllWeapons();
    void setWeaponFromParser(int type, bool iniEnabled, int iniLvl, int iniEnergy, int iniMaxAmmo, int iniCurrentAmmo);
    void reorderTable(QVector<int> weapons);

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
