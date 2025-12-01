#include "weaponslot.h"
#include "ui_weaponslot.h"

WeaponSlot::WeaponSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeaponSlot)
{
    ui->setupUi(this);
}

WeaponSlot::~WeaponSlot()
{
    delete ui;
}
