# Description (EN)

"In the Haunted Prison" is a game initially
developed for Camputers Lynx 96k, a 8 bit machine subject of the game jam
"Retro Programmers United For Obscure Systems"

It is a graphic adventure game in which you venture into rooms, with doors, monsters and treasures.

# Description (FR)

« Dans la prison hantée » est un jeu pour Camputers Lynx 96k,
une machine 8 bits sujet d'une édition de la game jam
Retro Programmers United For Obscure        …Systems.

C'est un jeu d'aventure graphique dans lequel on s'aventure dans des
pièces, avec des portes, des monstres et des trésors.

# Description (EO)

"En la hantata prizono" estas ludo por Camputers Lynx 96k,
8-bita maŝino temo de la game jam "Retro Programmers United For Obscure Systems".

Ĝi estas grafika aventurludo en kiu oni aventuras en ĉambroj,
kun pordoj, monstroj kaj trezoroj.

## Build

You need to adapt `-DCMAKE_TOOLCHAIN_FILE` to your z88dk installation path.

### To build for Camputers Lynx

```bash
mkdir build
cd build
cmake -DZCCTARGET=lynx -DCMAKE_TOOLCHAIN_FILE=/snap/z88dk/current/share/z88dk/cmake/Toolchain-zcc.cmake ..
make
```

### To build for AgonLight

```bash
mkdir build
cd build
cmake -DZCCTARGET=agon -DCMAKE_TOOLCHAIN_FILE=/snap/z88dk/current/share/z88dk/cmake/Toolchain-zcc.cmake ..
make
```

## Development

### About generated files

The generated files are built into the cmake build folder, but they are expected by
the sources to be in the project folder. So you need to call the tools by hand
to generate them in the project folder.

As CMake expects the files to be present, you need to create them before with a manual call from
the project folder:

```bash
python3 tools/make_font.py
python3 tools/export_tiled.py
```

Not practical and error-prone. Waiting for a better solution.

### Unused data

You can spot also that there is a tool to generate a picture for the title screen.
I didn't use it, but it is there.

There are also sprites for the main characters in the different directions, but
I decided to use only one sprite for the main character, as it was more coherent
with the other characters. Anyway, the sprites are still there.