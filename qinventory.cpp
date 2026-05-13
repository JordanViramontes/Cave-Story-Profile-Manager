#include "qinventory.h"
#include "ui_qinventory.h"
#include "widgetfunctions.h"
#include <QFile>

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
    // check that the file is valid
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "qinventory.cpp: ERROR, parsing file isnt open!";
        return;
    }
    if (file.size() != 1540) {
        qDebug() << "qinventory.cpp: ERROR, parsing file size isn't correct for profile!";
        return;
    }

    // set variables for later
    QVector<int> enabledWeapons;

    // reset weapons table
    ui->weaponsTable->resetAllWeapons();

    // parse profile
    qDebug() << "qinventory.cpp: parsing " << filePath;

    // set buffer and parse!
    buffer = file.readAll();
    file.close();

    // parse easy bytes
    map         = buffer[0x008];
    song        = buffer[0x00C];
    horizPos[0] = buffer[0x010];
    horizPos[1] = buffer[0x011];
    horizPos[2] = buffer[0x012];
    horizPos[3] = buffer[0x013];
    vertPos[0]  = buffer[0x014];
    vertPos[1]  = buffer[0x015];
    vertPos[2]  = buffer[0x016];
    vertPos[3]  = buffer[0x017];
    facingDir   = buffer[0x018];
    whimsicalSt = buffer[0x01E];
    currWeapon  = buffer[0x024];
    equipIt[0]  = buffer[0x02C];
    equipIt[1]  = buffer[0x02D];
    time[0]     = buffer[0x034];
    time[1]     = buffer[0x035];
    time[2]     = buffer[0x036];

    // convert health arrays into a short, set health
    char maxHp[2] =  {buffer[0x01C], buffer[0x01D]}; // 01C-01D
    char currHp[2] = {buffer[0x020], buffer[0x021]}; // 020-021
    short hp = 0, mHp = 0;
    std::memcpy(&hp, currHp, sizeof(short));
    std::memcpy(&mHp, maxHp, sizeof(short));
    maxHealthChanged((int)mHp);
    healthChanged((int)hp);

    // parse weapons
    unsigned int weaponIt = 0x038;
    for (unsigned int i = 0; i < 8; i++) {
        char type = buffer[weaponIt];
        char level = buffer[weaponIt+0x04];
        char energy = buffer[weaponIt+0x08];
        char maxAmmo = buffer[weaponIt+0x0C];
        char currentAmmo = buffer[weaponIt+0x10];

        // apply to weapon
        if (type == 0x00) continue;

        // update our widgets with weapon data
        enabledWeapons.push_back((int)type);
        ui->weaponsTable->setWeaponFromParser(type, true, level, energy, maxAmmo, currentAmmo);

        // update iterator
        weaponIt += 0x14;
    }

    // update weapon widget table order
    ui->weaponsTable->reorderTable(enabledWeapons);

    // update weapons order table
    ui->weaponOrderTable->setAllSlots(enabledWeapons);

    // update selected weapon combo box
    onUpdateSelectWeaponChoices(ui->weaponsTable->getValidEnabledWidgets());
    ui->selectedWeaponCombo->setCurrentIndex((int)currWeapon);

    // parse items
    // unsigned int itemIt = 0x0D8;
    // for (unsigned int i = 0; i < items.size(); i++) {
    //     items[i] = buffer[itemIt];
    //     itemIt += 0x04;
    // }

    // parse teleporter
    // unsigned int teleportIt = 0x158;
    // for (unsigned int i = 0; i < teleporters.size(); i++) {
    //     TeleportDataSlot t;
    //     t.menu = buffer[teleportIt];
    //     t.location[0] = buffer[teleportIt + 0x05];
    //     t.location[1] = buffer[teleportIt + 0x06];

    //     teleporters[i] = t;
    //     teleportIt += 0x08;
    // }
}

void QInventory::PushInventoryToProfile(QString profilePath) {
    // new
    qDebug() << "profileloader.cpp: writing to file " << profilePath;

    // check that the file is valid
    QFile file(profilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "qinventory.cpp: ERROR, file isnt open!";
        return;
    }

    // convert health back to char array
    char charHp[2] = {0};
    char charMaxHp[2] = {0};
    std::memcpy(charHp, &hp, sizeof(short));
    std::memcpy(charMaxHp, &maxHp, sizeof(short));

    // set easy bytes
    buffer[0x008] = map;
    buffer[0x00C] = song;
    buffer[0x010] = horizPos[0];
    buffer[0x011] = horizPos[1];
    buffer[0x012] = horizPos[2];
    buffer[0x013] = horizPos[3];
    buffer[0x014] = vertPos[0];
    buffer[0x015] = vertPos[1];
    buffer[0x016] = vertPos[2];
    buffer[0x017] = vertPos[3];
    buffer[0x018] = facingDir;
    buffer[0x01C] = charMaxHp[0];
    buffer[0x01D] = charMaxHp[1];
    buffer[0x01E] = whimsicalSt;
    buffer[0x020] = charHp[0];
    buffer[0x021] = charHp[1];
    buffer[0x024] = (char)ui->selectedWeaponCombo->currentIndex(); // current weapon
    buffer[0x02C] = equipIt[0];
    buffer[0x02D] = equipIt[1];
    buffer[0x034] = time[0];
    buffer[0x035] = time[1];
    buffer[0x036] = time[2];

    // get our weapon table data
    QVector<QWeaponTableSlot*> enabledWeapons = ui->weaponsTable->getValidEnabledWeaponPointers();

    // iterate through all known weapons AND the buffer weapon iterator
    for (
        unsigned int weaponsIt = 0, parserIt = 0x038;
        parserIt < 0x0D4;
        weaponsIt++, parserIt += 0x14
        )
    {
        // set our initial data
        char newType = 0x00;
        char newLevel = 0x00;
        char newEnergy = 0x00;
        char newMaxAmmo = 0x00;
        char newAmmo = 0x00;

        // if we still have valid weapons to iterate throuhg, update data
        if (weaponsIt < enabledWeapons.size()) {
            newType = (char)enabledWeapons[weaponsIt]->getWeaponType();
            newLevel = (char)enabledWeapons[weaponsIt]->getWeaponLevel() + 1;
            newEnergy = (char)enabledWeapons[weaponsIt]->getWeaponEnergy();
            newMaxAmmo = (char)enabledWeapons[weaponsIt]->getWeaponMaxAmmo();
            newAmmo = (char)enabledWeapons[weaponsIt]->getWeaponAmmo();

            // qDebug() << "check type: " << enabledWeapons[weaponsIt]->getWeaponType();
        }

        // update buffer
        buffer[parserIt]        = newType;
        buffer[parserIt + 0x04] = newLevel;
        buffer[parserIt + 0x08] = newEnergy;
        buffer[parserIt + 0x0C] = newMaxAmmo;
        buffer[parserIt + 0x10] = newAmmo;
    }

    // set items
    // unsigned int itemIt = 0x0D8;
    // for (unsigned int i = 0; i < items.size(); i++) {
    //     buffer[itemIt] = items[i];
    //     itemIt += 0x04;
    // }

    // set teleporter
    // unsigned int teleportIt = 0x158;
    // for (unsigned int i = 0; i < teleporters.size(); i++) {
    //     TeleportDataSlot t = teleporters[i];
    //     buffer[teleportIt]        = t.menu;
    //     buffer[teleportIt + 0x05] = t.location[0];
    //     buffer[teleportIt + 0x06] = t.location[1];

    //     teleportIt += 0x08;
    // }

    // write buffer to file
    file.resize(0);
    file.write(buffer);

    file.close();
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


