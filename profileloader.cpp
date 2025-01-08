#include "profileloader.h"

#include <vector>
#include <qDebug>
#include <fstream>

using std::string, std::vector,
    std::ifstream, std::ostream, std::fstream,
    std::shared_ptr, std::make_shared, std::list;

ProfileLoader::ProfileLoader(string profilePath) {
    WeaponSlot defaultWeapon;
    weapons = QVector<WeaponSlot>(8, defaultWeapon);

    // create items and put them into the total items array
    for (unsigned int i = 0; i <= 0x27; i += 0x01) {
        shared_ptr<Item> item = make_shared<Item>();
        items.totalItems[i] = item;
    }


    if (!parseProfile(profilePath)) {
        qDebug() << "error parsing profile";
    }
    // else printContents();
}

bool ProfileLoader::parseProfile(string profilePath) {

    // qDebug() << "opening file";

    // // open and check file
    ifstream file(profilePath, std::ios::binary);
    if (!file) {
        qDebug() << "didn't get a file";
        return false;
    }

    // check if file is a doukutsu

    // note that profile.dat should ALWAYS be 1540 bytes long
    file.read(buffer, 1540);  // Read data into the buffer

    // Check how many bytes were actually read
    std::streamsize bytesRead = file.gcount();
    if (bytesRead != 1540) {
        qDebug() << "wrong file size";
        return false;
    }

    // check the beginning bytes, should be 44 6F 30 34 31 32 32 30
    for (int i = 0; i < 8; i++) {
        switch(i) {
            case (0): if (buffer[i] != 0x44) return false; break;
            case (1): if (buffer[i] != 0x6F) return false; break;
            case (2): if (buffer[i] != 0x30) return false; break;
            case (3): if (buffer[i] != 0x34) return false; break;
            case (4): if (buffer[i] != 0x31) return false; break;
            case (5): if (buffer[i] != 0x32) return false; break;
            case (6): if (buffer[i] != 0x32) return false; break;
            case (7): if (buffer[i] != 0x30) return false; break;
        }
    }

    // parse easy bytes
    map = buffer[0x008];
    song = buffer[0x00C];
    horizPos[0] = buffer[0x010];
    horizPos[1] = buffer[0x011];
    horizPos[2] = buffer[0x012];
    horizPos[3] = buffer[0x013];
    vertPos[0] = buffer[0x014];
    vertPos[1] = buffer[0x015];
    vertPos[2] = buffer[0x016];
    vertPos[3] = buffer[0x017];
    facingDir = buffer[0x018];
    maxHp[0] = buffer[0x01C];
    maxHp[1] = buffer[0x1D];
    whimsicalSt = buffer[0x01E];
    currHp[0] = buffer[0x020];
    currHp[1] = buffer[0x021];
    currWeapon = buffer[0x024];
    time[0] = buffer[0x034];
    time[1] = buffer[0x035];
    time[2] = buffer[0x036];

    // parse weapons
    unsigned int weaponIt = 0x038;
    for (unsigned int i = 0; i < 8; i++) {
        WeaponSlot w;
        w.type = buffer[weaponIt];
        w.level = buffer[weaponIt+0x04];
        w.energy = buffer[weaponIt+0x08];
        w.maxAmmo = buffer[weaponIt+0x0C];
        if (w.maxAmmo != 0x0) { maxAmmo = w.maxAmmo; }
        w.currentAmmo = buffer[weaponIt+0x10];

        weapons[i] = w;
        weaponIt += 0x14;
    }

    // parse items
    int itemsCnt = 0;
    for (unsigned int i = 0x0D8; i < 0x154; i+=0x04) {
        if (buffer[i] == 0) continue;

        int j = buffer[i];

        items.totalItems[j]->isActive = true;
        items.totalItems[j]->item = buffer[i];
        items.foundItems.push_back(items.totalItems[j]);
        itemsCnt++;
    }

    // parse equiped
    int eqL = buffer[0x02C];
    int eqR = buffer[0x02D];

    if ((eqL & 0x1) > 0) items.totalItems[0x12]->isEquiped = true; // booster 0.8
    if ((eqL & 0x2) > 0) items.totalItems[0x02]->isEquiped = true; // map
    if ((eqL & 0x4) > 0) items.totalItems[0x13]->isEquiped = true; // arms
    if ((eqL & 0x8) > 0) items.totalItems[0x14]->isEquiped = true; // turbo
    if ((eqL & 0x10) > 0) items.totalItems[0x15]->isEquiped = true; // curly
    if ((eqL & 0x20) > 0) items.totalItems[0x17]->isEquiped = true; // booster 2.0
    if ((eqL & 0x40) > 0) items.totalItems[0x18]->isEquiped = true; // mask
    if ((eqR & 0x1) > 0) items.totalItems[0x16]->isEquiped = true; // counter

    return true;
}

void ProfileLoader::printContents() {
    qDebug() << "TOTALITEMS:\n\t";
    for (unsigned int i = 0; i <= 0x27; i++) {
        if (!items.totalItems[i]->isActive) {
            // qDebug() << "\t" << i << " is not active\n";
            continue;
        }
        if (items.totalItems[i]->isEquiped) qDebug() << &std::hex << i << "(eq), ";
        else qDebug() << &std::hex << i << ", ";

    }
    qDebug() ;

    qDebug() << "INVENTORYLIST:\n\t";
    list<shared_ptr<Item>>::const_iterator it;
    for (it = items.foundItems.begin(); it != items.foundItems.end(); it++) {
        qDebug() << &std::hex << (*it)->item+0 << ", ";
    }
    qDebug() ;

    qDebug() << "WEAPONS:\n";
    for (unsigned int i = 0; i < 8; i++) {
        qDebug() << "\ttype: " << &std::hex << weapons[i].type+0 << ",\t"
                 << "level: " << &std::hex << weapons[i].level+0 << ",\t"
                 << "energy: " << &std::hex << weapons[i].energy+0 << ",\t"
                 << "maxAmmo: " << &std::hex << weapons[i].maxAmmo+0 << ",\t"
                 << "currentAmmo: " << &std::hex << weapons[i].currentAmmo+0 << ",\n";
    }

    // test print
    qDebug() << "ETC PARSES: " ;
    qDebug() << "map: " << &std::hex << map+0 << ",\t"
             << "song: " << &std::hex << song+0 << ",\t\n";

    qDebug() << "horizPos: " << &std::hex << (int)(unsigned char)horizPos[0]+0 << " " << (int)(unsigned char)horizPos[1]+0 << " "
             << (int)(unsigned char)horizPos[2]+0 << " " << (int)(unsigned char)horizPos[3]+0 << ",\t"

             << "vertPos: " << &std::hex << (int)(unsigned char)vertPos[0]+0 << " " << (int)(unsigned char)vertPos[1]+0
             << " " << (int)(unsigned char)vertPos[2]+0 << " " << (int)(unsigned char)vertPos[3]+0 << ",\t"

             << "facingDir: " << &std::hex << facingDir+0 << "\n";

    qDebug() << "maxHp: " << &std::hex << maxHp[0]+0 << " " << maxHp[1]+0 << ", \t";
    qDebug() << "whimsicalSt: " << &std::hex << whimsicalSt+0 << ", \t";
    qDebug() << "currHp: " << &std::hex << currHp[0]+0 << " " << currHp[1]+0 << ", \t\t";
    qDebug() << "currWeapon: " << &std::hex << currWeapon+0 << ", \t";
    // qDebug() << "equipIt: " << std::hex << equipIt[0]+0 << " " << equipIt[1]+0 << ", \t";
    qDebug() << "time: " << &std::hex << time[0]+0 << " " << time[1]+0 << " " << time[2]+0 << ", \t\n";
}
