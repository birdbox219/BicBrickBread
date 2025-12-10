#ifndef XO_inf_H
#define XO_inf_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

/**
 * @file XO_inf.h
 * @brief Infinite Tic-Tac-Toe game classes.
 *
 * @defgroup XO_inf Infinite XO
 * @brief Classes and interfaces for the infinite Tic-Tac-Toe game.
 */

using namespace std;

/**
 * @struct Vertex
 * @ingroup XO_inf
 * @brief Represents a single move on the board.
 *
 * Stores the coordinates and symbol of a move.
 */
struct Vertex {
    int x;       ///< Row coordinate.
    int y;       ///< Column coordinate.
    char symbol; ///< Symbol placed at this position.
};

/**
 * @class XO_inf_Board
 * @ingroup XO_inf
 * @brief Board class for infinite Tic-Tac-Toe.
 *
 * Maintains the board state, move history, and provides
 * logic for applying moves and checking game termination conditions.
 */
class XO_inf_Board : public Board<char> {
private:
    char blank_symbol = '.';      ///< Character used to represent an empty cell.
    deque<Vertex> history;        ///< Move history for redo/undo.
    stack<Vertex> undoHistory;    ///< Stack to support undo operations.
    int cnt = 0;                  ///< Move counter.

public:
    XO_inf_Board();

    /**
     * @brief Apply a move to the board.
     * @param move Pointer to the move object.
     * @return True if the move was applied successfully.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Check if a player has achieved a win condition.
     * @param player Pointer to the player being evaluated.
     * @return True if the player has won.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Placeholder: infinite Tic-Tac-Toe does not use lose detection.
     * @param player Pointer to the player.
     * @return Always returns false.
     */
    bool is_lose(Player<char>* player) { return false; };

    /**
     * @brief Check if the game is a draw.
     * @param player Pointer to the active player.
     * @return True if the game is drawn.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determine whether the game is over.
     * @param player Pointer to the active player.
     * @return True if the game has ended.
     */
    bool game_is_over(Player<char>* player);
};

/**
 * @class XO_inf_AI
 * @ingroup AI
 * @brief AI engine for infinite Tic-Tac-Toe.
 *
 * Provides board evaluation, minimax search with alpha–beta pruning,
 * and best move computation.
 */
class XO_inf_AI : public AI {
public:
    XO_inf_AI() {}

    /**
     * @brief Evaluate the score of the board state.
     * @param b Pointer to the board.
     * @param player Pointer to the player for whom the board is evaluated.
     * @return Score representing the board state.
     */
    float evaluate(Board<char>* b, Player<char>* player) override;

    /**
     * @brief Minimax search algorithm with alpha-beta pruning.
     * @param aiTurn True if it's the AI's turn.
     * @param player Pointer to the AI player.
     * @param alpha Alpha bound for pruning.
     * @param beta Beta bound for pruning.
     * @param blankCell Character used for empty cells.
     * @param depth Search depth limit.
     * @return Evaluation score for the position.
     */
    float minimax(bool aiTurn,
                  Player<char>* player,
                  float alpha,
                  float beta,
                  char blankCell,
                  int depth) override;

    /**
     * @brief Determine the best move for the AI.
     * @param player Pointer to the AI player.
     * @param blankCell Character representing an empty cell.
     * @param depth Depth limit for the minimax search (default = 6).
     * @return Pointer to the chosen move.
     */
    Move<char>* bestMove(Player<char>* player,
                         char blankCell,
                         int depth = 6) override;
};

/**
 * @class XO_inf_UI
 * @ingroup XO_inf
 * @brief User interface controller for infinite Tic-Tac-Toe.
 *
 * Handles player creation and move acquisition.
 */
class XO_inf_UI : public Custom_UI<char> {
public:
    XO_inf_UI();
    ~XO_inf_UI() {}

    /**
     * @brief Create a new player.
     * @param name Reference to the player's name string.
     * @param symbol Character symbol assigned to the player.
     * @param type Type of the player (human or AI).
     * @return Pointer to the created player instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Retrieve a player's move input.
     * @param player Pointer to the player providing the move.
     * @return Pointer to the constructed move object.
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif