/**
 * @file XO_Classes.cpp
 * @brief Implementation of Tic-Tac-Toe (X-O) classes including board, AI, and UI.
 *
 * Implements:
 *  - X_O_Board: Board logic (moves, win/draw/game-over detection)
 *  - X_O_AI: Minimax AI with alpha-beta pruning
 *  - XO_UI: Human input and computer/random moves
 */

#include "../header/XO_Classes.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <cstdlib>

using namespace std;

//--------------------------------------- X_O_Board Implementation

/**
 * @brief Construct a 3x3 empty X-O board.
 */
X_O_Board::X_O_Board() : Board(3, 3)
{
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

/**
 * @brief Update the board with a player's move.
 */
bool X_O_Board::update_board(Move<char>* move)
{
    if (!move) throw invalid_argument("Null move pointer");

    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        throw out_of_range("Move coordinates out of bounds");

    if (mark == 0 || mark == blank_symbol)
    {
        if (board[x][y] != blank_symbol) n_moves--;
        board[x][y] = blank_symbol;
        return true;
    }

    if (board[x][y] == blank_symbol)
    {
        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }

    return false; // Cell already occupied
}

/**
 * @brief Check if the player has won the game.
 */
bool X_O_Board::is_win(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");

    char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c){ return a == b && b == c && a != blank_symbol; };

    // Check rows and columns
    for (int i = 0; i < rows; ++i)
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;

    // Check diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Check if the player has lost (opponent has won).
 */
bool X_O_Board::is_lose(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");

    char sym = (player->get_symbol() == 'X') ? 'O' : 'X';
    auto all_equal = [&](char a, char b, char c){ return a == b && b == c && a != blank_symbol; };

    for (int i = 0; i < rows; ++i)
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;

    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

/**
 * @brief Check if the game ended in a draw.
 */
bool X_O_Board::is_draw(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");
    return (n_moves == rows * columns && !is_win(player) && !is_lose(player));
}

/**
 * @brief Determine if the game is over (win, lose, or draw).
 */
bool X_O_Board::game_is_over(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");
    return is_win(player) || is_lose(player) || is_draw(player);
}

//--------------------------------------- X_O_AI Implementation

/**
 * @brief Evaluate the board from AI perspective.
 */
float X_O_AI::evaluate(Board<char>* b, Player<char>* player)
{
    auto* board = dynamic_cast<X_O_Board*>(b);
    if (!board || !player) throw invalid_argument("Invalid board or player in evaluate()");

    if (board->is_win(player)) return 10.0f;
    if (board->is_lose(player)) return -10.0f;
    return 0.0f;
}

/**
 * @brief Minimax algorithm with alpha-beta pruning.
 */
float X_O_AI::minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth)
{
    auto* board = dynamic_cast<X_O_Board*>(player->get_board_ptr());
    if (!board || !player) throw invalid_argument("Invalid board or player in minimax()");

    float score = evaluate(board, player);
    if (score == 10 || score == -10 || board->is_draw(player) || depth == 0)
        return score;

    char ai = player->get_symbol();
    char opp = (ai == 'X') ? 'O' : 'X';
    char turn = aiTurn ? ai : opp;
    float best = aiTurn ? -INF : INF;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board->get_cell(i, j) == blankCell)
            {
                Move<char> move(i, j, turn);
                board->update_board(&move);

                float val = minimax(!aiTurn, player, alpha, beta, blankCell, depth - 1);

                move = Move<char>(i, j, 0); // Undo move
                board->update_board(&move);

                if (aiTurn) { best = max(best, val); alpha = max(alpha, val); }
                else        { best = min(best, val); beta  = min(beta, val); }

                if (beta <= alpha) return best;
            }

    return best;
}

/**
 * @brief Determine the best move for AI.
 */
Move<char>* X_O_AI::bestMove(Player<char>* player, char blankCell, int depth)
{
    auto* board = dynamic_cast<X_O_Board*>(player->get_board_ptr());
    if (!board || !player) throw invalid_argument("Invalid board or player in bestMove()");

    char ai = player->get_symbol();
    float bestVal = -INF;
    int bestR = -1, bestC = -1;

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board->get_cell(i, j) == blankCell)
            {
                Move<char> move(i, j, ai);
                board->update_board(&move);

                float val = minimax(false, player, -INF, INF, blankCell, depth - 1);

                move = Move<char>(i, j, 0); // Undo move
                board->update_board(&move);

                if (val > bestVal) { bestVal = val; bestR = i; bestC = j; }
            }

    if (bestR == -1 || bestC == -1)
        throw runtime_error("No valid moves left for AI");

    return new Move<char>(bestR, bestC, ai);
}

//--------------------------------------- XO_UI Implementation

/**
 * @brief Construct the UI for Tic-Tac-Toe.
 */
XO_UI::XO_UI()
    : Custom_UI<char>("Welcome to Tic-Tac-Toe", 3)
{
}

/**
 * @brief Get a move from the player (human or computer/random).
 */
Move<char>* XO_UI::get_move(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");

    int r = -1, c = -1;

    if (player->get_type() == PlayerType::HUMAN)
    {
        while (true)
        {
            try
            {
                cout << player->get_name() << " (" << player->get_symbol()
                     << ") enter your move (row col): ";
                if (!(cin >> r >> c))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw runtime_error("Invalid input, enter integers.");
                }
                if (r < 0 || r > 2 || c < 0 || c > 2)
                    throw out_of_range("Move out of board bounds.");
                break; // Valid input
            }
            catch (const exception& e)
            {
                cerr << "Error: " << e.what() << " Try again.\n";
            }
        }
    }
    else if (player->get_type() == PlayerType::COMPUTER)
    {
        r = rand() % 3;
        c = rand() % 3;
    }
    else if (player->get_type() == PlayerType::AI)
    {
        X_O_AI moveAI;
        return moveAI.bestMove(player, '.', 9);
    }

    return new Move<char>(r, c, player->get_symbol());
}