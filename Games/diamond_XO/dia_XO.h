/**
 * @file dia_XO.h
 * @brief Diamond-shaped 7x7 Tic-Tac-Toe variant with AI and UI support.
 *
 * @defgroup DiamondXO Diamond Tic-Tac-Toe
 * @brief Classes for the 7x7 diamond-shaped Tic-Tac-Toe game.
 *
 * Game rules:
 *  - The board is 7x7 with a diamond-shaped playable area.
 *  - Invalid cells are marked 'z', empty cells '.'.
 *  - A player wins by forming two lines in different directions:
 *        - One line of length >= 3
 *        - One line of length >= 4
 */

#pragma once

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/**
 * @typedef zengy
 * @brief Unordered set of vector of coordinate pairs (not currently used in logic).
 */
typedef std::unordered_set<std::vector<std::pair<int,int>>> zengy;

/**
 * @class dia_XO_Board
 * @brief Represents the 7x7 diamond-shaped Tic-Tac-Toe board.
 *
 * @ingroup DiamondXO
 *
 * Responsibilities:
 *  - Manage board state
 *  - Apply player moves
 *  - Detect win/draw/game-over conditions
 */
class dia_XO_Board: public Board<char>
{
private:
    int dx[8] = { -1, -1,  0, 1, 1,  1,  0, -1 }; ///< X-direction offsets for line scanning
    int dy[8] = {  0,  1,  1, 1, 0, -1, -1, -1 }; ///< Y-direction offsets for line scanning

    char invalid = 'z';   ///< Invalid cell symbol
    char free    = '.';   ///< Empty playable cell

    /**
     * @brief Checks whether a coordinate is inside the board and free.
     * @param x Row index
     * @param y Column index
     * @return True if (x,y) is on board and free
     */
    bool bounded(int x,int y);

public:
    /** @brief Constructs the 7x7 diamond-shaped board. */
    dia_XO_Board();

    /**
     * @brief Attempts to place a symbol on the board.
     * @param move Pointer to Move object with row, column, and symbol
     * @return True if the move was applied successfully
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Losing condition not used for this variant.
     * @param player Pointer to player
     * @return Always false
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks whether a player has won based on diamond rules.
     * @param player Pointer to player
     * @return True if the player satisfies win conditions
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if the board is full and no more moves are possible.
     * @param player Pointer to player (not used)
     * @return True if draw
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game is over (win or draw).
     * @param player Pointer to player
     * @return True if the game ended
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class dia_XO_AI
 * @brief Very simple AI for diamond Tic-Tac-Toe.
 *
 * @ingroup AI
 *
 * Responsibilities:
 *  - Picks a random empty cell
 */
class dia_XO_AI : public AI {
public:
    dia_XO_AI() = default;

    /**
     * @brief Evaluates board state (dummy, always 0).
     * @param board Pointer to board
     * @param player Pointer to AI player
     * @return 0.0
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax (dummy, always 0).
     * @param aiTurn True if AI's turn
     * @param player Pointer to AI player
     * @param alpha Alpha value
     * @param beta Beta value
     * @param blankCell Empty cell symbol
     * @param depth Search depth
     * @return 0.0
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Picks a random free cell for AI.
     * @param player Pointer to AI player
     * @param blankCell Symbol for empty cell
     * @param depth Max search depth (unused)
     * @return Pointer to chosen Move<char>
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class dia_XO_UI
 * @brief User interface for diamond-shaped Tic-Tac-Toe.
 *
 * @ingroup DiamondXO
 *
 * Responsibilities:
 *  - Handle human and AI player input
 *  - Connect with board and AI
 */
class dia_XO_UI : public UI<char>
{
public:
    /** @brief Constructs the UI handler. */
    dia_XO_UI();
    ~dia_XO_UI() {}

    /**
     * @brief Creates a player instance.
     * @param name Player name
     * @param symbol Player symbol
     * @param type Player type (human or AI)
     * @return Pointer to the created Player<char>
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Obtains the next move from human or AI.
     * @param player Pointer to current player
     * @return Pointer to Move<char>
     */
    Move<char>* get_move(Player<char>* player) override;
};