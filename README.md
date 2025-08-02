# Noah: reverse engineering of a Xenogears.

This is a decompilation of Xenogears for PS1, based on the US release. It is non-matching and doesn't compile for PS1, instead targetting more modern system. The code is also more strict than the original C code probably was.

# Build:
Only Windows builds are supported right now. You need vs2022 installed with CMake
* Clone with submodules
* Launch the vs2022.bat to create solution.
* Open build/Noah.sln
* Put the iso files as data/iso/1.bin and data/iso/2.bin (US version)
* Run the Noah project

# Status:
Still early stage and very partially playable.
* Field is mostly functional with buggy collisions and various missing features.
* World map has basic features implemented.
* Battles are not playable. Basic systems are in place but a lot is missing or buggy.

## Screenshots
![image](https://github.com/user-attachments/assets/82ca935d-a24e-4e0f-9c2c-fb8f20ace919)
![image](https://github.com/user-attachments/assets/2ac5617d-24ae-40f0-9639-99b79fd790f0)
![image](https://github.com/user-attachments/assets/cdcaf540-cd8c-4c9c-8d2a-56e65265bfa6)
![image](https://github.com/user-attachments/assets/8ae4f752-1361-4430-a4a5-f77925246b8b)


