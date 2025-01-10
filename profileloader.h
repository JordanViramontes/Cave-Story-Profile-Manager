#ifndef PROFILELOADER_H
#define PROFILELOADER_H

#include <list>
#include <memory>
#include <string>
#include <QVector>

using std::string, std::shared_ptr, std::list;

// each item has a bit for isactive. If inventoryPos = -1, then it wasn't included in the base Profile and needs to be appended
struct Item {
    bool isActive = false;
    bool isEquiped = false;
    char item = 0;
};

struct InventoryItems {
    // the items in both lists are the same, hence pointers
    list<shared_ptr<Item>> foundItems; // all items that were found in the inventory
    shared_ptr<Item> totalItems[0x028]; // total items in the game
};

struct WeaponSlot {
    char type = 0;
    char level = 0;
    char energy = 0;
    char maxAmmo = 0;
    char currentAmmo = 0;
};

class ProfileLoader
{
private:
    char buffer[1540];  // buffer with raw file input
    char map;           // 008
    char song;          // 00C
    char horizPos[4];   // 011-012
    char vertPos[4];    // 015-016
    char facingDir;     // 018
    char maxHp[2];      // 01C-01D
    char whimsicalSt;   // 01E
    char currHp[2];     // 020-021
    char currWeapon;    // 024
    // char equipIt[2];    // 02C-02D (low byte first)
    char time[3];       // 034-036
    QVector<WeaponSlot> weapons;    // 038-0D4
    InventoryItems items; // 0D8-153
    char maxAmmo = 0;

public:
    ProfileLoader();
    bool parseProfile(string profilePath);
    void printContents();
    bool updateBuffer();
    QVector<char> getBuffer();

    // get
    QVector<WeaponSlot> &getWeapons() { return weapons; }
    int getMaxAmmo() const { return static_cast<int>(maxAmmo); }

    // set
    void setWeapons(QVector<WeaponSlot> weapons);
};

#endif // PROFILELOADER_H
