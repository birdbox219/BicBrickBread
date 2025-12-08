
#include <iostream>
#include <iomanip>
#include <cctype> // for toupper()
#include <climits> //for INT_MAX INT_MIN
#include "four.h"

using namespace std;

//============= Board Implementation  =========

FOUR_Board::FOUR_Board() : Board(6, 7)
{
    // Initialize all cells with blank_symbol
    for (auto &row : board)
        for (auto &cell : row)
            cell = blank_symbol;
}

bool FOUR_Board::update_board(Move<char>* move)
{
    int y = move->get_y();
    char mark = move->get_symbol();

    int x = 5 - last_row[y];

    if (x < 0 || x >= rows || y < 0 || y >= columns)
        return false;

    if (mark == 0)
    {
        last_row[y]--;
        x = 5 - last_row[y];
        board[x][y] = blank_symbol;
        n_moves--;
        return true;
    }

    if (board[x][y] == blank_symbol)
    {
        board[x][y] = toupper(mark);
        last_row[y]++;
        n_moves++;
        return true;
    }

    return false;
}

bool FOUR_Board::is_win(Player<char> *player)
{

    const char sym = player->get_symbol();

    auto all_equal = [&](char a, char b, char c,char d)
    {
        return a == b && b == c && c==d&& a != blank_symbol;
    };

    // Check rows and columns
    for (int i=0; i < rows; ++i) {
        for (int j = 0; j < columns ; ++j) {  
            if (j+3<7&&all_equal(board[i][j], board[i][j+1], board[i][j+2], board[i][j+3]))
                return true;
        }
    }

    for (int i = 0; i <rows ; ++i) { 
        for (int j = 0; j < columns; ++j) {
            if (i+3<6&&all_equal(board[i][j], board[i+1][j], board[i+2][j], board[i+3][j]))
                return true;
        }
    }
    // Check diagonals
    for (int i=0; i<rows;++i){
        for (int j=0; j<columns;++j){
            if (i-3>=0&&j+3<7 &&all_equal(board[i][j], board[i-1][j+1], board[i-2][j+2],board[i-3][j+3]) && board[i][j] == sym)
            return true;
            if (i+3<6 && j+3<7 &&all_equal(board[i][j], board[i+1][j+1], board[i+2][j+2],board[i+3][j+3]) && board[i][j] == sym)
                return true;
        }
    }    

    return false;
}

bool FOUR_Board::is_draw(Player<char> *player)
{
    return (n_moves == 42 && !is_win(player));
}

bool FOUR_Board::game_is_over(Player<char> *player)
{
    return is_win(player) || is_draw(player);
}

//====================================UI Implementation ==============
FOUR_UI::FOUR_UI() : Custom_UI<char>("Four In A Rwo", 3) {}

Player<char> *FOUR_UI::create_player(string &name, char symbol, PlayerType type)
{
    return new Player<char>(name, symbol, type);
}

Move<char> *FOUR_UI::get_move(Player<char> *player)
{ 
    int  y; int x;
    if (player->get_type() == PlayerType::HUMAN)
    {
        cout << "\nPlease enter your column (0 to 6): ";
        cin >>  y;
    }

    else if (player->get_type() == PlayerType::COMPUTER) {
        y = rand() % 7;
    }

    else if (player->get_type() == PlayerType::AI) {
        Four_AI ai;
        return ai.bestMove(player, '.');   
    }
    
    return new Move<char>(0, y, player->get_symbol());
}

float Four_AI::evaluate(Board<char> *board, Player<char> *player)
{
    char AI = player->get_symbol();
    char OOP = (AI == 'X'? 'O' : 'X');
    
    // Check for a win in rows
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {  
            if (j + 3 < 7 && 
                board->get_cell(i, j) == board->get_cell(i, j + 1) &&
                board->get_cell(i, j + 1) == board->get_cell(i, j + 2) &&
                board->get_cell(i, j + 2) == board->get_cell(i, j + 3) && 
                board->get_cell(i, j) != '.') {
                if (board->get_cell(i, j) == AI)
                    return +10;
                else if (board->get_cell(i, j) == OOP)
                    return -10;
            }
        }
    }
   
    // Check for a win in columns
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 7; ++j) {  
            if (i + 3 < 6 && 
                board->get_cell(i, j) == board->get_cell(i + 1, j) && 
                board->get_cell(i + 1, j) == board->get_cell(i + 2, j) &&
                board->get_cell(i + 2, j) == board->get_cell(i + 3, j) && 
                board->get_cell(i, j) != '.') {
                if (board->get_cell(i, j) == AI)
                    return +10;
                else if (board->get_cell(i, j) == OOP)
                    return -10;
            }
        }
    }
    
    // Check for diagonal wins
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            // Diagonal down-right
            if (i + 3 < 6 && j + 3 < 7 && 
                board->get_cell(i, j) == board->get_cell(i + 1, j + 1) && 
                board->get_cell(i, j) == board->get_cell(i + 2, j + 2) && 
                board->get_cell(i, j) == board->get_cell(i + 3, j + 3) && 
                board->get_cell(i, j) != '.') {
                if (board->get_cell(i, j) == AI)
                    return +10;
                else if (board->get_cell(i, j) == OOP)
                    return -10;
            }
            
            // Diagonal up-right
            if (i - 3 >= 0 && j + 3 < 7 && 
                board->get_cell(i, j) == board->get_cell(i - 1, j + 1) && 
                board->get_cell(i, j) == board->get_cell(i - 2, j + 2) && 
                board->get_cell(i, j) == board->get_cell(i - 3, j + 3) && 
                board->get_cell(i, j) != '.') {
                if (board->get_cell(i, j) == AI)
                    return +10;
                else if (board->get_cell(i, j) == OOP)
                    return -10;
            }
        }
    }
    
    return 0;
}

float Four_AI::minimax(bool aiTurn, Player<char> *player, float alpha, float beta, char blankCell, int depth)
{
    auto* board = player->get_board_ptr();
    float score = evaluate(board, player);
    
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (!isMovesLeft(board) || depth > 6) return score;

    float best = aiTurn ? INT_MIN : INT_MAX;
    char symbol = aiTurn ? player->get_symbol() : (player->get_symbol() == 'X' ? 'O' : 'X');
    
    for (int j = 0; j < 7; j++) {
        int i = get_next_available_row(board, j);
        if (i != -1) {
            Move<char> move(i, j, symbol);
            board->update_board(&move);
            float val = minimax(!aiTurn, player, alpha, beta, blankCell, depth + 1);
            best = aiTurn ? max(best, val) : min(best, val);
            Move<char> undo(i, j, 0);
            board->update_board(&undo);
            
            if (aiTurn) alpha = max(alpha, best);
            else beta = min(beta, best);
            if (beta <= alpha) break;
        }  
    }
    return best;
}

bool Four_AI::isMovesLeft(Board<char>* board)
{
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            if (board->get_cell(i,j) == '.')
                return true;
    return false;
}

int Four_AI::get_next_available_row(Board<char> *board, int col)
{
    for (int i = 5; i >= 0; --i)
        if (board->get_cell(i,col) == '.')
            return i;
    return -1;
}

Move<char> *Four_AI::bestMove(Player<char> *player, char blankCell, int depth)
{
    auto* board = player->get_board_ptr();
    char AI = player->get_symbol();
    float bestVal = INT_MIN;
    int bestMove = -1;
    float alpha = INT_MIN, beta = INT_MAX;

    for (int j = 0; j < 7; j++)
    {
        int i = get_next_available_row(board, j);
        if (i != -1)
        {
            Move<char> move(i, j, AI);
            board->update_board(&move);
            float moveVal = minimax(false, player, alpha, beta, blankCell, depth + 1);
            Move<char> undo(i, j, 0);
            board->update_board(&undo);
            if (moveVal > bestVal)
            {
                bestVal = moveVal;
                bestMove = j;
            }
            alpha = max(alpha, bestVal);
        }
    }

    return new Move<char>(0, bestMove, AI);
}