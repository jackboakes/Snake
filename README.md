# <img width="32" height="32" alt="snake_logo" src="https://github.com/user-attachments/assets/29967f89-b8de-4582-a317-d8e71b1d36b4" />Snake Game <img width="32" height="32" alt="watermelon" src="https://github.com/user-attachments/assets/68420bae-6117-4959-bae1-ef09c6e266cb" />




A classic Snake game implemented in C-Style C++ using Raylib.
<p align="center">
<img width="260" height="auto" alt="snake_menu" src="https://github.com/user-attachments/assets/a80eb2b8-fbb5-4853-9227-a1088e65bece" />
<img width="260" height="auto" alt="snake_gameplay" src="https://github.com/user-attachments/assets/4470ebaa-c832-4f65-85be-c7fdb00422e6" />
<img width="260" height="auto" alt="snake_gameover" src="https://github.com/user-attachments/assets/278272fa-359b-4fd1-8d64-4649543f311d" />
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
