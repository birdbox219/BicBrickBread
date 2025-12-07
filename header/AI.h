#pragma once

#include "../../header/BoardGame_Classes.h"

#define INF INT_MAX

/**
 * @class AI
 * @brief AI Interface for all implementations in games.
 *
 * Supports:
 *  - Board evaluation for wins/losses/Draws
 *  - Minimax search with depth limit
 *  - Alpha-beta pruning
 *  - Finding the optimal move
 */
class AI {
public:
    /**
     * @brief Evaluates the board state from the perspective of the AI player.
     *
     * @param board Pointer to the game board
     * @param player AI player
     * @return Evaluation score
     */
    virtual float evaluate(Board<char>* board, Player<char>* player) = 0;

    /**
     * @brief Minimax algorithm with alpha-beta pruning.
     *
     * @param aiTurn True if it's the AI player's turn
     * @param player The AI player
     * @param alpha Current alpha value
     * @param beta Current beta value
     * @param blankCell The character representing an empty cell
     * @param depth Remaining search depth
     * @return Best value from minimax search
     */
    virtual float minimax(bool aiTurn, Player<char>* player,
                        float alpha, float beta, char blankCell, int depth) = 0;

    /**
     * @brief Determines the optimal move using minimax.
     *
     * @param player The AI player
     * @param blankCell The character representing an empty cell
     * @param depth Maximum search depth (default = 6)
     * @return Pointer to the chosen Move
     */
    virtual Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) = 0;

    virtual ~AI() = default;
};