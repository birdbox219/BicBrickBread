#include <iostream>
#include <iomanip>
#include <cctype>
#include "Ultimate.h"

// ==============================
// Ultimate_Board Implementation
// ==============================

Ultimate_Board::Ultimate_Board() : Board(9, 9) {
    // Initialize all cells as empty
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

/**
 * @brief Checks a 3x3 small board for a win or draw
 */
void Ultimate_Board::small_board_check(int x,int y, char sym) {
    auto all_equal = [&](char a, char b, char c) {
        return a == b && b == c && a != blank_symbol;
    };

    bool win = false;
    bool full = true;
    int row_start = (x / 3) * 3; 
    int col_start = (y / 3) * 3;

    // Check rows and columns
    for (int i = 0; i < 3; ++i) {
        if ((all_equal(board[row_start+i][col_start], board[row_start+i][col_start+1], board[row_start+i][col_start+2]) && board[row_start+i][col_start] == sym) ||
            (all_equal(board[row_start][col_start+i], board[row_start+1][col_start+i], board[row_start+2][col_start+i]) && board[row_start][col_start+i] == sym))
        {
            large_board[x/3][y/3] = sym;
            win = true;
        }
    }

    // Check diagonals
    if ((all_equal(board[row_start][col_start], board[row_start+1][col_start+1], board[row_start+2][col_start+2]) && board[row_start][col_start] == sym) ||
        (all_equal(board[row_start][col_start+2], board[row_start+1][col_start+1], board[row_start+2][col_start]) && board[row_start][col_start+2] == sym))
    {
        large_board[x/3][y/3] = sym;
        win = true;
    }

    // Check if small board is full (draw)
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[row_start+i][col_start+j] == blank_symbol)
                full = false;

    if (full && !win)
        large_board[x/3][y/3] = '#'; // Draw
}

/**
 * @brief Returns true if all small boards are completed
 */
bool Ultimate_Board::all_small_boards_done() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (large_board[i][j] == blank_symbol)
                return false;
    return true;
}

/**
 * @brief Applies or undoes a move and updates small board state
 */
bool Ultimate_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol || mark == 0) && large_board[x/3][y/3] == blank_symbol)
    {
        if (mark == 0) { // Undo
            n_moves--;
            board[x][y] = blank_symbol;
        } else { // Apply move
            n_moves++;
            board[x][y] = toupper(mark);
        }

        small_board_check(x, y, mark);

        // Display large board for debugging (can move to UI)
        cout << "\nLarge Board (3x3 small board results):\n";
        for (int i=0;i<3;i++){
            for (int j=0;j<3;j++) cout << setw(3) << large_board[i][j] << " ";
            cout << endl;
        }
        cout << endl;

        return true;
    }

    return false;
}

/**
 * @brief Returns true if player has won the large board
 */
bool Ultimate_Board::is_win(Player<char>* player) {
    const char sym = player->get_symbol();
    auto all_equal = [&](char a, char b, char c) { return a == b && b == c && a != blank_symbol && a != 'T'; };

    // Rows and columns
    for (int i = 0; i < 3; ++i)
        if ((all_equal(large_board[i][0], large_board[i][1], large_board[i][2]) && large_board[i][0]==sym) ||
            (all_equal(large_board[0][i], large_board[1][i], large_board[2][i]) && large_board[0][i]==sym))
            return true;

    // Diagonals
    if ((all_equal(large_board[0][0], large_board[1][1], large_board[2][2]) && large_board[1][1]==sym) ||
        (all_equal(large_board[0][2], large_board[1][1], large_board[2][0]) && large_board[1][1]==sym))
        return true;

    return false;
}

/**
 * @brief Returns true if the game ended in a draw
 */
bool Ultimate_Board::is_draw(Player<char>* player) {
    return all_small_boards_done() && !is_win(player);
}

/**
 * @brief Returns true if the game is over (win or draw)
 */
bool Ultimate_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ==============================
// Ultimate_UI Implementation
// ==============================

Ultimate_UI::Ultimate_UI() : UI<char>("Ultimate Tic_Tac_Toe", 6) {}

Player<char>* Ultimate_UI::create_player(string &name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

/**
 * @brief Displays 9x9 board with separators for small boards
 */
void Ultimate_UI::display_board_matrix(const vector<vector<char>>& matrix) const {
    if (matrix.empty() || matrix[0].empty()) return;

    int rows = matrix.size();
    int cols = matrix[0].size();

    cout << "\n    ";
    for (int j = 0; j < cols; ++j) cout << setw(cell_width + 1) << j;
    cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";

    for (int i = 0; i < rows; ++i) {
        cout << setw(2) << i << " |";
        for (int j = 0; j < cols; ++j)
            if (j == 2 || j == 5) cout << setw(cell_width) << matrix[i][j] << " ||";
            else cout << setw(cell_width) << matrix[i][j] << " |";

        if (i == 2 || i == 5) cout << "\n   " << string((cell_width + 2) * cols, '=') << "\n";
        else cout << "\n   " << string((cell_width + 2) * cols, '-') << "\n";
    }
    cout << endl;
}

/**
 * @brief Reads move from human or AI
 */
Move<char>* Ultimate_UI::get_move(Player<char>* player) {
    int x, y;

    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\nPlease enter your move x and y (0 to 8): ";
        cin >> x >> y;
    } else if (player->get_type() == PlayerType::COMPUTER) {
        Ultimate_AI ai;
        return ai.bestMove(player, '.');
    }

    return new Move<char>(x, y, player->get_symbol());
}

// ==============================
// Ultimate_AI Implementation
// ==============================

/**
 * @brief Returns a random available move
 */
Move<char>* Ultimate_AI::bestMove(Player<char>* player, char blankCell, int depth) {
    vector<pair<int,int>> available;
    auto* board = player->get_board_ptr();

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (board->get_cell(r,c) == blankCell)
                available.push_back({r,c});

    int idx = rand() % available.size();
    return new Move<char>(available[idx].first, available[idx].second, player->get_symbol());
}