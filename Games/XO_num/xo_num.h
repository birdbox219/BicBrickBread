#ifndef XO_num_H
#define XO_num_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/**
 * @file xo_num.h
 * @brief Numerical Tic-Tac-Toe (Magic Square XO) game classes
 *
 * @defgroup XO_num Numerical XO
 * @brief Classes and interfaces for the numerical Tic-Tac-Toe game.
 * @{
 */

using namespace std;

/**
 * @class XO_NUM_Board
 * @brief Numerical Tic-Tac-Toe board.
 *
 * Players place numbers instead of X/O. Player 1 uses odd numbers {1,3,5,7,9},
 * Player 2 uses even numbers {2,4,6,8}. The goal is to place numbers such that
 * a row, column, or diagonal sums to 15.
 */
class XO_NUM_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol for empty cell

public:
    vector<char> odd{'1','3','5','7','9'};   ///< Available odd numbers
    vector<char> even{'2','4','6','8'};      ///< Available even numbers

    XO_NUM_Board();

    /// @brief Applies or undoes a move
    bool update_board(Move<char>* move) override;

    /// @brief Returns true if the board has a winning line summing to 15
    bool is_win(Player<char>* player) override;

    /// @brief Always false, included for completeness
    bool is_lose(Player<char>* player) { return false; }

    /// @brief Returns true if the board is full and no win
    bool is_draw(Player<char>* player) override;

    /// @brief Returns true if game is over (win or draw)
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class XO_NUM_AI
 * @brief Random-move AI for Numerical Tic-Tac-Toe
 */
class XO_NUM_AI : public AI {
public:
    XO_NUM_AI() = default;

    float evaluate(Board<char>* b, Player<char>* player) override { return 0.0f; }
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /// @brief Chooses a random available cell and number
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class XO_NUM_UI
 * @brief User interface for Numerical Tic-Tac-Toe
 */
class XO_NUM_UI : public UI<char> {
public:
    XO_NUM_UI();
    ~XO_NUM_UI() {} 

    char num; ///< Last number chosen

    /// @brief Creates a player
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /// @brief Sets up two players (human or computer)
    Player<char>** setup_players() override;

    /// @brief Gets next move from human or AI
    Move<char>* get_move(Player<char>* player) override;
};

/** @} */ // end of XO_num

#endif