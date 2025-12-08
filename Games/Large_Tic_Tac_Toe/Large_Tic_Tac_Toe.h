#ifndef LARGE_TIC_TAC_TOE_H
#define LARGE_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"
#include "../../Neural_Network/Include/NeuralNetwork.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <random>

/**
 * @brief 5x5 Tic-Tac-Toe board implementation using a bitboard for fast evaluation.
 *
 * The board tracks positions using three 32-bit integers:
 *  - boardX  : bitmask for X
 *  - boardO  : bitmask for O
 *  - boardXO : bitmask for all occupied cells (X | O)
 *
 * This representation dramatically accelerates win detection and evaluation.
 * The class also stores all possible 3-in-a-row masks for scoring.
 */
class Large_XO_Board : public Board<char>
{
public:
    // ---------------------------------------------------------------------
    // Constructors
    // ---------------------------------------------------------------------

    /** @brief Construct an empty 5x5 board and initialize bitboards. */
    Large_XO_Board();

    // ---------------------------------------------------------------------
    // Accessors
    // ---------------------------------------------------------------------

    /**
     * @brief Read the board state at a given cell.
     * @param r Row index [0–4]
     * @param c Column index [0–4]
     * @return 'X', 'O', or empty symbol
     */
    char getCell(size_t r, size_t c);

    /** @brief @return Character representing an empty cell (e.g., '.') */
    char getEmptyCell();

    /** @brief @return Number of played moves in [0, 24]. */
    int getMoveCount();

    // ---------------------------------------------------------------------
    // Mutators
    // ---------------------------------------------------------------------

    /**
     * @brief Update a cell using the bitboard representation.
     * @param r Row index [0–4]
     * @param c Column index [0–4]
     * @param s New symbol: 'X', 'O', or emptyCell to clear
     * @return True if the update was valid
     */
    bool updateCell(size_t r, size_t c, char s);

    /**
     * @brief Update board using a Move object (overrides base class).
     * @param move Move containing row, column, and symbol
     * @return True if the move is valid and applied
     */
    bool update_board(Move<char>* move) override;

    // ---------------------------------------------------------------------
    // Game Logic
    // ---------------------------------------------------------------------

    /**
     * @brief Check if the game is over (board full in this variant).
     * @return True if the board is full
     */
    bool game_is_over(Player<char>* /*player*/) override;

    /**
     * @brief Determine if a player wins (based on 3-in-a-row counts).
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Determine if a player loses.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Determine if the game is a draw.
     */
    bool is_draw(Player<char>* player) override;

    // ---------------------------------------------------------------------
    // Helpers
    // ---------------------------------------------------------------------

    /**
     * @brief Count all 3-in-a-row patterns for a given symbol.
     * @param sym 'X' or 'O'
     * @return Number of 3-in-a-row patterns
     */
    float countWin(char sym);

    /**
     * @brief Encode board state into a matrix for the neural network.
     * AI = +1, Opponent = -1, Empty = 0.
     * @param ai Symbol of AI player
     * @param input Matrix to be filled
     */
    void encode(char ai, Matrix<double>& input);

private:
    uint32_t boardX   = 0;        ///< Bitmask for X positions
    uint32_t boardO   = 0;        ///< Bitmask for O positions
    uint32_t boardXO  = 0;        ///< Bitmask for all occupied positions
    static uint32_t win3Masks[48]; ///< All 3-in-a-row combinations

    char emptyCell = '.';         ///< Character representing empty cell
    int  nMoves    = 0;           ///< Number of moves played
};

// ============================================================================
// AI Class
// ============================================================================

/**
 * @brief AI for 5x5 Tic-Tac-Toe using Minimax + Neural Network heuristic.
 */
class Large_XO_AI : public AI
{
public:
    Large_XO_AI();

    /** @brief Evaluate a board using NN + terminal heuristics. */
    float evaluate(Board<char>* board, Player<char>* player) override;

    /** @brief Minimax with alpha-beta pruning and NN-based move ordering. */
    float minimax(bool aiTurn,
                  Player<char>* player,
                  float alpha,
                  float beta,
                  char blankCell,
                  int depth) override;

    /** @brief Compute the best move for the player. */
    Move<char>* bestMove(Player<char>* player,
                         char blankCell,
                         int depth = 6) override;

private:
    std::shared_ptr<NeuralNetwork> NN;   ///< Shared NN heuristic
    std::shared_ptr<NeuralNetwork> NNX;  ///< NN specialized for X
    std::shared_ptr<NeuralNetwork> NNO;  ///< NN specialized for O
    char aiSymbol  = 'X';
    char oppSymbol = 'O';
};

// ============================================================================
// UI Class
// ============================================================================

/**
 * @brief UI for 5x5 Tic-Tac-Toe (human and AI interaction).
 *
 * This class handles:
 *  - board rendering
 *  - player input
 *  - invoking the AI move
 */
class Large_XO_UI : public Custom_UI<char>
{
public:
    /** @brief Construct UI and load AI networks. */
    Large_XO_UI();

    ~Large_XO_UI() override = default;

    /**
     * @brief Request a move from Human, Random, or AI player.
     * @return Move object
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Render the board in 5x5 grid format.
     *
     * @note `matrix` parameter is unused (kept for interface compatibility).
     */
    void display_board_matrix(const std::vector<std::vector<char>>& /*matrix*/) const override;

private:
    Large_XO_AI AI;
    Large_XO_Board* board = nullptr; ///< Board reference for rendering
};

#endif // LARGE_TIC_TAC_TOE_H