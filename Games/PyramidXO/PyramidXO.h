/**
 * @file PyramidXO.h
 * @brief Pyramid XO game classes
 *
 * @defgroup PyramidXO_XO Pyramid XO
 * @brief Classes and interfaces for the Pyramid Tic-Tac-Toe variant.
 */

#pragma once

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/**
 * @class PyramidXO_Board
 * @brief Represents the game board for the Pyramid XO variant.
 * @ingroup PyramidXO_XO
 *
 * Only certain cells are playable, forming a pyramid shape:
 * Row 0:        (0,2)
 * Row 1:     (1,1)(1,2)(1,3)
 * Row 2: (2,0)(2,1)(2,2)(2,3)(2,4)
 */
class PyramidXO_Board : public Board<char>
{
public:
    /**
     * @brief Constructs and initializes the pyramid board.
     */
    PyramidXO_Board();

    /**
     * @brief Attempts to place a symbol on the board.
     * @param move Pointer to the move to apply
     * @return True if the move is legal and successfully executed; false otherwise
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the player has lost (not used in this game).
     * @param player Pointer to the player
     * @return False always
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks if the player has won.
     * @param player Pointer to the player to check
     * @return True if the player has a winning line; false otherwise
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the game ended in a draw.
     * @param player Pointer to the player (used for consistency)
     * @return True if no valid moves remain and no player has won; false otherwise
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks whether the game is finished (win or draw).
     * @param player Pointer to the player (used for consistency)
     * @return True if the game is over; false otherwise
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class PyramidXO_AI
 * @brief Simple AI choosing a random valid empty pyramid cell.
 * @ingroup AI
 */
class PyramidXO_AI : public AI {
public:
    PyramidXO_AI() = default;

    /**
     * @brief Evaluation function (not implemented for this game)
     * @param board Pointer to the game board
     * @param player Pointer to the player
     * @return Always returns 0.0f
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax search (not used, returns default value)
     * @param aiTurn True if AI's turn; false otherwise
     * @param player Pointer to the player
     * @param alpha Alpha value for pruning
     * @param beta Beta value for pruning
     * @param blankCell Symbol representing empty cells
     * @param depth Depth of search
     * @return Always returns 0.0f
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Generates a random valid move within the pyramid.
     * @param player Pointer to the player making the move
     * @param blankCell Symbol representing empty cells
     * @param depth Depth of search (default 6, unused here)
     * @return Pointer to the chosen Move<char>
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class PyramidXO_UI
 * @brief Handles user interaction and drawing of the pyramid board.
 * @ingroup PyramidXO_XO
 */
class PyramidXO_UI : public UI<char>
{
public:
    /**
     * @brief Constructs UI with game title.
     */
    PyramidXO_UI();

    ~PyramidXO_UI() {}

    /**
     * @brief Creates a player object.
     * @param name Reference to the player name string
     * @param symbol Symbol assigned to the player
     * @param type Player type (HUMAN or AI)
     * @return Pointer to the created Player<char>
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Reads human move or requests AI move.
     * @param player Pointer to the player making the move
     * @return Pointer to the Move<char> object chosen
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Draws the pyramid-shaped board with proper spacing.
     * @param matrix The full 2D board matrix (3x5)
     */
    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
};