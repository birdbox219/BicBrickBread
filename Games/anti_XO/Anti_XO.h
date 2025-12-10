/**
 * @file Anti_XO.h
 * @brief Anti Tic-Tac-Toe variant where forming 3-in-a-row causes a loss.
 *
 * @defgroup AntiXO Anti Tic-Tac-Toe
 * @brief Classes implementing Anti-XO board, AI, and UI.
 *
 * Game rules:
 *  - Board size: 3x3
 *  - Players alternate placing symbols (X or O)
 *  - Completing 3-in-a-row results in a loss
 *  - Draw occurs if the board is full without a losing line
 */

#pragma once
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

/**
 * @class Anti_XO_Board
 * @brief Board implementation for Anti Tic-Tac-Toe.
 *
 * @ingroup AntiXO
 *
 * Responsibilities:
 *  - Manage board state
 *  - Apply player moves
 *  - Detect losing and draw conditions
 */
class Anti_XO_Board : public Board<char>
{
public:
    /**
     * @brief Constructs a 3×3 Anti-XO board initialized with empty cells ('.').
     */
    Anti_XO_Board();

    /**
     * @brief Places a symbol on the board if the position is valid and empty.
     * @param move Pointer to the move containing row, column, and symbol
     * @return True if the move was applied, false otherwise
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the player loses by forming a 3-in-a-row line.
     * @param player Pointer to the player to evaluate
     * @return True if the player has a losing line
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Anti-XO has no winning state.
     * @param player Pointer to player
     * @return Always returns false
     */
    bool is_win(Player<char>* player) override { return false; }

    /**
     * @brief Checks if the game is a draw (board full and no losing line).
     * @param player Pointer to the last player who played
     * @return True if draw
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game is over (loss or draw).
     * @param player Pointer to last played player
     * @return True if game finished
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class Anti_AI
 * @brief Simple AI for Anti-XO that avoids dangerous positions.
 *
 * @ingroup AI
 *
 * Responsibilities:
 *  - Select safe moves
 *  - Avoid forming 3-in-a-row
 */
class Anti_AI : public AI {
public:
    Anti_AI() = default;

    /**
     * @brief Evaluation function (not used for Anti-XO).
     * @param board Pointer to board
     * @param player Pointer to AI player
     * @return Always returns 0
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax placeholder (not used for Anti-XO).
     * @param aiTurn True if AI turn
     * @param player Pointer to AI player
     * @param alpha Alpha value
     * @param beta Beta value
     * @param blankCell Symbol for empty cells
     * @param depth Search depth
     * @return Always returns 0
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Checks that coordinates are within board bounds.
     * @param x Row index
     * @param y Column index
     * @return True if (x,y) is valid
     */
    bool bounded(int x, int y);

    /**
     * @brief Computes the best move by selecting the least dangerous adjacent cell.
     * @param player Pointer to AI player
     * @param blankCell Symbol representing empty cells
     * @param depth Unused
     * @return Pointer to the selected Move<char>
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class Anti_XO_UI
 * @brief User interface for Anti Tic-Tac-Toe.
 *
 * @ingroup AntiXO
 *
 * Responsibilities:
 *  - Handle human and AI player input
 *  - Connect with board and AI logic
 */
class Anti_XO_UI : public Custom_UI<char>
{
public:
    /**
     * @brief Constructs the Anti-XO UI.
     */
    Anti_XO_UI();
    ~Anti_XO_UI() {}

    /**
     * @brief Creates a player instance.
     * @param name Player name
     * @param symbol Player symbol
     * @param type Player type (human or AI)
     * @return Pointer to the created Player<char>
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Retrieves the next move from human or AI.
     * @param player Pointer to current player
     * @return Pointer to Move<char>
     */
    Move<char>* get_move(Player<char>* player) override;
};