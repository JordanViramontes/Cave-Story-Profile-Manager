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

    // set values
    weaponLvls = {10, 20, 30, 30};
    lvlChanged(0);

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
    qDebug() << "qeapontableslot.cpp: TODO: XP CHANGE! " << newXp;
}

void QWeaponTableSlot::ammoChanged(int newAmmo) {
    qDebug() << "qeapontableslot.cpp: TODO: AMMO CHANGE! " << newAmmo;
}

void QWeaponTableSlot::ammoMaxChanged(int newAmmoMax) {
    qDebug() << "qeapontableslot.cpp: TODO: MAX AMMO CHANGE! " << newAmmoMax;
}

void QWeaponTableSlot::lvlChanged(int newlvl) {
    qDebug() << "qeapontableslot.cpp: TODO: LVL CHANGE! " << newlvl;

    // for initializing
    ui->lvlComboBox->setCurrentIndex(newlvl);
    lvl = newlvl;

    // set level values
    int maxXp = weaponLvls[newlvl];
    ui->xpSlider->setMinimum(0);
    ui->xpSlider->setMaximum(maxXp);
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

    ui->xpSlider->setEnabled(true);
    ui->xpCurrentSpin->setEnabled(true);
    ui->slashTopLabel->setEnabled(true);
    ui->xpMaxLabel->setEnabled(true);


}


















