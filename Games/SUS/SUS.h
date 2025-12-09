#ifndef SUS
#define SUS

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"
#include <random>
using namespace std;

/**
 * @class SUS_Board
 * @brief Represents the 3x3 SUS game board.
 *
 * The game counts how many "S-U-S" patterns appear in rows, columns, and diagonals.
 * Each pattern increments the corresponding player's score.
 */
class SUS_Board: public Board<char> {
private:
    char blank_symbol = '.'; ///< Empty cell symbol
    int s_score{};           ///< Player 'S' score
    int u_score{};           ///< Player 'U' score

public:
    /// @brief Constructs an empty 3x3 SUS board
    SUS_Board();

    /// @brief Applies or undoes a move on the board
    bool update_board(Move<char>* move);

    /// @brief Checks if the given player has won
    bool is_win(Player<char>* player) override;

    /// @brief Checks if the given player has lost
    bool is_lose(Player<char>* player) override;

    /// @brief Checks if the game ended in a draw
    bool is_draw(Player<char>* player) override;

    /// @brief Returns true if the game is over (win or draw)
    bool game_is_over(Player<char>* player) override;

    /**
     * @brief Updates the scores based on the last move
     * @param x Row index of the last move
     * @param y Column index of the last move
     * @param sym Symbol placed
     */
    void score(int x, int y, char sym);
};

/**
 * @class SUS_AI
 * @brief Implements AI for the SUS game using Minimax with alpha-beta pruning
 */
class SUS_AI : public AI {
public:
    SUS_AI() = default;

    /// @brief Evaluates the board state for the given player
    float evaluate(Board<char>* board, Player<char>* player) override;

    /// @brief Minimax algorithm with alpha-beta pruning
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override;

    /// @brief Returns true if there are empty moves left
    bool isMovesLeft(Board<char>* board);

    /// @brief Computes the best move for the player
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 9) override;
};

/**
 * @class SUS_UI
 * @brief Handles user interaction for SUS game
 */
class SUS_UI: public Custom_UI<char> {
public:
    SUS_UI();
    ~SUS_UI() {}

    /// @brief Sets up two players
    Player<char>** setup_players() override;

    /// @brief Creates a player instance
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /// @brief Gets the next move from a human or AI
    Move<char>* get_move(Player<char>* player) override;
};

#endif //SUS