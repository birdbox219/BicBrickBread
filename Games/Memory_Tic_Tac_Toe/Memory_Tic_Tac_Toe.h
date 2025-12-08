/**
 * @file Memory_TicTacToe.h
 * @brief Defines the specialized Memory Tic-Tac-Toe variant with AI support.
 *
 * This variant implements:
 *  - Custom board logic with player moves (X/O placement)
 *  - Win, draw, and game-over detection
 *  - Custom board display through `display_board_matrix`
 *  - AI player integration via `Memory_AI` implementing the `AI` interface
 */

#ifndef MEMORY_TIC_TAC_TOE_H
#define MEMORY_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"
#include <limits>
#include <vector>

using namespace std;

/**
 * @class Memory_Board
 * @brief Specialized board class for Memory Tic-Tac-Toe.
 *
 * @ingroup BoardClasses
 *
 * Responsibilities:
 *  - Maintain board state
 *  - Apply player moves
 *  - Detect winning, draw, and game-over conditions
 *
 * Inherits from `Board<char>` and overrides core board behaviors.
 */
class Memory_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol representing empty cells

public:
    Memory_Board();

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Returns the symbol representing blank cells.
     * @return Blank cell symbol
     */
    char get_blank_symbol() const { return blank_symbol; }
};

/**
 * @class Memory_AI
 * @brief AI controller for Memory Tic-Tac-Toe implementing the AI interface.
 *
 * @ingroup AIClasses
 *
 * Responsibilities:
 *  - Evaluate board states
 *  - Generate the best move using minimax
 */
class Memory_AI : public AI {
public:
    Memory_AI() = default;
    /**
     * @brief Evaluates the board state from AI perspective.
     * @param board Pointer to board
     * @param player Pointer to AI player
     * @return Score evaluation
     */
    float evaluate(Board<char>* board, Player<char>* player) override;

    /**
     * @brief Minimax algorithm with alpha-beta pruning.
     * @param aiTurn True if it's AI's turn
     * @param player Pointer to AI player
     * @param alpha Alpha value
     * @param beta Beta value
     * @param blankCell Symbol for empty cells
     * @param depth Remaining depth
     * @return Minimax evaluation score
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override;

    /**
     * @brief Determines the optimal move for AI using minimax.
     * @param player Pointer to AI player
     * @param blankCell Symbol for empty cells
     * @param depth Maximum search depth
     * @return Pointer to the chosen Move<char>
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 9) override;
};

/**
 * @class Memory_UI
 * @brief User interface for Memory Tic-Tac-Toe.
 *
 * @ingroup UIClasses
 *
 * Responsibilities:
 *  - Display the board
 *  - Obtain player moves (human or AI)
 */
class Memory_UI : public Custom_UI<char> {
public:
    Memory_UI();
    ~Memory_UI() {}

    /**
     * @brief Obtains the next move from a player.
     * @param player Pointer to the player
     * @return Pointer to Move<char>
     */
    Move<char>* get_move(Player<char>* player);

    /**
     * @brief Renders the board.
     * @param matrix Two-dimensional grid of board characters
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const;
};

#endif // MEMORY_TIC_TAC_TOE_H