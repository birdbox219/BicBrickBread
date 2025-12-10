#include <iostream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include "xo_num.h"

using namespace std;

// ======================= XO_NUM_Board =======================

XO_NUM_Board::XO_NUM_Board() : Board(3, 3) {
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

bool XO_NUM_Board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    if (board[x][y] != blank_symbol && mark != 0)
        return false;

    if (mark == 0) { // undo
        board[x][y] = blank_symbol;
        n_moves++;
        // Restore the number to the vector
        if (move->get_symbol() % 2 == 0) even.push_back(mark);
        else odd.push_back(mark);
    } else { // apply
        board[x][y] = mark;
        n_moves++;
        // Remove number from odd/even vector
        if (mark % 2 == 0) {
            auto it = find(even.begin(), even.end(), mark);
            if (it != even.end()) even.erase(it);
        } else {
            auto it = find(odd.begin(), odd.end(), mark);
            if (it != odd.end()) odd.erase(it);
        }
    }
    return true;
}

bool XO_NUM_Board::is_win(Player<char> *player) {
    auto sum = [](char a, char b, char c) { return (a - '0') + (b - '0') + (c - '0'); };

    // Rows & columns
    for (int i = 0; i < 3; i++)
        if (sum(board[i][0], board[i][1], board[i][2]) == 15 ||
            sum(board[0][i], board[1][i], board[2][i]) == 15)
            return true;

    // Diagonals
    if (sum(board[0][0], board[1][1], board[2][2]) == 15 ||
        sum(board[0][2], board[1][1], board[2][0]) == 15)
        return true;

    return false;
}

bool XO_NUM_Board::is_draw(Player<char> *player) {
    return n_moves == 9 && !is_win(player);
}

bool XO_NUM_Board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player);
}

// ======================= XO_NUM_UI =======================

XO_NUM_UI::XO_NUM_UI() : UI<char>("Numerical Tic-Tac-Toe", 3) {}

Player<char>** XO_NUM_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    string name1 = get_player_name("Player 1");
    PlayerType type1 = get_player_type_choice("Player 1", type_options);
    players[0] = create_player(name1, '1', type1);

    string name2 = get_player_name("Player 2");
    PlayerType type2 = get_player_type_choice("Player 2", type_options);
    players[1] = create_player(name2, '2', type2);

    return players;
}

Player<char>* XO_NUM_UI::create_player(string &name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* XO_NUM_UI::get_move(Player<char>* player) {
    auto* board = dynamic_cast<XO_NUM_Board*>(player->get_board_ptr());
    int x, y;
    char num;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nEnter row and column (0-2): ";
        cin >> x >> y;

        vector<char>& choices = (player->get_symbol() == '1') ? board->odd : board->even;

        cout << "Choose a number: ";
        for (char ch : choices) cout << ch << " ";
        cout << endl;

        cin >> num;

        auto exist = [](vector<char>& v, char n) -> bool {
            return find(v.begin(), v.end(), n) != v.end(); // just check existence here
        };

        while (!exist(choices, num)) {
            cout << "Invalid number, choose again: ";
            for (char ch : choices) cout << ch << " ";
            cout << endl;
            cin >> num;
        }
    } else {
        XO_NUM_AI ai;
        return ai.bestMove(player, '.');
    }

    return new Move<char>(x, y, num);
}

// ======================= XO_NUM_AI =======================

Move<char>* XO_NUM_AI::bestMove(Player<char>* player, char blankCell, int depth) {
    auto* board = dynamic_cast<XO_NUM_Board*>(player->get_board_ptr());
    vector<pair<int,int>> emptyCells;

    // Collect all empty cells
    for (int i = 0; i < board->get_rows(); i++) {
        for (int j = 0; j < board->get_columns(); j++) {
            if (board->get_cell(i,j) == blankCell) {
                emptyCells.push_back({i,j});
            }
        }
    }

    if (emptyCells.empty()) return nullptr; // no moves left

    // Pick a random empty cell
    int cellIndex = rand() % emptyCells.size();
    int x = emptyCells[cellIndex].first;
    int y = emptyCells[cellIndex].second;

    // Pick a number from remaining odd/even choices
    vector<char>& choices = (player->get_symbol() == '1') ? board->odd : board->even;
    if (choices.empty()) return nullptr; // no numbers left

    int numIndex = rand() % choices.size();
    char num = choices[numIndex];

    return new Move<char>(x, y, num);
}