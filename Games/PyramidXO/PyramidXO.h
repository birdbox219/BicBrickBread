#pragma once

#include "../../header/BoardGame_Classes.h"
<<<<<<< HEAD
#include <random>
=======
#include "../../header/AI.h"
>>>>>>> e7a0258e6f01ed74b31d15248568283261fb25d0

class PyramidXO_Board : public Board<char>
{
public:
    PyramidXO_Board();

    bool update_board(Move<char>* move) override;

    bool is_lose(Player<char>* player) override;

    bool is_win(Player<char>* player) override;

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};

class PyramidXO_AI : public AI {
public:
    PyramidXO_AI() = default;

<<<<<<< HEAD
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.f; }

    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.f; }
=======
    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }
>>>>>>> e7a0258e6f01ed74b31d15248568283261fb25d0

    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

class PyramidXO_UI : public UI<char>
{
public:
    PyramidXO_UI();
    ~PyramidXO_UI() {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;

    void display_board_matrix(const std::vector<std::vector<char>>& matrix) const override;
};