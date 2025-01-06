#include "tablewidgetdragrows.h"

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

    setRowCount(weaponTotal);
    setColumnCount(1);

    // fill table
    for (int i = 0; i < weaponTotal; i++) {
        // create widget
        WeaponWidget *widget = new WeaponWidget(this);
        widget->setParent(this);
        widget->setTablePosition(i);

        // UI
        setCellWidget(i, 0, widget);
        setRowHeight(i, rowHeight);

        // connection for updating the painter for each row dynamically
        connect(widget, &WeaponWidget::isEnabledChanged, this, [this, i]() {
            updatePainter(i, 0); // Call your custom update logic
        });
    }

    // manually edit values
    qobject_cast<WeaponWidget*>(cellWidget(0, 0))->setName("PS");
    qobject_cast<WeaponWidget*>(cellWidget(0, 0))->setXpNeeded({10, 20, 10, 0});

    qobject_cast<WeaponWidget*>(cellWidget(1, 0))->setName("MG");
    qobject_cast<WeaponWidget*>(cellWidget(1, 0))->setXpNeeded({30, 40, 10, 0});

    qobject_cast<WeaponWidget*>(cellWidget(2, 0))->setName("SK");
    qobject_cast<WeaponWidget*>(cellWidget(2, 0))->setXpNeeded({30, 40, 16, 0});

    qobject_cast<WeaponWidget*>(cellWidget(3, 0))->setName("SP");
    qobject_cast<WeaponWidget*>(cellWidget(3, 0))->setXpNeeded({40, 60, 200, 0});

    qobject_cast<WeaponWidget*>(cellWidget(4, 0))->setName("ML");
    qobject_cast<WeaponWidget*>(cellWidget(4, 0))->setXpNeeded({10, 20, 10, 0});

    qobject_cast<WeaponWidget*>(cellWidget(5, 0))->setName("SM");
    qobject_cast<WeaponWidget*>(cellWidget(5, 0))->setXpNeeded({30, 60, 10, 0});

    qobject_cast<WeaponWidget*>(cellWidget(6, 0))->setName("FB");
    qobject_cast<WeaponWidget*>(cellWidget(6, 0))->setXpNeeded({10, 20, 20, 0});

    qobject_cast<WeaponWidget*>(cellWidget(7, 0))->setName("BB");
    qobject_cast<WeaponWidget*>(cellWidget(7, 0))->setXpNeeded({10, 20, 5, 0});

    qobject_cast<WeaponWidget*>(cellWidget(8, 0))->setName("BL");
    qobject_cast<WeaponWidget*>(cellWidget(8, 0))->setXpNeeded({30, 60, 0, 0});

    qobject_cast<WeaponWidget*>(cellWidget(9, 0))->setName("NS");
    qobject_cast<WeaponWidget*>(cellWidget(9, 0))->setXpNeeded({1, 1, 1, 0});


    // delegate
    WeaponsDelegate * delegate = new WeaponsDelegate(5, weaponTotal, this);
    setItemDelegate(delegate);



}
