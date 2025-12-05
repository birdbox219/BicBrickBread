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
#include <fstream>

// Global instances
static Board<char> *board = nullptr;
static Player<char> *playerX = nullptr;
static Player<char> *playerO = nullptr;
static UI<char> *ui = nullptr;
static int *boardBuffer = nullptr;
static int bufferSize = 0;
static int currentGameId = 0;

extern "C"
{

#ifdef _WIN32
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT
#endif

    DLLEXPORT void CleanupGame()
    {
        if (board)
        {
            delete board;
            board = nullptr;
        }
        if (playerX)
        {
            delete playerX;
            playerX = nullptr;
        }
        if (playerO)
        {
            delete playerO;
            playerO = nullptr;
        }
        if (boardBuffer)
        {
            delete[] boardBuffer;
            boardBuffer = nullptr;
        }
        if (ui)
        {
            delete ui;
            ui = nullptr;
        }
        bufferSize = 0;
        currentGameId = 0;
    }

    DLLEXPORT int InitGame(int gameId)
    {
        CleanupGame();
        currentGameId = gameId;

        try
        {
            switch (gameId)
            {
            case 1:
                board = new X_O_Board();
                ui = new XO_UI();
                break;
            case 2:
                board = new PyramidXO_Board();
                ui = new PyramidXO_UI();
                break;
            case 3:
                board = new FOUR_Board();
                ui = new FOUR_UI();
                break;
            case 4:
                board = new Large_XO_Board();
                ui = new Large_XO_UI();
                break;
            case 5:
            {
                // Pre-check for dic.txt to avoid exception crash
                std::ifstream f("dic.txt");
                if (!f.good())
                    return -2; // File missing
                f.close();
                board = new Word_XO_Board();
                ui = new Word_XO_UI();
                break;
            }
            case 6:
                board = new XO_inf_Board();
                ui = new XO_inf_UI();
                break;
            case 7:
                board = new XO_NUM_Board();
                ui = new XO_NUM_UI();
                break;
            case 8:
                board = new Anti_XO_Board();
                ui = new Anti_XO_UI();
                break;
            case 9:
                board = new Ultimate_Board();
                ui = new Ultimate_UI();
                break;
            default:
                return -1; // Invalid ID
            }
        }
        catch (const std::exception &e)
        {
            return -2;
        }
        catch (...)
        {
            return -3;
        }

        // Create dummy players
        playerX = new Player<char>("PlayerX", 'X', PlayerType::HUMAN);
        playerO = new Player<char>("PlayerO", 'O', PlayerType::HUMAN);

        playerX->set_board_ptr(board);
        playerO->set_board_ptr(board);

        // Allocate buffer
        int rows = board->get_rows();
        int cols = board->get_columns();

        if (gameId == 4)
        {
            rows = 5;
            cols = 5;
        }

        bufferSize = rows * cols;
        boardBuffer = new int[bufferSize];
        for (int i = 0; i < bufferSize; ++i)
            boardBuffer[i] = 0;

        return 0;
    }

    DLLEXPORT void ResetGame(int gameId)
    {
        InitGame(gameId);
    }

    DLLEXPORT void GetBoardDimensions(int *rows, int *cols)
    {
        if (board)
        {
            *rows = board->get_rows();
            *cols = board->get_columns();

            // Fix for Large XO
            if (currentGameId == 4)
            {
                *rows = 5;
                *cols = 5;
            }
        }
        else
        {
            *rows = 0;
            *cols = 0;
        }
    }

    DLLEXPORT int PerformMove(int x, int y, int extraChar)
    {
        if (ui == nullptr || board == nullptr || playerX == nullptr || playerO == nullptr)
            return 2;

        Move<char> move = Move<char>(x, y, char(extraChar));

        if (board->update_board(&move))
        {
            return 0;
        }
        return 1;
    }

    DLLEXPORT int GetAiMove(int playerVal)
    {
        if (ui == nullptr || board == nullptr || playerX == nullptr || playerO == nullptr)
            return 2;

        Player<char> *p;
        if (playerVal == 1)
        {
            p = new Player<char>("PlayerX", 'X', PlayerType::AI);
        }
        else
        {
            p = new Player<char>("PlayerX", 'O', PlayerType::AI);
        }

        auto *move = ui->get_move(p);

        if (board->update_board(move))
        {
            delete move;
            delete p;
            return 0;
        }
        delete move;
        delete p;
        return 1;
    }

    DLLEXPORT int *GetBoardState()
    {
        if (!board || !boardBuffer)
            return nullptr;

        // Special handling for Large XO (ID 4) which uses bitboards
        if (currentGameId == 4)
        {
            auto *largeBoard = dynamic_cast<Large_XO_Board *>(board);
            if (largeBoard)
            {
                for (int r = 0; r < 5; ++r)
                {
                    for (int c = 0; c < 5; ++c)
                    {
                        char cell = largeBoard->getCell(r, c);
                        if (cell == 'X')
                            boardBuffer[r * 5 + c] = 1;
                        else if (cell == 'O')
                            boardBuffer[r * 5 + c] = 2;
                        else
                            boardBuffer[r * 5 + c] = 0;
                    }
                }
                return boardBuffer;
            }
        }

        // Default handling for other games using base board vector
        auto matrix = board->get_board_matrix();
        int rows = board->get_rows();
        int cols = board->get_columns();

        for (int r = 0; r < rows; ++r)
        {
            for (int c = 0; c < cols; ++c)
            {
                char cell = matrix[r][c];
                if (cell == '.' || cell == 0)
                    boardBuffer[r * cols + c] = 0;
                else
                    boardBuffer[r * cols + c] = cell;
            }
        }
        return boardBuffer;
    }

    DLLEXPORT bool CheckWin(int playerVal)
    {
        if (!board)
            return false;
        Player<char> *p = (playerVal == 1) ? playerX : playerO;
        return board->is_win(p);
    }

    DLLEXPORT bool CheckDraw()
    {
        if (!board)
            return false;
        return board->is_draw(playerX);
    }

    DLLEXPORT bool IsGameOver()
    {
        if (!board)
            return true;
        return board->game_is_over(playerX);
    }
}
