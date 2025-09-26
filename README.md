# PowderSimulator
A cellular automata powder simulator in C and windows.
<br />Used Windows api for creating a window.
<br />Can simulate sand, water, stone and metal.

## Materials
### sand
Can fall either down, left down, or right down.

### water
Can fall either down, left down, or right down.
<br />And can move left and right.

### stone
Can only fall down.

### metal
Can't move.

## Video Example
![PowderSimGif](https://github.com/user-attachments/assets/9cfb2ca9-2b0b-47a4-a1ee-126503004abe)


## How To Play
- Left click- put material
- Right click- Switch current material
- 1-9- Switch brush size

## Requirements
- Windows 10 or above
- C23 compiler
- CMake 4.0 or higher

## Build & Run
```bash
git clone https://github.com/eitanVIP/PowderSimulator.git
cd PowderSimulator
mkdir build
cd build
cmake ..
cmake --build .
cd Debug
.\PowderSimulator.exe
```
