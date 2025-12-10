/**
 * @file 4by4XO.h
 * @brief 4x4 sliding Tic-Tac-Toe variant where players move existing tokens.
 *
 * @defgroup SlidingXO 4x4 Sliding XO
 * @brief Classes implementing the 4x4 sliding Tic-Tac-Toe variant.
 *
 * Rules:
 *  - Board size: 4x4
 *  - Players MOVE existing X or O tokens to adjacent empty cells
 *  - Win: 3 consecutive identical symbols (row, column, diagonal)
 *  - Draw: too many moves without a win
 */

#pragma once
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/**
 * @class _4by4XO_Move
 * @brief Represents a move in the 4x4 sliding Tic-Tac-Toe game.
 *
 * @ingroup SlidingXO
 *
 * Stores both starting and ending coordinates since players slide tokens instead of placing new symbols.
 */
class _4by4XO_Move : public Move<char> {
    int from_x; ///< Starting row
    int from_y; ///< Starting column

public:
    /**
     * @brief Constructs a sliding XO move.
     * @param fx Starting row
     * @param fy Starting column
     * @param tx Ending row
     * @param ty Ending column
     * @param s Symbol being moved (X or O)
     */
    _4by4XO_Move(int fx, int fy, int tx, int ty, char s)
        : Move<char>(tx, ty, s), from_x(fx), from_y(fy) {}

    /** @return Starting row of the move */
    int get_from_x() const { return from_x; }

    /** @return Starting column of the move */
    int get_from_y() const { return from_y; }
};

/**
 * @class _4by4XO_AI
 * @brief Simple AI for sliding 4x4 Tic-Tac-Toe.
 *
 * @ingroup AI
 *
 * Responsibilities:
 *  - Generate valid moves
 *  - Currently random; placeholder for Minimax
 */
class _4by4XO_AI : public AI {
public:
    _4by4XO_AI() = default;

    /**
     * @brief Evaluates board for Minimax (unused currently).
     * @param board Pointer to board
     * @param player Pointer to AI player
     * @return Always 0.0f
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Placeholder Minimax algorithm.
     * @param aiTurn True if AI turn
     * @param player Pointer to AI player
     * @param alpha Alpha value
     * @param beta Beta value
     * @param blankCell Empty cell symbol
     * @param depth Search depth
     * @return Always 0.0f
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta,
                  char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Computes the best move for AI.
     * @param player Pointer to AI player
     * @param blankCell Empty cell symbol
     * @param depth Maximum search depth (unused)
     * @return Pointer to selected Move<char> (caller responsible for memory)
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/**
 * @class _4by4XO_Board
 * @brief 4x4 board for sliding Tic-Tac-Toe.
 *
 * @ingroup SlidingXO
 *
 * Responsibilities:
 *  - Track board state
 *  - Apply sliding moves
 *  - Check win/draw/game-over conditions
 */
class _4by4XO_Board : public Board<char>
{
public:
    /** @brief Initializes the board with starting X/O positions */
    _4by4XO_Board();

    /**
     * @brief Applies a sliding move.
     * @param move Pointer to _4by4XO_Move
     * @return True if move is valid and applied
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if a player has 3 consecutive symbols.
     * @param player Pointer to player
     * @return True if the player wins
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Loss condition (unused).
     * @param player Pointer to player
     * @return Always false
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks for draw condition.
     * @param player Pointer to last played player
     * @return True if draw
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if the game is over (win or draw).
     * @param player Pointer to last played player
     * @return True if game finished
     */
    bool game_is_over(Player<char>* player) override;
};

/**
 * @class _4by4XO_UI
 * @brief Handles user interaction for sliding 4x4 XO.
 *
 * @ingroup SlidingXO
 *
 * Responsibilities:
 *  - Receive human or AI moves
 *  - Display board
 */
class _4by4XO_UI : public UI<char>
{
public:
    /** @brief Constructs 4x4 sliding XO UI */
    _4by4XO_UI();

    /**
     * @brief Creates a player instance.
     * @param name Player display name
     * @param symbol Player symbol (X/O)
     * @param type Human or AI
     * @return Pointer to created Player<char>
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets a move from human or AI.
     * @param player Pointer to current player
     * @return Pointer to Move<char>
     */
    Move<char>* get_move(Player<char>* player) override;
};