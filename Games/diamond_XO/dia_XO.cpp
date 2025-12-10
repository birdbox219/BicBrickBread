// dia_XO.cpp
#include "dia_XO.h"
#include <algorithm>
#include <vector>
#include <utility>
#include <unordered_set>
#include <random>

bool dia_XO_Board::bounded(int x, int y)
{
    return (x >= 0 && x < 7 && y >= 0 && y < 7 && (board[x][y] == free));
}

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

bool dia_XO_Board::update_board(Move<char> *move)
{
    int x = move->get_x();
    int y = move->get_y();
    char s = move->get_symbol();

    if (x < 0 || x >= 7 || y < 0 || y >= 7) {
        return false;
    }

    if (board[x][y] != free) {
        return false;
    }

    board[x][y] = s;
    return true;
}

bool dia_XO_Board::is_win(Player<char> *player)
{
    char s = player->get_symbol();

    int best_len[4] = {0, 0, 0, 0};

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {

            if (board[i][j] != s) continue;
            if (board[i][j] == invalid) continue;

            for (int d = 0; d < 4; ++d) {
                int ddx = dx[d];
                int ddy = dy[d];

                int sx = i, sy = j;
                while (true) {
                    int px = sx - ddx;
                    int py = sy - ddy;
                    if (px < 0 || px >= 7 || py < 0 || py >= 7) break;
                    if (board[px][py] != s) break;
                    sx = px; sy = py;
                }

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

    for (int d3 = 0; d3 < 4; ++d3) {
        if (best_len[d3] < 3) continue;

        for (int d4 = 0; d4 < 4; ++d4) {
            if (d3 == d4) continue;
            if (best_len[d4] >= 4) return true;
        }
    }

    return false;
}


bool dia_XO_Board::is_lose(Player<char> *player)
{
    return false;
}


bool dia_XO_Board::is_draw(Player<char> *player)
{
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (board[i][j] == free) return false;
        }
    }
    return true;
}

bool dia_XO_Board::game_is_over(Player<char> *player)
{
    if (is_win(player)) return true;
    if (is_draw(player)) return true;
    return false;
}

dia_XO_UI::dia_XO_UI()
    : UI<char>("dia_XO", 3) {}

Player<char>* dia_XO_UI::create_player(string& name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

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

Move<char> *dia_XO_AI::bestMove(Player<char> *player, char blankCell, int depth)
{
    vector<pair<int,int>> emptyCells;

    auto* board = player->get_board_ptr();

    for (int r = 0; r < 7; ++r) {
        for (int c = 0; c < 7; ++c) {
            if(board->get_cell(r,c) == blankCell)
                emptyCells.push_back({r,c});
        }
    }

    int idx = rand() % emptyCells.size();
    return new Move<char>(emptyCells[idx].first, emptyCells[idx].second, player->get_symbol());
}