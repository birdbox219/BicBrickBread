#include "Large_Tic_Tac_Toe.h"

#include <iostream>


Large_XO_Board::Large_XO_Board():Board(5,5), emptyCell('.')
{
    board.assign(5, vector<char>(5, emptyCell));
}

bool Large_XO_Board::update_board(Move<char>* move)
{
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (s == 0 || s == emptyCell) {
        board[r][c] = emptyCell;
        --nMoves;
        return true;
    }

    // Safety Check
    if (r < 0 || r >= board.size() || c < 0 || c >= board[0].size())
        return false;

    if (board[r][c] != emptyCell)
        return false;

    board[r][c] = s;
    ++nMoves;
    return true;
}

bool Large_XO_Board::is_win(Player<char>* player)
{
    return 2*countWin(player->get_symbol()) > countTotal() && nMoves == 24;
}

bool Large_XO_Board::is_lose(Player<char>* player) 
{
    return 2*countWin(player->get_symbol()) < countTotal() && nMoves == 24;
}

bool Large_XO_Board::is_draw(Player<char>* player)
{
    return 2*countWin(player->get_symbol()) == countTotal() && nMoves == 24;
}

bool Large_XO_Board::game_is_over(Player<char>* player)
{
    return nMoves == 24;
}

int Large_XO_Board::countWin(char sym){
    int score = 0;

    // Horizontal
    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c <= 2; ++c) {
            if (board[r][c] == sym && board[r][c+1] == sym && board[r][c+2] == sym)
                ++score;
        }
    }

    // Vertical
    for (int c = 0; c < 5; ++c) {
        for (int r = 0; r <= 2; ++r) {
            if (board[r][c] == sym && board[r+1][c] == sym && board[r+2][c] == sym)
                ++score;
        }
    }

    // Diagonal: top-left to bottom-right
    for (int r = 0; r <= 2; ++r) {
        for (int c = 0; c <= 2; ++c) {
            if (board[r][c] == sym && board[r+1][c+1] == sym && board[r+2][c+2] == sym)
                ++score;
        }
    }

    // Diagonal: top-right to bottom-left
    for (int r = 0; r <= 2; ++r) {
        for (int c = 2; c < 5; ++c) {
            if (board[r][c] == sym && board[r+1][c-1] == sym && board[r+2][c-2] == sym)
                ++score;
        }
    }

    return score;
}

int Large_XO_Board::countTotal()
{
    return countWin('X') + countWin('O');
}

Large_XO_UI::Large_XO_UI() 
    : Custom_UI<char>("5x5 XO"s, 5) {}

Move<char> *Large_XO_UI::get_move(Player<char> *player)
{
    int r, c;
    
    if (player->get_type() == PlayerType::HUMAN) {
        cout << player->get_name() << " (" << player->get_symbol()
        << ") enter your move (row col): ";
        cin >> r >> c;
    } else if (player->get_type() == PlayerType::COMPUTER) {
        r = std::rand()%5, c = std::rand()%5;
    } else if (player->get_type() == PlayerType::AI) {
        std::pair move = bestMove(player, 6);
        r = move.first, c = move.second;
    }
    
    return new Move<char>(r, c, player->get_symbol());
}

int Large_XO_UI::evaluate(Large_XO_Board *board, Player<char>* player)
{
    char ai = player->get_symbol();
    char opp = (ai == 'X' ? 'O' : 'X');

    int aiScore  = board->countWin(ai);
    int oppScore = board->countWin(opp);;

    return aiScore - oppScore;
}

int Large_XO_UI::minimax(Player<char> *player, bool maximizing, int alpha, int beta, int depth)
{
    auto* board = dynamic_cast<Large_XO_Board*>(player->get_board_ptr());
    char ai = player->get_symbol();
    char opp = (ai == 'X'? 'O' : 'X');

    if(depth == 0 || board->game_is_over(player)) {
        return evaluate(board, player);
    }

    if (maximizing) {
        int best = -INF;

        for (int r = 0; r < 5; r++) {
            for (int c = 0; c < 5; c++) {
                if (board->get_cell(r,c) == '.') {

                    Move<char> move(r, c, ai);
                    board->update_board(&move);

                    int val = minimax(player, false, alpha, beta, depth - 1);

                    Move<char> undo(r, c, '.');
                    board->update_board(&undo);

                    best = std::max(best, val);
                    alpha = std::max(alpha, val);

                    if (beta <= alpha)
                        return best;
                }
            }
        }
        return best;
    }
    else { // minimizing (opponent)
        int best = INF;

        for (int r = 0; r < board->get_rows(); r++) {
            for (int c = 0; c < board->get_columns(); c++) {
                if (board->get_cell(r,c) == '.') {

                    Move<char> move(r, c, opp);
                    board->update_board(&move);

                    int val = minimax(player, true, alpha, beta, depth - 1);

                    Move<char> undo(r, c, '.');
                    board->update_board(&undo);

                    best = std::min(best, val);
                    beta = std::min(beta, val);

                    if (beta <= alpha)
                        return best;
                }
            }
        }
        return best;
    }

}

std::pair<int, int> Large_XO_UI::bestMove(Player<char> *player, int depth)
{
    auto* board = player->get_board_ptr();
    char ai = player->get_symbol();

    int bestScore = -INF;
    std::pair<int,int> move;
    
    for (int r = 0; r < 5; ++r) {
        for (int c =0 ; c < 5; ++c) {
            if(board->get_cell(r, c) == '.') {
                Move<char> putIn(r, c, ai);
                board->update_board(&putIn);

                int score = minimax(player, false, INT_MIN, INT_MAX, depth - 1);

                Move<char> undo(r,c,'.');
                board->update_board(&undo);

                if (score > bestScore) {
                    bestScore = score;
                    move = {r,c};
                }
            }
        }
    }

    return move;
}