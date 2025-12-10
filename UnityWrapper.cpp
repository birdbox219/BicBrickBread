#include "header/XO_Classes.h"
#include "Games/4x4_Tic_Tac_Toe/4by4_XO.h"
#include "Games/anti_XO/Anti_XO.h"
#include "Games/diamond_XO/dia_XO.h"
#include "Games/Four_in_a_row/four.h"
#include "Games/Large_Tic_Tac_Toe/Large_Tic_Tac_Toe.h"
#include "Games/Memory_Tic_Tac_Toe/Memory_Tic_Tac_Toe.h"
#include "Games/Obstacles_Tic_Tac_Toe/Obstacles_Tic_Tac_Toe.h"
#include "Games/PyramidXO/PyramidXO.h"
#include "Games/SUS/SUS.h"
#include "Games/Ultimate_Tic_Tac_Toe/Ultimate.h"
#include "Games/Word_Tic_Tac_Toe/Word_Tic_Tac_Toe.h"
#include "Games/XO_inf/XO_inf.h"
#include "Games/XO_num/xo_num.h"
#include "Neural_Network/Include/NeuralNetwork.h"

#include <vector>
#include <iostream>
#include <fstream>

// Global instances
static Board<char> *board = nullptr;
static Player<char> *playerX = nullptr;
static Player<char> *playerO = nullptr;
static AI *ai = nullptr;
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
        try
        {
            if (board != nullptr)
            {
                delete board;
                board = nullptr;
            }
            if (playerX != nullptr)
            {
                delete playerX;
                playerX = nullptr;
            }
            if (playerO != nullptr)
            {
                delete playerO;
                playerO = nullptr;
            }
            if (boardBuffer != nullptr)
            {
                delete[] boardBuffer;
                boardBuffer = nullptr;
            }
            if (ai != nullptr)
            {
                delete ai;
                ai = nullptr;
            }
            bufferSize = 0;
            currentGameId = 0;
        }
        catch (const std::exception &e)
        {
            std::cerr << "CleanupGame exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "CleanupGame unknown exception" << std::endl;
        }
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
                ai = new X_O_AI();
                break;
            case 2:
                board = new _4by4XO_Board();
                ai = new _4by4XO_AI();
                break;
            case 3:
                board = new Anti_XO_Board();
                ai = new Anti_AI();
                break;
            case 4:
                board = new dia_XO_Board();
                ai = new dia_XO_AI();
                break;
            case 5:
                board = new FOUR_Board();
                ai = new Four_AI();
                break;
            case 6:
                board = new Large_XO_Board();
                ai = new Large_XO_AI();
                break;
            case 7:
                board = new Memory_Board();
                ai = new Memory_AI();
                break;
            case 8:
                board = new Obstacles_Board();
                ai = new Obstacles_AI();
                break;
            case 9:
                board = new PyramidXO_Board();
                ai = new PyramidXO_AI();
                break;
            case 10:
                board = new SUS_Board();
                ai = new SUS_AI();
                break;
            case 11:
                board = new Ultimate_Board();
                ai = new Ultimate_AI();
                break;
            case 12:
            {
                std::ifstream f("dic.txt");
                if (!f.good())
                {
                    std::cerr << "dic.txt file not found" << std::endl;
                    return -2;
                }
                f.close();
                board = new Word_XO_Board();
                ai = new Word_AI();
                break;
            }
            case 13:
                board = new XO_inf_Board();
                ai = new XO_inf_AI();
                break;
            case 14:
                board = new XO_NUM_Board();
                ai = new XO_NUM_AI();
                break;
            default:
                std::cerr << "Invalid game ID: " << gameId << std::endl;
                return -1;
            }

            if (!board)
            {
                std::cerr << "Failed to create board" << std::endl;
                return -2;
            }

            playerX = new Player<char>("PlayerX", 'X', PlayerType::HUMAN);
            playerO = new Player<char>("PlayerO", 'O', PlayerType::HUMAN);

            if (!playerX || !playerO)
            {
                std::cerr << "Failed to create players" << std::endl;
                return -2;
            }

            playerX->set_board_ptr(board);
            playerO->set_board_ptr(board);

            int rows = board->get_rows();
            int cols = board->get_columns();

            if (gameId == 6)
            {
                rows = 5;
                cols = 5;
            }
            else if (gameId == 8)
            {
                rows = 6;
                cols = 6;
            }

            bufferSize = rows * cols;
            boardBuffer = new int[bufferSize];
            
            if (!boardBuffer)
            {
                std::cerr << "Failed to allocate board buffer" << std::endl;
                return -2;
            }

            for (int i = 0; i < bufferSize; ++i)
                boardBuffer[i] = 0;

            return 0;
        }
        catch (const std::exception &e)
        {
            std::cerr << "InitGame exception: " << e.what() << std::endl;
            CleanupGame();
            return -2;
        }
        catch (...)
        {
            std::cerr << "InitGame unknown exception" << std::endl;
            CleanupGame();
            return -3;
        }
    }

    DLLEXPORT void ResetGame(int gameId)
    {
        try
        {
            int result = InitGame(gameId);
            if (result != 0)
            {
                std::cerr << "ResetGame failed with code: " << result << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "ResetGame exception: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "ResetGame unknown exception" << std::endl;
        }
    }

    DLLEXPORT void GetBoardDimensions(int *rows, int *cols)
    {
        try
        {
            if (!rows || !cols)
            {
                std::cerr << "GetBoardDimensions: null pointer parameters" << std::endl;
                return;
            }

            if (board != nullptr)
            {
                *rows = board->get_rows();
                *cols = board->get_columns();

                if (currentGameId == 6)
                {
                    *rows = 5;
                    *cols = 5;
                }
                else if (currentGameId == 8)
                {
                    *rows = 6;
                    *cols = 6;
                }
            }
            else
            {
                std::cerr << "GetBoardDimensions: board is null" << std::endl;
                *rows = 0;
                *cols = 0;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "GetBoardDimensions exception: " << e.what() << std::endl;
            if (rows && cols)
            {
                *rows = 0;
                *cols = 0;
            }
        }
        catch (...)
        {
            std::cerr << "GetBoardDimensions unknown exception" << std::endl;
            if (rows && cols)
            {
                *rows = 0;
                *cols = 0;
            }
        }
    }

    DLLEXPORT int PerformMove(int x, int y, int playerVal, int extraChar)
    {
        try
        {
            if (board == nullptr || playerX == nullptr || playerO == nullptr)
            {
                std::cerr << "PerformMove: null pointers detected" << std::endl;
                return 2;
            }

            Player<char> *p = (playerVal == 1 ? playerX : playerO);

            Move<char> *move = new Move<char>(x, y, static_cast<char>(extraChar));

            if (move == nullptr)
            {
                std::cerr << "PerformMove: failed to create move" << std::endl;
                return 2;
            }

            bool ok = board->update_board(move);

            delete move;

            if (ok && currentGameId == 12)
            {
                auto *w = dynamic_cast<Word_XO_Board *>(board);
                if (w)
                    w->setLastPlayer(p);
            }

            return ok ? 0 : 1;
        }
        catch (const std::exception &e)
        {
            std::cerr << "PerformMove exception: " << e.what() << std::endl;
            return 2;
        }
        catch (...)
        {
            std::cerr << "PerformMove unknown exception" << std::endl;
            return 2;
        }
    }

    DLLEXPORT int GetAiMove(int playerVal)
    {
        try
        {
            if (ai == nullptr || board == nullptr || playerX == nullptr || playerO == nullptr)
            {
                std::cerr << "GetAiMove: null pointers detected (ai=" << (ai ? "ok" : "null") << ")" << std::endl;
                return 2;
            }

            Player<char> *p = (playerVal == 1 ? playerX : playerO);

            int depth = 9;
            
            if (currentGameId == 5)
            {
                depth = 6;
            }
            else if (currentGameId == 6)
            {
                int movesMade = 0;
                auto matrix = board->get_board_matrix();
                for (int r = 0; r < 5; ++r)
                {
                    for (int c = 0; c < 5; ++c)
                    {
                        if (matrix[r][c] != '.' && matrix[r][c] != 0)
                            movesMade++;
                    }
                }
                depth = (movesMade <= 4) ? 3 : (movesMade <= 16 ? 4 : 5);
            }

            Move<char> *move = ai->bestMove(p, '.', depth);

            if (move == nullptr)
            {
                std::cerr << "GetAiMove: AI returned null move" << std::endl;
                return 2;
            }

            bool ok = board->update_board(move);

            delete move;

            if (ok && currentGameId == 12)
            {
                auto *w = dynamic_cast<Word_XO_Board *>(board);
                if (w)
                    w->setLastPlayer(playerVal == 1 ? playerX : playerO);
            }

            return ok ? 0 : 1;
        }
        catch (const std::exception &e)
        {
            std::cerr << "GetAiMove exception: " << e.what() << std::endl;
            return 2;
        }
        catch (...)
        {
            std::cerr << "GetAiMove unknown exception" << std::endl;
            return 2;
        }
    }

    DLLEXPORT int *GetBoardState()
    {
        try
        {
            if (!board || !boardBuffer)
            {
                std::cerr << "GetBoardState: null pointers" << std::endl;
                return nullptr;
            }

            if (currentGameId == 6)  // Large XO
            {
                auto *largeBoard = dynamic_cast<Large_XO_Board *>(board);
                if (largeBoard)
                {
                    for (int r = 0; r < 5; ++r)
                    {
                        for (int c = 0; c < 5; ++c)
                        {
                            char cell = largeBoard->getCell(r, c);
                            boardBuffer[r * 5 + c] = (cell == '.' ? 0 : cell);
                        }
                    }
                    return boardBuffer;
                }
            }
            else if (currentGameId == 8)  // Obstacles
            {
                auto *obstacleBoard = dynamic_cast<Obstacles_Board *>(board);
                if (obstacleBoard)
                {
                    for (int r = 0; r < 6; ++r)
                    {
                        for (int c = 0; c < 6; ++c)
                        {
                            char cell = obstacleBoard->getCell(r, c);
                            boardBuffer[r * 6 + c] = (cell == '.' || cell == 0) ? 0 : cell;
                        }
                    }
                    return boardBuffer;
                }
            }
            else if (currentGameId == 7) {
                for (int r = 0; r < 3; ++r) {
                    for (int c = 0; c < 3; ++c) {
                        boardBuffer[r * 3 + c] = (board->get_cell(r,c) != '.'? '?' : 0);
                    }
                }
            }

            auto matrix = board->get_board_matrix();
            int rows = board->get_rows();
            int cols = board->get_columns();

            for (int r = 0; r < rows; ++r)
            {
                for (int c = 0; c < cols; ++c)
                {
                    char cell = matrix[r][c];
                    boardBuffer[r * cols + c] = (cell == '.' || cell == 0) ? 0 : cell;
                }
            }
            return boardBuffer;

            
        }
        catch (const std::exception &e)
        {
            std::cerr << "GetBoardState exception: " << e.what() << std::endl;
            return nullptr;
        }
        catch (...)
        {
            std::cerr << "GetBoardState unknown exception" << std::endl;
            return nullptr;
        }
    }

    DLLEXPORT bool CheckWin(int playerVal)
    {
        try
        {
            if (board == nullptr)
            {
                std::cerr << "CheckWin: board is null" << std::endl;
                return false;
            }
            Player<char> *p = (playerVal == 1) ? playerX : playerO;
            if (p == nullptr)
            {
                std::cerr << "CheckWin: player is null" << std::endl;
                return false;
            }
            return board->is_win(p);
        }
        catch (const std::exception &e)
        {
            std::cerr << "CheckWin exception: " << e.what() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "CheckWin unknown exception" << std::endl;
            return false;
        }
    }

    DLLEXPORT bool CheckDraw()
    {
        try
        {
            if (board == nullptr || playerX == nullptr)
            {
                std::cerr << "CheckDraw: null pointers" << std::endl;
                return false;
            }
            return board->is_draw(playerX);
        }
        catch (const std::exception &e)
        {
            std::cerr << "CheckDraw exception: " << e.what() << std::endl;
            return false;
        }
        catch (...)
        {
            std::cerr << "CheckDraw unknown exception" << std::endl;
            return false;
        }
    }

    DLLEXPORT bool IsGameOver()
    {
        try
        {
            if (board == nullptr || playerX == nullptr)
            {
                std::cerr << "IsGameOver: null pointers" << std::endl;
                return true;
            }
            return board->game_is_over(playerX);
        }
        catch (const std::exception &e)
        {
            std::cerr << "IsGameOver exception: " << e.what() << std::endl;
            return true;
        }
        catch (...)
        {
            std::cerr << "IsGameOver unknown exception" << std::endl;
            return true;
        }
    }
}