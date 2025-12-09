#pragma once

#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"

/*
 * zengy:
 *   - An unordered_set of vectors of coordinate pairs.
 *   - (Not used in current code, likely from older logic)
 */
typedef std::unordered_set<std::vector<std::pair<int,int>>> zengy;

/*
 * dia_XO_Board:
 *   - Represents a 7×7 diamond-shaped tic-tac-toe board.
 *   - Some cells are invalid ('z'), some are usable ('.').
 *   - A player wins if they form two lines:
 *         - One of length >= 3
 *         - One of length >= 4
 *     in different directions.
 */
class dia_XO_Board: public Board<char>
{
private:
    // 8 directions for line scanning
    int dx[8] = { -1, -1,  0, 1, 1,  1,  0, -1 };
    int dy[8] = {  0,  1,  1, 1, 0, -1, -1, -1 };

    char invalid = 'z';   // Invalid (blocked) cell
    char free    = '.';   // Empty playable cell

    // Returns true if (x,y) is on board and free for scanning
    bool bounded(int x,int y);

public:
    // Creates the 7×7 board with the diamond shape
    dia_XO_Board();

    // Attempts to place a symbol on the board
    bool update_board(Move<char>* move) override;

    // Not used — this game has no "loss" condition
    bool is_lose(Player<char>* player) override;

    // Checks the diamond XO win condition
    bool is_win(Player<char>* player) override;

    // True when no free cells remain
    bool is_draw(Player<char>* player) override;

    // Game ends on win or draw
    bool game_is_over(Player<char>* player) override;
};

/*
 * dia_XO_AI:
 *   - Very simple AI.
 *   - Picks a random empty cell.
 */
class dia_XO_AI : public AI {
public:
    dia_XO_AI() = default;

    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    // Picks a random free cell
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

/*
 * dia_XO_UI:
 *   - Connects input/output for diamond tic-tac-toe.
 *   - Allows human or AI players.
 */
class dia_XO_UI : public UI<char>
{
public:
    dia_XO_UI();
    ~dia_XO_UI() {}

    // Create player with name, symbol, and type
    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    // Read human move OR use AI move
    Move<char>* get_move(Player<char>* player) override;
};