#include "tablewidgetdragrows.h"
#include "weaponwidget.h"

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
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    horizontalHeader()->setVisible(false);

    setRowCount(weaponTotal);
    setColumnCount(1);

    // fill table
    for (int i = 0; i < weaponTotal; i++) {
        WeaponWidget *widget = new WeaponWidget(this);
        // QLabel *widget = new QLabel(QString::number(i));

        setCellWidget(i, 0, widget);
        setRowHeight(i, rowSize);
    }

    // delegate
    WeaponsDelegate * delegate = new WeaponsDelegate(5, weaponTotal, this);
    setItemDelegate(delegate);

}
