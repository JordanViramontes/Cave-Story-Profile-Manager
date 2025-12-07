#include "qweaponordertable.h"
#include "ui_qweaponordertable.h"

QWeaponOrderTable::QWeaponOrderTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponOrderTable)
{
    ui->setupUi(this);

    // set pixmaps
    for (auto i : {ui->slot1, ui->slot2, ui->slot3, ui->slot4, ui->slot5,
        ui->PS, ui->ML, ui->FB, ui->BB, ui->MG, ui->BL, ui->SN, ui->SML, ui->NS, ui->SP, }) {

        // get pixmap dimensions
        QPixmap p = i->pixmap();
        int w = i->width();
        int h = i->height();

        // set pixmap aspect ratio!
        i->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));

        if (i == ui->slot1) {
            qDebug() << "wh: " << w << ", " << h;
        }
    }



    // adjustSize();
}

QWeaponOrderTable::~QWeaponOrderTable()
{
    delete ui;
}
