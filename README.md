
# Cave Story Profile Manager

A simple Windows application to load and customize save  files for the freeware version of Cave Story.

Save Files included (taken from https://www.speedrun.com/cave_story/resources/zlwse),

All you need is to have your own local installation of Doukutsu (found on https://www.cavestory.org/)

![alt text](https://github.com/JordanViramontes/Cave-Story-Profile-Manager/blob/main/CSManagerScreenShot.png?raw=true)


## Features

* Weapon Manipulation
  * Inventory order
  * Weapon Lvl (including MAX)
  * Weapon Xp
  * Weapon Ammo (for Missiles ONLY)

Planned Features :

* Item Manipulation
  * Inventory order
  * Equipt trigger

## Instructions

Download Cave Story Profile Manager from the Releases Tab. After extracting, the folder containing CS-Profile-Manager.exe should also have a *"saves"* folder. **PLEASE KEEP THE "saves" FOLDER IN THE SAME DIRECTORY AS "CS-Profile-Manager.exe"**.

After running, the application will prompt you to pick the directory of your local "Doukutsu.exe". Do this by clicking the *"Update Doukutsu Directory"* and follow the popup window.

Once you click on a valid Profile.dat file (all of the included files are titled as "location".dat), the table of weapons will update to that of the selected Profile.dat. You can edit any values you want as well as drag and drop table cells in order to get them in the order you want.

In order for a weapon to be included in the save file, it must be BOTH:
* Enabled (the check box must be on)
* Within the first 5 rows of the table

Once you click the *"quick apply"* Button, the local Profile.dat file at your Doukutsu.exe directory will update and the game will Launch. 

If the game is already launched, pressing the *"quick apply"* button will still update the Profile.dat file. After pressing the button, in the game press ESC followed by F2 which will reset the game to the title screen. Simply load the save from there.
