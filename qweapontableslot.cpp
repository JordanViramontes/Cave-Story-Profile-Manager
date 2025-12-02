#include "qweapontableslot.h"
#include "ui_qweapontableslot.h"

QWeaponTableSlot::QWeaponTableSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponTableSlot)
{
    ui->setupUi(this);
}

QWeaponTableSlot::QWeaponTableSlot(bool hasAmmo, QString text, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponTableSlot)
{
    ui->setupUi(this);

    // ammo UI bool
    if (!hasAmmo) {
        ui->ammoCurrentSpin->setEnabled(false);
        ui->ammoMaxSpin->setEnabled(false);
        ui->ammoSlider->setEnabled(false);
        ui->ammoSliderLabel->setEnabled(false);
        ui->ammoSliderLayout->setEnabled(false);
        ui->slashBottomLabel->setEnabled(false);

        ui->ammoCurrentSpin->setVisible(false);
        ui->ammoMaxSpin->setVisible(false);
        ui->ammoSlider->setVisible(false);
        ui->ammoSliderLabel->setVisible(false);
        ui->slashBottomLabel->setVisible(false);
        // ui->ammoSliderLayout->setVisible(false);
    }

    // visual changes
    ui->weaponIconLabel->setText(text);

    // adjust size!
    adjustSize();
}

QWeaponTableSlot::~QWeaponTableSlot()
{
    delete ui;
}
