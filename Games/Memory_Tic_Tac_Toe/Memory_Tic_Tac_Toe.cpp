/**
 * @file Memory_TicTacToe.cpp
 * @brief Implementation of Memory Tic-Tac-Toe with AI support.
 *
 * Features:
 *  - Board logic: moves, game state checks, win/draw detection
 *  - AI logic: minimax algorithm with alpha-beta pruning
 *  - UI logic: human input, AI/computer moves, board display
 */

#include "Memory_Tic_Tac_Toe.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <cstdlib>

using namespace std;

//--------------------------------------- Memory_Board Implementation

// Constructor: initialize 3x3 board with blank symbols
Memory_Board::Memory_Board() : Board(3, 3)
{
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol; // empty cell symbol
}

// Update board with a player's move
bool Memory_Board::update_board(Move<char>* move)
{
    if (!move) throw invalid_argument("Null move pointer");

    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move coordinates
    if (x < 0 || x >= rows || y < 0 || y >= columns)
        throw out_of_range("Move coordinates out of bounds");

    // Undo move if mark is blank or zero
    if (mark == 0 || mark == blank_symbol)
    {
        if (board[x][y] != blank_symbol) n_moves--; // decrement move count
        board[x][y] = blank_symbol;
        return true;
    }

    // Apply move if cell is empty
    if (board[x][y] == blank_symbol)
    {
        board[x][y] = toupper(mark);
        n_moves++;
        return true;
    }

    return false; // Cell already occupied
}

// Check if the given player has a winning line
bool Memory_Board::is_win(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");
    char sym = player->get_symbol();

    // Lambda to check if three cells are equal and non-blank
    auto all_equal = [&](char a, char b, char c) { return a == b && b == c && a != blank_symbol; };

    // Check all rows and columns
    for (int i = 0; i < rows; ++i)
        if ((all_equal(board[i][0], board[i][1], board[i][2]) && board[i][0] == sym) ||
            (all_equal(board[0][i], board[1][i], board[2][i]) && board[0][i] == sym))
            return true;

    // Check both diagonals
    if ((all_equal(board[0][0], board[1][1], board[2][2]) && board[1][1] == sym) ||
        (all_equal(board[0][2], board[1][1], board[2][0]) && board[1][1] == sym))
        return true;

    return false;
}

// Check if the given player has lost (opponent wins)
bool Memory_Board::is_lose(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");
    char sym = (player->get_symbol() == 'X' ? 'O' : 'X');

    auto all_equal = [&](char a, char b, char c) { return a == b && b == c && a != blank_symbol; };

    // Check all rows and columns
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

// Check if the game is a draw
bool Memory_Board::is_draw(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");
    // Draw if all cells are filled and no winner
    return (n_moves == rows * columns && !is_win(player) && !is_lose(player));
}

// Check if the game has ended (win, lose, or draw)
bool Memory_Board::game_is_over(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");
    return is_win(player) || is_lose(player) || is_draw(player);
}

//--------------------------------------- Memory_AI Implementation

// Evaluate the board from AI perspective
float Memory_AI::evaluate(Board<char>* b, Player<char>* player)
{
    auto* board = dynamic_cast<Memory_Board*>(b);
    if (!board || !player) throw invalid_argument("Invalid board or player in evaluate()");

    if (board->is_win(player)) return 10.0f;   // AI wins
    if (board->is_lose(player)) return -10.0f; // AI loses
    return 0.0f;                               // Draw or undecided
}

// Minimax algorithm with alpha-beta pruning
float Memory_AI::minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth)
{
    auto* board = dynamic_cast<Memory_Board*>(player->get_board_ptr());
    if (!board || !player) throw invalid_argument("Invalid board or player in minimax()");

    float score = evaluate(board, player);
    if (score == 10 || score == -10 || board->is_draw(player) || depth == 0)
        return score;

    char ai = player->get_symbol();
    char opp = (ai == 'X') ? 'O' : 'X';
    char turn = aiTurn ? ai : opp;

    float best = aiTurn ? -INF : INF;

    // Iterate through all cells
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board->get_cell(i, j) == blankCell)
            {
                // Try move
                Move<char> move(i, j, turn);
                board->update_board(&move);

                // Recursive evaluation
                float val = minimax(!aiTurn, player, alpha, beta, blankCell, depth - 1);

                // Undo move
                move = Move<char>(i, j, 0);
                board->update_board(&move);

                // Update best score and alpha/beta
                if (aiTurn) { best = max(best, val); alpha = max(alpha, val); }
                else        { best = min(best, val); beta  = min(beta, val); }

                // Alpha-beta pruning
                if (beta <= alpha) return best;
            }

    return best;
}

// Compute the best move for AI
Move<char>* Memory_AI::bestMove(Player<char>* player, char blankCell, int depth)
{
    auto* board = dynamic_cast<Memory_Board*>(player->get_board_ptr());
    if (!board || !player) throw invalid_argument("Invalid board or player in bestMove()");

    char ai = player->get_symbol();
    float bestVal = -INF;
    int bestR = -1, bestC = -1;

    // Evaluate all possible moves
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board->get_cell(i, j) == blankCell)
            {
                Move<char> move(i, j, ai);
                board->update_board(&move);

                float val = minimax(false, player, -INF, INF, blankCell, depth - 1);

                // Undo move
                move = Move<char>(i, j, 0);
                board->update_board(&move);

                if (val > bestVal) { bestVal = val; bestR = i; bestC = j; }
            }

    if (bestR == -1 || bestC == -1)
        throw runtime_error("No valid moves left for AI");

    return new Move<char>(bestR, bestC, ai);
}

//--------------------------------------- Memory_UI Implementation

Memory_UI::Memory_UI()
    : Custom_UI<char>("Welcome to Memory Tic-Tac-Toe Variant", 3)
{
}

// Get move from player (human, computer, or AI)
Move<char>* Memory_UI::get_move(Player<char>* player)
{
    if (!player) throw invalid_argument("Null player pointer");

    int r = -1, c = -1;

    if (player->get_type() == PlayerType::HUMAN)
    {
        // Loop until valid human input
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
                break; // valid input
            }
            catch (const exception& e)
            {
                cerr << "Error: " << e.what() << " Try again.\n";
            }
        }
    }
    else if (player->get_type() == PlayerType::COMPUTER)
    {
        // Random move for computer
        r = rand() % 3;
        c = rand() % 3;
    }
    else if (player->get_type() == PlayerType::AI)
    {
        try
        {
            Memory_AI aiMove;
            return aiMove.bestMove(player, '.', 9);
        }
        catch (const exception& e)
        {
            cerr << "AI Error: " << e.what() << " Picking random move instead.\n";
            r = rand() % 3;
            c = rand() % 3;
        }
    }

    return new Move<char>(r, c, player->get_symbol());
}

// Display the board in formatted table
void Memory_UI::display_board_matrix(const vector<vector<char>>& matrix) const
{
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    // Print column indices
    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

    // Print each row with symbols
    for (int i = 0; i < rows; ++i)
    {
        cout << setw(2) << i << " |";
        for (int j = 0; j < cols; ++j)
            cout << setw(cell_width)
                 << (matrix[i][j] != '.' ? '?' : matrix[i][j]) << " |";
        cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    cout << endl;
}