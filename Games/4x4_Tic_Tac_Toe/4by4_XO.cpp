#include "4by4_XO.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <random>

using namespace std;

// ======================= BOARD IMPLEMENTATION ==============================

_4by4XO_Board::_4by4XO_Board() : Board<char>(4, 4) {
    n_moves = 0;

    // Initial predefined arrangement
    board[0][0] = 'X'; board[0][1] = 'O'; board[0][2] = 'X'; board[0][3] = 'O';
    board[3][0] = 'O'; board[3][1] = 'X'; board[3][2] = 'O'; board[3][3] = 'X';
}

/**
 * @brief Attempts to move a token from its source to destination.
 * 
 * Rules enforced:
 * - Move must be within board bounds.
 * - Source must contain the correct player's symbol.
 * - Destination must be empty.
 * - Move must be exactly one step in Manhattan distance.
 */
bool _4by4XO_Board::update_board(Move<char>* move) {
    _4by4XO_Move* m = static_cast<_4by4XO_Move*>(move);

    int fx = m->get_from_x();
    int fy = m->get_from_y();
    int tx = m->get_x();    
    int ty = m->get_y();
    char s = m->get_symbol();

    // Bounds check
    if (fx < 0 || fx >= 4 || fy < 0 || fy >= 4 || tx < 0 || tx >= 4 || ty < 0 || ty >= 4)
        return false;

    // Must move own symbol
    if (board[fx][fy] != s)
        return false;

    // Destination must be empty
    if (board[tx][ty] != 0)
        return false;

    // Must be adjacent
    if (abs(fx - tx) + abs(fy - ty) != 1)
        return false;

    // Apply move
    board[tx][ty] = s;
    board[fx][fy] = 0;
    n_moves++;
    return true;
}


/**
 * @brief Checks 3-in-a-row in all directions.
 */
bool _4by4XO_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();

    // Horizontal & vertical 3-in-a-row
    for (int i = 0; i < 4; i++) {
        if (board[i][0] == s && board[i][1] == s && board[i][2] == s) return true;
        if (board[i][1] == s && board[i][2] == s && board[i][3] == s) return true;

        if (board[0][i] == s && board[1][i] == s && board[2][i] == s) return true;
        if (board[1][i] == s && board[2][i] == s && board[3][i] == s) return true;
    }

    // Diagonals (8 different length-3 diagonals)
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[1][1] == s && board[2][2] == s && board[3][3] == s) return true;
    if (board[0][1] == s && board[1][2] == s && board[2][3] == s) return true;
    if (board[1][0] == s && board[2][1] == s && board[3][2] == s) return true;

    if (board[0][3] == s && board[1][2] == s && board[2][1] == s) return true;
    if (board[1][2] == s && board[2][1] == s && board[3][0] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    if (board[1][3] == s && board[2][2] == s && board[3][1] == s) return true;

    return false;
}

bool _4by4XO_Board::is_lose(Player<char>* player) {
    return false;
}

bool _4by4XO_Board::is_draw(Player<char>* player) {
    return n_moves > 100; // Prevent infinite sliding games
}

bool _4by4XO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


// ======================= UI IMPLEMENTATION ==============================

_4by4XO_UI::_4by4XO_UI()
    : UI<char>("Welcome to 4x4 Tic-Tac-Toe. Move your tokens to align 3!", 4) {}

Player<char>* _4by4XO_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Gets a human move or calls AI for its move.
 */
Move<char>* _4by4XO_UI::get_move(Player<char>* player) {
    int fx, fy, tx, ty;
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol() << ")\n";
        cout << "Select token to move (row col): ";
        cin >> fx >> fy;
        cout << "Select destination (row col): ";
        cin >> tx >> ty;
        return new _4by4XO_Move(fx, fy, tx, ty, player->get_symbol());
    } else {
        _4by4XO_AI ai;
        return ai.bestMove(player, '.');
    }
}


// ======================= AI IMPLEMENTATION ==============================

/**
 * @brief Generates all possible legal adjacent moves and picks one randomly.
 */
Move<char>* _4by4XO_AI::bestMove(Player<char>* player, char blankCell, int depth)
{
    Board<char>* b = player->get_board_ptr();
    char s = player->get_symbol();

    vector<_4by4XO_Move*> possibleMoves;

    int dx[4] = { -1, 1, 0, 0 };
    int dy[4] = { 0, 0, -1, 1 };

    // Scan all tokens belonging to the AI
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {

            if (b->get_cell(x, y) == s) {

                for (int k = 0; k < 4; k++) {
                    int nx = x + dx[k];
                    int ny = y + dy[k];

                    if (nx < 0 || nx >= 4 || ny < 0 || ny >= 4)
                        continue;

                    if (b->get_cell(nx, ny) == blankCell || b->get_cell(nx, ny) == 0) {
                        possibleMoves.push_back(new _4by4XO_Move(x, y, nx, ny, s));
                    }
                }
            }
        }
    }

    if (possibleMoves.empty()) {
        return nullptr;
    }

    int idx = rand() % possibleMoves.size();
    _4by4XO_Move* chosen = possibleMoves[idx];

    for (int i = 0; i < (int)possibleMoves.size(); i++) {
        if (i != idx) {
            delete possibleMoves[i];
        }
    }

    return chosen;
}