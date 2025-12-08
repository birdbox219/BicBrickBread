#ifndef XO_inf_H
#define X_inf_H
#include <deque>
#include <stack>
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

using namespace std;

struct Vertex { int x; int y;};

class XO_inf_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    deque<Vertex> history;
    stack<Vertex> undoHistory;
    int cnt = 0;
public:

    XO_inf_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};

class XO_inf_AI : public AI {
public:
    /** @brief Default constructor */
    XO_inf_AI() {}

    /**
     * @brief Evaluate the board from the AI perspective.
     * @param b Pointer to the board to evaluate.
     * @param player Pointer to the AI player.
     * @return Numeric score of the board state.
     */
    float evaluate(Board<char>* b, Player<char>* player) override;

    /**
     * @brief Minimax algorithm with alpha-beta pruning.
     * @param aiTurn True if it is AI's turn; false for opponent.
     * @param player Pointer to the AI player.
     * @param alpha Alpha value for pruning.
     * @param beta Beta value for pruning.
     * @param blankCell Symbol used for empty cells.
     * @param depth Maximum search depth.
     * @return Best minimax score from the current position.
     */
    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override;

    /**
     * @brief Determine the best move for AI.
     * @param player Pointer to the AI player.
     * @param blankCell Symbol used for empty cells.
     * @param depth Maximum search depth (default 6).
     * @return Pointer to a Move<char> representing the optimal move.
     */
    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

class XO_inf_UI : public Custom_UI<char> {
public:

    XO_inf_UI();

    ~XO_inf_UI() {};


    Player<char>* create_player(string& name, char symbol, PlayerType type);


    virtual Move<char>* get_move(Player<char>* player);
};

#endif 