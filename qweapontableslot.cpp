#include "qweapontableslot.h"
#include "ui_qweapontableslot.h"

QWeaponTableSlot::QWeaponTableSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponTableSlot)
{
    ui->setupUi(this);
}

QWeaponTableSlot::QWeaponTableSlot(int intType, int intMaxAmmo, bool hasAmmo, QString text, QVector<int> iniWeaponLvls, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponTableSlot)
{
    ui->setupUi(this);

    // set values
    type = intType;
    weaponLvls = iniWeaponLvls;
    qDebug() << "qeapontableslot.cpp: weaponLvls: " << weaponLvls;
    lvlChanged(0);
    ammoMaxChanged(intMaxAmmo);
    initialMaxAmmo = intMaxAmmo;


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
    qDebug() << "test" << newXp;
    xp = newXp;

    // update uis
    ui->xpSlider->setValue(xp);
    ui->xpCurrentSpin->setValue(xp);

    // check if we've hit max and update the combo box if we have
    if (lvl == 2 && xp == maxXp) ui->lvlComboBox->setCurrentIndex(3);
    else if (lvl == 2) ui->lvlComboBox->setCurrentIndex(2);
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
    // set values
    lvl = newlvl;
    maxXp = weaponLvls[lvl];

    // update UI
    ui->xpSlider->setMinimum(0);
    ui->xpSlider->setMaximum(maxXp);
    ui->xpMaxLabel->setText(QString::number(maxXp));
    ui->lvlComboBox->setCurrentIndex(lvl);

    // set level values

    // check if we're at max and set xp to max, move lvl back to 2!
    if (newlvl >= 3) {
        lvl = 2;
        xpChanged(maxXp);
        return;
    }

    // edge case to update combo box UI if the new xp lvl hits lvl 3 MAX
    xpChanged(xp);
}


















