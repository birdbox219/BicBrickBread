/**
 * @file Large_TicTacToe.h
 * @brief Defines the 5x5 Tic-Tac-Toe variant with AI and UI support.
 *
 * @defgroup Large_XO Large XO
 * @brief Classes and interfaces for 5x5 Tic-Tac-Toe game with neural network AI.
 *
 * This variant implements:
 *  - 5x5 board with bitboard representation
 *  - Win, lose, draw detection
 *  - AI with Minimax + Neural Network heuristic
 *  - UI for human/AI gameplay
 */

#ifndef LARGE_TIC_TAC_TOE_H
#define LARGE_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"
#include "../../Neural_Network/Include/NeuralNetwork.h"
#include <memory>
#include <vector>

/**
 * @class Large_XO_Board
 * @brief 5x5 Tic-Tac-Toe board implementation using a bitboard for fast evaluation.
 *
 * @ingroup Large_XO
 *
 * The board tracks positions using three 32-bit integers:
 *  - boardX  : bitmask for X
 *  - boardO  : bitmask for O
 *  - boardXO : bitmask for all occupied cells (X | O)
 *
 * Stores all possible 3-in-a-row masks for scoring.
 */
class Large_XO_Board : public Board<char>
{
public:
    /** @brief Construct an empty 5x5 board and initialize bitboards. */
    Large_XO_Board();

    /**
     * @brief Read the board state at a given cell.
     * @param r Row index [0–4]
     * @param c Column index [0–4]
     * @return Character at the cell ('X', 'O', or empty)
     */
    char getCell(size_t r, size_t c);

    /** @brief Returns the character representing an empty cell. */
    char getEmptyCell();

    /** @brief Returns the number of moves played so far. */
    int getMoveCount();

    /**
     * @brief Update a cell using the bitboard representation.
     * @param r Row index [0–4]
     * @param c Column index [0–4]
     * @param s Symbol to place ('X', 'O', or empty to clear)
     * @return True if the update was valid
     */
    bool updateCell(size_t r, size_t c, char s);

    /**
     * @brief Update board using a Move object.
     * @param move Move containing row, column, and symbol
     * @return True if the move is valid and applied
     */
    bool update_board(Move<char>* move) override;

    /** @brief Check if the game is over (board full). */
    bool game_is_over(Player<char>* /*player*/) override;

    /** @brief Check if the player has a winning configuration. */
    bool is_win(Player<char>* player) override;

    /** @brief Check if the player has lost. */
    bool is_lose(Player<char>* player) override;

    /** @brief Check if the game ended in a draw. */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Count all 3-in-a-row patterns for a given symbol.
     * @param sym Symbol to count ('X' or 'O')
     * @return Number of 3-in-a-row patterns
     */
    float countWin(char sym);

    /**
     * @brief Encode board state into a matrix for neural network input.
     * AI = +1, Opponent = -1, Empty = 0
     * @param ai Symbol of AI player
     * @param input Matrix to fill
     */
    void encode(char ai, Matrix<double>& input);

private:
    uint32_t boardX   = 0;        ///< Bitmask for X positions
    uint32_t boardO   = 0;        ///< Bitmask for O positions
    uint32_t boardXO  = 0;        ///< Bitmask for all occupied positions
    static uint32_t win3Masks[48];///< All 3-in-a-row combinations

    char emptyCell = '.';         ///< Empty cell symbol
    int  nMoves    = 0;           ///< Number of moves played
};

/**
 * @class Large_XO_AI
 * @brief AI for 5x5 Tic-Tac-Toe using Minimax + Neural Network heuristic.
 *
 * @ingroup AI
 */
class Large_XO_AI : public AI
{
public:
    Large_XO_AI();

    /**
     * @brief Evaluate a board using neural network + terminal heuristics.
     * @param board Pointer to the board
     * @param player Pointer to AI player
     * @return Score evaluation
     */
    float evaluate(Board<char>* board, Player<char>* player) override;

    /**
     * @brief Minimax algorithm with alpha-beta pruning.
     * @param aiTurn True if AI's turn
     * @param player Pointer to AI player
     * @param alpha Alpha value
     * @param beta Beta value
     * @param blankCell Symbol for empty cells
     * @param depth Remaining search depth
     * @return Minimax evaluation score
     */
    float minimax(bool aiTurn,
                  Player<char>* player,
                  float alpha,
                  float beta,
                  char blankCell,
                  int depth) override;

    /**
     * @brief Compute the best move for the player.
     * @param player Pointer to AI player
     * @param blankCell Symbol for empty cells
     * @param depth Maximum search depth
     * @return Pointer to the chosen Move<char>
     */
    Move<char>* bestMove(Player<char>* player,
                         char blankCell,
                         int depth = 6) override;

private:
    std::shared_ptr<NeuralNetwork> NN;   ///< Shared neural network
    std::shared_ptr<NeuralNetwork> NNX;  ///< NN specialized for X
    std::shared_ptr<NeuralNetwork> NNO;  ///< NN specialized for O
    char aiSymbol  = 'X';
    char oppSymbol = 'O';
};

/**
 * @class Large_XO_UI
 * @brief User interface for 5x5 Tic-Tac-Toe (human and AI interaction).
 *
 * @ingroup Large_XO
 *
 * Responsibilities:
 *  - Board rendering
 *  - Player input
 *  - Invoking AI moves
 */
class Large_XO_UI : public Custom_UI<char>
{
public:
    /** @brief Construct UI and load AI networks. */
    Large_XO_UI();

    ~Large_XO_UI() override = default;

    /**
     * @brief Request a move from human or AI player.
     * @param player Pointer to the player making the move
     * @return Pointer to the chosen Move<char>
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Render the board in 5x5 grid format.
     * @param matrix Unused parameter, kept for interface compatibility
     */
    void display_board_matrix(const std::vector<std::vector<char>>& /*matrix*/) const override;

private:
    Large_XO_AI AI;
    Large_XO_Board* board = nullptr; ///< Pointer to the board
};

#endif // LARGE_TIC_TAC_TOE_H