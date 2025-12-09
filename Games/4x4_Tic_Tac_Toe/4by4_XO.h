#pragma once

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/**
 * @class _4by4XO_Move
 * @brief Represents a move in the 4x4 Tic-Tac-Toe game where tokens slide to adjacent cells.
 * 
 * This move type stores both the starting and ending cell for a moving token.
 * It is used because this version of the game does not place new symbols; instead,
 * players MOVE their existing tokens.
 */
class _4by4XO_Move : public Move<char> {
    int from_x; ///< Starting row of the moved token
    int from_y; ///< Starting column of the moved token

public:
    /**
     * @brief Constructs a movement-based XO move.
     * 
     * @param fx Starting row index.
     * @param fy Starting column index.
     * @param tx Ending row index.
     * @param ty Ending column index.
     * @param s  Symbol being moved (X or O).
     */
    _4by4XO_Move(int fx, int fy, int tx, int ty, char s)
        : Move<char>(tx, ty, s), from_x(fx), from_y(fy) {}

    /** @return Starting row of the move. */
    int get_from_x() const { return from_x; }

    /** @return Starting column of the move. */
    int get_from_y() const { return from_y; }
};



/**
 * @class _4by4XO_AI
 * @brief Basic AI for 4x4 sliding Tic-Tac-Toe.
 * 
 * Currently uses random valid moves, but structure is prepared for Minimax.
 */
class _4by4XO_AI : public AI {
public:
    _4by4XO_AI() = default;

    /**
     * @brief Evaluates the board for Minimax (unused currently).
     * @return Fixed value 0.0f.
     */
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    /**
     * @brief Minimax search algorithm (placeholder).
     * @return Always returns 0.0f.
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta,
                  char blankCell, int depth) override { return 0.0f; }

    /**
     * @brief Generates the best move for the AI.
     * 
     * Currently random selection among all legal moves.
     * 
     * @param player     AI player pointer.
     * @param blankCell  Character representing empty cells.
     * @param depth      Search depth (unused).
     * @return Pointer to selected move (caller must manage memory).
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};



/**
 * @class _4by4XO_Board
 * @brief 4x4 game board implementation for sliding XO.
 * 
 * Features:
 * - Players MOVE existing tokens instead of placing new ones.
 * - Win condition: Any 3 consecutive identical symbols (rows, columns, diagonals).
 */
class _4by4XO_Board : public Board<char>
{
public:
    /** @brief Initializes the board with predefined X/O starting positions. */
    _4by4XO_Board();

    /**
     * @brief Applies a move to the board.
     * 
     * Validates:
     * - Boundaries
     * - Correct symbol at source
     * - Destination cell empty
     * - Manhattan distance = 1 (adjacent move)
     * 
     * @param move Pointer to a _4by4XO_Move.
     * @return true if move is valid and applied; false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the given player has 3 consecutive symbols.
     * @return true if the player wins.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Loss condition (currently unused).
     * @return Always false.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Considers the game a draw if too many moves occur without a win.
     * @return true if draw.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if game is over (win or draw).
     */
    bool game_is_over(Player<char>* player) override;
};



/**
 * @class _4by4XO_UI
 * @brief Handles user interaction for the sliding XO game.
 * 
 * Supports human and AI move responses.
 */
class _4by4XO_UI : public UI<char>
{
public:
    /** @brief Displays welcome message for 4x4 XO. */
    _4by4XO_UI();
    
    /**
     * @brief Creates a player object for the game.
     * @param name Player's display name.
     * @param symbol Player symbol (X/O).
     * @param type Human or AI.
     */
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets move input from human or AI.
     * @return Newly allocated Move object.
     */
    Move<char>* get_move(Player<char>* player) override;
};