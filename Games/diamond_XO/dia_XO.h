#pragma once

#include <vector>
#include <utility>
#include <unordered_set>
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include <random>


typedef std::unordered_set<std::vector<std::pair<int,int>>> zengy;
class dia_XO_Board: public Board<char>
{

private:
    int dx[8] = { -1, -1,  0, 1, 1,  1,  0, -1 };
    int dy[8] = {  0,  1,  1, 1, 0, -1, -1, -1 };
    char  invalid = 'z';
    char  free = '.';
    bool bounded(int x,int y);
public:
    dia_XO_Board();

    bool update_board(Move<char>* move) override;

    bool is_lose(Player<char>* player) override;

    bool is_win(Player<char>* player) override;

    bool is_draw(Player<char>* player) override;

    bool game_is_over(Player<char>* player) override;
};

class dia_XO_AI : public AI {
public:
    dia_XO_AI() = default;

    float evaluate(Board<char>* board, Player<char>* player) override { return 0.f; }

    float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override { return 0.f; }

    Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 6) override;
};

class dia_XO_UI : public UI<char>
{
public:
    dia_XO_UI();
    ~dia_XO_UI() {}

    Player<char>* create_player(std::string& name, char symbol, PlayerType type) override;

    Move<char>* get_move(Player<char>* player) override;
};
