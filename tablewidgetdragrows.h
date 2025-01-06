#include <QApplication>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QAbstractItemView>
#include <QStyledItemDelegate>
#include <QVariant>
#include <QPainter>

class TableWidgetDragRows : public QTableWidget {
    Q_OBJECT

public:
    TableWidgetDragRows(QWidget* parent = nullptr);

    // get
    int getWeaponTotal() const { return weaponTotal; }

protected:
    void dropEvent(QDropEvent* event) override {
        if (event->source() == this && (event->dropAction() == Qt::MoveAction || dragDropMode() == QAbstractItemView::InternalMove)) {
            // get dropRow
            int dropRow = rowAt(event->position().toPoint().y());
            if (dropRow == -1) dropRow = rowCount();

            // get the list of selected rows
            QList<int> selectedRows = getSelectedRowsFast();
            if (selectedRows.size() <= 0) return; // in case its empty
            int offset = dropRow - selectedRows.first() + 1;

            // iterate over selected rows and then swap
            for (int row : selectedRows) {
                int targetRow = row + offset;
                if (targetRow < 0) targetRow = 0;
                if (targetRow >= rowCount()) targetRow = weaponTotal;

                // check if we're moving top->bottom or bottom->top
                if (dropRow < row) {
                    targetRow = targetRow - 1;
                    row++;
                }

                // insert row and then set the widget to that row, then remove the og
                insertRow(targetRow);
                setRowHeight(targetRow, rowSize);

                QWidget *widget = cellWidget(row, 0);
                if (widget) {
                    setCellWidget(targetRow, 0, widget);
                }

                removeRow(row);
            }
            event->accept();

            // clear all ui stuff
            clearSelection();
        }
        else {
            qDebug() << "ELSE";
            QTableWidget::dropEvent(event);
        }
    }

private:
    int weaponTotal = 8;
    const int rowSize = 40;

    QList<int> getSelectedRowsFast() {
        QSet<int> uniqueRows; // Use QSet to avoid duplicates
        for (const QModelIndex& index : selectionModel()->selectedIndexes()) {
            uniqueRows.insert(index.row());
        }
        return QList<int>(uniqueRows.begin(), uniqueRows.end());
    }
};

class WeaponsDelegate : public QStyledItemDelegate
{
public:
    WeaponsDelegate(int startRow, int endRow, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), m_startRow(startRow), m_endRow(endRow) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        if (index.row() >= m_startRow && index.row() <= m_endRow) {
            // Set the background color to red
            painter->fillRect(option.rect, Qt::darkRed);
        }

        // Call the base class to draw the rest of the cell (text, borders, etc.)
        QStyledItemDelegate::paint(painter, option, index);
    }

private:
    int m_startRow;
    int m_endRow;
};
