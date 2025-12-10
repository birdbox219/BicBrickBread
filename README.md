# BicBrickBread - Ultimate Board Game Collection

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-green.svg)
![License](https://img.shields.io/badge/License-MIT-purple.svg)

**BicBrickBread** is a comprehensive collection of classic and modern board games implemented in C++. Developed by the **CodeArt Team** at **FCAI-CU**, this project features a unified console interface to play 14 different strategy games against a friend or the computer.

---

## 🎮 Included Games

This collection includes **14 unique games**, each with its own set of rules and challenges:

1.  **Classic Tic-Tac-Toe**: The standard 3x3 game we all know.
2.  **Infinite XO (Block XO)**: A 3x3 variant where pieces disappear after a certain number of moves.
3.  **Four in a Row (Connect 4)**: Drop pieces into a 6x7 grid to connect four in a line.
4.  **Anti Tic-Tac-Toe**: The goal is to **avoid** getting three in a row.
5.  **5x5 XO (Large Tic-Tac-Toe)**: A larger grid requiring a longer connection to win.
6.  **Numerical Tic-Tac-Toe**: Use numbers instead of X/O. The sum of 15 wins.
7.  **Ultimate Tic-Tac-Toe**: A nested game of 9 small Tic-Tac-Toe boards forming one large board.
8.  **Pyramid XO**: A triangular board structure.
9.  **Word XO**: Form valid 3-letter words from a dictionary to win.
10. **Obstacles Tic-Tac-Toe**: The 5x5 grid contains random obstacles that cannot be played on.
11. **Memory Tic-Tac-Toe**: The board state is hidden; you must remember piece locations.
12. **4x4 Tic-Tac-Toe**: A 4x4 grid variant where corners or lines win.
13. **SUS**: A game focused on forming the specific pattern "S-U-S".
14. **Diamond XO**: Played on a diamond-shaped grid layout.

---

## 🛠️ Installation & Compilation

The project is built using standard C++. You need a C++ compiler (like `g++` or `clang++`) installed on your system.

### Prerequisites

*   **Compiler**: GCC, Clang, or MSVC supporting C++11 or later.
*   **Dictionary File**: Ensure `dic.txt` is in the same directory as the executable (for Word XO).

### Building with G++ (Terminal)

Open your terminal in the project root directory and run the following command to compile all games and the main menu:

```bash
g++ -o TheGame.exe TheGame.cpp Refrence/XO_Classes.cpp \
Games/4x4_Tic_Tac_Toe/4by4_XO.cpp \
Games/Four_in_a_row/four.cpp \
Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.cpp \
Games/Memory_Tic_Tac_Toe/Memory_Tic_Tac_Toe.cpp \
Games/Obstacles_Tic_Tac_Toe/Obstacles_Tic_Tac_Toe.cpp \
Games/PyramidXO/PyramidXO.cpp \
Games/SUS/SUS.cpp \
Games/Ultimate_Tic_Tac_Toe/Ultimate.cpp \
Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.cpp \
Games/XO_inf/XO_inf.cpp \
Games/XO_num/xo_num.cpp \
Games/anti_XO/Anti_XO.cpp \
Games/diamond_XO/dia_XO.cpp \
Neural_Network/Source/Layer.cpp \
Neural_Network/Source/Matrix.cpp \
Neural_Network/Source/NeuralNetwork.cpp \
-I.
```

> **Note:** On Windows, you might need to remove the backslashes `\` and put everything on one line if using Command Prompt (cmd.exe). PowerShell supports multi-line commands.

---

## 🚀 How to Run

1.  Compile the project using the instructions above.
2.  Run the executable:
    *   **Windows:** `.\TheGame.exe`
    *   **Linux/Mac:** `./TheGame.exe`
3.  You will be greeted by the **BicBrickBread** main menu.
4.  Enter the number corresponding to the game you want to play (1-14).
5.  Follow the on-screen instructions to select player types (Human vs Human, Human vs Computer).

---

## 📂 Project Structure

The codebase is organized by game module:

```
├── TheGame.cpp               # Main Entry Point and Menu System
├── Games/                    # Game Implementations
│   ├── Word_Tic_Tac_Toe/     # Source for Word XO
│   ├── Ultimate_Tic_Tac_Toe/ # Source for Ultimate XO
│   └── ... (other games)
├── header/                   # Shared Base Classes (Board, Player, UI)
├── Neural_Network/           # AI components
├── dic.txt                   # Dictionary resource for Word XO
└── README.md                 # Documentation
```

---

## 👥 Authors

**Team CodeArt** - FCAI-CU

*   Development & Implementation of various game modules.
*   Design of the unified `BoardGame_Classes` framework.

---

© 2025 CodeArt. All Rights Reserved.
