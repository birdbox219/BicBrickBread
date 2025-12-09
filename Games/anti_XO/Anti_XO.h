#pragma once
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

/**
 * @class Anti_XO_Board
 * @brief Board implementation for Anti Tic-Tac-Toe (the player who completes 3-in-a-row loses).
 */
class Anti_XO_Board : public Board<char>
{
public:
    /**
     * @brief Constructs a 3×3 Anti-XO board initialized with '.'.
     */
    Anti_XO_Board();

    /**
     * @brief Places a symbol on the board if the position is valid and empty.
     * @param move Pointer to the move containing (x, y, symbol).
     * @return true if the move was applied, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the player loses by forming a 3-in-a-row line.
     * @param player Player to evaluate.
     * @return true if the player has a losing line.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Anti-XO has no winning state. Always returns false.
     */
    bool is_win(Player<char>* player) override { return false; }

    /**
     * @brief Checks if the game is a draw (board full and no losing line).
     * @param player The last player who played.
     * @return true if draw.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game is over (lose or draw).
     * @param player Last played player.
     * @return true if finished.
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class Anti_AI
 * @brief Simple AI for Anti-XO that tries to avoid dangerous positions.
 */
class Anti_AI : public AI {
public:
    Anti_AI() = default;

    /**
     * @brief Evaluation function (not used for Anti-XO).
     * @return Always returns 0.
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax placeholder (not used for Anti-XO).
     * @return Always returns 0.
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Checks that coordinates are within the board bounds.
     */
    bool bounded(int x, int y);

    /**
     * @brief Computes the best move by selecting the least dangerous adjacent cell.
     * @param player The AI player.
     * @param blankCell Representation of empty cells.
     * @param depth Unused.
     * @return Pointer to the selected Move.
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class Anti_XO_UI
 * @brief User interface handler for Anti-XO.
 */
class Anti_XO_UI : public Custom_UI<char>
{
public:
    /**
     * @brief Creates the Anti-XO UI with game identifier and board size.
     */
    Anti_XO_UI();
    ~Anti_XO_UI() {}

    /**
     * @brief Creates a player with given properties.
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Retrieves the player's move (human, computer random, or AI).
     */
    Move<char>* get_move(Player<char>* player) override;
};