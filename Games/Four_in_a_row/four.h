#ifndef FOUR_H
#define FOUR_H
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"
#include <random>
using namespace std;

/**
 * @class FOUR_Board
 * @brief Represents a 6x7 Connect-4 board.
 *
 * Manages board state, move placement, win detection, and draw logic
 * for the Connect-Four game variant.
 */
class FOUR_Board : public Board<char> {
private:
    char blank_symbol = '.';     ///< Character representing an empty cell.

public:
    int last_row[7]{};           ///< Tracks number of filled cells in each column.

    FOUR_Board();

    /**
     * @brief Updates the board with the given move.
     * @param move Pointer to a Move object containing (x,y,symbol).
     * @return True if the move is valid and applied.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the given player has won.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Connect-4 does not use losing detection.
     */
    bool is_lose(Player<char>* player) { return false; }

    /**
     * @brief Checks if the game resulted in a draw.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks whether the game ended (win or draw).
     */
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Returns the actual last filled row index in a column.
     */
    int get_last_row(int y) { return 5 - last_row[y]; }

    /**
     * @brief Increases the filled count of a column and returns new row.
     */
    int increase_last_row(int y) { return 5 - last_row[y]++; }

    /**
     * @brief Decreases the filled count of a column and returns new row.
     */
    int decrease_last_row(int y) { return 5 - last_row[y]--; }
};

/**
 * @class Four_AI
 * @brief Implements a minimax-based AI for Connect-Four.
 */
class Four_AI : public AI {
public:
    Four_AI() = default;

    /**
     * @brief Evaluates current board state.
     * @return Score from AI perspective.
     */
    float evaluate(Board<char>* board, Player<char>* player) override;

    /**
     * @brief Minimax with alpha-beta pruning.
     */
    float minimax(bool aiTurn, Player<char>* player,
                  float alpha, float beta,
                  char blankCell, int depth) override;

    /**
     * @brief Checks if any moves are still available.
     */
    bool isMovesLeft(Board<char>* board);

    /**
     * @brief Finds next open row in the given column.
     */
    int get_next_available_row(Board<char>* board, int col);

    /**
     * @brief Computes the best possible move for the AI.
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class FOUR_UI
 * @brief Handles UI and player interaction for Connect-Four.
 */
class FOUR_UI : public Custom_UI<char> {
private:
    char AI, OOP;

public:
    FOUR_UI();
    ~FOUR_UI() {}

    /**
     * @brief Creates a player instance.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets the move from a human, computer, or AI player.
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif