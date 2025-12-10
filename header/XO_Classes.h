#ifndef XO_CLASSES_H
#define XO_CLASSES_H

#include "BoardGame_Classes.h"
#include "AI.h"
#include "Custom_UI.h"

using namespace std;

/**
 * @file XO_Classes.h
 * @brief Classic Tic-Tac-Toe game classes
 *
 * @defgroup XO Classic XO
 * @brief Classes and interfaces for the classic Tic-Tac-Toe game.
 */

/**
 * @class X_O_Board
 * @ingroup XO
 * @brief Represents a Tic-Tac-Toe (X-O) board.
 *
 * This class inherits from the generic Board<char> and implements
 * the specific logic for a 3x3 Tic-Tac-Toe game.
 *
 * Responsibilities:
 * - Maintain board state.
 * - Apply player moves.
 * - Check win, draw, and game-over conditions.
 */
class X_O_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol used for empty cells.

public:
    /** @brief Default constructor initializing a 3x3 board. */
    X_O_Board();

    /**
     * @brief Apply a player's move to the board.
     * @param move Pointer to a Move<char> containing row, column, and symbol.
     * @return true if the move is valid and applied; false if the cell is occupied.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Check if a player has won the game.
     * @param player Pointer to the player to check.
     * @return true if the player has a complete row, column, or diagonal.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Check if a player has lost the game.
     * @param player Pointer to the player to check.
     * @return true if the opponent has a winning line.
     */
    bool is_lose(Player<char>* player);

    /**
     * @brief Check if the game ended in a draw.
     * @param player Pointer to the player (used to evaluate win/lose).
     * @return true if all cells are filled and no player has won.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Determine if the game is over.
     * @param player Pointer to the player (used to evaluate game state).
     * @return true if the game has ended in a win or draw.
     */
    bool game_is_over(Player<char>* player);

    /** @brief Get the symbol representing an empty cell. */
    char get_blank_symbol() const { return blank_symbol; }
};

/**
 * @class X_O_AI
 * @ingroup AI
 * @brief AI controller for Tic-Tac-Toe.
 *
 * Implements the AI interface using minimax with alpha-beta pruning.
 */
class X_O_AI : public AI {
public:
    /** @brief Default constructor */
    X_O_AI() {}

    /**
     * @brief Evaluate the board from the AI perspective.
     * @param b Pointer to the board to evaluate.
     * @param player Pointer to the AI player.
     * @return Numeric score of the board state.
     */
    float evaluate(Board<char>* b, Player<char>* player) override;

    /**
     * @brief Minimax algorithm with alpha-beta pruning.
     * @param aiTurn True if it is AI's turn; false for opponent.
     * @param player Pointer to the AI player.
     * @param alpha Alpha value for pruning.
     * @param beta Beta value for pruning.
     * @param blankCell Symbol used for empty cells.
     * @param depth Maximum search depth.
     * @return Best minimax score from the current position.
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override;

    /**
     * @brief Determine the best move for AI.
     * @param player Pointer to the AI player.
     * @param blankCell Symbol used for empty cells.
     * @param depth Maximum search depth (default 6).
     * @return Pointer to a Move<char> representing the optimal move.
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class XO_UI
 * @ingroup XO
 * @brief User interface for Tic-Tac-Toe.
 *
 * Handles human input and computer/random moves.
 */
class XO_UI : public Custom_UI<char> {
public:
    /** @brief Default constructor initializing the UI. */
    XO_UI();

    /** @brief Default destructor. */
    ~XO_UI() {}

    /**
     * @brief Get the next move from a player.
     * @param player Pointer to the player whose move is requested.
     * @return Pointer to a new Move<char> representing the chosen move.
     */
    Move<char>* get_move(Player<char>* player);
};

#endif // XO_CLASSES_H