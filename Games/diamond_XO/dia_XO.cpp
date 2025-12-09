// dia_XO.cpp
#include "dia_XO.h"
#include <algorithm>
#include <vector>
#include <utility>
#include <unordered_set>
#include <random>

/*
 * bounded():
 *   - True only if inside board AND the cell is free.
 *   - Used to ensure scanning does not go through invalid cells.
 */
bool dia_XO_Board::bounded(int x, int y)
{
    return (x >= 0 && x < 7 && y >= 0 && y < 7 && (board[x][y] == free));
}

/*
 * Constructor:
 *   - Creates the 7×7 board.
 *   - Marks the outer corners as invalid ('z') forming a diamond shape.
 *   - Marks remaining valid cells as free ('.').
 */
dia_XO_Board::dia_XO_Board() : Board(7, 7)
{
    int stop = 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < stop; j++) {
            board[i][j] = invalid;
            board[i][6 - j] = invalid;
            board[6 - i][j] = invalid;
            board[6 - i][6 - j] = invalid;
        }
        stop--;
    }
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (board[i][j] != invalid) {
                board[i][j] = free;
            }
        }
    }
}

/*
 * update_board():
 *   - Places player's symbol if the move is valid.
 */
bool dia_XO_Board::update_board(Move<char> *move)
{
    int x = move->get_x();
    int y = move->get_y();
    char s = move->get_symbol();

    // Out of bounds
    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return false;
    }
    // Must be free cell
    if (board[x][y] != free) {
        return false;
    }

    board[x][y] = s;
    return true;
}

/*
 * is_win():
 *   - Checks for longest line in each of 4 directions.
 *   - Requires:
 *       * One line >= 3
 *       * A different direction with a line >= 4
 *   - If so → win.
 */
bool dia_XO_Board::is_win(Player<char> *player)
{
    char s = player->get_symbol();

    int best_len[4] = {0, 0, 0, 0}; // Track max length for each direction pair

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {

            if (board[i][j] != s) continue;
            if (board[i][j] == invalid) continue;

            // Check only first 4 directions (the remaining 4 are opposites)
            for (int d = 0; d < 4; ++d) {
                int ddx = dx[d];
                int ddy = dy[d];

                // Backtrack to start of line
                int sx = i, sy = j;
                while (true) {
                    int px = sx - ddx;
                    int py = sy - ddy;
                    if (px < 0 || px >= 7 || py < 0 || py >= 7) break;
                    if (board[px][py] != s) break;
                    sx = px; sy = py;
                }

                // Count line forward
                int len = 0;
                int cx = sx, cy = sy;
                while (cx >= 0 && cx < 7 && cy >= 0 && cy < 7 && board[cx][cy] == s) {
                    ++len;
                    cx += ddx;
                    cy += ddy;
                }

                if (len > best_len[d])
                    best_len[d] = len;
            }
        }
    }

    // Win condition: one short line (>=3) AND one long line (>=4)
    for (int d3 = 0; d3 < 4; ++d3) {
        if (best_len[d3] < 3) continue;

        for (int d4 = 0; d4 < 4; ++d4) {
            if (d3 == d4) continue;
            if (best_len[d4] >= 4) return true;
        }
    }

    return false;
}

// No losing condition exists in this game
bool dia_XO_Board::is_lose(Player<char> *player)
{
    return false;
}

/*
 * is_draw():
 *   - True if no free cells remain.
 */
bool dia_XO_Board::is_draw(Player<char> *player)
{
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (board[i][j] == free) return false;
        }
    }
    return true;
}

/*
 * game_is_over():
 *   - Ends on win OR draw.
 */
bool dia_XO_Board::game_is_over(Player<char> *player)
{
    if (is_win(player)) return true;
    if (is_draw(player)) return true;
    return false;
}

//----------------------------------------------UI------------------------------------------------------

/*
 * UI constructor:
 *   - Game name = "dia_XO"
 *   - Moves required to win = 3 (symbolic)
 */
dia_XO_UI::dia_XO_UI()
    : UI<char>("dia_XO", 3) {}

/*
 * create_player():
 *   - Creates a Player object with the given information.
 */
Player<char>* dia_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

/*
 * get_move():
 *   - Human → enter row/col
 *   - Computer → use AI
 */
Move<char>* dia_XO_UI::get_move(Player<char>* player)
{
    int r, c;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
             << ") enter your move (row col): ";
        cin >> r >> c;
    } 
    else if (player->get_type() == PlayerType::COMPUTER) {
        dia_XO_AI ai;
        return ai.bestMove(player, '.');
    }

    return new Move<char>(r, c, player->get_symbol());
}

/*
 * bestMove():
 *   - Finds all empty cells.
 *   - Chooses one randomly.
 */
Move<char> *dia_XO_AI::bestMove(Player<char> *player, char blankCell, int depth)
{
    vector<pair<int,int>> emptyCells;

    auto* board = player->get_board_ptr();

    // NOTE: Bug in provided code: loops go 0..5 (6×6)
    for (int r = 0; r < 6; ++r) {
        for (int c = 0; c < 6; ++c) {
            if(board->get_cell(r,c) == blankCell)
                emptyCells.push_back({r,c});
        }
    }

    int idx = rand() % emptyCells.size();
    return new Move<char>(emptyCells[idx].first, emptyCells[idx].second, player->get_symbol());
}