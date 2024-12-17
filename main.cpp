/*
CS Profile Manager Profile Script - Jordan Viramontes

The purpose of this script is to:
    1) Parse through an already existing profile.dat file and copy its contents, and then
    2) Create an entirely new profile.dat with user inputted variables

Written by Jordan Viramontes with proofreading and verification by Periwinkle
*/

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <memory>
#include <list>

using std::string, std::vector,
      std::ifstream, std::ostream, std::fstream,
      std::cout, std::cin, std::endl,
      std::shared_ptr, std::make_shared, std::list;

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

class Profile {
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
        WeaponSlot weapons[8];    // 038-0D4
        InventoryItems items; // 0D8-153

        bool parseProfile(string profilePath);
    public:
        Profile(string profilePath);
};

int main(int argc, char* argv[]) {
    // string filePath(argv[1]);
    string path = "./Profile-Hell.dat";
    Profile p(path);


    return 0;
}

Profile::Profile(string profilePath) {
    // create items and put them into the total items array
    for (unsigned int i = 0; i <= 0x27; i += 0x01) {
        shared_ptr<Item> item = make_shared<Item>();
        items.totalItems[i] = item;
    }  


    if (!parseProfile(profilePath)) {
        cout << "error parsing profile" << endl;
    }
}

bool Profile::parseProfile(string profilePath) {
    // open and check file
    ifstream file(profilePath, std::ios::binary);
    if (!file) {
        cout << "didn't get a file" << endl;
        return false;
    }

    // note that profile.dat should ALWAYS be 1540 bytes long
    file.read(buffer, 1540);  // Read data into the buffer
    
    // Check how many bytes were actually read
    std::streamsize bytesRead = file.gcount();
    if (bytesRead != 1540) {
        cout << "wrong file size" << endl;
        return false;
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

    cout << "TODO: PARSE THROUGH WEAPONS AND ITEMS USING LOOPS\n\n";

    // parse weapons
    unsigned int weaponIt = 0x038;
    for (unsigned int i = 0; i < 8; i++) {
        WeaponSlot w;
        w.type = buffer[weaponIt];
        w.level = buffer[weaponIt+0x04];
        w.energy = buffer[weaponIt+0x08];
        w.maxAmmo = buffer[weaponIt+0x0C];
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


    cout << "TOTALITEMS:\n\t";
    for (unsigned int i = 0; i <= 0x27; i++) {
        if (!items.totalItems[i]->isActive) {
            // cout << "\t" << i << " is not active\n";
            continue;
        }
        if (items.totalItems[i]->isEquiped) cout << std::hex << i << "(eq), ";
        else cout << std::hex << i << ", ";

    }
    cout << endl;

    cout << "INVENTORYLIST:\n\t";
    list<shared_ptr<Item>>::const_iterator it;
    for (it = items.foundItems.begin(); it != items.foundItems.end(); it++) {
        cout << std::hex << (*it)->item+0 << ", ";
    }
    cout << endl;

    cout << "WEAPONS:\n";
    for (unsigned int i = 0; i < 8; i++) {
        cout << "\ttype: " << std::hex << weapons[i].type+0 << ",\t"
         << "level: " << std::hex << weapons[i].level+0 << ",\t"
         << "energy: " << std::hex << weapons[i].energy+0 << ",\t"
         << "maxAmmo: " << std::hex << weapons[i].maxAmmo+0 << ",\t"
         << "currentAmmo: " << std::hex << weapons[i].currentAmmo+0 << ",\n";
    }
    
    // test print
    cout << "ETC PARSES: " << endl;
    cout << "map: " << std::hex << map+0 << ",\t"
         << "song: " << std::hex << song+0 << ",\t\n";

    cout << "horizPos: " << std::hex << (int)(unsigned char)horizPos[0]+0 << " " << (int)(unsigned char)horizPos[1]+0 << " " 
         << (int)(unsigned char)horizPos[2]+0 << " " << (int)(unsigned char)horizPos[3]+0 << ",\t"

         << "vertPos: " << std::hex << (int)(unsigned char)vertPos[0]+0 << " " << (int)(unsigned char)vertPos[1]+0 
         << " " << (int)(unsigned char)vertPos[2]+0 << " " << (int)(unsigned char)vertPos[3]+0 << ",\t"

         << "facingDir: " << std::hex << facingDir+0 << "\n";

    cout << "maxHp: " << std::hex << maxHp[0]+0 << " " << maxHp[1]+0 << ", \t";
    cout << "whimsicalSt: " << std::hex << whimsicalSt+0 << ", \t";
    cout << "currHp: " << std::hex << currHp[0]+0 << " " << currHp[1]+0 << ", \t\t";
    cout << "currWeapon: " << std::hex << currWeapon+0 << ", \t";
    // cout << "equipIt: " << std::hex << equipIt[0]+0 << " " << equipIt[1]+0 << ", \t";
    cout << "time: " << std::hex << time[0]+0 << " " << time[1]+0 << " " << time[2]+0 << ", \t\n";

    return true;
}