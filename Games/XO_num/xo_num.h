#ifndef XO_NUM_H
#define XO_NUM_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/**
 * @file XO_num.h
 * @brief Numerical Tic-Tac-Toe (Magic Square XO) game classes.
 *
 * @defgroup XO_num XO Numerical
 * @brief Classes and interfaces for the Numerical Tic-Tac-Toe game.
 */

using namespace std;

/**
 * @class XO_NUM_Board
 * @ingroup XO_num
 * @brief Numerical Tic-Tac-Toe board.
 *
 * @details Players place numbers instead of X/O:
 * - Player 1 uses odd numbers {1,3,5,7,9}
 * - Player 2 uses even numbers {2,4,6,8}
 * The objective is to place numbers such that a row, column, or diagonal sums to 15.
 */
class XO_NUM_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Symbol representing empty cells

public:
    vector<char> odd{'1','3','5','7','9'};   ///< Available odd numbers
    vector<char> even{'2','4','6','8'};      ///< Available even numbers

    /**
     * @brief Construct a new XO_NUM_Board object
     */
    XO_NUM_Board();

    /**
     * @brief Apply or undo a move on the board
     * @param move Pointer to the move being applied
     * @return True if the move was applied successfully, false otherwise
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Check if a player has won
     * @param player Pointer to the player to check
     * @return True if the player has a line summing to 15, false otherwise
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Check if a player has lost (not used)
     * @param player Pointer to the player
     * @return Always false
     */
    bool is_lose(Player<char>* player) { return false; }

    /**
     * @brief Check if the game is a draw
     * @param player Pointer to the player (used for interface consistency)
     * @return True if the board is full and no win, false otherwise
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Check if the game is over
     * @param player Pointer to the player (used for interface consistency)
     * @return True if the game ended (win or draw), false otherwise
     */
    bool game_is_over(Player<char>* player) override;
};

// ============================================================================
// XO_NUM AI
// ============================================================================

/**
 * @class XO_NUM_AI
 * @ingroup AI
 * @brief Random-move AI for Numerical Tic-Tac-Toe
 *
 * @details Chooses a random available cell and number from the player's pool.
 */
class XO_NUM_AI : public AI {
public:
    XO_NUM_AI() = default;

    /**
     * @brief Evaluate the board (not implemented)
     * @param b Pointer to the board
     * @param player Pointer to the player
     * @return Default 0.0f
     */
    float evaluate(Board<char>* b, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax (not implemented)
     * @param aiTurn True if AI's turn
     * @param player Pointer to the player
     * @param alpha Alpha value
     * @param beta Beta value
     * @param blankCell Empty cell symbol
     * @param depth Search depth
     * @return Default 0.0f
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Compute the best move
     * @param player Pointer to the player
     * @param blankCell Symbol for empty cells
     * @param depth Search depth (default 6)
     * @return Pointer to the chosen Move<char>
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

// ============================================================================
// XO_NUM UI
// ============================================================================

/**
 * @class XO_NUM_UI
 * @ingroup XO_num
 * @brief User interface for Numerical Tic-Tac-Toe
 *
 * @details Handles human or AI input, player creation, and move retrieval.
 */
class XO_NUM_UI : public UI<char> {
public:
    XO_NUM_UI();
    ~XO_NUM_UI() {}

    char num; ///< Last number chosen

    /**
     * @brief Create a player instance
     * @param name Reference to the player's name
     * @param symbol Player's symbol
     * @param type Player type (human or AI)
     * @return Pointer to the created Player<char>
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Set up two players
     * @return Array of two Player<char> pointers
     */
    Player<char>** setup_players() override;

    /**
     * @brief Get the next move
     * @param player Pointer to the active player
     * @return Pointer to the generated Move<char>
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // XO_NUM_H