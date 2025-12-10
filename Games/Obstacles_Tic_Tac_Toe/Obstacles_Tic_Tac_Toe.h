/**
 * @file Obstacles_Tic_Tac_Toe.h
 * @brief Obstacles Tic-Tac-Toe game classes
 *
 * @defgroup Obstacles_XO Obstacles XO
 * @brief Classes and interfaces for Tic-Tac-Toe variant with traps and bitboard logic.
 */

#ifndef OBSTACLES_TIC_TAC_TOE_H
#define OBSTACLES_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"

/**
 * @class Obstacles_Board
 * @brief Modified Tic-Tac-Toe board supporting traps and 64-bit bitboard logic.
 * @ingroup Obstacles_XO
 */
class Obstacles_Board : public Board<char>
{
public:
    /**
     * @brief Constructs an empty Obstacles Tic-Tac-Toe board.
     */
    Obstacles_Board();

    /**
     * @brief Get the symbol at a given cell.
     * @param r Row index (0-based)
     * @param c Column index (0-based)
     * @return Character at the requested cell ('X', 'O', trap, or empty)
     */
    char getCell(size_t r, size_t c);

    /**
     * @brief Get the character used for empty cells.
     * @return Character representing empty cells
     */
    char getEmptyCell();

    /**
     * @brief Get all available moves.
     * @return Vector of valid cell indices (0–63)
     */
    std::vector<size_t> getAvailableMove();

    /**
     * @brief Get the total number of moves played.
     * @return Number of moves played
     */
    int getMoveCount();

    /**
     * @brief Attempt to place a symbol at the given position.
     * @param r Row index (0-based)
     * @param c Column index (0-based)
     * @param s Symbol to place ('X' or 'O')
     * @return True if the move was applied successfully, false otherwise
     */
    bool updateCell(size_t r, size_t c, char s);

    /**
     * @brief Applies a Move<char> to the board.
     * @param move Pointer to the move object
     * @return True if move was applied successfully, false otherwise
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the game is over for a player.
     * @param player Pointer to the player
     * @return True if the game ended (win, lose, or draw), false otherwise
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Checks if a player has won.
     * @param player Pointer to the player
     * @return True if the player has a winning configuration, false otherwise
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if a player has lost.
     * @param player Pointer to the player
     * @return True if the player has lost (opponent has 4-in-a-row), false otherwise
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks for a draw condition.
     * @param player Pointer to the player
     * @return True if the game is a draw, false otherwise
     */
    bool is_draw(Player<char>* player) override;

private:
    uint64_t boardX = 0;                 ///< Bitboard representing X positions
    uint64_t boardO = 0;                 ///< Bitboard representing O positions
    uint64_t boardTraps = 0;             ///< Bitboard representing traps/obstacles
    static uint64_t win4Masks[54];       ///< Precomputed 4-in-a-row winning masks
    char emptyCell;                      ///< Symbol used for empty cells
    int nMoves = 0;                      ///< Number of moves played
};

/**
 * @class Obstacles_AI
 * @brief AI implementation for Obstacles Tic-Tac-Toe.
 * @ingroup AI
 */
class Obstacles_AI : public AI {
public:
    Obstacles_AI() = default;

    /**
     * @brief Evaluation function (not implemented, placeholder).
     * @param board Pointer to the board
     * @param player Pointer to the AI player
     * @return Always returns 0.0f
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax search (not implemented, placeholder).
     * @param aiTurn True if it is the AI's turn
     * @param player Pointer to the AI player
     * @param alpha Alpha pruning value
     * @param beta Beta pruning value
     * @param blankCell Symbol representing empty cells
     * @param depth Search depth
     * @return Always returns 0.0f
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Computes the best move for the player.
     * @param player Pointer to the player
     * @param blankCell Symbol representing empty cells
     * @param depth Search depth (default 6)
     * @return Pointer to the selected Move<char>
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class Obstacles_UI
 * @brief User interface handler for Obstacles Tic-Tac-Toe.
 * @ingroup Obstacles_XO
 */
class Obstacles_UI : public UI<char>
{
public:
    /**
     * @brief Constructs a new UI handler for Obstacles Tic-Tac-Toe.
     */
    Obstacles_UI();

    ~Obstacles_UI() {}

    /**
     * @brief Obtains a move from a human or AI player.
     * @param player Pointer to the player whose turn it is
     * @return Pointer to a Move<char> describing the action
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Renders the game board.
     * @param matrix 2D vector representing board state
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;

private:
    Obstacles_Board* board = nullptr;    ///< Pointer to the associated Obstacles_Board
};

#endif // OBSTACLES_TIC_TAC_TOE_H