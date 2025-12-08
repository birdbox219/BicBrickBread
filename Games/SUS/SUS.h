#ifndef SUS
#define SUS
#include "../../header/BoardGame_Classes.h"
#include "../../header/AI.h"
#include "../../header/Custom_UI.h"
#include <random>
using namespace std;

class SUS_Board: public Board<char>{
private:
   char blank_symbol='.';

   int s_score{};
   int u_score{};

public:
   SUS_Board();
   bool update_board(Move<char>* move);
   bool is_win(Player<char>* player) override;
   bool is_lose(Player<char>* player)override;
   bool is_draw(Player<char>* player) override;
   bool game_is_over(Player<char>* player) override;
   void score(int x,int y, char sym);
};

class SUS_AI : public AI {
public:
   SUS_AI() = default;

   float evaluate(Board<char>* board, Player<char>* player) override;

   float minimax(bool aiTurn, Player<char>* player, float alpha, float beta, char blankCell, int depth) override;

   bool isMovesLeft(Board<char>* board);

   Move<char>* bestMove(Player<char>* player, char blankCell, int depth = 9) override;
};

class SUS_UI: public Custom_UI<char> {
public:
   SUS_UI();

   ~SUS_UI() {}

   Player<char>** setup_players() override;

   Player<char>* create_player(string& name, char symbol, PlayerType type) override;

   Move<char>* get_move(Player<char>* player) override;
};

#endif //SUS
