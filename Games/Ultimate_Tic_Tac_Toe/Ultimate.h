#ifndef Ultimate_H
#define Ultimate_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
using namespace std;

/**
 * @class Ultimate_Board
 * @brief Represents a 9x9 Ultimate Tic-Tac-Toe board.
 *
 * The board is divided into 3x3 "small boards". Each small board
 * is checked for a winner after every move, and a 3x3 "large board"
 * keeps track of the results of each small board.
 */
class Ultimate_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol for empty cell
    int small_board = 0;     ///< Current small board index
    bool full = true;        ///< Flag indicating if small board is full
    bool win = false;        ///< Flag indicating if a small board has a win
    vector<vector<char>> large_board = vector<vector<char>>(3, vector<char>(3, '.')); ///< Tracks results of small boards

public:
    Ultimate_Board();

    /// @brief Applies or undoes a move
    bool update_board(Move<char>* move) override;

    /// @brief Checks if the player has won the game
    bool is_win(Player<char>* player) override;

    /// @brief Checks if the game is a draw
    bool is_draw(Player<char>* player) override;

    /// @brief Always returns false (used for completeness)
    bool is_lose(Player<char>* player) { return false; }

    /// @brief Returns true if the game is over
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Checks the small board containing the last move for win/draw
     * @param x Row of the move
     * @param y Column of the move
     * @param sym Symbol placed
     */
    void small_board_check(int x, int y, char sym);

    /// @brief Returns true if all small boards are done (win or draw)
    bool all_small_boards_done();
};

/**
 * @class Ultimate_AI
 * @brief AI for Ultimate Tic-Tac-Toe. Currently chooses random empty cell.
 */
class Ultimate_AI : public AI {
public:
    Ultimate_AI() = default;

    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /// @brief Returns a random available move
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class Ultimate_UI
 * @brief User interface for Ultimate Tic-Tac-Toe
 */
class Ultimate_UI : public UI<char> {
public:
    Ultimate_UI();
    ~Ultimate_UI() {};

    /// @brief Creates a player instance
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /// @brief Reads next move from human or AI
    Move<char>* get_move(Player<char>* player) override;

    /// @brief Displays the 9x9 board with separators for small boards
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif  //Ultimate_H