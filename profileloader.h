#ifndef PROFILELOADER_H
#define PROFILELOADER_H

#include <QVector>

// each item has a bit for isactive. If inventoryPos = -1, then it wasn't included in the base Profile and needs to be appended

// struct InventoryItems {
//     // the items in both lists are the same, hence pointers
//     QVector<Item> foundItems; // all items that were found in the inventory
//     Item totalItems[0x028]; // total items in the game
// };

// each weapon!
struct WeaponDataSlot {
    char type = 0x00;
    char level = 0x00;
    char energy = 0x00;
    char maxAmmo = 0x00; // 0-255
    char currentAmmo = 0x00; // 0-255
};

struct TeleportDataSlot {
    char menu = 0x00;
    char location[2] = {0};
};

class ProfileLoader
{
private:
    QByteArray buffer; // buffer with raw file input
    char map = 0;               // 008
    char song = 0;              // 00C
    char horizPos[4] = {0};     // 010-013
    char vertPos[4] = {0};      // 014-017
    char facingDir = 0;         // 018
    char maxHp[2] = {0};        // 01C-01D
    char whimsicalSt = 0;       // 01E (0-3 stars)
    char currHp[2] = {0};       // 020-021
    char currWeapon = 0;        // 024 (0-04 AKA slots 1-5)
    char equipIt[2];            // 02C-02D (low byte first)
    char time[3] = {0};         // 034-036
    QVector<WeaponDataSlot> weapons;    // 038-0D4
    QVector<char> items;      // 0D8-154
    QVector<TeleportDataSlot> teleporters; // 158-195

public:
    ProfileLoader();
    bool parseProfile(QString profilePath);
    bool writeToFile(QString profilePath, QVector<WeaponDataSlot> weaponDataSlots, char weaponCurrentlySelected);
    // bool updateBuffer();
    // QVector<char> getBuffer();

    // debug
    void printSaveData();
    void printBuffer();
    QString printHex(const char* ch, int len) {
        QString str = "";
        for (int i = 0; i < len; i++) {
            unsigned char byte = static_cast<unsigned char>(ch[i]);
            str += QString("%1").arg(byte, 2, 16, QLatin1Char('0')) + " ";
        }
        return str;
    }

    // get
    QVector<WeaponDataSlot> &getWeapons() { return weapons; }
    int getCurrentWeapon() const { return static_cast<int>(currWeapon); }

    // set
    void setWeapons(QVector<WeaponDataSlot> weapons);
    void setCurrentWeapon(int i) { currWeapon = static_cast<char>(i); }
};

#endif // PROFILELOADER_H
