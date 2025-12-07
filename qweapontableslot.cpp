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
    QWeaponLvlComboBoxEventFilters * filters = new QWeaponLvlComboBoxEventFilters(this);
    ui->lvlComboBox->installEventFilter(filters);


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

    connect(ui->enableCheck, SIGNAL(toggled(bool)), this, SLOT(enableChanged(bool)));
}

QWeaponTableSlot::~QWeaponTableSlot()
{
    delete ui;
}

// signal locks
void QWeaponTableSlot::lockSignals() {
    // xp
    ui->xpSlider->blockSignals(true);
    ui->xpCurrentSpin->blockSignals(true);

    // ammo
    ui->ammoSlider->blockSignals(true);
    ui->ammoCurrentSpin->blockSignals(true);

    // ammoMax
    ui->ammoMaxSpin->blockSignals(true);

    // lvl
    ui->lvlComboBox->blockSignals(true);

    // check
    ui->enableCheck->blockSignals(true);
}

void QWeaponTableSlot::unlockSignals() {
    // xp
    ui->xpSlider->blockSignals(false);
    ui->xpCurrentSpin->blockSignals(false);

    // ammo
    ui->ammoSlider->blockSignals(false);
    ui->ammoCurrentSpin->blockSignals(false);

    // ammoMax
    ui->ammoMaxSpin->blockSignals(false);

    // lvl
    ui->lvlComboBox->blockSignals(false);

    // check
    ui->enableCheck->blockSignals(false);
}

// slots
void QWeaponTableSlot::enableChanged(bool enable) {
    lockSignals(); // lock

    qDebug() << "qweapontableslot.cpp: " << type << "enable changed to: " << enable;

    enableChecked = enable;
    ui->enableCheck->setChecked(enable);
    emit enabledChanged();

    unlockSignals(); // unlock!
}

void QWeaponTableSlot::xpChanged(int newXp) {
    lockSignals(); // lock

    // set values
    xp = newXp;

    // update uis
    ui->xpSlider->setValue(xp);
    ui->xpCurrentSpin->setValue(xp);

    // check if we've hit max and update the combo box if we have
    if (lvl == 2 && xp >= maxXp) ui->lvlComboBox->setCurrentIndex(3);
    else if (lvl == 2) ui->lvlComboBox->setCurrentIndex(2);

    unlockSignals(); // unlock!
}

void QWeaponTableSlot::ammoChanged(int newAmmo) {
    lockSignals(); // lock

    ammo = newAmmo;

    // update uis
    ui->ammoSlider->setValue(ammo);
    ui->ammoCurrentSpin->setValue(ammo);

    unlockSignals(); // unlock!
}

void QWeaponTableSlot::ammoMaxChanged(int newAmmoMax) {
    lockSignals(); // lock

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

    unlockSignals(); // unlock!
}

void QWeaponTableSlot::lvlChanged(int newlvl) {
    lockSignals(); // lock

    // set values
    double ogXpRatio = (1.0 * xp) / (1.0 * maxXp);
    lvl = newlvl;
    maxXp = weaponLvls[lvl];

    // update UI
    ui->xpSlider->setMinimum(0);
    ui->xpSlider->setMaximum(maxXp);
    ui->xpMaxLabel->setText(QString::number(maxXp));
    ui->lvlComboBox->setCurrentIndex(lvl);

    // change xp value if needed
    if (testLvlChangeXpRatio) xp = maxXp * ogXpRatio;
    else if (xp > maxXp) xp = maxXp; // we aren't using the ratio AND xp is higher than max

    // check if we're at max and set xp to max, move lvl back to 2!
    if (newlvl >= 3) {
        lvl = 2;
        xpChanged(maxXp);
        return;
    }

    // change xp if needed!
    xpChanged(xp);

    unlockSignals(); // unlock!
}

// set
void QWeaponTableSlot::setColor() {

}
















