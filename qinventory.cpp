#include "qinventory.h"
#include "ui_qinventory.h"
#include "widgetfunctions.h"

QInventory::QInventory(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QInventory)
{
    ui->setupUi(this);

    // widgets
    ui->healthSlider->setMinimum(0);
    ui->maxHealthSpinBox->setMinimum(1);
    ui->maxHealthSpinBox->setValue(1);
    importantWidgets = {
        ui->healthSlider,
        ui->healthSpinBox,
    };

    // connections to big widgets
    connect(ui->weaponsTable, SIGNAL(weaponTableChanged(QVector<int>)), ui->weaponOrderTable, SLOT(weaponUiChanged(QVector<int>)));
    connect(ui->weaponsTable, SIGNAL(weaponTableChanged(QVector<int>)), this, SLOT(onUpdateSelectWeaponChoices(QVector<int>)));
    connect(ui->selectedWeaponCombo, SIGNAL(currentIndexChanged(int)), ui->weaponOrderTable, SLOT(setHighlightedSlot(int)));

    // connections to health bars
    connect(ui->healthSlider, SIGNAL(valueChanged(int)), this, SLOT(healthChanged(int)));
    connect(ui->healthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(healthChanged(int)));
    connect(ui->maxHealthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(maxHealthChanged(int)));
}

QInventory::~QInventory()
{
    delete ui;
}

//=============================
// HEALTH BAR
//=============================

// updating health bar
void QInventory::healthChanged(int newHp) {
    // check bounds
    if (newHp < 0) {
        qDebug() << "qinventory.cpp: ERROR incoming new health too low";
        return;
    }

    signalLock(false, importantWidgets); // lock

    // set values
    if (newHp > maxHp) newHp = maxHp;
    hp = newHp;

    // update uis
    ui->healthSlider->setValue(hp);
    ui->healthSpinBox->setValue(hp);

    signalLock(true, importantWidgets); // unlock
}

void QInventory::maxHealthChanged(int newMaxHp) {
    // check bounds
    if (newMaxHp < 0) {
        qDebug() << "qinventory.cpp: ERROR incoming new max health too low";
        return;
    }

    signalLock(false, importantWidgets); // lock

    maxHp = newMaxHp;
    if (hp > maxHp) healthChanged(maxHp);

    // update box
    ui->maxHealthSpinBox->setValue(maxHp);

    // update slider
    ui->healthSlider->setMaximum(maxHp);


    signalLock(true, importantWidgets); // unlock
}

//====================================
// METHODS
//=====================================

// communicating with main window
void QInventory::onSelectFile(QString filePath) {
    if (!parser.parseProfile(filePath)) {
        qDebug() << "mainwindowslots.cpp: Parsing DID NOT complete";
        return;
    }
    // qDebug() << "mainwindowslots.cpp: Parsing completed with no error";

    // parser.printSaveData();
    // parser.printBuffer();

    // get the enabled weapons!
    QVector<int> enabledWeapons;
    for (auto i : parser.getWeapons()) {
        if (i.type == 0x00) continue;
        // add to known weapons
        enabledWeapons.push_back((int)i.type);
    }

    // get the health!
    QVector<int> healthStuff = parser.getHealthData();
    // qDebug() << "qinventory.cpp: new hp and maxHp: " << healthStuff[0] << ", " << healthStuff[1];
    maxHealthChanged(healthStuff[1]);
    healthChanged(healthStuff[0]);

    // update the weapons table via parser information
    ui->weaponsTable->setWeaponsFromParser(parser.getWeapons(), enabledWeapons);

    // update weapons order table
    ui->weaponOrderTable->setAllSlots(enabledWeapons);

    // update selected weapon
    onUpdateSelectWeaponChoices(ui->weaponsTable->getValidEnabledWidgets());
    ui->selectedWeaponCombo->setCurrentIndex(parser.getCurrentWeapon());
}

void QInventory::PushInventoryToProfile(QString profilePath) {
    // get weapon data!
    QVector<QWeaponTableSlot*> enabledWeapons = ui->weaponsTable->getValidEnabledWeaponPointers();
    QVector<WeaponDataSlot> weaponDataSlots;
    for (int i = 0; i < enabledWeapons.size(); i++) {
        // iterate through the enabled weapons pointer and fill out a WeaponDataSlot
        WeaponDataSlot weaponSlot;
        weaponSlot.type = (char)enabledWeapons[i]->getWeaponType();
        qDebug() << "check type: " << enabledWeapons[i]->getWeaponType();
        weaponSlot.level = (char)enabledWeapons[i]->getWeaponLevel() + 1;
        weaponSlot.energy = (char)enabledWeapons[i]->getWeaponEnergy();
        weaponSlot.maxAmmo = (char)enabledWeapons[i]->getWeaponMaxAmmo();
        weaponSlot.currentAmmo = (char)enabledWeapons[i]->getWeaponAmmo();

        weaponDataSlots.push_back(weaponSlot);;
    }

    // pass health data to parser as a short, parser will convert to char
    parser.setHealthData(static_cast<short>(hp), static_cast<short>(maxHp));


    int check = ui->selectedWeaponCombo->currentIndex();
    char weaponCurrentlySelected = (char)ui->selectedWeaponCombo->currentIndex();
    qDebug() << "check weaponCurrentlySelected count: " << check;

    if (!parser.writeToFile(profilePath, weaponDataSlots, weaponCurrentlySelected)) {
        qDebug() << "mainwindowslots.cpp: Writing to file DID NOT complete";
        return;
    }
}

// communicating with weapon order table
void QInventory::onUpdateSelectWeaponChoices(QVector<int> weapons) {
    // dictionary for later
    QHash<int, QString> weaponTextDictionary = {
        {0x01, "Snake"},
        {0x02, "Polar Star"},
        {0x03, "Fireball"},
        {0x04, "Machine Gun"},
        {0x05, "Missile Launcher"},
        {0x07, "Bubbler"},
        {0x09, "Blade"},
        {0x0A, "Super Missile Launcher"},
        {0x0C, "Nemesis"},
        {0x0D, "Spur"},
    };

    // get the combo box
    QComboBox * combo = ui->selectedWeaponCombo;

    // get the current selection
    QString prevSelection = "";
    if (combo->count() > 0) prevSelection = combo->currentText();

    // resize combo box and add contents
    combo->clear();
    for (auto i : weapons) {
        combo->addItem(weaponTextDictionary[i]);
    }

    // if prev selection is still here keep it!
    for (int i = 0; i < combo->count(); i++) {
        if (combo->itemText(i) == prevSelection) {
            combo->setCurrentIndex(i);
            break;
        }
    }

    // change the color of the selected thing!
    ui->weaponOrderTable->setHighlightedSlot(combo->currentIndex());
}
