#include <iostream>
#include <vector>

extern "C" {
    int InitGame(int gameId);
    void CleanupGame();
    void ResetGame(int gameId);
    void GetBoardDimensions(int* rows, int* cols);
    int PerformMove(int x, int y, int playerVal, int extraChar);
    int* GetBoardState();
    bool CheckWin(int playerVal);
    bool CheckDraw();
    bool IsGameOver();
    int GetAIMove(int playerVal, int* outX, int* outY, int* outChar);
}

int main() {
    // Test Standard XO (ID 1)
    std::cout << "--- Testing Standard XO (ID 1) ---\n";
    if (InitGame(1) == 0) {
        int rows, cols;
        GetBoardDimensions(&rows, &cols);
        std::cout << "Initialized. Size: " << rows << "x" << cols << "\n";
        PerformMove(1, 1, 1, 0);
        int* board = GetBoardState();
        std::cout << "Center cell: " << board[4] << " (Should be 1)\n";
    } else {
        std::cout << "Failed to init XO\n";
    }

    // Test Four in a Row (ID 3)
    std::cout << "\n--- Testing Four in a Row (ID 3) ---\n";
    if (InitGame(3) == 0) {
        int rows, cols;
        GetBoardDimensions(&rows, &cols);
        std::cout << "Initialized. Size: " << rows << "x" << cols << " (Should be 6x7)\n";
        
        // Move in col 0
        PerformMove(5, 0, 1, 0); // Bottom-left
        int* board = GetBoardState();
        // Index of (5,0) = 5*7 + 0 = 35
        std::cout << "Cell (5,0): " << board[35] << " (Should be 1)\n";
    } else {
        std::cout << "Failed to init Four in a Row\n";
    }

    CleanupGame();
    return 0;
}
