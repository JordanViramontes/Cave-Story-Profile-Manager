#include "dialog.h"
#include "ui_dialog.h"
#include "qweapontableslot.h"
#include "ui_qweapontableslot.h"

#include <QStackedWidget>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // overwrite button for save as, if clicked output a reject
    connect(ui->overwriteButton, &QPushButton::clicked, this, &QDialog::reject);

    // add an example tablet widget, and set custom data to the example
    auto weapon = qobject_cast<QWeaponTableSlot*>(ui->exampleTableWidget->cellWidget(7, 0));
    ui->exampleTableWidget->setCellWidget(0, 0, weapon);
    ui->exampleTableWidget->setRowCount(1);
    ui->exampleTableWidget->adjustSize();
    weapon->adjustSize();

    // set weapon tooltips and info
    weapon->setData(true, 1, 10, 10, 5);
    auto weaponUi = weapon->getUi();
    weaponUi->grabHandleIcon->setToolTip("Click and Drag this icon to reorder the Weapon in the Weapons Table.");
    weaponUi->enableCheck->setToolTip("Click to Enable/Disable the Weapon");
    weaponUi->weaponIconLabel->setToolTip("Icon of the Weapon");
    weaponUi->lvlComboBox->setToolTip("Click and Select to change the Weapon's current Level");
    weaponUi->xpSlider->setToolTip("Click and Drag to change the Weapon's Current XP");
    weaponUi->xpCurrentSpin->setToolTip("Type here to change the Weapon's Current XP");
    weaponUi->xpMaxLabel->setToolTip("The XP required for this Weapon to Level Up.\nNOTE: This number depends on the Weapon's currently selected Level");
    weaponUi->ammoSlider->setToolTip("Click and Drag to change the Weapon's Current Ammo\nNOTE: Some Weapons don't have Ammo");
    weaponUi->ammoCurrentSpin->setToolTip("Type here to change the Weapon's Current Ammo\nNOTE: Some Weapons don't have Ammo");
    weaponUi->ammoMaxSpin->setToolTip("Type here to change the Weapon's Maximum Ammo\nNOTE: Some Weapons don't have Ammo");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setStackedWidgetPage(QString pageName) {
    QStackedWidget* widget = ui->stackedWidget;

    // get the page number
    if (!stackedWidgetPageDictionary.contains(pageName)) {
        qDebug() << "dialog.cpp: given page \'" << pageName << "\' doesn't exist!";
    }

    // set the page index
    int pageNumber = stackedWidgetPageDictionary[pageName].index;
    widget->setCurrentIndex(pageNumber);

    // set the window header
    setWindowTitle(stackedWidgetPageDictionary[pageName].header);

    // set all other pages to ignore size policy
    for (int i = 0; i < widget->count(); i++) {
        if (i == pageNumber) {
            widget->widget(i)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            continue;
        }
        widget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    }

    // resize
    if (pageName == "helpScreen") {
        resize(QSize(720, 400));
    }
    else {
        resize(minimumSizeHint());
    }



}
