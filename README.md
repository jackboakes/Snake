# Snake Game

A classic Snake game implemented in C-Style C++ using Raylib.

## Prerequisites

- Windows 10 or later
- Git
- CMake (version 3.11 or later)
- A C++17-compatible compiler


## Getting Started

1. Clone the repository:
```
git clone https://github.com/jackboakes/Snake.git
```
2. Open the project folder:
```
cd Snake
```

3. Genenerate build files and build the project:
```
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release
```

4. After building, run the game from the command line:
```
build\Release\Snake.exe
```

## Controls

- **W**: Move Up
- **A**: Move Left
- **S**: Move Down
- **D**: Move Right
- **ESC**: Quit Game
- **ENTER**: Select Menu Option