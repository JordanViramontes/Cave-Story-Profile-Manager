#ifndef QINVENTORY_H
#define QINVENTORY_H

#include "profileloader.h"
#include <QWidget>

namespace Ui {
class QInventory;
}

class QInventory : public QWidget
{
    Q_OBJECT

public:
    explicit QInventory(QWidget *parent = nullptr);
    ~QInventory();

private:
    ProfileLoader parser;

public slots:
    void _onSelectFile(QString filePath);
    void _onUpdateSelectWeaponChoices(QVector<int> weapons);
    void _PushInventoryToProfile(QString gameDirectory);

private:
    Ui::QInventory *ui;
};

#endif // QINVENTORY_H
