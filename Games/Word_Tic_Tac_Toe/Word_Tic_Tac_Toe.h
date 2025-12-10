#ifndef WORD_TIC_TAC_TOE_H
#define WORD_TIC_TAC_TOE_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/Custom_UI.h"
#include "../../header/AI.h"

/**
 * @file Word_Tic_Tac_Toe.h
 * @brief Header for Word Tic-Tac-Toe board, AI, and UI.
 *
 * @defgroup Word_XO Word XO
 * @brief Classes for the Word-based Tic-Tac-Toe variant.
 */

#include <set>
#include <string>
#include <vector>

/**
 * @class Word_XO_Board
 * @ingroup Word_XO
 * @brief Game board for Word Tic-Tac-Toe.
 *
 * @details Tracks moves and validates word formation. Supports checking
 * wins, losses, draws, and keeping track of the last player.
 */
class Word_XO_Board : public Board<char> {
public:
    /**
     * @brief Construct a new Word Tic-Tac-Toe board.
     * @details Initializes the empty board, move counter, and dictionaries.
     */
    Word_XO_Board();

    /**
     * @brief Get the empty cell symbol.
     * @return Character representing empty cells.
     */
    char getEmptyCell();

    /**
     * @brief Get the number of moves played.
     * @return Number of moves made so far.
     */
    int getMoveCount();

    /**
     * @brief Check if any valid word exists on the board.
     * @return True if a word is present; false otherwise.
     */
    bool wordExist();

    /**
     * @brief Apply a move to the board.
     * @details Validates and updates the board according to Word XO rules.
     * @param move Pointer to the Move<char> object.
     * @return True if move was successfully applied; false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Set the last player who moved.
     * @param player Pointer to the player who last moved.
     */
    void setLastPlayer(Player<char>* player);

    /**
     * @brief Check if the game is over.
     * @param player Player to check.
     * @return True if the game ended; false otherwise.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Check if a player has won.
     * @param player Player to check.
     * @return True if the player won; false otherwise.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if a player has lost.
     * @param player Player to check.
     * @return True if the player lost; false otherwise.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Check if the game ended in a draw.
     * @param player Player to check.
     * @return True if the game is a draw; false otherwise.
     */
    bool is_draw(Player<char>* player) override;

    /// @brief Forward dictionary of valid words.
    static std::set<std::string> dict;

    /// @brief Reversed dictionary of valid words.
    static std::set<std::string> revDict;

private:
    int nMoves = 0;                ///< Number of moves made.
    char emptyCell;                ///< Symbol for empty cell.
    Player<char>* lastPlayer;      ///< Pointer to last player.
};

// ============================================================================
// Word XO AI
// ============================================================================

/**
 * @class Word_AI
 * @ingroup AI
 * @brief AI engine for Word Tic-Tac-Toe.
 *
 * @details Provides evaluation, minimax search with alpha-beta pruning,
 * and best-move computation.
 */
class Word_AI : public AI {
public:
    Word_AI() = default;

    /**
     * @brief Evaluate the board for a player.
     * @param board Pointer to the game board.
     * @param player Player being evaluated.
     * @return Heuristic score.
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax search with alpha-beta pruning.
     * @param aiTurn True if AI's turn.
     * @param player Player being evaluated.
     * @param alpha Alpha value for pruning.
     * @param beta Beta value for pruning.
     * @param blankCell Symbol representing empty cell.
     * @param depth Depth of search.
     * @return Evaluation score.
     */
    float minimax(bool aiTurn, Player<char>* player,
                  float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Compute the best move for a player.
     * @param player Player requesting move.
     * @param blankCell Symbol representing empty cells.
     * @param depth Depth of search (default 6).
     * @return Pointer to the optimal Move<char>.
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

// ============================================================================
// Word XO UI
// ============================================================================

/**
 * @class Word_XO_UI
 * @ingroup Word_XO
 * @brief User interface for Word Tic-Tac-Toe.
 *
 * @details Handles human input, random computer moves, and AI moves.
 */
class Word_XO_UI : public Custom_UI<char>
{
public:
    /**
     * @brief Construct the UI.
     */
    Word_XO_UI();

    /**
     * @brief Get the next move from a player.
     * @param player Pointer to the active player.
     * @return Pointer to the generated Move<char>.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Evaluate potential characters for a board position.
     * @param r Row index.
     * @param c Column index.
     * @return Vector of (score, character) pairs for AI evaluation.
     */
    std::vector<std::pair<int, char>> evaluate(size_t r, size_t c);

    /// @brief Precomputed scoring tables for AI heuristics.
    static std::vector<std::pair<int, char>> score[3][3];
};

#endif // WORD_TIC_TAC_TOE_H