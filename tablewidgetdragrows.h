#include "profileloader.h"

#include "qcheckbox.h"
#include "qcombobox.h"
#include "qlineedit.h"
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
#include <QLabel>
#include <QTextEdit>


#pragma once // to avoid redefinition error

class TableWidgetDragRows : public QTableWidget {
    Q_OBJECT

public:
    TableWidgetDragRows(QWidget* parent = nullptr);
    void newWeaponTable(QVector<WeaponSlot> &weapons);

    // get
    int getWeaponTotal() const { return weaponTotal; }

    // set

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
                setRowHeight(targetRow, rowHeight);

                QWidget *widget = cellWidget(row, 0);
                if (widget) {
                    setCellWidget(targetRow, 0, widget);
                }

                removeRow(row);

                // set scrollbar in a good spot
                if (targetRow < 5) scrollTo(model()->index(0, 0));
                else scrollTo(model()->index(rowCount()-1, 0));

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
    int weaponTotal = 10;
    int maxMissileAmmo = 50;
    const int rowHeight = 60;

    QList<int> getSelectedRowsFast() {
        QSet<int> uniqueRows; // Use QSet to avoid duplicates
        for (const QModelIndex& index : selectionModel()->selectedIndexes()) {
            uniqueRows.insert(index.row());
        }
        return QList<int>(uniqueRows.begin(), uniqueRows.end());
    }

public slots:
    void updatePainter(int row, int column) {
        QRect rowRect = visualRect(model()->index(row, 0));
        rowRect.setRight(viewport()->width());  // Ensure it covers the full width of the table
        update(rowRect);  // Update the entire row
    }

    void updateMaxAmmo(QString str) {
        maxMissileAmmo = str.toInt(); // should always be int due to validators

        qDebug() << "in: " << maxMissileAmmo;

        emit ammoChanged(maxMissileAmmo);
    }

    void updateMaxAmmo(int n) {
        maxMissileAmmo = n; // should always be int due to validators

        emit ammoChanged(maxMissileAmmo);
    }

signals:
    void ammoChanged(int n);
};

class WeaponWidget : public QWidget
{
    Q_OBJECT

public:
    WeaponWidget(int &w, bool &missile, QString &n, int &l, int &x, QVector<int> &xN,
                 TableWidgetDragRows *pT, QWidget *parent = nullptr);

    // get
    QString getName() const { return name; }
    int getlvl() const { return lvl; }
    int getXp() const { return xp; }
    QVector<int> getxpNeeded() const { return xpNeeded; }
    bool getIsEnabled() const { return isEnabled; }
    int getWeaponType() const { return weaponType; }
    bool getIsMissile() const { return isMissile; }

    // set
    void setName(QString n) {
        name = n;

        // update widget
        icon->setText(name);
        icon->update();
    }
    void setLvl(int l) {
        // qDebug() << "incoming level: " << l;
        lvl = l;

        // update xp amount
        if (xp > xpNeeded[lvl-1]) xp = xpNeeded[lvl-1];

        // update lvl label
        level->setCurrentIndex(lvl-1);

        // update text label
        xpNeededLabel->setText("/" + QString::number(xpNeeded[lvl-1]));
        xpNeededLabel->update();

        // update slider values
        slider->setRange(0, xpNeeded[lvl-1]);
        slider->update();

        // update xpText validator
        QValidator *validator = const_cast<QValidator *>(xpText->validator());
        if (QIntValidator *intVal = dynamic_cast<QIntValidator *>(validator)) {
            // Set a new range for the integer validator
            intVal->setBottom(0.0);  // New minimum value
            intVal->setTop(xpNeeded[lvl-1]);    // New maximum value

            // qDebug() << "New range:" << intVal->bottom() << "to" << intVal->top();
        }
        xpText->update();

        // update xpText value if MAX
        if (xpText->text().toInt() > xpNeeded[lvl-1]) xpText->setText(QString::number(xpNeeded[lvl-1]));

        // qDebug() << "xp: " << xp;
    }
    void setXp(int x) {
        xp = x;

        // slider
        slider->setValue(xp);

        //
        xpText->setText(QString::number(xp));
    }
    void setXpNeeded(QVector<int> xN) {
        xpNeeded = xN;

        // update widget
        QString str = "/" + QString::number(xpNeeded[lvl-1]);
        xpNeededLabel->setText(str);
        xpNeededLabel->update();

        // update slider values
        slider->setRange(0, xpNeeded[lvl-1]);
        slider->update();

        // update xpEdit validator
        QValidator *validator = const_cast<QValidator *>(xpText->validator());
        if (QIntValidator *intVal = dynamic_cast<QIntValidator *>(validator)) {
            // Set a new range for the integer validator
            intVal->setBottom(0.0);  // New minimum value
            intVal->setTop(xpNeeded[lvl-1]);    // New maximum value

            qDebug() << "New range:" << intVal->bottom() << "to" << intVal->top();
        }
        xpText->update();
    }
    void setParent(TableWidgetDragRows *parent) { parentTable = parent; }
    void setTablePosition(int newPos) { tablePosition = newPos; }
    void setAmmoMax(int n) {
        if (!isMissile) return;
        ammoMax = n;

        // update values
        if (ammoMax < ammo) {
            setAmmo(ammoMax);
        }

        // update labels
        ammoLine->setText("/" + QString::number(ammoMax));


        // update validator
        QValidator *validator = const_cast<QValidator *>(ammoEdit->validator());
        if (QIntValidator *intVal = dynamic_cast<QIntValidator *>(validator)) {
            // Set a new range for the integer validator
            intVal->setBottom(0.0);  // New minimum value
            intVal->setTop(ammoMax);    // New maximum value

            // qDebug() << "New range:" << intVal->bottom() << "to" << intVal->top();
        }

        qDebug() << "set3: " << n;

        ammoEdit->update();
    }
    void setAmmo(int n) {
        if (!isMissile) return;
        ammo = n;

        // qDebug() << "ammo now: " << ammo << ", max: " << ammoMax;
        ammoEdit->setText(QString::number(ammo));
    }
    void setWeaponType(int n) { weaponType = n; }
    void setEnabled(bool e) {
        // change click
        if (e == isEnabled) return;

        // we will actually change isEnabled in the slot for the checkbox
        if (check->checkState() == Qt::Checked) {
            check->setCheckState(Qt::Unchecked);
        }
        else {
            check->setCheckState(Qt::Checked);
        }
    }

    // etc
    void updatePaint() {
        // qDebug() << "updating";
        // update the state of the parent table
        if (parentTable == nullptr) return;

        // emit signals
        emit isEnabledChanged();
        update();  // Force a repaint
    }

private:
    int weaponType = -1;
    bool isEnabled = true;
    bool isMissile = false;
    int tablePosition = 0;
    QString name = "temp";
    int lvl = 1;
    int xp = 0;
    QVector<int> xpNeeded = {-1, -1, -1, 0};
    bool hasAmmo = false;
    int ammo = 0;
    int ammoMax = 0;

    // parent
    TableWidgetDragRows *parentTable = nullptr;

    // subwidgets
    QCheckBox *check;
    QLabel *icon;
    QLabel *levelText;
    QComboBox *level;
    QLabel *xpLabel;
    QSlider *slider;
    QLineEdit *xpText;
    QLabel * xpNeededLabel;
    QLabel * ammoLabel;
    QLineEdit *ammoEdit;
    QLabel * ammoLine;

    QGridLayout * layout;

    // constructors
    QLabel * createIcon();
    QComboBox * createLevel();
    QSlider * createSlider();
    QCheckBox * createCheck();
    QLineEdit * createXpText();
    QLabel * createXpNeededLabel();
    QLineEdit * createAmmoEdit();



signals:
    void isEnabledChanged();

public slots:
    void maxAmmoSlot(int n) {
        qDebug() << "slot";
        setAmmoMax(n);
    }

private slots:
    void enabledUpdate() {
        isEnabled = !isEnabled;

        // update
        updatePaint();
    }
};

Q_DECLARE_METATYPE(WeaponWidget*)

class WeaponsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    WeaponsDelegate(int startRow, int endRow, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), m_startRow(startRow), m_endRow(endRow) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        // get table
        const QTableWidget* table = qobject_cast<const QTableWidget*>(option.widget);
        if (table) {
            // do red rows first
            if (index.row() <= m_endRow && index.row() >= m_startRow) {
                painter->fillRect(option.rect, QColor(0xff363636));
                return;
            }

            // Get the custom widget for this cell
            QWidget* widget = table->cellWidget(index.row(), index.column());
            if (widget) {
                // Cast to your custom class and retrieve the bool value
                WeaponWidget* customWidget = qobject_cast<WeaponWidget*>(widget);
                if (customWidget) {
                    bool isEnabled = customWidget->getIsEnabled();
                    // qDebug() << "isE: " << isEnabled;

                    // Decide background color based on the bool value
                    QColor backgroundColor = Qt::darkBlue;

                    if (!isEnabled) backgroundColor = Qt::darkGray;

                    // Fill the background
                    QRect fullRowRect = option.rect;
                    fullRowRect.setLeft(0); // Extend to the left edge of the table
                    fullRowRect.setRight(table->viewport()->width()); // Extend to the right edge of the table
                    painter->fillRect(fullRowRect, backgroundColor);
                }
            }
        }

        // Call the base class to draw the rest of the cell (text, borders, etc.)
        QStyledItemDelegate::paint(painter, option, index);
    }

private:
    int m_startRow;
    int m_endRow;
};
