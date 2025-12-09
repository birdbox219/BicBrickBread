#pragma once

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/**
 * @class PyramidXO_Board
 * @brief Represents the game board for the Pyramid XO variant.
 *
 * The board has size 3x5, but valid playable positions follow a pyramid shape:
 *
 * Row 0:        (0,2)
 * Row 1:     (1,1)(1,2)(1,3)
 * Row 2: (2,0)(2,1)(2,2)(2,3)(2,4)
 *
 * Only these coordinates are allowed for moves.
 */
class PyramidXO_Board : public Board<char>
{
public:
    /// @brief Constructs and initializes the pyramid board.
    PyramidXO_Board();

    /// @brief Attempts to place a symbol on the board.
    /// @return True if move is legal and executed.
    bool update_board(Move<char>* move) override;

    /// @brief Checks if the player has lost (not used in this game).
    bool is_lose(Player<char>* player) override;

    /// @brief Checks if the player has won.
    /// Winning lines include horizontal, vertical, and diagonal triples.
    bool is_win(Player<char>* player) override;

    /// @brief Checks if the game ended in a draw (all valid cells filled).
    bool is_draw(Player<char>* player) override;

    /// @brief Checks whether the game is finished (win or draw).
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class PyramidXO_AI
 * @brief Simple AI choosing a random valid empty pyramid cell.
 */
class PyramidXO_AI : public AI {
public:
    PyramidXO_AI() = default;

    /// @brief Evaluation function (not implemented for this game).
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /// @brief Minimax search (not used, returns default value).
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /// @brief Generates a random valid move within the pyramid.
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class PyramidXO_UI
 * @brief Handles user interaction and drawing of the pyramid board.
 */
class PyramidXO_UI : public UI<char>
{
public:
    /// @brief Constructs UI with game title.
    PyramidXO_UI();
    ~PyramidXO_UI() {}

    /// @brief Creates a player object.
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /// @brief Reads human move or requests AI move.
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Draws the pyramid-shaped board with proper spacing.
     * @param matrix The full 2D board matrix (3x5).
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
};
