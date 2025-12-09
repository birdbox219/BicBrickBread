#ifndef XO_inf_H
#define XO_inf_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

/**
 * @file XO_inf.h
 * @brief Infinite Tic-Tac-Toe game classes
 *
 * @defgroup XO_inf Infinite XO
 * @brief Classes and interfaces for the infinite Tic-Tac-Toe game.
 * @{
 */

using namespace std;

/**
 * @struct Vertex
 * @brief Represents a single move on the board.
 */
struct Vertex {
    int x;       ///< Row coordinate
    int y;       ///< Column coordinate
    char symbol; ///< Symbol placed at this position
};

/**
 * @class XO_inf_Board
 * @brief Board class for infinite Tic-Tac-Toe.
 *
 * Maintains the board state, history of moves, and provides
 * win/draw checking and move updating.
 */
class XO_inf_Board : public Board<char> {
private:
    char blank_symbol = '.';      ///< Character used to represent an empty cell
    deque<Vertex> history;        ///< Move history for redo/undo
    stack<Vertex> undoHistory;    ///< Stack to support undo operations
    int cnt = 0;                  ///< Move counter

public:
    XO_inf_Board();

    bool update_board(Move<char>* move);                ///< Apply a move
    bool is_win(Player<char>* player);                 ///< Check win condition
    bool is_lose(Player<char>* player) { return false; }; ///< Placeholder for lose
    bool is_draw(Player<char>* player);                ///< Check draw
    bool game_is_over(Player<char>* player);           ///< Check if game over
};

/**
 * @class XO_inf_AI
 * @brief AI for infinite Tic-Tac-Toe.
 *
 * Implements evaluation, minimax with alpha-beta pruning,
 * and best move selection.
 */
class XO_inf_AI : public AI {
public:
    XO_inf_AI() {}

    float evaluate(Board<char>* b, Player<char>* player) override; ///< Evaluate board score
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override; ///< Minimax algorithm
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override; ///< Compute best AI move
};

/**
 * @class XO_inf_UI
 * @brief User interface for infinite Tic-Tac-Toe.
 *
 * Handles player creation and input of moves.
 */
class XO_inf_UI : public Custom_UI<char> {
public:
    XO_inf_UI();
    ~XO_inf_UI() {}

    Player<char>* create_player(string& name, char symbol, PlayerType type); ///< Create a player
    Move<char>* get_move(Player<char>* player) override;                     ///< Get player's move
};

/** @} */ // end of XO_inf

#endif