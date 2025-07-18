# <img width="32" height="32" alt="snake_logo" src="https://github.com/user-attachments/assets/600d56db-1ba5-4c11-a5b6-ecfc7e4fabbe" /> Snake Game <img width="32" height="32" alt="watermelon" src="https://github.com/user-attachments/assets/2cec05df-5692-4a30-81bb-05e1a8151799" />


A classic Snake game implemented in C-Style C++ using Raylib.
<p align="center">
<img width="280" height="auto" alt="snake_menu" src="https://github.com/user-attachments/assets/9af5ad89-7164-40e2-b00b-7f31184781ac" />
<img width="280" height="auto" alt="snake_gameplay" src="https://github.com/user-attachments/assets/6297da4a-2ce8-4485-81af-4ad7491581c0" />
<img width="280" height="auto" alt="snake_gameover" src="https://github.com/user-attachments/assets/040294f3-b1d2-48ba-9cf8-1b5f2bdc75d3" />
</p>

## Requirements

- Windows 10 or later
- Git
- CMake (version 3.11 or later)
- A C++17 compatible compiler


## Getting Started

1. Clone the repository:
```
git clone https://github.com/jackboakes/Snake.git
```
2. Open the project folder:
```
cd Snake
```

3. Generate build files and build the project:
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
- **Left Click**: Select Menu Option
