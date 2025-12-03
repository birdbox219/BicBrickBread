#include "header/XO_Classes.h"
#include "Games/PyramidXO/PyramidXO.h"
#include "Games/Four_in_a_row/four.h"
#include "Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.h"
#include "Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.h"
#include "Games/XO_inf/XO_inf.h"
#include "Games/XO_num/xo_num.h"
#include "Games/anti_XO/Anti_XO.h"
#include "Games/Ultimate_Tic_Tac_Toe/Ultimate.h"

#include <vector>
#include <iostream>

// Global instances
static Board<char>* board = nullptr;
static Player<char>* playerX = nullptr;
static Player<char>* playerO = nullptr;
static AI* ai = nullptr;
static int* boardBuffer = nullptr;
static int bufferSize = 0;

extern "C" {

    #ifdef _WIN32
    #define DLLEXPORT __declspec(dllexport)
    #else
    #define DLLEXPORT
    #endif

    // Game IDs:
    // 1: Standard XO
    // 2: Pyramid XO
    // 3: Four in a Row
    // 4: 5x5 Tic Tac Toe
    // 5: Word Tic Tac Toe
    // 6: Infinite XO
    // 7: Numerical XO
    // 8: Anti Tic Tac Toe
    // 9: Ultimate Tic Tac Toe

    DLLEXPORT void CleanupGame() {
        if (board) { delete board; board = nullptr; }
        if (playerX) { delete playerX; playerX = nullptr; }
        if (playerO) { delete playerO; playerO = nullptr; }
        if (ai) { delete ai; ai = nullptr; }
        if (boardBuffer) { delete[] boardBuffer; boardBuffer = nullptr; }
        bufferSize = 0;
    }

    DLLEXPORT int InitGame(int gameId) {
        CleanupGame();

        try {
            switch (gameId) {
                case 1: board = new X_O_Board(); break;
                case 2: board = new PyramidXO_Board(); break;
                case 3: board = new FOUR_Board(); break;
                case 4: board = new Large_XO_Board(); break;
                case 5: board = new Word_XO_Board(); break;
                case 6: board = new XO_inf_Board(); break;
                case 7: board = new XO_NUM_Board(); break;
                case 8: board = new Anti_XO_Board(); break;
                case 9: board = new Ultimate_Board(); break;
                default: return -1; // Invalid ID
            }
        } catch (...) {
            return -2; // Exception (e.g. missing dic.txt)
        }

        // Create dummy players
        playerX = new Player<char>("PlayerX", 'X', PlayerType::HUMAN);
        playerO = new Player<char>("PlayerO", 'O', PlayerType::HUMAN);
        
        playerX->set_board_ptr(board);
        playerO->set_board_ptr(board);

        ai = new AI();

        // Allocate buffer
        int rows = board->get_rows();
        int cols = board->get_columns();
        bufferSize = rows * cols;
        boardBuffer = new int[bufferSize];
        for(int i=0; i<bufferSize; ++i) boardBuffer[i] = 0;

        return 0; // Success
    }

    DLLEXPORT void ResetGame(int gameId) {
        InitGame(gameId);
    }

    DLLEXPORT void GetBoardDimensions(int* rows, int* cols) {
        if (board) {
            *rows = board->get_rows();
            *cols = board->get_columns();
        } else {
            *rows = 0;
            *cols = 0;
        }
    }

    // Returns: 0 = success, 1 = invalid move, 2 = game not initialized
    DLLEXPORT int PerformMove(int x, int y, int playerVal, int extraChar) {
        if (!board || !playerX || !playerO) return 2;

        Player<char>* p = (playerVal == 1) ? playerX : playerO;
        char symbol = p->get_symbol(); 

        // Special handling for Word XO (extraChar is the char to place)
        // and Numerical XO (extraChar is the digit)
        if (extraChar != 0) {
            symbol = (char)extraChar;
        }

        Move<char> move(x, y, symbol);
        if (board->update_board(&move)) {
            return 0;
        }
        return 1;
    }

    DLLEXPORT int* GetBoardState() {
        if (!board || !boardBuffer) return nullptr;

        auto matrix = board->get_board_matrix();
        int rows = board->get_rows();
        int cols = board->get_columns();
        
        int idx = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (idx >= bufferSize) break;
                
                char cell = matrix[r][c];
                // Map chars to ints for Unity
                // 0 = empty ('.')
                // 1 = X
                // 2 = O
                // For other games, we might return ASCII value directly if it's not X/O
                if (cell == '.') boardBuffer[idx] = 0;
                else if (cell == 'X') boardBuffer[idx] = 1;
                else if (cell == 'O') boardBuffer[idx] = 2;
                else boardBuffer[idx] = (int)cell; // Return ASCII for Word/Num XO
                
                idx++;
            }
        }
        return boardBuffer;
    }

    DLLEXPORT bool CheckWin(int playerVal) {
        if (!board) return false;
        Player<char>* p = (playerVal == 1) ? playerX : playerO;
        return board->is_win(p);
    }

    DLLEXPORT bool CheckDraw() {
        if (!board) return false;
        // Check draw for current player context (usually doesn't matter for is_draw logic)
        return board->is_draw(playerX);
    }

    DLLEXPORT bool IsGameOver() {
        if (!board) return true;
        return board->game_is_over(playerX);
    }

    DLLEXPORT int GetAIMove(int playerVal, int* outX, int* outY, int* outChar) {
        if (!ai || !board || !playerX || !playerO) return 0;

        Player<char>* p = (playerVal == 1) ? playerX : playerO;
        
        // Note: AI::bestMove returns pair<int,int> which only supports simple moves.
        // It does NOT support Word XO or Num XO which need a char/digit.
        // For those games, the generic AI might fail or return invalid moves.
        // But we will expose what we have.
        
        std::pair<int, int> move = ai->bestMove(p);
        
        if (move.first == -1 && move.second == -1) {
            return 0; 
        }

        *outX = move.first;
        *outY = move.second;
        *outChar = 0; // Generic AI doesn't pick chars
        return 1;
    }
}
