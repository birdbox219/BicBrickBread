#include "PyramidXO.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;


// ==============================
// PyramidXO_Board Implementation
// ==============================

PyramidXO_Board::PyramidXO_Board() : Board<char>(3, 5) {
    n_moves = 0;   // Count filled pyramid cells
}

bool PyramidXO_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    // Invalid indices
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }

    // Check pyramid-shaped playable region
    bool valid_shape = false;
    if (x == 0 && y == 2) valid_shape = true;
    else if (x == 1 && (y >= 1 && y <= 3)) valid_shape = true;
    else if (x == 2 && (y >= 0 && y <= 4)) valid_shape = true;

    if (!valid_shape) {
        return false;
    }

    // Cell already used
    if (board[x][y] != 0) {
        return false;
    }

    // Apply move
    board[x][y] = symbol;
    n_moves++;
    return true;
}

bool PyramidXO_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();

    // Horizontal wins
    if (board[1][1] == s && board[1][2] == s && board[1][3] == s) return true;
    if (board[2][0] == s && board[2][1] == s && board[2][2] == s) return true;
    if (board[2][1] == s && board[2][2] == s && board[2][3] == s) return true;
    if (board[2][2] == s && board[2][3] == s && board[2][4] == s) return true;

    // Vertical (only column 2 has three cells)
    if (board[0][2] == s && board[1][2] == s && board[2][2] == s) return true;

    // Diagonals
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    if (board[0][2] == s && board[1][3] == s && board[2][4] == s) return true;

    return false;
}

bool PyramidXO_Board::is_lose(Player<char>* player) {
    // No losing state defined
    return false;
}

bool PyramidXO_Board::is_draw(Player<char>* player) {
    // Draw when pyramid is full and no wins
    return (n_moves == 9 && !is_win(player));
}

bool PyramidXO_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}


// ==============================
// PyramidXO_UI Implementation
// ==============================

PyramidXO_UI::PyramidXO_UI() : UI<char>("Welcome to Pyramid XO Game", 3) {
}

Player<char>* PyramidXO_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* PyramidXO_UI::get_move(Player<char>* player) {
    int x, y;

    // Human enters coordinates
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
             << "), enter your move (row col): ";
        cin >> x >> y;
    }
    else {
        // Computer generates random pyramid move
        PyramidXO_AI ai;
        return ai.bestMove(player, 0);
    }

    return new Move<char>(x, y, player->get_symbol());
}

void PyramidXO_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    // Pretty pyramid rendering (unchanged; comments added)

    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();
    int center_col = cols / 2; 

    // Column indices
    cout << "\n    ";
    for (int j = 0; j < cols; ++j)
        cout << setw(cell_width + 1) << j;
    cout << "\n";

    for (int i = 0; i < rows; ++i) {

        // Top horizontal bars
        cout << "    ";
        for (int j = 0; j < cols; ++j) {
            bool isValid = (j >= center_col - i) && (j <= center_col + i);
            if (isValid)
                cout << " " << string(cell_width, '-');
            else
                cout << string(cell_width + 1, ' ');
        }
        cout << "\n";

        // Row index
        cout << setw(2) << i << "  ";

        // Contents
        for (int j = 0; j < cols; ++j) {
            bool isValid = (j >= center_col - i) && (j <= center_col + i);

            if (isValid) {
                char c = matrix[i][j];
                if (c == '\0') c = ' ';
                cout << "|" << setw(cell_width) << c;
            }
            else {
                cout << string(cell_width + 1, ' ');
            }
        }
        cout << "\n";

        // Bottom line of the last row only
        if (i == rows - 1) {
            cout << "    ";
            for (int j = 0; j < cols; ++j) {
                bool isValid = (j >= center_col - i) && (j <= center_col + i);
                if (isValid)
                    cout << " " << string(cell_width, '-');
            }
        }
    }
    cout << endl;
}


// ==============================
// PyramidXO_AI Implementation
// ==============================

Move<char>* PyramidXO_AI::bestMove(Player<char>* player, char blankCell, int depth) {
    vector<pair<int,int>> available;
    auto* board = player->get_board_ptr();

    // Collect all legal pyramid cells
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            bool valid_shape = false;
            if (r == 0 && c == 2) valid_shape = true;
            else if (r == 1 && (c >= 1 && c <= 3)) valid_shape = true;
            else if (r == 2 && (c >= 0 && c <= 4)) valid_shape = true;

            if (!valid_shape) continue;

            if (board->get_cell(r,c) == 0)
                available.push_back({r,c});
        }
    }

    // Pick random move
    int idx = rand() % available.size();
    return new Move<char>(available[idx].first,
                          available[idx].second,
                          player->get_symbol());
}