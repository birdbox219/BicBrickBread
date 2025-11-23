#ifndef XO_inf_H
#define X_inf_H
#include <queue>
#include "../../header/BoardGame_Classes.h"
using namespace std;

class XO_inf_Board : public Board<char> {
private:
    char blank_symbol = '.'; ///< Character used to represent an empty cell on the board.
    queue<pair<int, int>> history1;
    queue<pair<int, int>> history2;
public:

    XO_inf_Board();
    bool update_board(Move<char>* move);
    bool is_win(Player<char>* player);
    bool is_lose(Player<char>*) { return false; };
    bool is_draw(Player<char>* player);
    bool game_is_over(Player<char>* player);
};



class XO_inf_UI : public UI<char> {
public:

    XO_inf_UI();

    ~XO_inf_UI() {};


    Player<char>* create_player(string& name, char symbol, PlayerType type);


    virtual Move<char>* get_move(Player<char>* player);
};

#endif 

