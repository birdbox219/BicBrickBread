/**
 * @file SUS.h
 * @brief SUS game classes
 *
 * @defgroup SUS_XO SUS XO
 * @brief Classes and interfaces for the 3x3 SUS game.
 */

#ifndef SUS_H
#define SUS_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"
#include <random>

using namespace std;

/**
 * @class SUS_Board
 * @brief Represents the 3x3 SUS game board.
 * @ingroup SUS_XO
 *
 * Tracks occurrences of "S-U-S" in rows, columns, and diagonals.
 * Each detected sequence increases the score of the associated player.
 */
class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';      ///< Symbol used to denote an empty cell.
    int s_score{};                ///< Score for player using symbol 'S'.
    int u_score{};                ///< Score for player using symbol 'U'.

public:
    /**
     * @brief Constructs an empty 3x3 SUS board.
     */
    SUS_Board();

    /**
     * @brief Applies or undoes a move on the board.
     * @param move Pointer to the move to apply
     * @return True if the board was successfully updated
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the given player has won.
     * @param player Pointer to the player
     * @return True if the player has won
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the given player has lost.
     * @param player Pointer to the player
     * @return True if the player has lost
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the game ended in a draw.
     * @param player Pointer to the player
     * @return True if the game is a draw
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Determines whether the game is over.
     * @param player Pointer to the player
     * @return True if the game has ended (win or draw)
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Updates scoring after a move.
     * @param x Row index of the move
     * @param y Column index of the move
     * @param sym Symbol placed
     */
    void score(int x, int y, char sym);
};

/**
 * @class SUS_AI
 * @brief Artificial intelligence for the SUS game using Minimax with alpha-beta pruning.
 * @ingroup AI
 */
class SUS_AI : public AI {
public:
    SUS_AI() = default;

    /**
     * @brief Evaluates the board state for the given player.
     * @param board Pointer to the board
     * @param player Pointer to the player
     * @return Evaluation score
     */
    float evaluate(Board<char>* board, Player<char>* player) override;

    /**
     * @brief Minimax with alpha-beta pruning.
     * @param aiTurn True if it's the AI's turn
     * @param player Pointer to the AI player
     * @param alpha Alpha boundary
     * @param beta Beta boundary
     * @param blankCell Symbol used for empty cells
     * @param depth Maximum recursion depth
     * @return Resulting Minimax score
     */
    float minimax(bool aiTurn, Player<char>* player,
                  float alpha, float beta,
                  char blankCell, int depth) override;

    /**
     * @brief Checks if any empty cells remain.
     * @param board Pointer to the board
     * @return True if there are remaining moves
     */
    bool isMovesLeft(Board<char>* board);

    /**
     * @brief Computes the best move for the player.
     * @param player Pointer to the player
     * @param blankCell Empty cell symbol
     * @param depth Search depth (default: 9)
     * @return Pointer to the chosen move
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 9) override;
};

/**
 * @class SUS_UI
 * @brief User interface for the SUS game.
 * @ingroup SUS_XO
 */
class SUS_UI : public Custom_UI<char> {
public:
    /**
     * @brief Constructs the SUS UI.
     */
    SUS_UI();
    ~SUS_UI() {}

    /**
     * @brief Configures the two players.
     * @return Array of two player pointers
     */
    Player<char>** setup_players() override;

    /**
     * @brief Creates a new player instance.
     * @param name Reference to player name
     * @param symbol Player symbol ('S' or 'U')
     * @param type Player type (human or AI)
     * @return Pointer to created player
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Retrieves the next move.
     * @param player Pointer to the player making the move
     * @return Pointer to the move entered or chosen
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif // SUS_H