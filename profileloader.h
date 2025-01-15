#ifndef PROFILELOADER_H
#define PROFILELOADER_H

#include <list>
#include <memory>
#include <string>
#include <QVector>
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
    QByteArray buffer; // buffer with raw file input
    char map = 0;           // 008
    char song = 0;          // 00C
    char horizPos[4] = {0};   // 011-012
    char vertPos[4] = {0};    // 015-016
    char facingDir = 0;     // 018
    char maxHp[2] = {0};      // 01C-01D
    char whimsicalSt = 0;   // 01E
    char currHp[2] = {0};     // 020-021
    char currWeapon = 0;    // 024
    // char equipIt[2];    // 02C-02D (low byte first)
    char time[3] = {0};       // 034-036
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
    int getCurrentWeapon() const { return static_cast<int>(currWeapon); }

    // set
    void setWeapons(QVector<WeaponSlot> weapons);
    void setCurrentWeapon(int i) { currWeapon = static_cast<char>(i); }
};

#endif // PROFILELOADER_H
