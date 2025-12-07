#ifndef WORD_TIC_TAC_TOE_H
#define WORD_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"
#include <cstdint>
#include <set>
#include <fstream>
#include <tuple>
#include <algorithm>

/**
 * @file Word_Tic_Tac_Toe.h
 * @brief Header file for the Word-based Tic-Tac-Toe board, AI, and UI modules.
 *
 * @details
 * This file defines all components necessary to operate the Word Tic-Tac-Toe variant,
 * including:
 * - The game board and word-validation logic
 * - The AI system for generating optimal moves
 * - The user interface for human, computer, and AI interaction
 */

// ============================================================================
// MODULE: Word Tic-Tac-Toe Board
// ============================================================================

/**
 * @defgroup Word_TicTacToe_Board Word Tic-Tac-Toe Board
 * @brief Core board and game-logic functionality.
 *
 * @details
 * Contains the implementation of the game board, including:
 * - Move validation
 * - Word detection
 * - Win/lose/draw evaluation
 * - Dictionary management
 * @{
 */

/**
 * @class Word_XO_Board
 * @ingroup Word_TicTacToe_Board
 *
 * @brief Game board implementation for the Word-based Tic-Tac-Toe variant.
 *
 * @details
 * This class specializes the generic `Board<char>` template to support:
 * - Word formation and dictionary lookup
 * - Forward and backward word detection
 * - Last-player tracking
 * - Terminal-state evaluation
 */
class Word_XO_Board : public Board<char> {
public:

    /** @name Constructors */
    ///@{

    /**
     * @brief Constructs and initializes the Word Tic-Tac-Toe board.
     *
     * @details
     * Initializes:
     * - Board dimensions
     * - Empty cell marker
     * - Move counter
     * - Word dictionary needed for validation
     */
    Word_XO_Board();
    ///@}

    /** @name Board State Queries */
    ///@{

    /**
     * @brief Returns the symbol representing an unoccupied cell.
     * @return Character used for empty slots.
     */
    char getEmptyCell();

    /**
     * @brief Retrieves the number of moves made so far.
     * @return Integer in the range [0, 9].
     */
    int getMoveCount();

    /**
     * @brief Checks if any valid word is currently formed on the board.
     * @return `true` if a word exists; otherwise `false`.
     */
    bool wordExist();
    ///@}

    /** @name Board Modification */
    ///@{

    /**
     * @brief Validates and applies a move to the board.
     *
     * @details
     * Validates:
     * - Board boundaries
     * - Empty-cell availability
     * - Word-based placement rules
     *
     * If valid, the board is updated and internal state adjusted.
     *
     * @param move Pointer to the move to apply.
     * @return `true` if applied, `false` otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Records the player who performed the last action.
     *
     * @param player Pointer to the last player to move.
     */
    void setLastPlayer(Player<char>* player);
    ///@}

    /** @name Game State Evaluation */
    ///@{

    /**
     * @brief Determines whether the game has reached a terminal state.
     *
     * @param player Player requesting evaluation.
     * @return `true` if game ended; otherwise `false`.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Checks if the specified player has won.
     * @param player Player to evaluate.
     * @return `true` if the player has won.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the specified player has lost.
     * @param player Player to evaluate.
     * @return `true` if the player lost.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Determines whether the game ended in a draw.
     * @param player Player to evaluate.
     * @return `true` if the game is a draw.
     */
    bool is_draw(Player<char>* player) override;
    ///@}

    /** @name Static Dictionaries */
    ///@{

    /**
     * @brief Dictionary of valid forward words.
     */
    static std::set<std::string> dict;

    /**
     * @brief Dictionary of reversed valid words.
     */
    static std::set<std::string> revDict;
    ///@}

private:
    int nMoves = 0;                ///< Number of moves made.
    char emptyCell;                ///< Character representing an empty cell.
    Player<char>* lastPlayer;      ///< Pointer to last player who acted.
};

///@}  // End of Word_TicTacToe_Board group



// ============================================================================
// MODULE: Word Tic-Tac-Toe AI
// ============================================================================

/**
 * @defgroup Word_TicTacToe_AI Word Tic-Tac-Toe AI
 * @brief AI logic for move evaluation and minimax strategy.
 *
 * @details
 * Contains:
 * - Heuristic evaluation
 * - Minimax with alpha-beta pruning
 * - Best-move computation
 * @{
 */

/**
 * @class Word_AI
 * @ingroup Word_TicTacToe_AI
 *
 * @brief AI engine for the Word Tic-Tac-Toe game.
 *
 * @details
 * Provides:
 * - Heuristic scoring based on potential word creation
 * - Minimax search tree evaluation
 * - Best move selection given a board state
 */
class Word_AI : public AI {
public:
    Word_AI() = default;

    /**
     * @brief Evaluates the board from the player's perspective.
     *
     * @param board Pointer to the board.
     * @param player Player to evaluate for.
     * @return Heuristic score.
     */
    float evaluate(Board<char>* board, Player<char>* player) override {}

    /**
     * @brief Performs minimax search with alpha-beta pruning.
     *
     * @param aiTurn True if it's the AI's turn.
     * @param player Player being evaluated.
     * @param alpha Alpha bound.
     * @param beta Beta bound.
     * @param blankCell Character representing an empty cell.
     * @param depth Remaining search depth.
     * @return Evaluation score.
     */
    float minimax(bool aiTurn, Player<char>* player,
                  float alpha, float beta, char blankCell, int depth) override {}

    /**
     * @brief Computes and returns the optimal move.
     *
     * @param player Player requesting the move.
     * @param blankCell Symbol representing empty cells.
     * @param depth Search depth (default = 6).
     * @return Pointer to the best move.
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

///@} // End of Word_TicTacToe_AI group



// ============================================================================
// MODULE: Word Tic-Tac-Toe UI
// ============================================================================

/**
 * @defgroup Word_TicTacToe_UI Word Tic-Tac-Toe UI
 * @brief User interface for human, random, and AI players.
 *
 * @details
 * Handles:
 * - Player input
 * - Display logic
 * - AI move generation
 * @{
 */

/**
 * @class Word_XO_UI
 * @ingroup Word_TicTacToe_UI
 *
 * @brief User Interface controller for the game.
 *
 * @details
 * Supports:
 * - Human move entry
 * - Random computer moves
 * - AI-driven move generation
 */
class Word_XO_UI : public Custom_UI<char>
{
public:
    /** @name Constructors */
    ///@{
    Word_XO_UI();
    ///@}

    /** @name Player Interaction */
    ///@{

    /**
     * @brief Retrieves the move from the current player.
     *
     * @details
     * Behavior depends on player type:
     * - Human → Read input
     * - Random bot → Generate random valid move
     * - AI → Call AI::bestMove()
     *
     * @param player Active player.
     * @return Pointer to the generated move.
     */
    Move<char>* get_move(Player<char>* player) override;
    ///@}

    /** @name AI Helper Tools */
    ///@{

    /**
     * @brief Evaluates potential characters at position (r, c).
     *
     * @param r Row index.
     * @param c Column index.
     * @return Vector of (score, character) pairs.
     */
    std::vector<std::pair<int, char>> evaluate(size_t r, size_t c);

    /**
     * @brief Precomputed scoring tables for AI heuristics.
     */
    static std::vector<std::pair<int, char>> score[3][3];
    ///@}
};

///@} // End of Word_TicTacToe_UI group

#endif // WORD_TIC_TAC_TOE_H