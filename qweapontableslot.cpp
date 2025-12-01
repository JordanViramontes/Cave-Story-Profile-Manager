#include "qweapontableslot.h"
#include "ui_qweapontableslot.h"

QWeaponTableSlot::QWeaponTableSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponTableSlot)
{
    ui->setupUi(this);
}

QWeaponTableSlot::~QWeaponTableSlot()
{
    delete ui;
}
