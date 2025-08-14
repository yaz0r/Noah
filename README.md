# Noah: a Xenogears decompilation.
[![Build](https://github.com/yaz0r/Noah/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/yaz0r/Noah/actions/workflows/cmake-multi-platform.yml)

This is a decompilation of Xenogears for PS1, based on the US release. All the code executing has been decompiled and reimplemented. Only the PSX sound chip is emulated.  
The user must provide their own copy of Xenogears. No game data are provided in this project.
It is **_not_** a matching decompilation, opting instead to reimplement the logic in modern(-ish) C++ with cross-platform in mind. For more informations, see [this page on the wiki](https://github.com/yaz0r/Noah/wiki/Why-not-make-a-matching-decompilation%3F)  
This project currently builds and run on Windows/Linux (Mac will be added soon too).

# Status:
Parts of the game are playable, but missing features will prevent progression. A player can use the in-engine debugger to jump over bugs to progress.

## General
### Rendering
* Bulk of the rendering has been implemented and emulate the original PSX rendering for now
* Some primitive and effects are missing
### Audio
* Sound is implemented and music can be played
* Sound effects are missing/corrupted
### Field Module
![image](https://github.com/user-attachments/assets/2ac5617d-24ae-40f0-9639-99b79fd790f0)
* Functional for the most part
* Known issues in some collision
* Occasional missing Opcode in the VM can prevent progression
* Implemts a field VM decompiler and debugger
![image](https://github.com/user-attachments/assets/82ca935d-a24e-4e0f-9c2c-fb8f20ace919)
### Battles
![image](https://github.com/user-attachments/assets/cdcaf540-cd8c-4c9c-8d2a-56e65265bfa6)
* Basic battles can be played but anything advanced will have issues
* Battle events system (cutscenes with the battle engine) have the basics implemented and some events start executing
### Worldmap
![image](https://github.com/user-attachments/assets/8ae4f752-1361-4430-a4a5-f77925246b8b)
* World map have basic features implemented.
* More progress will be done when normal player progression is unblocked

## Prebuilts
Prebuilt versions aren't available at this moment.

## Building
Only Windows and Linux builds are supported right now.

### Windows
You need vs2022 installed with CMake
* Clone with submodules
* Launch the vs2022.bat to create solution.
* Open the generated build/Noah.sln
* Put the iso files as data/iso/1.bin and data/iso/2.bin (US version)
* Run the Noah project




