/**
 * @file FOUR.h
 * @brief Defines the 6x7 Connect-Four game variant with AI and UI support.
 *
 * @defgroup Connect4 Connect Four
 * @brief Classes and interfaces for 6x7 Connect-Four gameplay.
 *
 * This variant implements:
 *  - 6x7 board with column-based placement
 *  - Win, draw detection
 *  - Minimax-based AI
 *  - UI for human and AI interaction
 */

#ifndef FOUR_H
#define FOUR_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"
using namespace std;

/**
 * @class FOUR_Board
 * @brief Represents a 6x7 Connect-Four board.
 *
 * @ingroup Connect4
 *
 * Responsibilities:
 *  - Track board state
 *  - Apply player moves
 *  - Detect winning and draw conditions
 */
class FOUR_Board : public Board<char> {
private:
    char blank_symbol = '.';     ///< Symbol representing empty cells

public:
    int last_row[7]{};           ///< Tracks the number of filled cells in each column

    /** @brief Constructs an empty Connect-Four board. */
    FOUR_Board();

    /**
     * @brief Applies a move to the board.
     * @param move Pointer to a Move object containing row, column, and symbol
     * @return True if the move was applied successfully
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks whether the given player has won.
     * @param player Pointer to the player to check
     * @return True if the player has a winning configuration
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Connect-Four does not use losing detection.
     * @param player Pointer to the player to check
     * @return Always false
     */
    bool is_lose(Player<char>* player) { return false; }

    /**
     * @brief Checks whether the game ended in a draw.
     * @param player Pointer to the player (not used)
     * @return True if all columns are full without a winner
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks whether the game is over (win or draw).
     * @param player Pointer to the player (not used)
     * @return True if game has ended
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Returns the actual last filled row index in a column.
     * @param y Column index
     * @return Row index of the topmost filled cell
     */
    int get_last_row(int y) { return 5 - last_row[y]; }

    /**
     * @brief Increases the filled count of a column.
     * @param y Column index
     * @return New row index after increment
     */
    int increase_last_row(int y) { return 5 - last_row[y]++; }

    /**
     * @brief Decreases the filled count of a column.
     * @param y Column index
     * @return New row index after decrement
     */
    int decrease_last_row(int y) { return 5 - last_row[y]--; }
};

/**
 * @class Four_AI
 * @brief Implements a minimax-based AI for Connect-Four.
 *
 * @ingroup AI
 *
 * Responsibilities:
 *  - Evaluate board states
 *  - Generate optimal moves using minimax
 */
class Four_AI : public AI {
public:
    Four_AI() = default;

    /**
     * @brief Evaluates the current board state from AI perspective.
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
     * @param blankCell Symbol representing empty cells
     * @param depth Remaining search depth
     * @return Minimax evaluation score
     */
    float minimax(bool aiTurn, Player<char>* player,
                  float alpha, float beta,
                  char blankCell, int depth) override;

    /**
     * @brief Checks if any moves are still available.
     * @param board Pointer to the board
     * @return True if at least one column is not full
     */
    bool isMovesLeft(Board<char>* board);

    /**
     * @brief Finds the next open row in a given column.
     * @param board Pointer to the board
     * @param col Column index
     * @return Row index of the next empty cell in the column
     */
    int get_next_available_row(Board<char>* board, int col);

    /**
     * @brief Computes the best possible move for the AI.
     * @param player Pointer to AI player
     * @param blankCell Symbol representing empty cells
     * @param depth Maximum search depth
     * @return Pointer to the chosen Move<char>
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class FOUR_UI
 * @brief Handles user interaction and UI for Connect-Four.
 *
 * @ingroup Connect4
 *
 * Responsibilities:
 *  - Rendering the 6x7 board
 *  - Handling human and AI player input
 */
class FOUR_UI : public Custom_UI<char> {
private:
    char AI, OOP; ///< Symbols for AI and opponent

public:
    /** @brief Constructs the Connect-Four UI handler. */
    FOUR_UI();

    /** @brief Destructor. */
    ~FOUR_UI() {}

    /**
     * @brief Creates a player instance.
     * @param name Player name
     * @param symbol Player symbol ('X' or 'O')
     * @param type Player type (human or AI)
     * @return Pointer to the created Player<char>
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Obtains the next move from a human, computer, or AI player.
     * @param player Pointer to the current player
     * @return Pointer to the chosen Move<char>
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // FOUR_H