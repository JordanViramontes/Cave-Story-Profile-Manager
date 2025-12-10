#include "qweaponordertable.h"
#include "ui_qweaponordertable.h"

QWeaponOrderTable::QWeaponOrderTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QWeaponOrderTable)
{
    ui->setupUi(this);

    // test
    // ui->weaponCatalogueLabel->setVisible(false);
    // ui->weaponCatalogueContainer->setVisible(false);

    // set pixmaps
    for (auto i : {ui->slot0, ui->slot1, ui->slot2, ui->slot3, ui->slot4,
        // ui->EMPTY, ui->PS, ui->ML, ui->FB, ui->BB, ui->MG, ui->BL, ui->SN, ui->SML, ui->NS, ui->SP, }) {
         }) {

        // get pixmap dimensions
        QPixmap p = i->pixmap();
        int w = i->width();
        int h = i->height();

        // set pixmap aspect ratio!
        i->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));

        // if (i == ui->PS) {
        //     qDebug() << "qweaponordertable: w, h: " << w << ", " << h;
        // }
    }
}

QWeaponOrderTable::~QWeaponOrderTable()
{
    delete ui;
}

// set the slots!
void QWeaponOrderTable::setAllSlots(QVector<int> weapons) {
    for (int i = 0; i < 5; i++) {
        // if we're still in enable weapons territory
        if (i < weapons.size()) {
            setSlot(i, weapons[i]);
            continue;
        }

        // set to default
        setSlot(i, -1);
    }
}

void QWeaponOrderTable::setSlot(int slot, int weapon) {
    // find the label we need to change
    QLabel * label = nullptr;
    switch(slot) {
    case(0): label = ui->slot0; break;
    case(1): label = ui->slot1; break;
    case(2): label = ui->slot2; break;
    case(3): label = ui->slot3; break;
    case(4): label = ui->slot4; break;
    default: break;
    }

    if (label == nullptr) {
        qDebug() << "qweaponordertable.cpp: ERROR: label is null!" ;
        return;
    }

    // set the image!
    QString imagePath = weaponImageDictionary[weapon];
    QPixmap p(imagePath);
    int w = label->width();
    int h = label->height();
    label->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));
}

// set the highlighted slot
void QWeaponOrderTable::setHighlightedSlot(int slot) {
    // if nothing just reset everything
    if (slot < 0) {
        // change the color of the frames
        for (auto i : {ui->frame, ui->frame_1, ui->frame_2, ui->frame_3, ui->frame_4}) {
            i->setStyleSheet(QString(R"(
            QWidget { background-color: %1 }
            )").arg("none"));
        }

        return;
    }

    // get the correct frame to color
    QFrame* frame = nullptr;
    switch(slot) {
    case(0): frame = ui->frame; break;
    case(1): frame = ui->frame_1; break;
    case(2): frame = ui->frame_2; break;
    case(3): frame = ui->frame_3; break;
    case(4): frame = ui->frame_4; break;
    default: break;
    }

    if (frame == nullptr) { qDebug() << "ERROR WITH SELECTING WEAPON PREVIEW HIGHLIGHT!!"; return; }
    // change the color of the frames
    for (auto i : {ui->frame, ui->frame_1, ui->frame_2, ui->frame_3, ui->frame_4}) {
        // if we're at the selected frame, set color to blue
        if (i == frame) {
            i->setStyleSheet(QString(R"(
                QWidget { background-color: %1 }
            )").arg(colorFrameSelected));
        }

        // else reset it
        else {
            i->setStyleSheet(QString(R"(
                QWidget { background-color: %1 }
            )").arg("none"));
        }
    }


}

// slots

// used for changing the UI icons
void QWeaponOrderTable::weaponUiChanged(QVector<int> enabledWeapons) {
    int overallIterator = 0;
    for (int i = 0; i < enabledWeapons.size(); i++) {
        setSlot(i, enabledWeapons[i]);
        overallIterator++;
    }

    for (int i = overallIterator; i < 5; i++) {
        setSlot(i, -1);
    }
}











