#pragma once
#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"

class Anti_XO_Board : public Board<char>
{
public:
    Anti_XO_Board();

    bool update_board(Move<char>* move) override;

    bool is_lose(Player<char>* player) override;

    bool is_win(Player<char>* player) override { return false; }

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};

class Anti_AI : public AI {
public:
    Anti_AI() = default;

    float evaluate(Board<char>* board, Player<char>* player) override { return 0.0f; }

    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.0f; }

    bool bounded(int x,int y);

    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

class Anti_XO_UI : public Custom_UI<char>
{
public:
    Anti_XO_UI();
    ~Anti_XO_UI() {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;
};


