/**
 * @file Ultimate.h
 * @brief Ultimate Tic-Tac-Toe game classes.
 *
 * @defgroup Ultimate_XO Ultimate XO
 * @brief Classes and interfaces for the Ultimate Tic-Tac-Toe game.
 */

#ifndef Ultimate_H
#define Ultimate_H

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

using namespace std;

/**
 * @class Ultimate_Board
 * @ingroup Ultimate_XO
 * @brief Represents a 9x9 Ultimate Tic-Tac-Toe board.
 *
 * The board consists of nine 3x3 small boards arranged in a 3x3 grid.
 * Each small board is evaluated independently. A player wins by
 * taking three small boards in a row on the large board.
 */
class Ultimate_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character representing an empty cell.
    int small_board = 0;     ///< Index of the active small board.
    bool full = true;        ///< Indicates whether the active small board is full.
    bool win = false;        ///< Indicates whether the active small board has a winner.
    vector<vector<char>> large_board = vector<vector<char>>(3, vector<char>(3, '.')); ///< Tracks results of completed small boards.

public:
    /**
     * @brief Constructs an empty Ultimate Tic-Tac-Toe board.
     */
    Ultimate_Board();

    /**
     * @brief Apply or undo a move on the board.
     * @param move Pointer to the move being executed.
     * @return True if the move was successfully applied or undone.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the player has won the game.
     * @param player Pointer to the player to check.
     * @return True if the player has won.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the game ended in a draw.
     * @param player Pointer to the active player.
     * @return True if the game is a draw.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Lose detection is not used.
     * @param player Pointer to the player.
     * @return Always returns false.
     */
    bool is_lose(Player<char>* player) { return false; }

    /**
     * @brief Determines whether the game has ended.
     * @param player Pointer to the active player.
     * @return True if the game is finished.
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Evaluates the small board corresponding to the last move.
     * @param x Row of the last move.
     * @param y Column of the last move.
     * @param sym Symbol placed on the board.
     */
    void small_board_check(int x, int y, char sym);

    /**
     * @brief Checks if all small boards are completed.
     * @return True if every small board is won or drawn.
     */
    bool all_small_boards_done();
};

/**
 * @class Ultimate_AI
 * @ingroup AI
 * @brief AI controller for Ultimate Tic-Tac-Toe (random move selection).
 */
class Ultimate_AI : public AI {
public:
    Ultimate_AI() = default;

    /**
     * @brief Evaluates the board state (placeholder, returns 0.0).
     * @param board Pointer to the board.
     * @param player Pointer to the player.
     * @return Always returns 0.0.
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax search (not implemented, returns 0.0).
     * @param aiTurn True if it is the AI's turn.
     * @param player Pointer to the player.
     * @param alpha Alpha pruning value.
     * @param beta Beta pruning value.
     * @param blankCell Symbol representing empty cells.
     * @param depth Search depth (unused).
     * @return Always returns 0.0.
     */
    float minimax(bool aiTurn,
                  Player<char>* player,
                  float alpha,
                  float beta,
                  char blankCell,
                  int depth) override { return 0.0f; }

    /**
     * @brief Selects a random valid move for the AI.
     * @param player Pointer to the AI player.
     * @param blankCell Symbol representing empty cells.
     * @param depth Depth parameter (unused).
     * @return Pointer to the chosen Move<char>.
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class Ultimate_UI
 * @ingroup Ultimate_XO
 * @brief User interface for Ultimate Tic-Tac-Toe.
 *
 * Handles player creation, move input, and board display.
 */
class Ultimate_UI : public UI<char> {
public:
    Ultimate_UI();
    ~Ultimate_UI() {}

    /**
     * @brief Create a player instance.
     * @param name Reference to the player's name.
     * @param symbol Symbol assigned to the player.
     * @param type Player type (human or AI).
     * @return Pointer to the created player.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Retrieve the next move from a player.
     * @param player Pointer to the player making the move.
     * @return Pointer to the Move<char> chosen by the player.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Display the 9x9 board with separators for small boards.
     * @param matrix Reference to the 2D board matrix.
     */
    void display_board_matrix(const vector<vector<char>>& matrix) const override;
};

#endif // Ultimate_H