#include "qweapontableslot.h"
#include "ui_qweapontableslot.h"

QWeaponTableSlot::QWeaponTableSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponTableSlot)
{
    ui->setupUi(this);
}

QWeaponTableSlot::QWeaponTableSlot(int intType, bool hasAmmo, QString text, int intMaxAmmo, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponTableSlot)
{
    ui->setupUi(this);

    // set values
    type = intType;
    weaponLvls = {10, 20, 30, 30};
    lvlChanged(0);
    ammoMaxChanged(intMaxAmmo);

    // visual changes
    ui->weaponIconLabel->setText(text);

    // event filters
    QWeaponSlotEventFilters* eventFilters = new QWeaponSlotEventFilters(this);
    ui->xpSlider->installEventFilter(eventFilters);
    ui->ammoSlider->installEventFilter(eventFilters);
    ui->lvlComboBox->installEventFilter(eventFilters);
    ui->xpCurrentSpin->installEventFilter(eventFilters);
    ui->ammoCurrentSpin->installEventFilter(eventFilters);
    ui->ammoMaxSpin->installEventFilter(eventFilters);


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

    // adjust size!
    adjustSize();

    // connections
    connect(ui->xpSlider, SIGNAL(valueChanged(int)), this, SLOT(xpChanged(int)));
    connect(ui->xpCurrentSpin, SIGNAL(valueChanged(int)), this, SLOT(xpChanged(int)));

    connect(ui->ammoSlider, SIGNAL(valueChanged(int)), this, SLOT(ammoChanged(int)));
    connect(ui->ammoCurrentSpin, SIGNAL(valueChanged(int)), this, SLOT(ammoChanged(int)));

    connect(ui->ammoMaxSpin, SIGNAL(valueChanged(int)), this, SLOT(ammoMaxChanged(int)));

    connect(ui->lvlComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(lvlChanged(int)));
}

QWeaponTableSlot::~QWeaponTableSlot()
{
    delete ui;
}

// slots
void QWeaponTableSlot::xpChanged(int newXp) {
    xp = newXp;

    // update uis
    ui->xpSlider->setValue(xp);
    ui->xpCurrentSpin->setValue(xp);
}

void QWeaponTableSlot::ammoChanged(int newAmmo) {
    ammo = newAmmo;

    // update uis
    ui->ammoSlider->setValue(ammo);
    ui->ammoCurrentSpin->setValue(ammo);
}

void QWeaponTableSlot::ammoMaxChanged(int newAmmoMax) {
    maxAmmo = newAmmoMax;

    // update uis
    ui->ammoSlider->setMaximum(maxAmmo);
    ui->ammoMaxSpin->setValue(newAmmoMax);

    if (newAmmoMax == 0) {
        ui->ammoSlider->setEnabled(false);
        ui->ammoSliderLabel->setEnabled(false);
        ui->ammoCurrentSpin->setEnabled(false);
    }
    else {
        ui->ammoSlider->setEnabled(true);
        ui->ammoSliderLabel->setEnabled(true);
        ui->ammoCurrentSpin->setEnabled(true);
    }
}

void QWeaponTableSlot::lvlChanged(int newlvl) {
    lvl = newlvl;

    // set level values
    double ogXpRatio = (1.0 * xp) / (1.0 * maxXp);
    maxXp = weaponLvls[newlvl];
    ui->xpSlider->setMinimum(0);
    ui->xpSlider->setMaximum(maxXp);
    xpChanged(maxXp * ogXpRatio);
    ui->xpMaxLabel->setText(QString::number(maxXp));

    // check if we're at max, if we are disable all xp stuff p much
    if (newlvl >= 3) {
        // disable ui elements
        ui->xpSlider->setEnabled(false);
        ui->xpCurrentSpin->setEnabled(false);
        ui->slashTopLabel->setEnabled(false);
        ui->xpMaxLabel->setEnabled(false);

        // change the XP amount visually to the max of lvl3
        xpChanged(maxXp);

        return;
    }
    else {
        // turn on ui's in case the prev lvl was max
        ui->xpSlider->setEnabled(true);
        ui->xpCurrentSpin->setEnabled(true);
        ui->slashTopLabel->setEnabled(true);
        ui->xpMaxLabel->setEnabled(true);
    }
}


















