#include "profileloader.h"

#include <qdebug.h>
#include <QFile>

ProfileLoader::ProfileLoader() {
    // the size of the file is 60a bytes long
    buffer.resize(0x60a);  // Safer dynamic allocation
    buffer.fill(0);

    // create weapons - 8 weapon slots
    WeaponDataSlot defaultWeapon;
    weapons = QVector<WeaponDataSlot>(8, defaultWeapon);

    // create items - 32 (5rows*6columns + 2) slots
    items = QVector<char>(32, 0);

    // create teleporters - 8 slots
    TeleportDataSlot defaultTele;
    teleporters = QVector<TeleportDataSlot>(8, defaultTele);

    qDebug() << "profileloader.cpp: TODO: IMPLEMENT FLAG SAVING";
}

bool ProfileLoader::parseProfile(QString profilePath) {
    qDebug() << "profileloader.cpp: parsing " << profilePath;

    // check that the file is valid
    QFile file(profilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "profileloader.cpp: ERROR, file isnt open!";
        return false;
    }
    if (file.size() != 1540) {
        qDebug() << "profileloader.cpp: ERROR, file size isn't correct for profile!";
        return false;
    }

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
    maxHp[0]    = buffer[0x01C];
    maxHp[1]    = buffer[0x1D];
    whimsicalSt = buffer[0x01E];
    currHp[0]   = buffer[0x020];
    currHp[1]   = buffer[0x021];
    currWeapon  = buffer[0x024];
    equipIt[0]  = buffer[0x02C];
    equipIt[1]  = buffer[0x02D];
    time[0]     = buffer[0x034];
    time[1]     = buffer[0x035];
    time[2]     = buffer[0x036];

    short hp = 0, mHp = 0;
    std::memcpy(&hp, currHp, sizeof(short));
    std::memcpy(&mHp, maxHp, sizeof(short));
    // qDebug() << "profileloader.cpp: loaded health: " << hp << ", " << mHp;

    // parse weapons
    unsigned int weaponIt = 0x038;
    for (unsigned int i = 0; i < weapons.size(); i++) {
        WeaponDataSlot w;
        w.type = buffer[weaponIt];
        w.level = buffer[weaponIt+0x04];
        w.energy = buffer[weaponIt+0x08];
        w.maxAmmo = buffer[weaponIt+0x0C];
        w.currentAmmo = buffer[weaponIt+0x10];

        weapons[i] = w;
        weaponIt += 0x14;
    }

    // parse items
    unsigned int itemIt = 0x0D8;
    for (unsigned int i = 0; i < items.size(); i++) {
        items[i] = buffer[itemIt];
        itemIt += 0x04;
    }

    // parse teleporter
    unsigned int teleportIt = 0x158;
    for (unsigned int i = 0; i < teleporters.size(); i++) {
        TeleportDataSlot t;
        t.menu = buffer[teleportIt];
        t.location[0] = buffer[teleportIt + 0x05];
        t.location[1] = buffer[teleportIt + 0x06];

        teleporters[i] = t;
        teleportIt += 0x08;
    }

    return true;
}

bool ProfileLoader::writeToFile(QString profilePath, QVector<WeaponDataSlot> weaponDataSlots, char weaponCurrentlySelected) {
    qDebug() << "profileloader.cpp: writing to file " << profilePath;

    // check that the file is valid
    QFile file(profilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "profileloader.cpp: ERROR, file isnt open!";
        return false;
    }

    short hp = 0, mHp = 0;
    std::memcpy(&hp, currHp, sizeof(short));
    std::memcpy(&mHp, maxHp, sizeof(short));
    qDebug() << "profileloader.cpp: writing health: " << hp << ", " << mHp;

    // we need to set the buffer values with the updated stuff!

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
    buffer[0x01C] = maxHp[0];
    buffer[0x01D] = maxHp[1];
    buffer[0x01E] = whimsicalSt;
    buffer[0x020] = currHp[0];
    buffer[0x021] = currHp[1];
    buffer[0x024] = weaponCurrentlySelected; // current weapon
    buffer[0x02C] = equipIt[0];
    buffer[0x02D] = equipIt[1];
    buffer[0x034] = time[0];
    buffer[0x035] = time[1];
    buffer[0x036] = time[2];

    // set weapons
    qDebug() << "profileloader.cpp: printing incoming weapons";
    for (auto i : weaponDataSlots) {
        qDebug() << "type: " << printHex(&i.type, 1) << ";\t"
                 << "level: " << printHex(&i.level, 1) << ";\t"
                 << "energy: " << printHex(&i.energy, 1) << ";\t"
                 << "ammo: " << printHex(&i.currentAmmo, 1) << ";\t"
                 << "maxAmmo: " << printHex(&i.maxAmmo, 1)  << ";\t";
    }

    unsigned int weaponIt = 0x038;
    for (unsigned int i = 0, weaponDataIt = 0; i < weapons.size(); i++) {
        WeaponDataSlot w;

        // if our weaponDataIt is inside the vector, set our weapon to set as the vector weapon
        if (weaponDataIt < weaponDataSlots.size()) {
            w = weaponDataSlots[weaponDataIt];
            weaponDataIt++;
            qDebug() << "check: " << printHex(&w.type, 1);
        }

        buffer[weaponIt]        = w.type;
        buffer[weaponIt + 0x04] = w.level;
        buffer[weaponIt + 0x08] = w.energy;
        buffer[weaponIt + 0x0C] = w.maxAmmo;
        buffer[weaponIt + 0x10] = w.currentAmmo;

        weaponIt += 0x14;
    }

    // set items
    unsigned int itemIt = 0x0D8;
    for (unsigned int i = 0; i < items.size(); i++) {
        buffer[itemIt] = items[i];
        itemIt += 0x04;
    }

    // set teleporter
    unsigned int teleportIt = 0x158;
    for (unsigned int i = 0; i < teleporters.size(); i++) {
        TeleportDataSlot t = teleporters[i];
        buffer[teleportIt]        = t.menu;
        buffer[teleportIt + 0x05] = t.location[0];
        buffer[teleportIt + 0x06] = t.location[1];

        teleportIt += 0x08;
    }

    // write buffer to file
    file.resize(0);
    file.write(buffer);


    file.close();
    return true;
}

void ProfileLoader::printSaveData() {

    // easy stuff
    qDebug().noquote() << "profileloader.cpp: PRINTING DATA:"
                    << "\n\nmap: "          << printHex(&map, 1)
            << "," << "song: "         << printHex(&song, 1)
            << "," << "horizPos: "     << printHex(horizPos, 4)
            << "," << "vertPos: "      << printHex(vertPos, 4)
            << "," << "facingDir: "    << printHex(&facingDir, 1)
            << "," << "maxHp: "        << printHex(maxHp, 2)
            << "," << "whimsicalSt: "  << printHex(&whimsicalSt, 1)
            << "," << "currHp: "       << printHex(currHp, 2)
            << "," << "currWeapon: "   << printHex(&currWeapon, 1)
            << "," << "equipIt: "       << printHex(equipIt, 2)
            << "," << "time: "         << printHex(time, 3)
            << "\n";

    // weapons
    for (unsigned int i = 0; i < weapons.size(); i++) {
        qDebug() << "weapon: " << i + 1 << "\t"
                << "type: "         << printHex(&weapons[i].type, 1)
                << ",level: "        << printHex(&weapons[i].level, 1)
                << ",energy: "       << printHex(&weapons[i].energy, 1)
                << ",maxAmmo: "      << printHex(&weapons[i].maxAmmo, 1)
                << ",currentAmmo: "  << printHex(&weapons[i].currentAmmo, 1);
    }

    // items
    QString itemStr = "";
    for (unsigned int i = 0; i < items.size(); i++) {
        itemStr += printHex(&items[i], 1) + ", ";
    }
    qDebug() << "\nitems: " << itemStr << "\n";

    // teleporters
    for (unsigned int i = 0; i < teleporters.size(); i++) {
        qDebug() << "teleporter: " << i + 1 << "\t"
                 << "menu: "         << printHex(&teleporters[i].menu, 1)
                 << ",location: "        << printHex(teleporters[i].location, 2);
    }

}

void ProfileLoader::printBuffer() {
    qDebug() << "profileloader.cpp: Printing full buffer: " << printHex(buffer, 0x60a);
}


























